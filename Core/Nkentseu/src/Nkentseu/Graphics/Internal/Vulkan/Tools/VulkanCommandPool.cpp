//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:21:37 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanCommandPool.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
	bool VulkanCommandPool::Create(VulkanDevice* gpu) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = gpu->queue.queueFamily.graphicsIndex;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkCheckError(first, result, vkCreateCommandPool(gpu->device, &poolInfo, 0, &commandPool), "cannot getswapchain image khr");

		if (result.success) {
			Log_nts.Info("Create command pool is good");
		}

		return result.success;
	}

	bool VulkanCommandPool::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr || commandPool == nullptr) return false;
		vkCheckErrorVoid(vkDestroyCommandPool(gpu->device, commandPool, nullptr));
		commandPool = VK_NULL_HANDLE;
		return false;
	}

}  //  nkentseu