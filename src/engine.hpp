// src/engine.hpp
#pragma once
// Internals
#include <dans/to_string.hpp>
#include <platform/platform.hpp>
#include <vk/vk_context.hpp>
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <format>
#include <memory>
#include <string>
//
namespace dans
{
struct EngineCfg
{
    platform::PlatformCfg platform{};
    vk::VulkanCfg vk{};
};

class EngineContext
{
  public:
    EngineContext(EngineCfg cfg)
        : platform_{std::make_unique<platform::PlatformContext>(std::move(cfg.platform))},
          vk_{std::make_unique<vk::VulkanContext>(std::move(cfg.vk))}
    {
    }

    ~EngineContext()
    {
        vk_.reset();
        platform_.reset();
    }

    [[nodiscard]] def platform() -> platform::PlatformContext*
    {
        return platform_.get();
    }
    [[nodiscard]] def platform() const -> const platform::PlatformContext*
    {
        return platform_.get();
    }
    [[nodiscard]] def vk() -> vk::VulkanContext*
    {
        return vk_.get();
    }
    [[nodiscard]] def vk() const -> const vk::VulkanContext*
    {
        return vk_.get();
    }

    [[nodiscard]] def to_string(bool include_name = false) const -> std::string
    {
        const auto body = std::format(
            "platform={},vk={}",
            platform_ ? platform_->to_string(true) : std::string{"none"},
            vk_ ? vk_->to_string(true) : std::string{"none"}
        );
        return with_name("EngineContext", body, include_name);
    }

  private:
    std::unique_ptr<platform::PlatformContext> platform_{};
    std::unique_ptr<vk::VulkanContext> vk_{};
};
}  // namespace dans
