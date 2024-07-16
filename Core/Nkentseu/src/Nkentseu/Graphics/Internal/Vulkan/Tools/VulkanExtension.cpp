//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 11:43:38 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanExtension.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan_xlib.h>
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan_xcb.h>
#endif

namespace nkentseu {
    
	// Vulkan extension
	void VulkanExtension::Defined()
	{
		//  Instance extension
#ifdef NKENTSEU_PLATFORM_WINDOWS
		instanceExtension.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
		instanceExtension.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
		instanceExtension.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
		instanceExtension.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		instanceExtension.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		instanceExtension.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		//instanceExtension.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);

		// Device Extension
		//deviceExtension.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);
		deviceExtension.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME);

		// Layer
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	bool VulkanExtension::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtension.begin(), deviceExtension.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

}  //  nkentseu