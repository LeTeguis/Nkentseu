//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:08:56 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_FRAMEBUFFER_H__
#define __VULKAN_FRAMEBUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanFramebuffer {
        bool Create(VulkanDevice* gpu, const maths::Vector2u& size, VulkanRenderPass* renderPass, VulkanSwapchain* swapchain, bool depth = true);
        bool Destroy(VulkanDevice* gpu);

        std::vector<VkFramebuffer> framebuffer = {};
        maths::Vector2u size = {};
        VulkanImage depthImage = {};
        bool useDepth = false;
    };

}  //  nkentseu

#endif  // __VULKAN_FRAMEBUFFER_H__!