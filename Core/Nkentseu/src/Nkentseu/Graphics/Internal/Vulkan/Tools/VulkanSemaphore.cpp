//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:00 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanSemaphore.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
	bool VulkanSemaphore::Create(VulkanDevice* gpu) {
		if (gpu == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		vkCheckError(first, result, vkCreateSemaphore(gpu->device, &semaphoreInfo, 0, &aquireSemaphore), "cannot create acquire semaphore");
		vkCheckError(first, result, vkCreateSemaphore(gpu->device, &semaphoreInfo, 0, &submitSemaphore), "cannot create submit semaphore");

		if (result.success) {
			Log_nts.Info("Create semaphore is good");
		}

		return result.success;
	}

	bool VulkanSemaphore::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		if (submitSemaphore != nullptr) {
			vkDestroySemaphore(gpu->device, submitSemaphore, nullptr);
			submitSemaphore = VK_NULL_HANDLE;
		}

		if (aquireSemaphore != nullptr) {
			vkDestroySemaphore(gpu->device, aquireSemaphore, nullptr);
			aquireSemaphore = VK_NULL_HANDLE;
		}
		return false;
	}

}  //  nkentseu