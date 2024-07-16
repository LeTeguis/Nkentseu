//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:11:09 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_QUEUE_H__
#define __VULKAN_QUEUE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>

#include <vulkan/vulkan.hpp>

namespace nkentseu {
    
    struct NKENTSEU_API VulkanQueueFamilyIndices {
        int32   graphicsIndex = -1;
        int32   presentIndex = -1;

        bool    hasGraphicsFamily = false;
        bool    hasPresentFamily = false;

        bool    IsComplete() { return hasGraphicsFamily && hasPresentFamily; }
        bool    FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    };

    struct NKENTSEU_API VulkanQueue {
        VulkanQueueFamilyIndices queueFamily;
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_QUEUE_H__!