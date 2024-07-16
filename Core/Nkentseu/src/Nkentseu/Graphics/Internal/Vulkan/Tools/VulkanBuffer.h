//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:17:32 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_BUFFER_H__
#define __VULKAN_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanCommandPool.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanBuffer {
        bool WriteToBuffer(const void* data, usize size, usize offset);
        bool Destroy(VulkanDevice* gpu);
        bool Mapped(VulkanDevice* gpu, usize size, usize offset = 0, VkMemoryMapFlags flag = 0);
        bool UnMapped(VulkanDevice* gpu);
        bool Flush(VulkanDevice* gpu, usize size, usize offset = 0);

        static int64 FindMemoryType(VulkanDevice* gpu, uint32 typeFilter, VkMemoryPropertyFlags properties);
        static bool CreateBuffer(VulkanDevice* gpu, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        static bool CopyBuffer(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        static bool CopyBufferToImage(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkBuffer srcBuffer, VkImage dstImage, const maths::Vector2u& size);

        VkBuffer buffer = nullptr;
        VkDeviceMemory bufferMemory = nullptr;
        void* mappedData = nullptr;
        VkDeviceSize size = 0;
    };

}  //  nkentseu

#endif  // __VULKAN_BUFFER_H__!