// app/main.cpp
// Internals
#include <dans/dev.hpp>
#include <dans/development_markers.hpp>
#include <dans/fix_str.hpp>
#include <dans/logic.hpp>
#include <dans/strings.hpp>
#include <dans/types.hpp>
//
#include <vk/pch.hpp>
#include <vk/vk_types.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
// StdLib
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <format>
#include <memory>
#include <optional>
#include <print>
#include <ranges>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
//
namespace vk_message_severity
{
using Flag = VkDebugUtilsMessageSeverityFlagBitsEXT;
constexpr Flag verbose{VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT};
constexpr Flag info{VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT};
constexpr Flag warn{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT};
constexpr Flag error{VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT};

static constexpr std::array<Flag, 4> flags = {verbose, info, warn, error};
constexpr dans::usize k_count{flags.size()};

[[nodiscard]] static def to_string(Flag severity) -> std::string_view
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

namespace
{
using namespace dans;

constexpr CZString k_validation_layer{"VK_LAYER_KHRONOS_validation"};

auto glfw_error_callback(int code, const char* description) -> void
{
    std::println(stderr, "[glfw] error {}: {}", code, description);
}
constexpr std::array<std::string_view, vk_message_severity::k_count> k_severity_log_markers{{
    "ERRO",
    "WARN",
    "INFO",
    "VERB",
}};

using MessageSeverityFlags = VkDebugUtilsMessageSeverityFlagsEXT;
using MessageType = VkDebugUtilsMessageTypeFlagsEXT;
using MessengerCallbackData = VkDebugUtilsMessengerCallbackDataEXT;
constexpr auto k_severity_threshold{VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT};
[[nodiscard]] auto high_severity(vk_message_severity::Flag severity) -> bool
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
    namespace sv = std::ranges::views;
    const auto& flags = vk_message_severity::flags;
    const auto& markers = k_severity_log_markers;
    for (const auto [sev, mrk] : sv::zip(flags, markers))
    {
        if (sev != severity) continue;
        std::println(
            high_severity(severity) ? stderr : stdout, "[VULK][{}] {}", mrk, data->pMessage
        );
        return VK_FALSE;
    }
    DANS_PANIC_FMT("Failed to match severity {}", vk_message_severity::to_string(severity));
}

}  // namespace

namespace dans::vk
{
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
[[maybe_unused]] [[nodiscard]] def to_string(const ApiVersion& api) -> std::string;
[[nodiscard]] def is_valid(const ApiVersion& api) -> bool
{
    if (api.major == 0 and api.minor == 0)
    {
        std::println("major=0,minor=1 is the smalled valid api version, got {}", to_string(api));
        return false;
    }
    return true;
}

struct ApplicationCfg
{
    CZString app_name{"dans-vk2"};
    ApiVersion app_version{ApiVersion{}};
    CZString engine_name{"dans-vk2"};
    ApiVersion engine_version{ApiVersion{}};
    ApiVersion api_version{ApiVersion::vulkan()};
};
[[maybe_unused]] [[nodiscard]] def to_string(const ApplicationCfg& cfg) -> std::string;
[[nodiscard]] def is_valid(const ApplicationCfg& cfg) -> bool
{
    if (not is_valid(cfg.app_version) or not is_valid(cfg.engine_version)
        or not is_valid(cfg.api_version))
    {
        return false;
    }
    if (cfg.api_version.major == 0)
    {
        std::println("Can't have Vulkan major version below 1");
        return false;
    }
    if (cfg.api_version.major == 1 and cfg.api_version.minor < 4)
    {
        std::println("Vulkan is at least 1.4");
        return false;
    }
    return true;
}

enum class MessageSeverity : u8
{
    verbose = 0,
    info = 1,
    warn = 2,
    error = 3
};
[[nodiscard]] def to_string(MessageSeverity message_severity)
{
    using enum MessageSeverity;
    switch (message_severity)
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
}
struct DebugMessengerEnabledMessagesCfg
{
    bool general{true};
    bool validation{true};
    bool performance{true};
    bool device_address_binding{false};
};
[[nodiscard]] def to_string(const DebugMessengerEnabledMessagesCfg& cfg) -> std::string;
[[nodiscard]] def is_valid(const DebugMessengerEnabledMessagesCfg& cfg) -> bool
{
    if (not(cfg.general || cfg.validation || cfg.performance || cfg.device_address_binding))
    {
        std::println("No message type enabeld in DebugMessenger");
    }
    return true;
}
struct DebugMessengerCfg
{
    MessageSeverity severity_threshold{MessageSeverity::warn};

