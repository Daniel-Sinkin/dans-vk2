// src/vk/vk_context.hpp
#pragma once
// Internals
#include <dans/chrono.hpp>
#include <dans/dev.hpp>
#include <vk/vk_config.hpp>
// Externals
#include <dans/development_markers.hpp>
#include <dans/types.hpp>
#include <vulkan/vulkan.h>
// StdLib
#include <cassert>
#include <memory>
#include <print>
#include <string>
#include <utility>
#include <vector>
//
namespace dans::vk
{
class DebugMessenger
{
  public:
    DebugMessenger(VkInstance* instance, const VkDebugUtilsMessengerCreateInfoEXT& create_info);
    ~DebugMessenger();
    DebugMessenger(const DebugMessenger&) = delete;
    DebugMessenger(DebugMessenger&&) = delete;
    def operator=(const DebugMessenger&)->DebugMessenger& = delete;
    def operator=(DebugMessenger&&)->DebugMessenger& = delete;

  private:
    VkDebugUtilsMessengerEXT vk_messenger_{VK_NULL_HANDLE};
    VkInstance* vk_instance_{VK_NULL_HANDLE};
};

class VulkanContext
{
  public:
    explicit VulkanContext(VulkanCfg cfg);
    ~VulkanContext();
    VulkanContext(const VulkanContext&) = delete;
    def operator=(const VulkanContext&)->VulkanContext& = delete;
    VulkanContext(VulkanContext&&) = delete;
    def operator=(VulkanContext&&)->VulkanContext& = delete;

    [[nodiscard]] def to_string(bool include_name = false) const -> std::string;

    def init_physical_device() -> void
    {
        DANS_SCOPE_TIMER("init_physical_device");
        u32 count{};
        vkEnumeratePhysicalDevices(instance_, &count, nullptr);
        std::vector<VkPhysicalDevice> physical_devices(count);
        vkEnumeratePhysicalDevices(instance_, &count, physical_devices.data());
        assert(physical_devices.size() == count);

        if (physical_devices.empty()) DANS_PANIC("Found no compatible physical device(s)");
        std::println("Found {} Vulkan compatible physical device(s).", physical_devices.size());
        physical_device_ = physical_devices[0];

        auto props = new VkPhysicalDeviceProperties2{};
        DANS_DEFER([&props] { delete props; });
        props->sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

        for (auto& physical_device : physical_devices)
        {
            vkGetPhysicalDeviceProperties2(physical_device, props);
            const auto& x = props->properties;
            std::println(
                "apiVersion={},driverVersion={},deviceID={},deviceType={},deviceName={}",
                x.apiVersion,
                x.driverVersion,
                x.deviceID,
                std::to_underlying(x.deviceType),
                std::string{x.deviceName}
            );
        }
    }

  private:
    def query_vulkan() -> void;
    [[nodiscard]] static def create_app_info(const ApplicationCfg& cfg) -> VkApplicationInfo;
    def vk_create_instance(const CreateInstanceCfg& cfg) -> void;

    VkInstance instance_{VK_NULL_HANDLE};
    std::unique_ptr<DebugMessenger> debug_messenger_{};

    [[maybe_unused]] VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
    std::unique_ptr<VkPhysicalDeviceProperties> physical_device_properties_{};

    std::vector<std::string> available_extensions_{};
    std::vector<std::string> required_extensions_{};
    std::vector<std::string> available_layers_{};
    std::vector<std::string> required_layers_{};
};
}  // namespace dans::vk
