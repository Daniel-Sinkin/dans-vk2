#!/usr/bin/env python3
"""Apply the dans include-block convention to this project's own source.

Each processed file gets, at the very top:

    // <path-from-repo-root>
    #pragma once                 (headers only)
    //                           (only when a paired header follows)
    #include "<stem>.hpp"        (.cpp paired with a same-stem header)
    // Internals
    #include <...>               (this project's own headers, found under src/)
    // Externals
    #include <...>               (dans-core + third-party, not under src/)
    // StdLib
    #include <...>               (standard library: no '/' and no '.')
    //

Group classification is by the filesystem, not a hardcoded list: an angle
include is Internals iff src/<path> exists, StdLib iff its path has neither '/'
nor '.', otherwise Externals. So <dans/dev.hpp> (in src/) lands in Internals
while <dans/types.hpp> (in include/ or vendor/) lands in Externals, and the rule
keeps working if the dans-core snapshot moves.

Within each group includes are sorted byte-wise (matches clang-format's
case-sensitive SortIncludes, so the two don't fight). Trailing comments such as
`// IWYU pragma: keep` are preserved.

Usage:
    python tools/format_includes.py                  # all of SCOPE_DIRS (default)
    python tools/format_includes.py FILE [FILE ...]  # just those files
    python tools/format_includes.py --check          # report only, exit 1 if any differ

This is deliberately specific to this repo's layout. To reuse it elsewhere, copy
it and adjust REPO_ROOT discovery and SCOPE_DIRS.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

# tools/format_includes.py -> repo root is the parent of tools/.
REPO_ROOT = Path(__file__).resolve().parent.parent
SRC = REPO_ROOT / "src"

# Directories swept by the default (global) run. include/ and vendor/ are
# intentionally excluded. main.cpp lives in app/, not src/ -- add "app" here or
# pass the file explicitly if you want it covered.
SCOPE_DIRS = ["src"]

SOURCE_SUFFIXES = {".hpp", ".h", ".cpp", ".cc"}
HEADER_SUFFIXES = {".hpp", ".h"}
GROUP_ORDER = ("Internals", "Externals", "StdLib")

INCLUDE_RE = re.compile(r'^\s*#\s*include\s+[<"]([^>"]+)[>"]\s*(.*?)\s*$')


def classify(path: str) -> str:
    if "/" not in path and "." not in path:
        return "StdLib"
    if (SRC / path).exists():
        return "Internals"
    return "Externals"


def split_leading_block(lines: list[str]) -> tuple[list[str], list[str]]:
    """Split off the leading include preamble (path comment, pragma, group
    comments, includes, separators, and the closing `//`) from the code below."""
    block: list[str] = []
    seen_anchor = False  # a #pragma or #include has appeared
    i = 0
    while i < len(lines):
        s = lines[i].strip()
        is_comment = s.startswith("//")
        is_pragma = s.startswith("#pragma")
        is_include = s.startswith("#include")
        if not (s == "" or is_comment or is_pragma or is_include):
            break  # first real code line
        block.append(lines[i])
        i += 1
        if s == "//" and seen_anchor:
            break  # closing terminator; do not swallow comments below it
        if is_pragma or is_include:
            seen_anchor = True
    return block, lines[i:]


def render_block(file: Path, block: list[str]) -> list[str]:
    rel = file.relative_to(REPO_ROOT).as_posix()
    is_header = file.suffix in HEADER_SUFFIXES
    paired_names = {file.stem + ".hpp", file.stem + ".h"}

    paired: tuple[str, str] | None = None
    groups: dict[str, list[tuple[str, str]]] = {name: [] for name in GROUP_ORDER}
    for line in block:
        m = INCLUDE_RE.match(line)
        if not m:
            continue
        path, trailing = m.group(1), m.group(2)
        if not is_header and Path(path).name in paired_names:
            paired = (path, trailing)
        else:
            groups[classify(path)].append((path, trailing))

    def code_of(path: str, quoted: bool) -> str:
        return f'#include "{path}"' if quoted else f"#include <{path}>"

    def emit_lines(incs: list[tuple[str, str]], quoted: bool) -> list[str]:
        # Align trailing comments within the group to max(code) + 2, matching
        # clang-format's AlignTrailingComments with SpacesBeforeTrailingComments=2.
        width = max((len(code_of(p, quoted)) for p, _ in incs), default=0)
        lines = []
        for path, trailing in sorted(incs, key=lambda x: x[0]):
            code = code_of(path, quoted)
            lines.append(code + " " * (width - len(code) + 2) + trailing if trailing else code)
        return lines

    out = [f"// {rel}"]
    if is_header:
        out.append("#pragma once")
    if paired is not None:
        out.append("//")
        out += emit_lines([paired], quoted=True)
    for name in GROUP_ORDER:
        if groups[name]:
            out.append(f"// {name}")
            out += emit_lines(groups[name], quoted=False)
    if paired is not None or any(groups.values()):
        out.append("//")
    return out


def format_file(file: Path) -> tuple[str, str]:
    with open(file, encoding="utf-8", newline="") as fh:
        raw = fh.read()
    newline = "\r\n" if "\r\n" in raw else "\n"
    block, rest = split_leading_block(raw.splitlines())
    new_lines = render_block(file, block) + rest
    new_text = newline.join(new_lines)
    if raw.endswith("\n"):
        new_text += newline
    return raw, new_text


def gather(args: list[str]) -> list[Path]:
    if args:
        return [Path(a).resolve() for a in args]
    files: list[Path] = []
    for d in SCOPE_DIRS:
        files += [p for p in (REPO_ROOT / d).rglob("*") if p.suffix in SOURCE_SUFFIXES]
    return sorted(files)


def main(argv: list[str]) -> int:
    check = "--check" in argv
    files = gather([a for a in argv if not a.startswith("--")])

    changed = 0
    for f in files:
        if not f.is_file():
            print(f"skip (missing): {f}")
            continue
        raw, new_text = format_file(f)
        if raw == new_text:
            continue
        changed += 1
        try:
            label = f.relative_to(REPO_ROOT).as_posix()
        except ValueError:
            label = str(f)
        if check:
            print(f"would reformat: {label}")
        else:
            with open(f, "w", encoding="utf-8", newline="") as fh:
                fh.write(new_text)
            print(f"reformatted: {label}")

    if not changed:
        print("all files already formatted")
    return 1 if (check and changed) else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