    VkFlags flags{};
    DebugMessengerEnabledMessagesCfg enabled_messages{};
};
[[nodiscard]] def to_string(const DebugMessengerCfg& cfg) -> std::string;
[[nodiscard]] def is_valid(const DebugMessengerCfg& cfg) -> bool
{
    if (not is_valid(cfg.enabled_messages)) return false;
    if (cfg.flags != VkFlags{0})
    {
        std::println("DebugMessenger must not have any flags enabled.");
        return false;
    }
    if (not is_valid(cfg.enabled_messages)) return false;
    return true;
}
struct CreateInstanceCfg
{
    std::span<std::string> layers{};
    std::span<std::string> extensions{};
    ApplicationCfg app{};
    bool enumerate_portability{};
    std::optional<DebugMessengerCfg> dbg_messenger{};
};
[[nodiscard]] def to_string(const CreateInstanceCfg& cfg) -> std::string;
[[nodiscard]] def is_valid(const CreateInstanceCfg& cfg) -> bool
{
    if (not is_valid(cfg.app)) return false;
    if (cfg.dbg_messenger and not std::ranges::contains(cfg.layers, k_validation_layer))
    {
        std::println("Have DebugMessengerCfg set but no validation layer enabled");
        return false;
    }
    return true;
}

struct VulkanCfg
{
    std::optional<DebugMessengerCfg> dbg_messenger{};
    bool use_glfw{false};
    std::vector<CZString> layers{};
    std::vector<CZString> extensions{};
    bool enumerate_portability{false};
    ApplicationCfg app{};
};
[[nodiscard]] def is_valid(const VulkanCfg& cfg) -> bool
{
    if (cfg.dbg_messenger and not is_valid(*cfg.dbg_messenger)) return false;
    if (not is_valid(cfg.app)) return false;
    return true;
}
[[nodiscard]] def to_string(const VulkanCfg& cfg) -> std::string;
}  // namespace dans::vk

DANS_FORMAT_WITH_TO_STRING(dans::vk::CreateInstanceCfg)
DANS_FORMAT_WITH_TO_STRING(dans::vk::ApiVersion)
DANS_FORMAT_WITH_TO_STRING(dans::vk::ApplicationCfg)
// DANS_FORMAT_WITH_TO_STRING(dans::vk::DebugMessengerCfg) // TODO
// DANS_FORMAT_WITH_TO_STRING(dans::vk::DebugMessengerEnabledMessagesCfg) // TODO
DANS_FORMAT_WITH_TO_STRING(dans::vk::VulkanCfg)

