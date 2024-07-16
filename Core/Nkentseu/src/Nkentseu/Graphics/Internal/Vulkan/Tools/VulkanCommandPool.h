//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:21:37 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_COMMAND_POOL_H__
#define __VULKAN_COMMAND_POOL_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>
#include "VulkanDevice.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanCommandPool {
        bool Create(VulkanDevice* gpu);
        bool Destroy(VulkanDevice* gpu);

        VkCommandPool commandPool = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_COMMAND_POOL_H__!