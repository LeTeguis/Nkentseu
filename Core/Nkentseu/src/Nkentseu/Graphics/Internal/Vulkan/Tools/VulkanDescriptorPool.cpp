//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:17 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanDescriptorPool.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
	bool VulkanDescriptorPool::Create(VulkanDevice* gpu, VulkanSwapchain* swapchain) {
		if (gpu == nullptr || swapchain == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		if (poolSizes.size() > 0) {
			poolInfo.poolSizeCount = poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
		}
		poolInfo.maxSets = static_cast<uint32_t>(swapchain->swapchainImages.size());

		vkCheckError(first, result, vkCreateDescriptorPool(gpu->device, &poolInfo, nullptr, &descriptorPool), "cannot create descriptor pool");

		return result.success;
	}

	bool VulkanDescriptorPool::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr) return false;

		if (descriptorPool != VK_NULL_HANDLE) {
			vkCheckErrorVoid(vkDestroyDescriptorPool(gpu->device, descriptorPool, nullptr));
			descriptorPool = VK_NULL_HANDLE;
		}
		return VulkanStaticDebugInfo::Result();
	}

	void VulkanDescriptorPool::Add(VkDescriptorType dType, uint32 count)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = dType;
		poolSize.descriptorCount = count;
	}

}  //  nkentseu