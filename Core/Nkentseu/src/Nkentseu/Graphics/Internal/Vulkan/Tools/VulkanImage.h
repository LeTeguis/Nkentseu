//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:19:32 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_IMAGE_H__
#define __VULKAN_IMAGE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>

#include <Ntsm/Vector/Vector2.h>

#include "VulkanDevice.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanImage {
        // Enum to specify image type (depth or color)
        enum class ImageType {
            STENCIL,
            DEPTH,
            COLOR
        };

        // Constructor that takes device, width, height, format, and image type
        bool Create(VulkanDevice* gpu, const maths::Vector2u& size, ImageType imageType, VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
        bool Destroy(VulkanDevice* gpu);
        bool CreateImage(VulkanDevice* gpu, ImageType imageType, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspectMask);
        bool CreateMemory(VulkanDevice* gpu);
        bool CreateImageView(VulkanDevice* gpu, VkImageAspectFlags aspectMask);
        static bool FindSupportedFormat(VulkanDevice* gpu, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format);
        static bool FindDepthFormat(VulkanDevice* gpu, VkFormat* format);
        void TransitionImageLayout(VulkanDevice* gpu, VkCommandPool commandPool, VkQueue graphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout);
        uint32 FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties);
        bool HasStencilComponent(VkFormat format);

        static VkSampler CreateTextureSampler(VulkanDevice* gpu, bool repeate, bool smooth);

        // Private members
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory memory;
        VkFormat format;
        maths::Vector2u size = {};
        uint32 mipLevels;
        VkImageLayout layout;
    };

}  //  nkentseu

#endif  // __VULKAN_IMAGE_H__!