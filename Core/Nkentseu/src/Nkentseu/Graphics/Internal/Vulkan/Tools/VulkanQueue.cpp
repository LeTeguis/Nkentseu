//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:11:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanQueue.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
	bool VulkanQueueFamilyIndices::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		if (device == nullptr || surface == nullptr) return false;

		uint32 queueFamilyCount = 0;

		vkCheckErrorVoid(vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr));

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

		vkCheckErrorVoid(vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()));

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsIndex = i;
				hasGraphicsFamily = true;
			}
			VkBool32 presentSupport = false;
			VulkanResult result;
			bool first = true;

			vkCheckError(first, result, vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport), "cannot get physical device surface support");

			if (queueFamily.queueCount > 0 && presentSupport) {
				presentIndex = i;
				hasPresentFamily = true;
			}
			if (IsComplete()) {
				break;
			}

			i++;
		}

		return true;
	}

}  //  nkentseu