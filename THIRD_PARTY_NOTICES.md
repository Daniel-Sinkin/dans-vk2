# Third-Party Notices

`dans-vk2` vendors third-party code directly under `vendor/` so the project can
be built from a single fixed Git commit without submodules or configure-time
downloads. Exact upstream URLs, tags, and commits are in `vendor/versions.md`.

The vendored trees are cut down in two ways. First, non-runtime files are
removed: tests, examples, generated docs, CI metadata, editor/IDE config,
debugger visualizers, and upstream notices that only cover those removed files.
For Dear ImGui the backends are gone too (the Vulkan backend is hand-written
here). Second, as of 2026-06-08 most comments were stripped out of the source to
slim it further. The stripping only removes comments, license and copyright
notices plus version/edit-history blocks are kept verbatim, and no code is
touched. Each trimmed file carries a short dated note near the top pointing back
here.

One exception: `vendor/imgui/imgui_demo.cpp` is left exactly as upstream, by
request, comments and all.

The short commit in each entry below pins the source on its own (it is unique
within its repo). The full 40-char SHA is in `vendor/versions.md` if you want
the exact object, and the repo link is there for convenience.

## Vendored Dependencies

### GLFW
- Location: `vendor/glfw`
- Upstream: https://github.com/glfw/glfw, 3.4, `7b6aead9`
- License: zlib/libpng, see `vendor/glfw/LICENSE.md`

### GLM
- Location: `vendor/glm`
- Upstream: https://github.com/g-truc/glm, 1.0.3, `8d1fd52e`
- License: MIT (Happy Bunny), see `vendor/glm/copying.txt`

### Dear ImGui
- Location: `vendor/imgui`
- Upstream: https://github.com/ocornut/imgui, docking branch, `ed9d1e74`
- License: MIT, see `vendor/imgui/LICENSE.txt`

### Vulkan Memory Allocator
- Location: `vendor/vma`
- Upstream: https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator, v3.3.0, `1d8f600f`
- License: MIT, see `vendor/vma/LICENSE.txt`

### nlohmann/json
- Location: `vendor/nlohmann`
- Upstream: https://github.com/nlohmann/json, v3.11.3, `9cca280a`
- License: MIT, see `vendor/nlohmann/LICENSE.MIT`

### Microsoft GSL
- Location: `vendor/gsl`
- Upstream: https://github.com/microsoft/gsl, v4.2.2, `152d6eb9`
- License: MIT, see `vendor/gsl/LICENSE`

### stb
- Location: `vendor/stb`
- Upstream: https://github.com/nothings/stb, `31c1ad37`
- License: MIT / public domain, see header notices

### Inter
- Location: `vendor/fonts`
- Upstream: https://github.com/rsms/inter, v4.1, `e3a3d4c5`
- License: OFL-1.1, see `vendor/fonts/LICENSE.txt`
