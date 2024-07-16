//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:12:19 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_RENDER_PASS_H__
#define __VULKAN_RENDER_PASS_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>

#include "VulkanSwapchain.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanRenderPass {
        bool Create(VulkanDevice* gpu, VulkanSwapchain* swapchain);
        bool Destroy(VulkanDevice* gpu);

        VkRenderPass renderPass = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_RENDER_PASS_H__!