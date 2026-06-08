// src/vk/vk_config.cpp
//
#include "vk/vk_config.hpp"
// Internals
#include <dans/strings.hpp>
#include <dans/to_string.hpp>
#include <dans/validity.hpp>
// Externals
#include <dans/development_markers.hpp>
#include <vulkan/vulkan.h>
// StdLib
#include <algorithm>
#include <format>
#include <string>
#include <string_view>
#include <utility>
//
namespace dans::vk
{
def validity(const ApiVersion& api) -> ApiVersionValidity
{
    using enum ApiVersionValidity;
    if (api.major == 0 and api.minor == 0) return zero_version;
    return valid;
}
def to_string(ApiVersionValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum ApiVersionValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case zero_version:
                return "zero_version";
        }
        std::unreachable();
    }();
    return with_name("ApiVersionValidity", body, include_name);
}
def to_string(const ApiVersion& api, bool include_name) -> std::string
{
    const auto body = std::format("{}.{}.{} [{}]", api.major, api.minor, api.patch, api.variant);
    return with_name("ApiVersion", body, include_name);
}

def validity(const ApplicationCfg& cfg) -> ApplicationValidity
{
    using enum ApplicationValidity;
    if (not is_valid(cfg.app_version)) return invalid_app_version;
    if (not is_valid(cfg.engine_version)) return invalid_engine_version;
    if (not is_valid(cfg.api_version)) return invalid_api_version;
    if (cfg.api_version.major == 0 or (cfg.api_version.major == 1 and cfg.api_version.minor < 4))
    {
        return api_version_below_1_4;
    }
    return valid;
}
def to_string(ApplicationValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum ApplicationValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case invalid_app_version:
                return "invalid_app_version";
            case invalid_engine_version:
                return "invalid_engine_version";
            case invalid_api_version:
                return "invalid_api_version";
            case api_version_below_1_4:
                return "api_version_below_1_4";
        }
        std::unreachable();
    }();
    return with_name("ApplicationValidity", body, include_name);
}
def to_string(const ApplicationCfg& cfg, bool include_name) -> std::string
{
    const auto body = std::format(
        "app_name={},app_version={},engine_name={},engine_version={},api_version={}",
        cfg.app_name,
        to_string(cfg.app_version, true),
        cfg.engine_name,
        to_string(cfg.engine_version, true),
        to_string(cfg.api_version, true)
    );
    return with_name("ApplicationCfg", body, include_name);
}

def to_string(MessageSeverity severity, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum MessageSeverity;
        switch (severity)
        {
            case verbose:
                return "verbose";
            case info:
                return "info";
            case warn:
                return "warn";
            case error:
                return "error";
        }
        std::unreachable();
    }();
    return with_name("MessageSeverity", body, include_name);
}

def validity(const DebugMessengerEnabledMessagesCfg& cfg) -> DebugMessengerEnabledMessagesValidity
{
    using enum DebugMessengerEnabledMessagesValidity;
    if (not(cfg.general or cfg.validation or cfg.performance or cfg.device_address_binding))
    {
        return no_message_type_enabled;
    }
    return valid;
}
def to_string(DebugMessengerEnabledMessagesValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum DebugMessengerEnabledMessagesValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case no_message_type_enabled:
                return "no_message_type_enabled";
        }
        std::unreachable();
    }();
    return with_name("DebugMessengerEnabledMessagesValidity", body, include_name);
}
def to_string(const DebugMessengerEnabledMessagesCfg& cfg, bool include_name) -> std::string
{
    const auto body = std::format(
        "general={},validation={},performance={},device_address_binding={}",
        cfg.general,
        cfg.validation,
        cfg.performance,
        cfg.device_address_binding
    );
    return with_name("DebugMessengerEnabledMessagesCfg", body, include_name);
}

def validity(const DebugMessengerCfg& cfg) -> DebugMessengerValidity
{
    using enum DebugMessengerValidity;
    if (not is_valid(cfg.enabled_messages)) return invalid_enabled_messages;
    if (cfg.flags != VkFlags{0}) return nonzero_flags;
    return valid;
}
def to_string(DebugMessengerValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum DebugMessengerValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case invalid_enabled_messages:
                return "invalid_enabled_messages";
            case nonzero_flags:
                return "nonzero_flags";
        }
        std::unreachable();
    }();
    return with_name("DebugMessengerValidity", body, include_name);
}
def to_string(const DebugMessengerCfg& cfg, bool include_name) -> std::string
{
    const auto body = std::format(
        "severity_threshold={},flags={},enabled_messages={}",
        to_string(cfg.severity_threshold, true),
        cfg.flags,
        to_string(cfg.enabled_messages, true)
    );
    return with_name("DebugMessengerCfg", body, include_name);
}

def validity(const CreateInstanceCfg& cfg) -> CreateInstanceValidity
{
    using enum CreateInstanceValidity;
    if (not is_valid(cfg.app)) return invalid_app;
    if (cfg.dbg_messenger and not std::ranges::contains(cfg.layers, k_validation_layer))
    {
        return missing_validation_layer;
    }
    return valid;
}
def to_string(CreateInstanceValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum CreateInstanceValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case invalid_app:
                return "invalid_app";
            case missing_validation_layer:
                return "missing_validation_layer";
        }
        std::unreachable();
    }();
    return with_name("CreateInstanceValidity", body, include_name);
}
def to_string(const CreateInstanceCfg& cfg, bool include_name) -> std::string
{
    const auto body = std::format(
        "layers=[{}],extensions=[{}],app={},enumerate_portability={},dbg_messenger={}",
        str::join(cfg.layers, ", "),
        str::join(cfg.extensions, ", "),
        to_string(cfg.app, true),
        cfg.enumerate_portability,
        cfg.dbg_messenger ? to_string(*cfg.dbg_messenger, true) : std::string{"none"}
    );
    return with_name("CreateInstanceCfg", body, include_name);
}

def validity(const VulkanCfg& cfg) -> VulkanValidity
{
    using enum VulkanValidity;
    if (cfg.dbg_messenger and not is_valid(*cfg.dbg_messenger)) return invalid_dbg_messenger;
    if (not is_valid(cfg.app)) return invalid_app;
    return valid;
}
def to_string(VulkanValidity v, bool include_name) -> std::string
{
    const auto body = [&]() -> std::string_view
    {
        using enum VulkanValidity;
        switch (v)
        {
            case valid:
                return "valid";
            case invalid_dbg_messenger:
                return "invalid_dbg_messenger";
            case invalid_app:
                return "invalid_app";
        }
        std::unreachable();
    }();
    return with_name("VulkanValidity", body, include_name);
}
def to_string(const VulkanCfg& cfg, bool include_name) -> std::string
{
    const auto body = std::format(
        "dbg_messenger={},use_glfw={},layers=[{}],extensions=[{}],enumerate_portability={},app={}",
        cfg.dbg_messenger ? to_string(*cfg.dbg_messenger, true) : std::string{"none"},
        cfg.use_glfw,
        str::join(cfg.layers, ", "),
        str::join(cfg.extensions, ", "),
        cfg.enumerate_portability,
        to_string(cfg.app, true)
    );
    return with_name("VulkanCfg", body, include_name);
}
}  // namespace dans::vk
