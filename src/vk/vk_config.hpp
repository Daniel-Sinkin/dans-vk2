// src/vk/vk_config.hpp
#pragma once
// Internals
#include <dans/format.hpp>
#include <dans/to_string.hpp>
#include <dans/validity.hpp>
// Externals
#include <dans/development_markers.hpp>
#include <dans/types.hpp>
#include <vulkan/vulkan.h>
// StdLib
#include <optional>
#include <span>
#include <string>
#include <vector>
//
namespace dans::vk
{
inline constexpr CZString k_validation_layer{"VK_LAYER_KHRONOS_validation"};

struct ApiVersion
{
    u16 variant{0};
    u16 major{0};
    u16 minor{1};
    u16 patch{0};
    [[nodiscard]] def combined() const -> u32
    {
        return static_cast<u32>(VK_MAKE_API_VERSION(variant, major, minor, patch));
    }
    [[nodiscard]] static def vulkan() -> ApiVersion
    {
        return ApiVersion{.variant = 0, .major = 1, .minor = 4, .patch = 0};
    }
};
enum class ApiVersionValidity : u8
{
    valid = 0,
    zero_version,
};
[[nodiscard]] def validity(const ApiVersion& api) -> ApiVersionValidity;
[[nodiscard]] def to_string(ApiVersionValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const ApiVersion& api, bool include_name = false) -> std::string;

struct ApplicationCfg
{
    CZString app_name{"dans-vk2"};
    ApiVersion app_version{};
    CZString engine_name{"dans-vk2"};
    ApiVersion engine_version{};
    ApiVersion api_version{ApiVersion::vulkan()};
};
enum class ApplicationValidity : u8
{
    valid = 0,
    invalid_app_version,
    invalid_engine_version,
    invalid_api_version,
    api_version_below_1_4,
};
[[nodiscard]] def validity(const ApplicationCfg& cfg) -> ApplicationValidity;
[[nodiscard]] def to_string(ApplicationValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const ApplicationCfg& cfg, bool include_name = false) -> std::string;

enum class MessageSeverity : u8
{
    verbose = 0,
    info = 1,
    warn = 2,
    error = 3,
};
[[nodiscard]] def to_string(MessageSeverity severity, bool include_name = false) -> std::string;

struct DebugMessengerEnabledMessagesCfg
{
    bool general{true};
    bool validation{true};
    bool performance{true};
    bool device_address_binding{false};
};
enum class DebugMessengerEnabledMessagesValidity : u8
{
    valid = 0,
    no_message_type_enabled,
};
[[nodiscard]] def validity(const DebugMessengerEnabledMessagesCfg& cfg)
    -> DebugMessengerEnabledMessagesValidity;
[[nodiscard]] def to_string(DebugMessengerEnabledMessagesValidity v, bool include_name = false)
    -> std::string;
[[nodiscard]] def to_string(const DebugMessengerEnabledMessagesCfg& cfg, bool include_name = false)
    -> std::string;

struct DebugMessengerCfg
{
    MessageSeverity severity_threshold{MessageSeverity::warn};
    VkFlags flags{};
    DebugMessengerEnabledMessagesCfg enabled_messages{};
};
enum class DebugMessengerValidity : u8
{
    valid = 0,
    invalid_enabled_messages,
    nonzero_flags,
};
[[nodiscard]] def validity(const DebugMessengerCfg& cfg) -> DebugMessengerValidity;
[[nodiscard]] def to_string(DebugMessengerValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const DebugMessengerCfg& cfg, bool include_name = false) -> std::string;

struct CreateInstanceCfg
{
    std::span<std::string> layers{};
    std::span<std::string> extensions{};
    ApplicationCfg app{};
    bool enumerate_portability{};
    std::optional<DebugMessengerCfg> dbg_messenger{};
};
enum class CreateInstanceValidity : u8
{
    valid = 0,
    invalid_app,
    missing_validation_layer,
};
[[nodiscard]] def validity(const CreateInstanceCfg& cfg) -> CreateInstanceValidity;
[[nodiscard]] def to_string(CreateInstanceValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const CreateInstanceCfg& cfg, bool include_name = false) -> std::string;

struct VulkanCfg
{
    std::optional<DebugMessengerCfg> dbg_messenger{};
    bool use_glfw{false};
    std::vector<CZString> layers{};
    std::vector<CZString> extensions{};
    bool enumerate_portability{false};
    ApplicationCfg app{};
};
enum class VulkanValidity : u8
{
    valid = 0,
    invalid_dbg_messenger,
    invalid_app,
};
[[nodiscard]] def validity(const VulkanCfg& cfg) -> VulkanValidity;
[[nodiscard]] def to_string(VulkanValidity v, bool include_name = false) -> std::string;
[[nodiscard]] def to_string(const VulkanCfg& cfg, bool include_name = false) -> std::string;
}  // namespace dans::vk

DANS_FORMAT_WITH_TO_STRING(dans::vk::ApiVersion)
DANS_FORMAT_WITH_TO_STRING(dans::vk::ApiVersionValidity)
DANS_FORMAT_WITH_TO_STRING(dans::vk::ApplicationCfg)
DANS_FORMAT_WITH_TO_STRING(dans::vk::ApplicationValidity)
DANS_FORMAT_WITH_TO_STRING(dans::vk::MessageSeverity)
DANS_FORMAT_WITH_TO_STRING(dans::vk::DebugMessengerEnabledMessagesCfg)
DANS_FORMAT_WITH_TO_STRING(dans::vk::DebugMessengerEnabledMessagesValidity)
DANS_FORMAT_WITH_TO_STRING(dans::vk::DebugMessengerCfg)
DANS_FORMAT_WITH_TO_STRING(dans::vk::DebugMessengerValidity)
DANS_FORMAT_WITH_TO_STRING(dans::vk::CreateInstanceCfg)
DANS_FORMAT_WITH_TO_STRING(dans::vk::CreateInstanceValidity)
DANS_FORMAT_WITH_TO_STRING(dans::vk::VulkanCfg)
DANS_FORMAT_WITH_TO_STRING(dans::vk::VulkanValidity)
