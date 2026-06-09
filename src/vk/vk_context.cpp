// src/vk/vk_context.cpp
//
#include "vk/vk_context.hpp"
// Internals
#include <dans/chrono.hpp>
#include <dans/dev.hpp>
#include <dans/strings.hpp>
#include <dans/to_string.hpp>
#include <dans/validity.hpp>
#include <vk/vk_config.hpp>
#include <vk/vk_types.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
#include <vulkan/vulkan.h>
// StdLib
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <format>
#include <memory>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
//
namespace
{
using namespace dans;

// GLFW exposes no init query; glfwGetPlatform returns the selected platform
// (nonzero) once initialized, 0 + GLFW_NOT_INITIALIZED before. Read back the
// error so a later glfwGetError can't misread the one this probe provoked.
[[nodiscard]] def glfw_is_initialized() -> bool
{
    const bool initialized = glfwGetPlatform() != 0;
    glfwGetError(nullptr);
    return initialized;
}

namespace vk_message_severity
{
using Flag = VkDebugUtilsMessageSeverityFlagBitsEXT;
constexpr Flag verbose{VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT};
constexpr Flag info{VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT};
constexpr Flag warn{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT};
constexpr Flag error{VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT};

constexpr std::array<Flag, 4> flags = {verbose, info, warn, error};
constexpr usize k_count{flags.size()};

[[nodiscard]] def to_string(Flag severity) -> std::string_view
{
    switch (severity)
    {
        case error:
            return "error";
        case warn:
            return "warning";
        case info:
            return "info";
        case verbose:
            return "verbose";
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            DANS_PANIC_FMT("Invalid severity {}", std::to_underlying(severity));
    }
    std::unreachable();
}
}  // namespace vk_message_severity

// Aligned with vk_message_severity::flags = {verbose, info, warn, error}.
constexpr std::array<std::string_view, vk_message_severity::k_count> k_severity_log_markers{{
    "VERB",
    "INFO",
    "WARN",
    "ERRO",
}};

using MessageSeverityFlags = VkDebugUtilsMessageSeverityFlagsEXT;
using MessageType = VkDebugUtilsMessageTypeFlagsEXT;
using MessengerCallbackData = VkDebugUtilsMessengerCallbackDataEXT;

constexpr auto k_severity_threshold{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT};
[[nodiscard]] def high_severity(vk_message_severity::Flag severity) -> bool
{
    return severity >= k_severity_threshold;
}
VKAPI_ATTR def VKAPI_CALL debug_callback(
    vk_message_severity::Flag severity,
    [[maybe_unused]] MessageType type,
    const MessengerCallbackData* data,
    [[maybe_unused]] void* user_data
) -> VkBool32
{
    using std::ranges::views::zip;
    const auto& flags = vk_message_severity::flags;
    const auto& markers = k_severity_log_markers;
    for (const auto [sev, mrk] : zip(flags, markers))
    {
        if (sev != severity) continue;
        std::println(
            high_severity(severity) ? stderr : stdout, "[VULK][{}] {}", mrk, data->pMessage
        );
        return VK_FALSE;
    }
    DANS_PANIC_FMT("Failed to match severity {}", vk_message_severity::to_string(severity));
}

def vk_dbg_messenger_create_info(const dans::vk::DebugMessengerCfg& cfg)
    -> VkDebugUtilsMessengerCreateInfoEXT
{
    using MS = dans::vk::MessageSeverity;
    namespace vk_ms = vk_message_severity;

    MessageSeverityFlags message_severity{vk_ms::error};
    if (cfg.severity_threshold <= MS::warn) message_severity |= vk_ms::warn;
    if (cfg.severity_threshold <= MS::info) message_severity |= vk_ms::info;
    if (cfg.severity_threshold <= MS::verbose) message_severity |= vk_ms::verbose;

    const auto& em = cfg.enabled_messages;
    VkDebugUtilsMessageTypeFlagsEXT message_type{};
    if (em.general) message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
    if (em.validation) message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    if (em.performance) message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    if (em.device_address_binding)
        message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    assert(message_type > 0);

    return {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = cfg.flags,
        .messageSeverity = message_severity,
        .messageType = message_type,
        .pfnUserCallback = debug_callback,
        .pUserData = nullptr,
    };
}
}  // namespace

