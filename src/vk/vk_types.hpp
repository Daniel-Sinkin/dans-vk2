#pragma once

#include <dans/development_markers.hpp>
#include <dans/format.hpp>
#include <dans/types.hpp>
#include <vulkan/vulkan.h>

namespace dans::vk
{
enum class VKResultCode
{
    success = VK_SUCCESS,
    not_ready = VK_NOT_READY,
    timeout = VK_TIMEOUT,
    event_set = VK_EVENT_SET,
    event_reset = VK_EVENT_RESET,
    incomplete = VK_INCOMPLETE,
    error_out_of_host_memory = VK_ERROR_OUT_OF_HOST_MEMORY,
    error_out_of_device_memory = VK_ERROR_OUT_OF_DEVICE_MEMORY,
    error_initialization_failed = VK_ERROR_INITIALIZATION_FAILED,
    error_device_lost = VK_ERROR_DEVICE_LOST,
    error_memory_map_failed = VK_ERROR_MEMORY_MAP_FAILED,
    error_layer_not_present = VK_ERROR_LAYER_NOT_PRESENT,
    error_extension_not_present = VK_ERROR_EXTENSION_NOT_PRESENT,
    error_feature_not_present = VK_ERROR_FEATURE_NOT_PRESENT,
    error_incompatible_driver = VK_ERROR_INCOMPATIBLE_DRIVER,
    error_too_many_objects = VK_ERROR_TOO_MANY_OBJECTS,
    error_format_not_supported = VK_ERROR_FORMAT_NOT_SUPPORTED,
    error_fragmented_pool = VK_ERROR_FRAGMENTED_POOL,
    error_unknown = VK_ERROR_UNKNOWN,
    error_validation_failed = VK_ERROR_VALIDATION_FAILED,
    error_out_of_pool_memory = VK_ERROR_OUT_OF_POOL_MEMORY,
    error_invalid_external_handle = VK_ERROR_INVALID_EXTERNAL_HANDLE,
    error_invalid_opaque_capture_address = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,
    error_fragmentation = VK_ERROR_FRAGMENTATION,
    pipeline_compile_required = VK_PIPELINE_COMPILE_REQUIRED,
    error_not_permitted = VK_ERROR_NOT_PERMITTED,
    error_surface_lost_khr = VK_ERROR_SURFACE_LOST_KHR,
    error_native_window_in_use_khr = VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,
    suboptimal_khr = VK_SUBOPTIMAL_KHR,
    error_out_of_date_khr = VK_ERROR_OUT_OF_DATE_KHR,
    error_incompatible_display_khr = VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,
    error_invalid_shader_nv = VK_ERROR_INVALID_SHADER_NV,
    error_image_usage_not_supported_khr = VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR,
    error_video_picture_layout_not_supported_khr = VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR,
    error_video_profile_operation_not_supported_khr =
        VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR,
    error_video_profile_format_not_supported_khr = VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR,
    error_video_profile_codec_not_supported_khr = VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR,
    error_video_std_version_not_supported_khr = VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR,
    error_invalid_drm_format_modifier_plane_layout_ext =
        VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT,
    error_present_timing_queue_full_ext = VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT,
    error_full_screen_exclusive_mode_lost_ext = VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,
    thread_idle_khr = VK_THREAD_IDLE_KHR,
    thread_done_khr = VK_THREAD_DONE_KHR,
    operation_deferred_khr = VK_OPERATION_DEFERRED_KHR,
    operation_not_deferred_khr = VK_OPERATION_NOT_DEFERRED_KHR,
    error_invalid_video_std_parameters_khr = VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR,
    error_compression_exhausted_ext = VK_ERROR_COMPRESSION_EXHAUSTED_EXT,
    incompatible_shader_binary_ext = VK_INCOMPATIBLE_SHADER_BINARY_EXT,
    pipeline_binary_missing_khr = VK_PIPELINE_BINARY_MISSING_KHR,
    error_not_enough_space_khr = VK_ERROR_NOT_ENOUGH_SPACE_KHR,
    error_validation_failed_ext = VK_ERROR_VALIDATION_FAILED_EXT,
    error_out_of_pool_memory_khr = VK_ERROR_OUT_OF_POOL_MEMORY_KHR,
    error_invalid_external_handle_khr = VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR,
    error_fragmentation_ext = VK_ERROR_FRAGMENTATION_EXT,
    error_not_permitted_ext = VK_ERROR_NOT_PERMITTED_EXT,
    error_not_permitted_khr = VK_ERROR_NOT_PERMITTED_KHR,
    error_invalid_device_address_ext = VK_ERROR_INVALID_DEVICE_ADDRESS_EXT,
    error_invalid_opaque_capture_address_khr = VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR,
    pipeline_compile_required_ext = VK_PIPELINE_COMPILE_REQUIRED_EXT,
    error_pipeline_compile_required_ext = VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT,
    error_incompatible_shader_binary_ext = VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT,
    result_max_enum = VK_RESULT_MAX_ENUM,
};
[[nodiscard]] def to_string(VKResultCode result) -> std::string_view
{
    using enum VKResultCode;
    // clang-format off
    switch(result) {
        case success: return "success";
        case not_ready: return "not_ready";
        case timeout: return "timeout";
        case event_set: return "event_set";
        case event_reset: return "event_reset";
        case incomplete: return "incomplete";
        case error_out_of_host_memory: return "error_out_of_host_memory";
        case error_out_of_device_memory: return "error_out_of_device_memory";
        case error_initialization_failed: return "error_initialization_failed";
        case error_device_lost: return "error_device_lost";
        case error_memory_map_failed: return "error_memory_map_failed";
        case error_layer_not_present: return "error_layer_not_present";
        case error_extension_not_present: return "error_extension_not_present";
        case error_feature_not_present: return "error_feature_not_present";
        case error_incompatible_driver: return "error_incompatible_driver";
        case error_too_many_objects: return "error_too_many_objects";
        case error_format_not_supported: return "error_format_not_supported";
        case error_fragmented_pool: return "error_fragmented_pool";
        case error_unknown: return "error_unknown";
        case error_validation_failed: return "error_validation_failed";
        case error_out_of_pool_memory: return "error_out_of_pool_memory";
        case error_invalid_external_handle: return "error_invalid_external_handle";
        case error_invalid_opaque_capture_address: return "error_invalid_opaque_capture_address";
        case error_fragmentation: return "error_fragmentation";
        case pipeline_compile_required: return "pipeline_compile_required";
        case error_not_permitted: return "error_not_permitted";
        case error_surface_lost_khr: return "error_surface_lost_khr";
        case error_native_window_in_use_khr: return "error_native_window_in_use_khr";
        case suboptimal_khr: return "suboptimal_khr";
        case error_out_of_date_khr: return "error_out_of_date_khr";
        case error_incompatible_display_khr: return "error_incompatible_display_khr";
        case error_invalid_shader_nv: return "error_invalid_shader_nv";
        case error_image_usage_not_supported_khr: return "error_image_usage_not_supported_khr";
        case error_video_picture_layout_not_supported_khr: return "error_video_picture_layout_not_supported_khr";
        case error_video_profile_operation_not_supported_khr: return "error_video_profile_operation_not_supported_khr";
        case error_video_profile_format_not_supported_khr: return "error_video_profile_format_not_supported_khr";
        case error_video_profile_codec_not_supported_khr: return "error_video_profile_codec_not_supported_khr";
        case error_video_std_version_not_supported_khr: return "error_video_std_version_not_supported_khr";
        case error_invalid_drm_format_modifier_plane_layout_ext: return "error_invalid_drm_format_modifier_plane_layout_ext";
        case error_present_timing_queue_full_ext: return "error_present_timing_queue_full_ext";
        case error_full_screen_exclusive_mode_lost_ext: return "error_full_screen_exclusive_mode_lost_ext";
        case thread_idle_khr: return "thread_idle_khr";
        case thread_done_khr: return "thread_done_khr";
        case operation_deferred_khr: return "operation_deferred_khr";
        case operation_not_deferred_khr: return "operation_not_deferred_khr";
        case error_invalid_video_std_parameters_khr: return "error_invalid_video_std_parameters_khr";
        case error_compression_exhausted_ext: return "error_compression_exhausted_ext";
        case incompatible_shader_binary_ext: return "incompatible_shader_binary_ext";
        case pipeline_binary_missing_khr: return "pipeline_binary_missing_khr";
        case error_not_enough_space_khr: return "error_not_enough_space_khr";
        case result_max_enum: return "result_max_enum";
    }
    // clang-format on
    std::unreachable();
}

[[nodiscard]] def is_valid(VKResultCode result) noexcept -> bool
{
    return result == VKResultCode::success;
}

struct VKResult
{
  public:
    VKResult(VKResultCode code) : code_(code)
    {
    }
    VKResult(VkResult res) : code_(VKResultCode{res})
    {
    }