namespace dans::vk
{
[[maybe_unused]] [[nodiscard]] def to_string(const ApiVersion& api) -> std::string
{
    return std::format("{}.{}.{} [{}]", api.major, api.minor, api.patch, api.variant);
}
[[maybe_unused]] [[nodiscard]] def to_string(const ApplicationCfg& cfg) -> std::string
{
    return std::format(
        "ApplicationCfg(app_name={},app_version={},engine_name={},engine_version={},api_version={}"
        ")",
        cfg.app_name,
        cfg.app_version,
        cfg.engine_name,
        cfg.engine_version,
        cfg.api_version
    );
}
[[nodiscard]] def to_string(const CreateInstanceCfg& cfg) -> std::string
{
    return std::format(
        "CreateInstanceCfg(enabled_layers=[{}],enabled_extensions=[{}]"
        ")",
        dans::str::join(cfg.layers, ", "),
        dans::str::join(cfg.extensions, ", ")
    );
}
[[nodiscard]] def to_string(const DebugMessengerEnabledMessagesCfg& cfg) -> std::string
{
    return std::format(
        "general={},validation={},performance={},device_address_binding={}",
        cfg.general,
        cfg.validation,
        cfg.performance,
        cfg.device_address_binding
    );
}
[[nodiscard]] def to_string(const DebugMessengerCfg& cfg) -> std::string
{
    return std::format(
        "severity_threshold={},flags={},enabled_messages={}",
        to_string(cfg.severity_threshold),
        cfg.flags,
        to_string(cfg.enabled_messages)
    );
}
[[nodiscard]] def to_string(const VulkanCfg& cfg) -> std::string
{
    return std::format(
        "VulkanCfg(validation={},use_glfw={},layers=[{}],extensions=[{}],enumerate_portability={},"
        "app={})",
        (cfg.dbg_messenger) ? to_string(*cfg.dbg_messenger) : "{}",
        cfg.use_glfw,
        dans::str::join(cfg.layers, ","),
        dans::str::join(cfg.extensions, ","),
        cfg.enumerate_portability,
        to_string(cfg.app)
    );
}
template <typename T>
[[nodiscard]] def data_or(std::span<const T> span, const T* alternative = nullptr) -> const T*
{
    if (span.empty()) return alternative;
    return span.data();
}
template <typename T>
[[nodiscard]] def data_or(std::span<T> span, T* alternative) -> T*
{
    if (span.empty()) return alternative;
    return span.data();
}
}  // namespace dans::vk

namespace dans::vk
{

struct WindowConfig
{
    u32 width{800};
    u32 height{600};
    CZString title{"Vulkan"};
};
struct WindowHint
{
    int hint{};
    int value{};
};
struct PlatformCfg
{
    std::vector<WindowHint> window_hints{};
    bool use_opengl{false};
    bool use_vulkan{false};
    WindowConfig window{};
};
[[nodiscard]] def is_valid(const PlatformCfg& cfg) -> bool
{
    if (cfg.use_opengl)
    {
        std::println("OpenGL is not supported");
        return false;
    }
    if (not cfg.use_vulkan)
    {
        std::println("This project needs to use Vulkan");
        return false;
    }
    if (not(cfg.use_opengl and cfg.use_vulkan))
    {
        std::println("Vulkan or OpenGl must be active");
        return false;
    }
    if (cfg.use_vulkan)
    {
        using std::ranges::find;
        const auto res = find(cfg.window_hints, GLFW_CLIENT_API, [](auto&& wh) { return wh.hint; });
        if (res == cfg.window_hints.end())
        {
            std::println("GLFW_CLIENT_API was not modified at all");
            return false;
        }
        if (res->value != GLFW_NO_API)
        {
            std::println(
                "GLFW_CLIENT_API was modified to be {} but should be GLFW_NO_API ({})",
                res->value,
                GLFW_NO_API
            );
            return false;
        }
    }
    return true;
}
struct GlfwWindowDeleter
{
    def operator()(GLFWwindow* window) const noexcept -> void
    {
        glfwDestroyWindow(window);
    }
};
class PlatformContext
{
  public:
    PlatformContext(PlatformCfg cfg) : cfg_{std::move(cfg)}
    {
        using std::ranges::for_each;

        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) DANS_PANIC("Failed to initialize GLFW!");

        if (cfg.use_vulkan and not glfwVulkanSupported()) DANS_PANIC("Vulkan is not supported!");

        for_each(cfg_.window_hints, [](auto&& wh) { glfwWindowHint(wh.hint, wh.value); });

