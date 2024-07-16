//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:50 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_COMMAND_BUFFER_H__
#define __VULKAN_COMMAND_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>
#include "VulkanSwapchain.h"
#include "VulkanCommandPool.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanCommandBuffer {
        bool Create(VulkanDevice* gpu, VulkanSwapchain* swapchain, VulkanCommandPool* commandPool);
        bool Destroy(VulkanDevice* gpu, VulkanCommandPool* commandPool);

        std::vector<VkCommandBuffer> commandBuffers = {};
    };

}  //  nkentseu

#endif  // __VULKAN_COMMAND_BUFFER_H__!