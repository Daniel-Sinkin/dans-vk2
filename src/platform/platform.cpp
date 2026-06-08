// src/platform/platform.cpp
//
#include "platform/platform.hpp"
// Internals
#include <dans/chrono.hpp>
#include <dans/dev.hpp>
#include <dans/strings.hpp>
#include <dans/to_string.hpp>
#include <dans/validity.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
// StdLib
#include <algorithm>
#include <cassert>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <utility>
//
namespace
{
def glfw_error_callback(int code, const char* description) -> void
{
    std::println(stderr, "[glfw] error {}: {}", code, description);
}
}  // namespace

namespace dans::platform
{
def validity(const WindowConfig& cfg) -> WindowValidity
{
    using enum WindowValidity;
    if (cfg.width == 0) return zero_width;
    if (cfg.height == 0) return zero_height;
    return valid;
}
def to_string(WindowValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum WindowValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case zero_width:
                return "zero_width";
            case zero_height:
                return "zero_height";
        }
        std::unreachable();
    }();
    return with_name("WindowValidity", body, include_name);
}
def to_string(const WindowConfig& cfg, bool include_name) -> std::string
{
    const auto body = std::format("width={},height={},title={}", cfg.width, cfg.height, cfg.title);
    return with_name("WindowConfig", body, include_name);
}
def to_string(const WindowHint& hint, bool include_name) -> std::string
{
    const auto body = std::format("hint={},value={}", hint.hint, hint.value);
    return with_name("WindowHint", body, include_name);
}
def validity(const PlatformCfg& cfg) -> PlatformValidity
{
    using enum PlatformValidity;
    if (cfg.use_opengl) return opengl_not_supported;
    if (not cfg.use_vulkan) return vulkan_required;
    if (not is_valid(cfg.window)) return invalid_window;

    using std::ranges::find;
    const auto hint =
        find(cfg.window_hints, GLFW_CLIENT_API, [](const WindowHint& wh) { return wh.hint; });
    if (hint == cfg.window_hints.end()) return vulkan_client_api_hint_missing;
    if (hint->value != GLFW_NO_API) return vulkan_client_api_not_no_api;
    return valid;
}
def to_string(PlatformValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum PlatformValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case opengl_not_supported:
                return "opengl_not_supported";
            case vulkan_required:
                return "vulkan_required";
            case invalid_window:
                return "invalid_window";
            case vulkan_client_api_hint_missing:
                return "vulkan_client_api_hint_missing";
            case vulkan_client_api_not_no_api:
                return "vulkan_client_api_not_no_api";
        }
        std::unreachable();
    }();
    return with_name("PlatformValidity", body, include_name);
}
def to_string(const PlatformCfg& cfg, bool include_name) -> std::string
{
    const auto hints =
        str::join(cfg.window_hints, [](const WindowHint& wh) { return to_string(wh, true); }, ", ");
    const auto body = std::format(
        "window_hints=[{}],use_opengl={},use_vulkan={},window={}",
        hints,
        cfg.use_opengl,
        cfg.use_vulkan,
        to_string(cfg.window, true)
    );
    return with_name("PlatformCfg", body, include_name);
}

def GlfwWindowDeleter::operator()(GLFWwindow* window) const noexcept -> void
{
    glfwDestroyWindow(window);
}

PlatformContext::PlatformContext(PlatformCfg cfg) : cfg_{std::move(cfg)}
{
    DANS_SCOPE_TIMER("PlatformContext");
    using std::ranges::for_each;

    DANS_CHECK_VALID(cfg_);

    glfwSetErrorCallback(glfw_error_callback);
    if (not glfwInit()) DANS_PANIC("Failed to initialize GLFW!");
    if (cfg_.use_vulkan and not glfwVulkanSupported()) DANS_PANIC("Vulkan is not supported!");

    for_each(cfg_.window_hints, [](const WindowHint& wh) { glfwWindowHint(wh.hint, wh.value); });

    const auto [width, height, title] = cfg_.window;

    auto* window = glfwCreateWindow(
        static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr
    );
    if (not window) DANS_PANIC("Failed to create window!");
    window_.reset(window);
}
PlatformContext::~PlatformContext()
{
    window_.reset();
    glfwTerminate();
}
def PlatformContext::is_open() const noexcept -> bool
{
    assert(window_);
    return glfwWindowShouldClose(window_.get()) == 0;
}
def PlatformContext::to_string(bool include_name) const -> std::string
{
    const auto body = std::format("window={}", dans::platform::to_string(cfg_.window, true));
    return with_name("PlatformContext", body, include_name);
}
}  // namespace dans::platform
