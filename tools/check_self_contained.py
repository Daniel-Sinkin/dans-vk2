#!/usr/bin/env python3
"""Compile every first-party file on its own with the PCH DISABLED, proving it
includes everything it uses and leans on nothing transitive.

The project force-includes src/vk/pch.hpp into every TU for speed. That cache
must never become a dependency: a file that only compiles because the PCH
happened to drag in <vector> is broken (it fails IWYU, fails for any non-PCH
consumer, and fails the day the PCH changes). This reproduces the real compile
flags from compile_commands.json, strips the -include-pch injection, and
-fsyntax-only's each file. Headers are wrapped in a one-line .cpp; sources are
compiled directly.

Usage:
    python tools/check_self_contained.py                 # all of SCOPE_DIRS
    python tools/check_self_contained.py FILE [FILE ...] # just those
"""
from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
SCOPE_DIRS = ["src", "app"]
HEADER_SUFFIXES = {".hpp", ".h"}
SOURCE_SUFFIXES = {".hpp", ".h", ".cpp", ".cc"}


def base_flags() -> tuple[list[str], str]:
    """Real compile flags minus the source, output, dep-gen, and PCH injection."""
    cc = json.loads((REPO / "build" / "compile_commands.json").read_text())
    # Pick a first-party TU; vendored entries (glm, imgui) carry only their own
    # include flags, not the app's -I src / -I vendor.
    def first_party(f: str) -> bool:
        p = f.replace("\\", "/")
        return "/src/" in p or "/app/" in p
    entry = next(e for e in cc if e["file"].endswith(".cpp") and first_party(e["file"]))
    toks = entry["command"].split()
    out: list[str] = []
    i = 0
    while i < len(toks):
        t = toks[i]
        nxt = toks[i + 1] if i + 1 < len(toks) else ""
        if t == "-c" or t == "-MD":
            i += 1
        elif t in ("-o", "-MT", "-MF"):
            i += 2
        elif t == "-Xclang" and nxt in ("-include-pch", "-include"):
            i += 2
        elif t == "-Xclang" and (nxt.endswith(".pch") or nxt.endswith("cmake_pch.hxx")):
            i += 2
        elif t.endswith(".cpp"):
            i += 1
        else:
            out.append(t)
            i += 1
    return out, entry["directory"]


def check(file: Path, flags: list[str], cwd: str) -> tuple[bool, str]:
    wrapper = None
    try:
        if file.suffix in HEADER_SUFFIXES:
            fd, wrapper = tempfile.mkstemp(suffix=".cpp", dir=str(REPO / "build"))
            os.write(fd, ('#include "%s"\n' % file.as_posix()).encode())
            os.close(fd)
            target = wrapper
        else:
            target = str(file)
        r = subprocess.run(
            flags + ["-fsyntax-only", "-x", "c++", target],
            cwd=cwd, capture_output=True, text=True
        )
        return r.returncode == 0, r.stderr
    finally:
        if wrapper:
            os.remove(wrapper)


def main(argv: list[str]) -> int:
    files = [Path(a).resolve() for a in argv if not a.startswith("--")]
    if not files:
        for d in SCOPE_DIRS:
            files += [p for p in (REPO / d).rglob("*") if p.suffix in SOURCE_SUFFIXES]
    files = sorted(set(files))

    flags, cwd = base_flags()
    failures: list[tuple[str, str]] = []
    for f in files:
        ok, err = check(f, flags, cwd)
        rel = f.relative_to(REPO).as_posix() if REPO in f.parents else str(f)
        print(("ok   " if ok else "FAIL ") + rel)
        if not ok:
            failures.append((rel, err))

    if failures:
        print("\n=== %d not self-contained ===" % len(failures))
        for rel, err in failures:
            line = next((l for l in err.splitlines() if "error:" in l), "")
            print("  %s\n    %s" % (rel, line.strip()))
        return 1
    print("\nall %d files self-contained (compile clean with PCH disabled)" % len(files))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
