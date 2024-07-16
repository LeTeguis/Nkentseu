//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanCommandBuffer.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
	bool VulkanCommandBuffer::Create(VulkanDevice* gpu, VulkanSwapchain* swapchain, VulkanCommandPool* commandPool)
	{
		if (gpu == nullptr || swapchain == nullptr || commandPool == nullptr) return false;

		VulkanResult result;
		bool first = true;

		commandBuffers.resize(swapchain->swapchainImages.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool->commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		vkCheckError(first, result, vkAllocateCommandBuffers(gpu->device, &allocInfo, commandBuffers.data()), "cannot allocate command buffer");
		return true;
	}

	bool VulkanCommandBuffer::Destroy(VulkanDevice* gpu, VulkanCommandPool* commandPool)
	{
		if (gpu == nullptr || gpu->device == nullptr || commandBuffers.size() == 0) return false;

		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkDeviceWaitIdle(gpu->device), "cannot wait device idle");
		vkCheckErrorVoid(vkFreeCommandBuffers(gpu->device, commandPool->commandPool, 1, commandBuffers.data()));
		commandBuffers.clear();
		return true;
	}

}  //  nkentseu