#!/usr/bin/env bash
# Optimized build: -O3, NDEBUG (asserts off), no debug info, and Vulkan
# validation compiled out (-DDANS_VK2_NO_VALIDATION). Built for performance.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/_build_common.sh"

build_dir="build-opt"
cmake_extra=(
    -DCMAKE_BUILD_TYPE=Release
    "-DCMAKE_C_FLAGS_RELEASE=-O3 -DNDEBUG"
    "-DCMAKE_CXX_FLAGS_RELEASE=-O3 -DNDEBUG"
    -DDANS_VK2_NO_VALIDATION=ON
)
dans_build_run "$@"
