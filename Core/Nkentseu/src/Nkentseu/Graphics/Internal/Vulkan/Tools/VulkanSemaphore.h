//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:00 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_SEMAPHORE_H__
#define __VULKAN_SEMAPHORE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>
#include "VulkanDevice.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanSemaphore {
        bool Create(VulkanDevice* gpu);
        bool Destroy(VulkanDevice* gpu);

        VkSemaphore submitSemaphore = nullptr;
        VkSemaphore aquireSemaphore = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_SEMAPHORE_H__!