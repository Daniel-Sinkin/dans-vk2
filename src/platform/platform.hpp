// src/platform/platform.hpp
#pragma once
// Internals
#include <dans/format.hpp>
#include <dans/to_string.hpp>
#include <dans/validity.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
#include <dans/types.hpp>
// StdLib
#include <memory>
#include <string>
#include <vector>
//
namespace dans::platform
{
struct WindowConfig
{
    u32 width{800};
    u32 height{600};
    CZString title{"Vulkan"};
};
enum class WindowValidity : u8
{
    valid = 0,
    zero_width,
    zero_height,
};
[[nodiscard]] def validity(const WindowConfig& cfg) -> WindowValidity;
[[nodiscard]] def to_string(WindowValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const WindowConfig& cfg, bool include_name = false) -> std::string;

struct WindowHint
{
    int hint{};
    int value{};
};
[[nodiscard]] def to_string(const WindowHint& hint, bool include_name = false) -> std::string;

struct PlatformCfg
{
    std::vector<WindowHint> window_hints{};
    bool use_opengl{false};
    bool use_vulkan{false};
    WindowConfig window{};
};
enum class PlatformValidity : u8
{
    valid = 0,
    opengl_not_supported,
    vulkan_required,
    invalid_window,
    vulkan_client_api_hint_missing,
    vulkan_client_api_not_no_api,
};
[[nodiscard]] def validity(const PlatformCfg& cfg) -> PlatformValidity;
[[nodiscard]] def to_string(PlatformValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const PlatformCfg& cfg, bool include_name = false) -> std::string;

struct GlfwWindowDeleter
{
    def operator()(GLFWwindow* window) const noexcept -> void;
};

class PlatformContext
{
  public:
    explicit PlatformContext(PlatformCfg cfg);
    ~PlatformContext();
    PlatformContext(const PlatformContext&) = delete;
    PlatformContext(PlatformContext&&) noexcept = delete;
    def operator=(const PlatformContext&)->PlatformContext& = delete;
    def operator=(PlatformContext&&)->PlatformContext& = delete;

    [[nodiscard]] def is_open() const noexcept -> bool;
    [[nodiscard]] def to_string(bool include_name = false) const -> std::string;

  private:
    std::unique_ptr<GLFWwindow, GlfwWindowDeleter> window_{};
    PlatformCfg cfg_{};
};
}  // namespace dans::platform

DANS_FORMAT_WITH_TO_STRING(dans::platform::WindowConfig)
DANS_FORMAT_WITH_TO_STRING(dans::platform::WindowValidity)
DANS_FORMAT_WITH_TO_STRING(dans::platform::WindowHint)
DANS_FORMAT_WITH_TO_STRING(dans::platform::PlatformCfg)
DANS_FORMAT_WITH_TO_STRING(dans::platform::PlatformValidity)