        const auto [width, height, title] = cfg.window;
        auto* window = glfwCreateWindow(
            static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr
        );
        if (not window) DANS_PANIC_FMT("Failed to create window!");
        assert(not window_);
        window_.reset(window);
    }
    ~PlatformContext()
    {
        window_.reset();
        glfwTerminate();
    }
    def operator=(PlatformContext&&)->PlatformContext& = delete;
    PlatformContext(PlatformContext&&) noexcept = delete;
    PlatformContext(const PlatformContext&) = delete;
    def operator=(const PlatformContext&)->PlatformContext& = delete;

    [[nodiscard]] def should_close() const noexcept -> bool
    {
        assert(window_);
        return glfwWindowShouldClose(window_.get()) == 0;
    }

  private:
    std::unique_ptr<GLFWwindow, GlfwWindowDeleter> window_{};

    PlatformCfg cfg_{};
};

auto vk_dbg_messenger_create_info(DebugMessengerCfg cfg) -> VkDebugUtilsMessengerCreateInfoEXT
{
    using MS = MessageSeverity;
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

class DebugMessenger
{
  public:
    DebugMessenger(VkInstance* instance, const VkDebugUtilsMessengerCreateInfoEXT& create_info)
        : vk_instance_{instance}
    {
        assert(instance);
        constexpr CZString k_proc_name{"vkCreateDebugUtilsMessengerEXT"};
        auto* const create = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(*vk_instance_, k_proc_name)
        );
        if (not create) DANS_PANIC_FMT("failed to load {}", k_proc_name);

        DANS_VK_CHECK(create(*vk_instance_, &create_info, nullptr, &vk_messenger_));
    }
    ~DebugMessenger()
    {
        assert(vk_instance_);
        constexpr CZString k_proc_name{"vkDestroyDebugUtilsMessengerEXT"};
        auto* const destroy = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(*vk_instance_, k_proc_name)
        );
        if (not destroy) DANS_PANIC_FMT("failed to load {}", k_proc_name);
        destroy(*vk_instance_, vk_messenger_, nullptr);
    }

  private:
    VkDebugUtilsMessengerEXT vk_messenger_{VK_NULL_HANDLE};
    VkInstance* vk_instance_{VK_NULL_HANDLE};
};

struct VulkanContext
{
  public:
    VulkanContext(VulkanCfg cfg)
    {
        using std::ranges::any_of;
        using std::ranges::contains;
        using std::ranges::for_each;

        query_vulkan();

        const auto insert_extension = [&](auto&& e) { required_extensions_.emplace_back(e); };
        const auto insert_layer = [&](auto&& l) { required_layers_.emplace_back(l); };

        for_each(cfg.extensions, insert_extension);
        for_each(cfg.layers, insert_layer);

        if (cfg.use_glfw)
        {
            u32 glfw_count{0};
            CZString* glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_count);
            for (auto i = 0zu; i < glfw_count; ++i)
            {
                insert_extension(glfw_extensions[i]);
            }
        }
        if (cfg.dbg_messenger)
        {
            insert_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            insert_layer(k_validation_layer);
        }

