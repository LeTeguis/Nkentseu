//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:28:10 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanUtils.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

namespace nkentseu {
    std::string VulkanStaticDebugInfo::file_call;
    uint32 VulkanStaticDebugInfo::line_call = 0;
    std::string VulkanStaticDebugInfo::methode_call;
    bool VulkanStaticDebugInfo::success = true;

	const char* VulkanConvert::VulkanResultToString(VkResult result) {
        switch (result) {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_EVENT_SET: return "VK_EVENT_SET";
        case VK_EVENT_RESET: return "VK_EVENT_RESET";
        case VK_INCOMPLETE: return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN: return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION: return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_PIPELINE_COMPILE_REQUIRED: return "VK_PIPELINE_COMPILE_REQUIRED";
        case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_KHR: return "VK_ERROR_NOT_PERMITTED_KHR";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR: return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR: return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR: return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR: return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
        default: return "VK_UNKNOWN_ERROR";
        }
    }

    void VulkanConvert::GetResourceLimits(VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceProperties deviceProperties = {};
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
        VkPhysicalDeviceLimits limits = deviceProperties.limits;

        // Limites minimales des ressources
        uint64 minUniformBufferOffsetAlignment = limits.minUniformBufferOffsetAlignment;
        uint64 minStorageBufferOffsetAlignment = limits.minStorageBufferOffsetAlignment;
        uint64 minTexelBufferOffsetAlignment = limits.minTexelBufferOffsetAlignment;
        uint64 minTexelOffset = limits.minTexelOffset;
        uint64 minTexelGatherOffset = limits.minTexelGatherOffset;

        // Limites maximales des ressources
        uint64 maxMemoryAllocationCount = limits.maxMemoryAllocationCount;
        uint64 maxSamplerAllocationCount = limits.maxSamplerAllocationCount;
        uint64 maxStorageBufferRange = limits.maxStorageBufferRange;
        uint64 maxPerStageDescriptorStorageBuffers = limits.maxPerStageDescriptorStorageBuffers;
        uint64 maxPerStageDescriptorSampledImages = limits.maxPerStageDescriptorSampledImages;

        // Affichage des limites
        Log_nts.Info("Limites minimales des ressources :");
        Log_nts.Info("    {0} : {1}", "minUniformBufferOffsetAlignment", minUniformBufferOffsetAlignment);
        Log_nts.Info("    {0} : {1}", "minStorageBufferOffsetAlignment", minStorageBufferOffsetAlignment);
        Log_nts.Info("    {0} : {1}", "minTexelBufferOffsetAlignment", minTexelBufferOffsetAlignment);
        Log_nts.Info("    {0} : {1}", "minTexelOffset", minTexelOffset);
        Log_nts.Info("    {0} : {1}", "minTexelGatherOffset", minTexelGatherOffset);

        Log_nts.Info("Limites maximales des ressources :");
        Log_nts.Info("    {0} : {1}", "maxMemoryAllocationCount", maxMemoryAllocationCount);
        Log_nts.Info("    {0} : {1}", "maxSamplerAllocationCount", maxSamplerAllocationCount);
        Log_nts.Info("    {0} : {1}", "maxStorageBufferRange", maxStorageBufferRange);
        Log_nts.Info("    {0} : {1}", "maxPerStageDescriptorStorageBuffers", maxPerStageDescriptorStorageBuffers);
        Log_nts.Info("    {0} : {1}", "maxPerStageDescriptorSampledImages", maxPerStageDescriptorSampledImages);
    }

    VkShaderStageFlagBits VulkanConvert::GetshaderStageType(ShaderType::Code type) {
        switch (type) {
        case ShaderType::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderType::Fragment:
        case ShaderType::Pixel: // Assuming Pixel is synonymous with Fragment
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderType::Geometry:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        case ShaderType::TesControl:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case ShaderType::TesEvaluation:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case ShaderType::Compute:
            return VK_SHADER_STAGE_COMPUTE_BIT;
        default:
            return VK_SHADER_STAGE_VERTEX_BIT;
        }
    }

    glslang_stage_t VulkanConvert::GetshaderStageTypeGLSLang(ShaderType::Code shaderType)
    {
        if (shaderType == ShaderType::Vertex)
            return GLSLANG_STAGE_VERTEX;

        if (shaderType == ShaderType::Fragment)
            return GLSLANG_STAGE_FRAGMENT;

        if (shaderType == ShaderType::Geometry)
            return GLSLANG_STAGE_GEOMETRY;

        if (shaderType == ShaderType::Compute)
            return GLSLANG_STAGE_COMPUTE;

        if (shaderType == ShaderType::TesControl)
            return GLSLANG_STAGE_TESSCONTROL;

        if (shaderType == ShaderType::TesEvaluation)
            return GLSLANG_STAGE_TESSEVALUATION;

        return GLSLANG_STAGE_VERTEX;
    }
    bool VulkanConvert::EndsWith(const std::string& s, const std::string& part) {
        if (s.size() >= part.size()) {
            return (s.compare(s.size() - part.size(), part.size(), part) == 0);
        }
        return false;
    }
}  //  nkentseu

#endif