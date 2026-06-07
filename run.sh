#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${repo_root}"

build_dir="${DANS_VK2_BUILD_DIR:-build}"
target="dans_vk2"

if [[ ! -f "${build_dir}/CMakeCache.txt" ]]; then
    # Windows has no default CMake generator without Visual Studio, and clang is
    # the toolchain here, so pin both. On macOS/Linux let CMake pick the default
    # generator and let CMakeLists.txt select the Homebrew LLVM toolchain.
    case "$(uname -s)" in
        MINGW*|MSYS*|CYGWIN*)
            cmake -S . -B "${build_dir}" -G Ninja \
                -DCMAKE_C_COMPILER="${CC:-clang}" \
                -DCMAKE_CXX_COMPILER="${CXX:-clang++}"
            ;;
        *)
            cmake -S . -B "${build_dir}"
            ;;
    esac
fi

build_log="$(mktemp "${TMPDIR:-/tmp}/dans_vk2_build.XXXXXX")"
cleanup() {
    rm -f "${build_log}"
}
trap cleanup EXIT

if ! cmake --build "${build_dir}" --target "${target}" -j >"${build_log}" 2>&1; then
    cat "${build_log}" >&2
    exit 1
fi

exec "${build_dir}/${target}" "$@"
