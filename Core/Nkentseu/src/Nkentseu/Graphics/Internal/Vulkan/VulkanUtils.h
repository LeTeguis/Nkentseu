//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:28:10 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_UTILS_H__
#define __VULKAN_UTILS_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include <vulkan/vulkan.hpp>
#include <string>
#include <Logger/Formatter.h>
#include <Nkentseu/Core/NkentseuLogger.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
//#include <glslang_c_shader_types.h>

namespace nkentseu {
    struct VulkanResult {
        bool success = false;
        VkResult result;
    };

    struct VulkanStaticDebugInfo {
        static std::string file_call;
        static uint32 line_call;
        static std::string methode_call;
        static bool success;

        static void SetInfo(const std::string& file, uint32 line, const std::string& method) {
            file_call = file;
            line_call = line;
            methode_call = method;
        }
    };

    class NKENTSEU_API VulkanConvert {
    public:
        static const char* VulkanResultToString(VkResult result);
        static void GetResourceLimits(VkPhysicalDevice physicalDevice);
        static VkShaderStageFlagBits GetshaderStageType(ShaderType::Code type);

        static VkCullModeFlagBits CullModeType(CullModeType::Code mode);
        static VkPolygonMode PolygonModeType(PolygonModeType::Code contentMode);

        static bool EndsWith(const std::string& s, const std::string& part);

        static VkFormat ShaderFormatToVkFormat(ShaderDataType::Code shaderDataType);
    };

    template<typename... Args>
    VulkanResult vkCheckError_(VkResult result, const char* format, Args&&... args) {
        VulkanResult result_;
        result_.result = result;
        result_.success = result == VK_SUCCESS;

        std::string file = VulkanStaticDebugInfo::file_call;
        std::string methode = VulkanStaticDebugInfo::methode_call;
        uint32 line = VulkanStaticDebugInfo::line_call;

        if (!result_.success) {
            std::string message = FORMATTER.Format(format, args...);
            message = FORMATTER.Format("code : {0}({1}); {2}", VulkanConvert::VulkanResultToString(result), result, message);
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", message);
        }

        return result_;
    }

#define vkCheckError(first, presult, result, format, ... ) VulkanStaticDebugInfo::SetInfo(__FILE__, __LINE__, __FUNCTION__); presult = (presult.success || first) ? vkCheckError_(result, format, ##__VA_ARGS__) : presult; first = false
#define vkCheckErrorVoid(function) VulkanStaticDebugInfo::SetInfo(__FILE__, __LINE__, __FUNCTION__); function
}  //  nkentseu

#endif

#endif  // __VULKAN_UTILS_H__!