    [[nodiscard]] def is_valid() const noexcept -> bool
    {
        return code_ == VKResultCode::success;
    }
    [[nodiscard]] explicit operator bool() const noexcept
    {
        return is_valid();
    }
    [[nodiscard]] def get() const noexcept -> VKResultCode
    {
        return code_;
    }
    [[nodiscard]] def get_underlying() const noexcept -> i32
    {
        return static_cast<i32>(std::to_underlying(code_));
    }

  private:
    VKResultCode code_{};
};
[[nodiscard]] inline def to_string(const VKResult&) -> std::string;

[[nodiscard]] def is_valid(const VKResult& res) noexcept -> bool
{
    return res.is_valid();
}
[[maybe_unused]] constexpr std::array k_error_codes_create_instance = {
    VKResultCode::error_extension_not_present,
    VKResultCode::error_incompatible_driver,
    VKResultCode::error_initialization_failed,
    VKResultCode::error_layer_not_present,
    VKResultCode::error_out_of_device_memory,
    VKResultCode::error_out_of_host_memory,
    VKResultCode::error_unknown,
    VKResultCode::error_validation_failed
};
[[maybe_unused]] constexpr std::array k_error_codes_create_debug_messenger = {
    VKResultCode::error_out_of_host_memory,
    VKResultCode::error_unknown,
    VKResultCode::error_validation_failed
};
}  // namespace dans::vk

DANS_FORMAT_WITH_TO_STRING(dans::vk::VKResult)

namespace dans::vk
{
[[nodiscard]] inline def to_string(const VKResult& res) -> std::string
{
    return std::format("VKResult({})", to_string(res.get()));
}

}  // namespace dans::vk
