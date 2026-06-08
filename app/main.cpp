// app/main.cpp
// Internals
#include <engine.hpp>
#include <platform/platform.hpp>
#include <vk/vk_config.hpp>
#include <vk/vk_context.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
// StdLib
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <print>
#include <vector>
//
namespace
{
def run() -> int
{
    using namespace dans;
    using namespace dans::platform;
    using namespace dans::vk;

    std::println("Hello ds-vk2!");

    std::vector<WindowHint> window_hints{};
    window_hints.emplace_back(GLFW_CLIENT_API, GLFW_NO_API);
    window_hints.emplace_back(GLFW_RESIZABLE, GLFW_FALSE);
    PlatformCfg platform_cfg{
        .window_hints = window_hints,
        .use_vulkan = true,
        .window = {.width = 800, .height = 600, .title = "Vulkan"},
    };
    VulkanCfg vulkan_cfg{.dbg_messenger = vk::DebugMessengerCfg{}, .use_glfw = true};

    EngineCfg engine_cfg{.platform = platform_cfg, .vk = vulkan_cfg};
    EngineContext engine{engine_cfg};

    auto platform = engine.platform();
    auto vk = engine.vk();

    vk->init_physical_device();

    while (platform->is_open())
    {
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}
}  // namespace

def main() -> int
{
    try
    {
        return run();
    }
    catch (const std::exception& error)
    {
        // NOLINTNEXTLINE(modernize-use-std-print): a last-resort handler must not throw
        std::fprintf(stderr, "fatal: %s\n", error.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        // NOLINTNEXTLINE(modernize-use-std-print): a last-resort handler must not throw
        std::fprintf(stderr, "fatal: unknown exception\n");
        return EXIT_FAILURE;
    }
}
