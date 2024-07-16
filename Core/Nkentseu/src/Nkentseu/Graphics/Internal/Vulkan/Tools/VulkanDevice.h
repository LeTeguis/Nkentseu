//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:15:30 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_DEVICE_H__
#define __VULKAN_DEVICE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>

#include <vulkan/vulkan.hpp>
#include "VulkanInstance.h"
#include "VulkanExtension.h"
#include "VulkanSurface.h"
#include "VulkanQueue.h"

namespace nkentseu {

    struct NKENTSEU_API VulkanSwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats = {};
        std::vector<VkPresentModeKHR>   presentMode = {};

        bool QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    };

    struct NKENTSEU_API VulkanDevice {
        bool Create(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension);
        bool Destroy();
        bool GetLogicalDevice(VulkanSurface* surface, VulkanExtension* extension);
        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanExtension* extension);
        uint32 FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties);

        VkCommandBuffer BeginSingleTimeCommands(VkCommandPool commandPool);
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool);

        bool IsValid();

        std::vector<VkPhysicalDevice> gpus = {};
        VkPhysicalDevice gpu = nullptr;
        VkPhysicalDeviceProperties properties = {};
        VulkanQueue queue;
        VkDevice device = nullptr;

        std::vector<VkQueueFamilyProperties> queueProperties = {};
        PFN_vkCmdSetPolygonModeEXT cmdSetPolygonModeEXT = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_DEVICE_H__!