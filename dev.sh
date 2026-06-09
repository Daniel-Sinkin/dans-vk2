#!/usr/bin/env bash
# Normal dev build: -O0 with cheap line-tables-only debug info (file:line for
# stack traces, no variable inspection) -- fastest edit->run loop, validation on.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/_build_common.sh"

build_dir="build"
cmake_extra=(
    -DCMAKE_BUILD_TYPE=Debug
    "-DCMAKE_C_FLAGS_DEBUG=-O0 -gline-tables-only -gcodeview"
    "-DCMAKE_CXX_FLAGS_DEBUG=-O0 -gline-tables-only -gcodeview"
)
dans_build_run "$@"
