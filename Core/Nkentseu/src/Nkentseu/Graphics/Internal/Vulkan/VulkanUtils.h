//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:28:10 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_UTILS_H__
#define __VULKAN_UTILS_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include <vulkan/vulkan.h>
#include <Logger/Formatter.h>
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    struct VkResultNts {
        bool success = false;
        VkResult result;
    };

    class NKENTSEU_API VulkanConvert {
    public:
        static const char* VkResultToString(VkResult result);
    };

    template<typename... Args>
    VkResultNts vkCheckError_(VkResult result, const std::string& file, int32 line, const std::string& function, const char* format, Args&&... args) {
        VkResultNts result_;
        result_.result = result;
        result_.success = result == VK_SUCCESS;

        if (!result_.success) {
            std::string message = FORMATTER.Format(format, args...);
            message = FORMATTER.Format("code : {0}({1}); {2}", VulkanConvert::VkResultToString(result), result, message);
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, function.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", message);
        }

        return result_;
    }

#define vkCheckError(result, format, ... ) vkCheckError_(result, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__) 
}  //  nkentseu

#endif

#endif  // __VULKAN_UTILS_H__!