namespace dans::vk
{
DebugMessenger::DebugMessenger(
    VkInstance* instance, const VkDebugUtilsMessengerCreateInfoEXT& create_info
)
    : vk_instance_{instance}
{
    assert(instance);
    auto* create = get_pfn<PFN_vkCreateDebugUtilsMessengerEXT>(
        *vk_instance_, "vkCreateDebugUtilsMessengerEXT"
    );
    DANS_VK_CHECK(create(*vk_instance_, &create_info, nullptr, &vk_messenger_));
}
DebugMessenger::~DebugMessenger()
{
    assert(vk_instance_);
    auto* destroy = get_pfn<PFN_vkDestroyDebugUtilsMessengerEXT>(
        *vk_instance_, "vkDestroyDebugUtilsMessengerEXT"
    );
    destroy(*vk_instance_, vk_messenger_, nullptr);
}

VulkanContext::VulkanContext(VulkanCfg cfg)
{
    DANS_SCOPE_TIMER("VulkanContext");

    using std::ranges::any_of;
    using std::ranges::contains;
    using std::ranges::for_each;

    DANS_CHECK_VALID(cfg);

    query_vulkan();

    const auto require_extension = [&](CZString e) { required_extensions_.emplace_back(e); };
    const auto require_layer = [&](CZString l) { required_layers_.emplace_back(l); };

    for_each(cfg.extensions, require_extension);
    for_each(cfg.layers, require_layer);

    if (cfg.use_glfw)
    {
        if (not glfw_is_initialized())
            DANS_PANIC("GLFW not initialized; construct PlatformContext before VulkanContext");

        auto const glfw_extensions = []
        {
            u32 glfw_count{0};
            CZString* raw_glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_count);
            if (not raw_glfw_extensions) DANS_PANIC("Got no GLFW Extensions (no Vulkan loader?)");
            return std::span{raw_glfw_extensions, static_cast<usize>(glfw_count)};
        }();
        for_each(glfw_extensions, require_extension);
    }
    if (cfg.dbg_messenger)
    {
        require_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        require_layer(k_validation_layer);
    }

    const auto is_not_subset = [](const auto& as, const auto& bs)
    { return any_of(as, [&bs](const auto& a) { return not contains(bs, a); }); };

    if (is_not_subset(required_extensions_, available_extensions_))
    {
        DANS_PANIC("At least one required extension is not available");
    }
    if (is_not_subset(required_layers_, available_layers_))
    {
        DANS_PANIC("At least one required layer is not available");
    }
    vk_create_instance({
        .layers = required_layers_,
        .extensions = required_extensions_,
        .app = cfg.app,
        .enumerate_portability = cfg.enumerate_portability,
        .dbg_messenger = cfg.dbg_messenger,
    });
    if (not instance_) DANS_PANIC("Instance not Created!");
}
VulkanContext::~VulkanContext()
{
    debug_messenger_.reset();
    if (instance_ != VK_NULL_HANDLE) vkDestroyInstance(instance_, nullptr);
}
def VulkanContext::query_vulkan() -> void
{
    u32 count{};
    DANS_VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));
    std::vector<VkExtensionProperties> extensions(count);
    DANS_VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data()));
    for (const auto& extension : extensions)
    {
        available_extensions_.emplace_back(extension.extensionName);
    }

    DANS_VK_CHECK(vkEnumerateInstanceLayerProperties(&count, nullptr));
    std::vector<VkLayerProperties> layers(count);
    DANS_VK_CHECK(vkEnumerateInstanceLayerProperties(&count, layers.data()));
    for (const auto& layer : layers)
    {
        available_layers_.emplace_back(layer.layerName);
    }
}
def VulkanContext::create_app_info(const ApplicationCfg& cfg) -> VkApplicationInfo
{
    DANS_CHECK_VALID(cfg);
    return {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = cfg.app_name,
        .applicationVersion = cfg.app_version.combined(),
        .pEngineName = cfg.engine_name,
        .engineVersion = cfg.engine_version.combined(),
        .apiVersion = cfg.api_version.combined(),
    };
}
def VulkanContext::vk_create_instance(const CreateInstanceCfg& cfg) -> void
{
    DANS_CHECK_VALID(cfg);
    VkFlags flags{0};
    if (cfg.enumerate_portability) flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    const auto application_info = create_app_info(cfg.app);

    const auto to_c_strings = [](std::span<const std::string> strings) -> std::vector<CZString>
    {
        std::vector<CZString> out{};
        out.reserve(strings.size());
        for (const auto& name : strings)
            out.emplace_back(name.c_str());
        return out;
    };
    const auto enabled_layers = to_c_strings(cfg.layers);
    const auto enabled_extensions = to_c_strings(cfg.extensions);

    const auto enabled_layer_count = static_cast<u32>(enabled_layers.size());
    const auto enabled_extension_count = static_cast<u32>(enabled_extensions.size());
    const auto create_info = [&](const void* next) -> VkInstanceCreateInfo
    {
        return {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = next,
            .flags = flags,
            .pApplicationInfo = &application_info,
            .enabledLayerCount = enabled_layer_count,
            .ppEnabledLayerNames = enabled_layers.data(),
            .enabledExtensionCount = enabled_extension_count,
            .ppEnabledExtensionNames = enabled_extensions.data(),
        };
    };
    std::optional<VkDebugUtilsMessengerCreateInfoEXT> dbg_ci{};
    if (cfg.dbg_messenger) dbg_ci = vk_dbg_messenger_create_info(*cfg.dbg_messenger);

    const auto instance_ci = create_info(dbg_ci ? &*dbg_ci : nullptr);
    DANS_VK_CHECK(vkCreateInstance(&instance_ci, nullptr, &instance_));

    if (dbg_ci) debug_messenger_ = std::make_unique<DebugMessenger>(&instance_, *dbg_ci);
}
def VulkanContext::to_string(bool include_name) const -> std::string
{
    const auto body = std::format(
        "instance={},required_layers=[{}],required_extensions=[{}]",
        instance_ != VK_NULL_HANDLE,
        str::join(required_layers_, ", "),
        str::join(required_extensions_, ", ")
    );
    return with_name("VulkanContext", body, include_name);
}
}  // namespace dans::vk
