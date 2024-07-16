//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:00:18 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_SWAPCHAIN_H__
#define __VULKAN_SWAPCHAIN_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanSurface.h"
#include "VulkanImage.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanSwapchain {
        bool Create(VulkanDevice* gpu, VulkanSurface* surface, const maths::Vector2u& size, const ContextProperties& contextProperties);
        bool Destroy(VulkanDevice* gpu);
        bool FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format);
        static uint32 GetMinImageCountFromPresentMode(VkPresentModeKHR present_mode);
        VkPresentModeKHR SelectPresentMode(VulkanDevice* gpu, VulkanSurface* surface, const ContextProperties& contextProperties);

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        VkSwapchainKHR swapchain = nullptr;
        VkSurfaceFormatKHR surfaceFormat = {};

        std::vector<VkImage> swapchainImages = {};
        std::vector<VkImageView> imageView = {};
    };

}  //  nkentseu

#endif  // __VULKAN_SWAPCHAIN_H__!