        const auto is_not_subset = [](auto&& as, auto&& bs)
        {
            const auto not_contained_in_bs = [&bs](auto&& a) { return not contains(bs, a); };
            return any_of(as, not_contained_in_bs);
        };
        if (is_not_subset(required_extensions_, availiable_extensions_))
        {
            DANS_PANIC("At least one required extension is not availiable");
        }
        if (is_not_subset(required_layers_, availiable_layers_))
        {
            DANS_PANIC("At least one required layer is not availiable");
        }
        vk_create_instance(
            {.layers = required_layers_,
             .extensions = required_extensions_,
             .app = cfg.app,
             .enumerate_portability = cfg.enumerate_portability,
             .dbg_messenger = cfg.dbg_messenger}
        );
        if (not instance_) DANS_PANIC("Instance not Created!");
    }

    ~VulkanContext()
    {
        debug_messenger_.reset();
        if (instance_ != VK_NULL_HANDLE) vkDestroyInstance(instance_, nullptr);
    }
    VulkanContext(const VulkanContext&) = delete;
    def operator=(const VulkanContext&)->VulkanContext& = delete;
    VulkanContext(VulkanContext&&) = delete;
    def operator=(VulkanContext&&)->VulkanContext& = delete;

    def query_vulkan() -> void
    {
        {
            u32 count{};
            vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
            std::vector<VkExtensionProperties> extensions(count);
            vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
            availiable_extensions_.reserve(count);
            for (const auto& extension : extensions)
            {
                availiable_extensions_.emplace_back(extension.extensionName);
            }
        }
        {
            u32 count{};
            vkEnumerateInstanceLayerProperties(&count, nullptr);
            std::vector<VkLayerProperties> layers(count);
            vkEnumerateInstanceLayerProperties(&count, layers.data());
            for (const auto& layer : layers)
            {
                availiable_layers_.emplace_back(layer.layerName);
            }
        }
    }

    [[nodiscard]] static def create_app_info(ApplicationCfg cfg) -> VkApplicationInfo
    {
        assert(is_valid(cfg));
        return {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = cfg.app_name,
            .applicationVersion = cfg.app_version.combined(),
            .pEngineName = cfg.engine_name,
            .engineVersion = cfg.engine_version.combined(),
            .apiVersion = cfg.api_version.combined()
        };
    }
    def vk_create_instance(const CreateInstanceCfg& cfg) -> void
    {
        assert(is_valid(cfg));
        VkFlags flags{0};
        if (cfg.enumerate_portability) flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        const auto application_info = create_app_info(cfg.app);

        const auto to_c_strings = [](std::span<const std::string> strings) -> std::vector<CZString>
        {
            std::vector<CZString> out{};
            out.reserve(strings.size());
            for (const auto& name : strings) out.emplace_back(name.c_str());
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
                .ppEnabledExtensionNames = enabled_extensions.data()
            };
        };
        std::optional<VkDebugUtilsMessengerCreateInfoEXT> dbg_ci{};
        if (cfg.dbg_messenger) dbg_ci = vk_dbg_messenger_create_info(*cfg.dbg_messenger);

        const auto instance_ci = create_info(dbg_ci ? &*dbg_ci : nullptr);
        DANS_VK_CHECK(vkCreateInstance(&instance_ci, nullptr, &instance_));

        if (dbg_ci) debug_messenger_ = std::make_unique<DebugMessenger>(&instance_, *dbg_ci);
    }

  private:
    VkInstance instance_{VK_NULL_HANDLE};
    std::unique_ptr<DebugMessenger> debug_messenger_{};

    std::vector<std::string> availiable_extensions_{};
    std::vector<std::string> required_extensions_{};

    std::vector<std::string> availiable_layers_{};
    std::vector<std::string> required_layers_{};
};

struct EngineContext
{
    std::unique_ptr<VulkanContext> vk{};
    std::unique_ptr<PlatformContext> platform{};
};
}  // namespace dans::vk

namespace
{
auto run() -> int
{
    using namespace dans::vk;
    std::println("Hello ds-vk2!");

    EngineContext engine_ctx{};

    engine_ctx.vk = std::make_unique<VulkanContext>(
        VulkanCfg{.dbg_messenger = DebugMessengerCfg{}, .use_glfw = true}
    );

    std::vector<WindowHint> window_hints{
        WindowHint{.hint = GLFW_CLIENT_API, .value = GLFW_NO_API},
        WindowHint{.hint = GLFW_RESIZABLE, .value = GLFW_FALSE},
    };
    engine_ctx.platform = std::make_unique<PlatformContext>(PlatformCfg{
        .window_hints = window_hints,
        .use_vulkan = true,
        .window = {.width = 800, .height = 600, .title = "Vulkan"}
    });

    while (engine_ctx.platform->should_close())
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
