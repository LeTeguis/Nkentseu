//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:30:26 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_H__
#define __VULKAN_INTERNAL_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#ifdef NKENTSEU_PLATFORM_WINDOWS
//#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#define VK_USE_PLATFORM_XCB_KHR
#endif

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>
#include <Nkentseu/Graphics/GraphicsProperties.h>

namespace nkentseu {
    class Window;

    struct NKENTSEU_API VulkanExtension {
        void Defined();
        std::vector<const char*> instanceExtension;
        std::vector<const char*> deviceExtension;
        std::vector<const char*> layers;
    };

    struct NKENTSEU_API VulkanInstance {
        bool Create(Window* window, const ContextProperties& contextProperties, VulkanExtension* extension);

        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debugMessenger = nullptr;
    };

    struct NKENTSEU_API VulkanSurface {
        bool Create(Window* window, VulkanInstance* instance);

        VkSurfaceKHR surface = nullptr;
    };

    struct NKENTSEU_API VulkanGpu {
        bool GetDevice(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension);
        bool GetLogicalDevice(VulkanSurface* surface, VulkanExtension* extension);

        std::vector<VkPhysicalDevice> gpus;
        VkPhysicalDevice gpu;
        VkDevice device;
        VkQueue graphicsQueue;

        std::vector<VkQueueFamilyProperties> queueProperties;

        int32 graphicsIndex = -1;
    };

    struct NKENTSEU_API VulkanSwapchain {
        bool Create(VulkanGpu* gpu, VulkanSurface* surface);

        VkSwapchainKHR swapchain = nullptr;
        VkSurfaceFormatKHR surfaceFormat;
        std::vector<VkImage> swapchainImages;
    };

    struct NKENTSEU_API VulkanCommandPool {
        bool Create(VulkanGpu* gpu);

        VkCommandPool commandPool = nullptr;
    };

    struct NKENTSEU_API VulkanSemaphore {
        bool Create(VulkanGpu* gpu);

        VkSemaphore submitSemaphore = nullptr;
        VkSemaphore aquireSemaphore = nullptr;
    };
    
}  //  nkentseu

#endif

#endif  // __VULKAN_INTERNAL_H__!