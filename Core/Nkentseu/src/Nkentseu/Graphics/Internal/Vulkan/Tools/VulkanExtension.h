//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 11:43:38 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_EXTENSION_H__
#define __VULKAN_EXTENSION_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>

#include <vulkan/vulkan.hpp>

namespace nkentseu {
    
    struct NKENTSEU_API VulkanExtension {
        void Defined();
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        std::vector<const char*> instanceExtension = {};
        std::vector<const char*> deviceExtension = {};
        std::vector<const char*> layers = {};
    };

}  //  nkentseu

#endif  // __VULKAN_EXTENSION_H__!