#!/usr/bin/env bash
# Shared core for dev.sh / dbg.sh / run_opt.sh. Not run directly.
#
# A caller sets two variables and then calls dans_build_run:
#   build_dir       e.g. build, build-dbg, build-opt   (each config gets its own)
#   cmake_extra=(…) array of -D… cache args for this config
#   dans_build_run "$@"   # "$@" is forwarded to the built exe
#
# Configure happens once per build dir (CMakeCache present == already configured),
# matching the fast-iteration model: only compile+link+run on every call.

dans_repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

dans_build_run() {
    cd "${dans_repo_root}"

    if [[ ! -f "${build_dir}/CMakeCache.txt" ]]; then
        # Windows (git-bash) has no default generator and clang is the toolchain,
        # so pin both. Elsewhere let CMake pick and CMakeLists select the toolchain.
        case "$(uname -s)" in
            MINGW* | MSYS* | CYGWIN*)
                cmake -S . -B "${build_dir}" -G Ninja \
                    -DCMAKE_C_COMPILER="${CC:-clang}" \
                    -DCMAKE_CXX_COMPILER="${CXX:-clang++}" \
                    "${cmake_extra[@]}"
                ;;
            *)
                cmake -S . -B "${build_dir}" "${cmake_extra[@]}"
                ;;
        esac
    fi

    cmake --build "${build_dir}" --target dans_vk2 -j
    exec "${build_dir}/dans_vk2" "$@"
}
