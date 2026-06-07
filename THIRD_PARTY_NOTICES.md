# Third-Party Notices

`dans-vk2` vendors third-party code directly under `vendor/` so the project can
be built from a single fixed Git commit without submodules or configure-time
downloads. Exact upstream URLs, tags, and commits are recorded in
`vendor/versions.md`.

The vendored trees are intentionally shortened copies. Removed material is
limited to non-runtime files such as tests, examples, generated docs, CI
metadata, and editor/IDE config. For Dear ImGui the backends were also removed:
the Vulkan renderer backend is written by hand in this project.

## Vendored Dependencies

| Dependency | Location | Upstream pin | License |
| --- | --- | --- | --- |
| GLFW | `vendor/glfw` | `glfw/glfw` 3.4, `7b6aead9` | zlib/libpng, see `vendor/glfw/LICENSE.md` |
| GLM | `vendor/glm` | `g-truc/glm` 1.0.3, `8d1fd52e` | MIT (Happy Bunny), see `vendor/glm/copying.txt` |
| Dear ImGui | `vendor/imgui` | `ocornut/imgui` docking, `ed9d1e74` | MIT, see `vendor/imgui/LICENSE.txt` |
| Vulkan Memory Allocator | `vendor/vma` | `GPUOpen` v3.3.0, `1d8f600f` | MIT, see `vendor/vma/LICENSE.txt` |
| nlohmann/json | `vendor/nlohmann` | `nlohmann/json` v3.11.3, `9cca280a` | MIT, see `vendor/nlohmann/LICENSE.MIT` |
| stb | `vendor/stb` | `nothings/stb` `31c1ad37` | MIT / public domain, see header notices |
| Inter | `vendor/fonts` | `rsms/inter` v4.1, `e3a3d4c5` | OFL-1.1, see `vendor/fonts/LICENSE.txt` |
