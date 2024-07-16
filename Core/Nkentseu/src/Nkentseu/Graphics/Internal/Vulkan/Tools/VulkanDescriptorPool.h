//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:17 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_DESCRIPTOR_POOL_H__
#define __VULKAN_DESCRIPTOR_POOL_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>
#include "VulkanSwapchain.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanDescriptorPool {
        bool Create(VulkanDevice* gpu, VulkanSwapchain* swapchain);
        bool Destroy(VulkanDevice* gpu);
        void Add(VkDescriptorType dType, uint32 count);
        VkDescriptorPool descriptorPool = nullptr;
        std::vector<VkDescriptorPoolSize> poolSizes;
    };

}  //  nkentseu

#endif  // __VULKAN_DESCRIPTOR_POOL_H__!