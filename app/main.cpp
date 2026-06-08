// app/main.cpp
// Internals
#include <dans/dev.hpp>
#include <dans/development_markers.hpp>
#include <dans/strings.hpp>
#include <dans/types.hpp>
#include <vk/pch.hpp>
#include <vk/vk_types.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
// StdLib
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <format>
#include <print>
#include <source_location>
#include <stdexcept>
#include <string_view>
#include <vector>
//
namespace
{
using namespace dans;

constexpr bool k_enable_validation{true};
constexpr const char* k_validation_layer{"VK_LAYER_KHRONOS_validation"};

static constexpr const char* k_fpn_create_debug_messenger{"vkCreateDebugUtilsMessengerEXT"};
static constexpr const char* k_fpn_destroy_debug_messenger{"vkDestroyDebugUtilsMessengerEXT"};

void glfw_error_callback(int code, const char* description)
{
    std::println(stderr, "[glfw] error {}: {}", code, description);
}

using MessageSeverity = VkDebugUtilsMessageSeverityFlagBitsEXT;
static constexpr std::array<MessageSeverity, 4> k_severities{{
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
}};

using MessageSeverityFlags = VkDebugUtilsMessageSeverityFlagsEXT;
using MessageType = VkDebugUtilsMessageTypeFlagsEXT;
using MessengerCallbackData = VkDebugUtilsMessengerCallbackDataEXT;
constexpr auto k_severity_threshold{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT};
[[nodiscard]] auto high_severity(MessageSeverity severity) -> bool
{
    return severity >= k_severity_threshold;
}

VKAPI_ATTR def VKAPI_CALL debug_callback(
    MessageSeverity severity,
    [[maybe_unused]] MessageType type,
    const MessengerCallbackData* data,
    [[maybe_unused]] void* user_data
) -> VkBool32
{
    std::println(high_severity(severity) ? stderr : stdout, "[vulkan] {}", data->pMessage);
    return VK_FALSE;
}

auto debug_messenger_create_info() -> VkDebugUtilsMessengerCreateInfoEXT
{
    const auto message_severities = []() -> VkDebugUtilsMessageSeverityFlagsEXT
    {
        MessageSeverityFlags out{0};
        for (const auto severity : k_severities)
        {
            if (k_severity_threshold <= severity) out |= severity;
        }
        return out;
    }();
    assert(message_severities != 0);
    return {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = VkFlags{0},
        .messageSeverity = message_severities,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debug_callback,
        .pUserData = nullptr,
    };
}

auto validation_layer_available() -> bool
{
    u32 count{};
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> layers(count);
    vkEnumerateInstanceLayerProperties(&count, layers.data());
    return std::ranges::any_of(
        layers,
        [](const auto& layer) noexcept
        { return std::string_view{layer.layerName} == k_validation_layer; }
    );
}

auto required_instance_extensions() -> std::vector<const char*>
{
    u32 glfw_count{0};
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_count);
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_count);
    if (k_enable_validation) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return extensions;
}
}  // namespace

namespace dans::vk
{
[[nodiscard]] constexpr def make_api_version(u32 variant, u32 major, u32 minor, u32 patch) -> u32
{
    return static_cast<u32>(VK_MAKE_API_VERSION(variant, major, minor, patch));
}

constexpr std::string_view k_application_name{"dans-vk2"};
constexpr u32 k_application_version{make_api_version(0u, 0u, 1u, 0u)};
constexpr std::string_view k_engine_name{"dans-vk2"};
constexpr u32 k_engine_version{make_api_version(0u, 0u, 1u, 0u)};

constexpr u32 k_vk_api_version{make_api_version(0, 1, 4, 0)};

[[nodiscard]] def create_app_info() -> VkApplicationInfo
{
    return {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = k_application_name.data(),
        .applicationVersion = k_application_version,
        .pEngineName = k_engine_name.data(),
        .engineVersion = k_engine_version,
        .apiVersion = k_vk_api_version
    };
}

struct CreateInstanceConfig
{
    const void* next{};
    bool enumerate_portability = false;
    std::span<const char* const> enabled_layers;
    std::span<const char* const> enabled_extensions;
};
[[maybe_unused]] [[nodiscard]] def to_string(const CreateInstanceConfig& cfg) -> std::string
{
    return std::format(
        "next={},enumerate_portability={},enabled_layers=[{}],enabled_extensions=[{}]",
        (cfg.next) ? "<Exists>" : "<NULL>",
        (cfg.enumerate_portability) ? "true" : "false",
        dans::str::join(cfg.enabled_layers, ", "),
        dans::str::join(cfg.enabled_extensions, ", ")
    );
}
auto check_vk_result(
    const VkResult result, const std::source_location loc = std::source_location::current()
) -> void
{
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error(
            std::format(
                "Vulkan call failed with {} at {}:{} in {}",
                to_string(result),
                loc.file_name(),
                loc.line(),
                loc.function_name()
            )
        );
    }
}

