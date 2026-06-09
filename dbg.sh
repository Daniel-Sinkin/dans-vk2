#!/usr/bin/env bash
# Full debug build: -O0 with complete codeview symbols for stepping in a
# debugger (breakpoints, variable inspection). Validation on.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/_build_common.sh"

build_dir="build-dbg"
cmake_extra=(
    -DCMAKE_BUILD_TYPE=Debug
    "-DCMAKE_C_FLAGS_DEBUG=-O0 -g -gcodeview"
    "-DCMAKE_CXX_FLAGS_DEBUG=-O0 -g -gcodeview"
)
dans_build_run "$@"
