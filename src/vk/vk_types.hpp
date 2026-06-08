#pragma once

#include <concepts>
#include <dans/dev.hpp>
#include <dans/development_markers.hpp>
#include <dans/format.hpp>
#include <dans/types.hpp>
#include <vulkan/vulkan.h>

namespace dans::vk
{

// These are the types that Vulkan uses, checking
// that my typedefs are compatible with them
static_assert(std::same_as<int8_t, i8>);
static_assert(std::same_as<int16_t, i16>);
static_assert(std::same_as<int32_t, i32>);
static_assert(std::same_as<int64_t, i64>);
static_assert(std::same_as<uint8_t, u8>);
static_assert(std::same_as<uint16_t, u16>);
static_assert(std::same_as<uint32_t, u32>);
static_assert(std::same_as<uint64_t, u64>);
static_assert(std::same_as<VkFlags, u32>);
static_assert(std::same_as<VkBool32, b32>);
static_assert(std::same_as<std::underlying_type_t<VkResult>, int>);

[[maybe_unused]] constexpr def to_string(int res) -> std::string_view
{
    // clang-format off
    switch(res) {
        case VK_SUCCESS : return "success";
        case VK_NOT_READY: return "not_ready";
        case VK_TIMEOUT : return "timeout";
        case VK_EVENT_SET: return "event_set";
        case VK_EVENT_RESET : return "event_reset";
        case VK_INCOMPLETE : return "incomplete";
        case VK_ERROR_OUT_OF_HOST_MEMORY : return "error_out_of_host_memory";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY : return "error_out_of_device_memory";
        case VK_ERROR_INITIALIZATION_FAILED : return "error_initialization_failed";
        case VK_ERROR_DEVICE_LOST : return "error_device_lost";
        case VK_ERROR_MEMORY_MAP_FAILED : return "error_memory_map_failed";
        case VK_ERROR_LAYER_NOT_PRESENT : return "error_layer_not_present";
        case VK_ERROR_EXTENSION_NOT_PRESENT : return "error_extension_not_present";
        case VK_ERROR_FEATURE_NOT_PRESENT : return "error_feature_not_present";
        case VK_ERROR_INCOMPATIBLE_DRIVER : return "error_incompatible_driver";
        case VK_ERROR_TOO_MANY_OBJECTS : return "error_too_many_objects";
        case VK_ERROR_FORMAT_NOT_SUPPORTED : return "error_format_not_supported";
        case VK_ERROR_FRAGMENTED_POOL : return "error_fragmented_pool";
        case VK_ERROR_UNKNOWN : return "error_unknown";
        case VK_ERROR_VALIDATION_FAILED : return "error_validation_failed";
        case VK_ERROR_OUT_OF_POOL_MEMORY : return "error_out_of_pool_memory";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE : return "error_invalid_external_handle";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS : return "error_invalid_opaque_capture_address";
        case VK_ERROR_FRAGMENTATION : return "error_fragmentation";
        case VK_PIPELINE_COMPILE_REQUIRED : return "pipeline_compile_required";
        case VK_ERROR_NOT_PERMITTED : return "error_not_permitted";
        case VK_ERROR_SURFACE_LOST_KHR : return "error_surface_lost_khr";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR : return "error_native_window_in_use_khr";
        case VK_SUBOPTIMAL_KHR : return "suboptimal_khr";
        case VK_ERROR_OUT_OF_DATE_KHR : return "error_out_of_date_khr";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR : return "error_incompatible_display_khr";
        case VK_ERROR_INVALID_SHADER_NV : return "error_invalid_shader_nv";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR : return "error_image_usage_not_supported_khr";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR : return "error_video_picture_layout_not_supported_khr";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR : return "error_video_profile_operation_not_supported_khr";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR : return "error_video_profile_format_not_supported_khr";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR : return "error_video_profile_codec_not_supported_khr";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR : return "error_video_std_version_not_supported_khr";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT : return "error_invalid_drm_format_modifier_plane_layout_ext";
        case VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT : return "error_present_timing_queue_full_ext";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT : return "error_full_screen_exclusive_mode_lost_ext";
        case VK_THREAD_IDLE_KHR : return "thread_idle_khr";
        case VK_THREAD_DONE_KHR : return "thread_done_khr";
        case VK_OPERATION_DEFERRED_KHR : return "operation_deferred_khr";
        case VK_OPERATION_NOT_DEFERRED_KHR : return "operation_not_deferred_khr";
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR : return "error_invalid_video_std_parameters_khr";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT : return "error_compression_exhausted_ext";
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT : return "incompatible_shader_binary_ext";
        case VK_PIPELINE_BINARY_MISSING_KHR : return "pipeline_binary_missing_khr";
        case VK_ERROR_NOT_ENOUGH_SPACE_KHR : return "error_not_enough_space_khr";
        case VK_RESULT_MAX_ENUM : return "result_max_enum";
        default: std::unreachable();
    }
    // clang-format on
}

[[maybe_unused]] constexpr std::array k_error_codes_create_instance = {
    VK_ERROR_EXTENSION_NOT_PRESENT,
    VK_ERROR_INCOMPATIBLE_DRIVER,
    VK_ERROR_INITIALIZATION_FAILED,
    VK_ERROR_LAYER_NOT_PRESENT,
    VK_ERROR_OUT_OF_DEVICE_MEMORY,
    VK_ERROR_OUT_OF_HOST_MEMORY,
    VK_ERROR_UNKNOWN,
    VK_ERROR_VALIDATION_FAILED
};
[[maybe_unused]] constexpr std::array k_error_codes_create_debug_messenger = {
    VK_ERROR_OUT_OF_HOST_MEMORY, VK_ERROR_UNKNOWN, VK_ERROR_VALIDATION_FAILED
};
}  // namespace dans::vk

// Panic when a Vulkan call does not return VK_SUCCESS. A macro, not a function,
// so DANS_PANIC reports the failing call's own file:line:func instead of a
// checker's.
#define DANS_VK_CHECK(expr)                                                                        \
    do                                                                                             \
    {                                                                                              \
        const VkResult dans_vk_result{(expr)};                                                     \
        if (dans_vk_result != VK_SUCCESS)                                                          \
        {                                                                                          \
            DANS_PANIC_FMT("Vulkan call failed with {}", ::dans::vk::to_string(dans_vk_result));   \
        }                                                                                          \
    } while (false)