[[nodiscard]] def create_instance(const CreateInstanceConfig& cfg) -> VkInstance
{
    const auto port_flag = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    const auto app_info = create_app_info();
    const VkInstanceCreateInfo instance_ci{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = cfg.next,
        .flags = (cfg.enumerate_portability) ? port_flag : 0u,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<u32>(cfg.enabled_layers.size()),
        .ppEnabledLayerNames = (cfg.enabled_layers.empty()) ? nullptr : cfg.enabled_layers.data(),
        .enabledExtensionCount = static_cast<u32>(cfg.enabled_extensions.size()),
        .ppEnabledExtensionNames = cfg.enabled_extensions.data(),
    };
    VkInstance instance{};
    check_vk_result(vkCreateInstance(&instance_ci, nullptr, &instance));
    return instance;
}

[[nodiscard]] def create_debug_messenger(const VkInstance& instance) -> VkDebugUtilsMessengerEXT
{
    assert(k_enable_validation);
    auto* const create = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, k_fpn_create_debug_messenger)
    );
    if (not create)
    {
        throw std::runtime_error("failed to load vkCreateDebugUtilsMessengerEXT");
    }
    const auto messenger_ci = debug_messenger_create_info();
    VkDebugUtilsMessengerEXT messenger{};
    check_vk_result(create(instance, &messenger_ci, nullptr, &messenger));
    return messenger;
}
def destroy_debug_messenger(const VkInstance& instance, VkDebugUtilsMessengerEXT& messenger) -> void
{
    if (messenger == VK_NULL_HANDLE) return;
    auto* const destroy = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, k_fpn_destroy_debug_messenger)
    );
    if (destroy != nullptr) destroy(instance, messenger, nullptr);
}

}  // namespace dans::vk

namespace
{
auto run() -> int
{
    using namespace dans::vk;
    std::println("Hello ds-vk2!");
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        std::println("Failed to initialize GLFW!");
        return EXIT_FAILURE;
    }
    DANS_DEFER([] { glfwTerminate(); });

    if (!glfwVulkanSupported())
    {
        std::println("Vulkan is not supported!");
        return EXIT_FAILURE;
    }
    std::println("Vulkan is supported!");

    if (k_enable_validation && !validation_layer_available())
    {
        std::println("validation requested but {} is unavailable", k_validation_layer);
        return EXIT_FAILURE;
    }

    const auto extensions = required_instance_extensions();
    const std::array layers = {k_validation_layer};
    const auto messenger_ci = debug_messenger_create_info();
    const auto instance = create_instance(
        CreateInstanceConfig{
            .next = (k_enable_validation) ? &messenger_ci : nullptr,
            .enumerate_portability = false,
            .enabled_layers = layers,
            .enabled_extensions = extensions,
        }
    );
    DANS_DEFER([&] { vkDestroyInstance(instance, nullptr); });

    auto messenger = create_debug_messenger(instance);
    DANS_DEFER([&] { destroy_debug_messenger(instance, messenger); });
    std::println("instance + validation layer are up.");
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
