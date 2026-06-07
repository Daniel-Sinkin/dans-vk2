# dans-vk2

Second pass at a hand-rolled Vulkan framework, built to learn the API from the
ground up. Windowing is GLFW, math is glm, allocation is VMA, and the UI will be
Dear ImGui with a Vulkan backend written by hand rather than vendored.

Right now `main` just prints a hello line. The point of this repo is the
scaffolding around it: build, vendored deps, and a precompiled header that are
ready for Vulkan code to go in.

The foundation headers under `include/dans` and `src/dans` are a vendored
snapshot of dans-platform. Third-party libraries live under `vendor/`; exact
upstream pins are in `vendor/versions.md`, license info in
`THIRD_PARTY_NOTICES.md`.

## Build

Needs clang, CMake, Ninja, and the Vulkan SDK (for the loader and `glslc`).

    ./run.sh        # configure on first run, then build and run dans_vk2
    ./watcher.sh    # rebuild and rerun on changes (needs watchexec)

Or drive CMake directly:

    cmake -S . -B build
    cmake --build build --target dans_vk2
    ./build/dans_vk2

## Layout

- `app/` - executable entry point
- `include/dans/` - public foundation headers (`types`, `pch`, `development_markers`)
- `src/dans/` - utility headers (vendored foundation)
- `src/vk/` - your Vulkan framework; `src/vk/pch.hpp` is the precompiled header (stdlib plus Vulkan/GLFW/glm/VMA)
- `vendor/` - vendored third-party sources
