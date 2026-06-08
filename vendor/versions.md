# Vendored Sources

Third-party code is committed directly under `vendor/` so the project builds from
a fixed repository commit without submodules or configure-time downloads. Vulkan
itself and shader tooling such as `glslc` are not vendored; install those through
the Vulkan SDK.

GLFW was taken from dans-platform; the rest from dans-vk.

As of 2026-06-08 the vendored source files are comment-stripped to slim the tree:
most comments are gone, but license/copyright notices and version/edit-history
blocks are kept verbatim and no code is changed. `vendor/imgui/imgui_demo.cpp` is
the one exception, left exactly as upstream. So the kept files are no longer
byte-for-byte upstream, they differ only by removed comments (and LF line endings
per `.gitattributes`). Each trimmed file carries a short dated note near the top.

## Dependencies

- GLFW: `https://github.com/glfw/glfw`
  - Tag: `3.4`
  - Commit: `7b6aead9fb88b3623e3b3725ebb42670cbe4c579`
  - Version macro: `3.4.0`
  - Vendored form: shortened to the buildable library, kept functional for every
    GLFW target (Win32/WGL, Cocoa/NSGL, X11/GLX, Wayland/EGL, plus the EGL,
    OSMesa, and null contexts). Kept: `src/`, the public `include/GLFW` headers,
    the `CMake/` modules, `deps/wayland` protocol XML, `deps/mingw` compat
    headers, and the license file. Deleted: `tests/`, `examples/`, `docs/`,
    the readme, the contributor list, CI metadata, editor config, and the example/test-only
    helpers under `deps/`. Kept files match upstream apart from the comment
    stripping noted above.
- GLM: `https://github.com/g-truc/glm`
  - Tag: `1.0.3`
  - Commit: `8d1fd52e5ab5590e2c81768ace50c72bae28f2ed`
  - Vendored form: shortened; deleted generated docs, the readme, tests, CI
    metadata, debugger/IDE utility files, and the standalone manual. Core
    headers, CMake integration, and license are kept.
- Dear ImGui: `https://github.com/ocornut/imgui`
  - Branch: `docking`
  - Commit: `ed9d1e742793f7e4333565f891b4e3821b205f09`
  - Version macro: `1.92.8 WIP` / `19275`
  - Vendored form: core library only. All backends were removed: the Vulkan
    backend is implemented by hand in this project, and a platform backend
    (`imgui_impl_glfw`) is added under `vendor/imgui/backends/` when the UI is
    wired up.
- Vulkan Memory Allocator: `https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator`
  - Tag: `v3.3.0`
  - Commit: `1d8f600fd424278486eade7ed3e877c99f0846b1`
  - Vendored form: shortened to `include/vk_mem_alloc.h` plus license notice.
- nlohmann/json: `https://github.com/nlohmann/json`
  - Tag: `v3.11.3`
  - Commit: `9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03`
  - Vendored form: shortened to the single-header distribution
    `nlohmann/json.hpp` plus license notice.
- Microsoft GSL: `https://github.com/microsoft/gsl`
  - Tag: `v4.2.2`
  - Commit: `152d6eb989a1ecd23fe9c9cfb2fb8cfc7c0cd0c1`
  - Vendored form: shortened to the `include/gsl/` headers plus the MIT license.
    Deleted tests, docs, CI metadata, the `GSL.natvis` debugger visualizer, and
    the CMake/IDE config. The upstream `ThirdPartyNotices.txt` covered only the
    test-only Google Test dependency, which is not vendored.
- stb: `https://github.com/nothings/stb`
  - Branch: `master`
  - Commit: `31c1ad37456438565541f4919958214b6e762fb4`
  - Vendored files: `stb_image.h` v2.30, `stb_image_write.h` v1.16, and
    `stb_truetype.h` v1.26.
- Inter font family: `https://github.com/rsms/inter`
  - Tag: `v4.1`
  - Commit: `e3a3d4c57d5ecc01453a575621882a384c1995a3`
  - Vendored files: `Inter-Regular.ttf` (from the release archive `extras/ttf/`)
    and the OFL-1.1 license text as `LICENSE.txt`.
