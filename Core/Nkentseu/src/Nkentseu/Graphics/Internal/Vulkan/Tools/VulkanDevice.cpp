//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:15:30 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanDevice.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
	bool VulkanSwapchainSupportDetails::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities), "cannot get physical device surface capabilities");

		uint32 formatCount;
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr), "cannot cout physical device surface format");

		if (formatCount != 0) {
			formats.resize(formatCount);
			vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data()), "cannot get physical device surface format");
		}

		uint32 presentModeCount;
		vkCheckError(first, result, vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr), "cannot count physical device surface present mode");

		if (presentModeCount != 0) {
			presentMode.resize(presentModeCount);
			vkCheckError(first, result, vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentMode.data()), "cannot get physical device surface present mode");
		}
		return result.success;
	}

	// Vulkan Gpu
	bool VulkanDevice::Create(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension)
	{
		if (instance == nullptr || surface == nullptr) return false;
		VulkanResult result;
		bool first = true;

		uint32 gpuCount = 0;
		vkCheckError(first, result, vkEnumeratePhysicalDevices(instance->instance, &gpuCount, 0), "Cannot count physical device");
		std::vector<VkPhysicalDevice> gpusAll(gpuCount);
		vkCheckError(first, result, vkEnumeratePhysicalDevices(instance->instance, &gpuCount, gpusAll.data()), "Cannot get all physical device");

		if (result.success) {
			for (uint32 index = 0; index < gpuCount; index++) {
				if (IsDeviceSuitable(gpusAll[index], surface->surface, extension)) {
					gpu = gpusAll[index];
					break;
				}
			}

			if (gpu == nullptr) {
				return false;
			}

			std::copy(gpus.begin(), gpus.end(), gpusAll.data());

			queue.queueFamily.FindQueueFamilies(gpu, surface->surface);

			if (!queue.queueFamily.IsComplete()) {
				return false;
			}

			vkCheckErrorVoid(vkGetPhysicalDeviceProperties(gpu, &properties));
			result.success = GetLogicalDevice(surface, extension);

			if (result.success) {
				if (cmdSetPolygonModeEXT == nullptr) {
					cmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)vkGetDeviceProcAddr(device, "vkCmdSetPolygonModeEXT");

					if (!cmdSetPolygonModeEXT) {
						Log_nts.Error("Failed to load vkCmdSetPolygonModeEXT");
					}
				}
			}
		}

		return result.success;
	}

	bool VulkanDevice::Destroy()
	{
		if (device == nullptr) return false;
		vkCheckErrorVoid(vkDestroyDevice(device, nullptr));
		device = VK_NULL_HANDLE;
		return true;
	}

	bool VulkanDevice::GetLogicalDevice(VulkanSurface* surface, VulkanExtension* extension)
	{
		float32 queuePriority = 1.0f;

		std::vector<VkDeviceQueueCreateInfo> queueInfos;
		std::set<int32> uniqueQueueFamilies = { queue.queueFamily.graphicsIndex, queue.queueFamily.presentIndex };

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceExtendedDynamicState3FeaturesEXT extendedDynamicState3Features = {};
		extendedDynamicState3Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT;
		extendedDynamicState3Features.extendedDynamicState3PolygonMode = VK_TRUE;

		/*VkPhysicalDeviceFeatures2 deviceFeatures2 = {};
		deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures2.pNext = &extendedDynamicState3Features;*/

		VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature{};
		extendedDynamicStateFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
		extendedDynamicStateFeature.extendedDynamicState = VK_TRUE;
		extendedDynamicStateFeature.pNext = &extendedDynamicState3Features;

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = &extendedDynamicStateFeature;
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.pQueueCreateInfos = queueInfos.data();
		deviceInfo.queueCreateInfoCount = (uint32)queueInfos.size();
		deviceInfo.ppEnabledExtensionNames = extension->deviceExtension.data();
		deviceInfo.enabledExtensionCount = (uint32)extension->deviceExtension.size();

		VulkanResult result;
		bool first = true;
		vkCheckError(first, result, vkCreateDevice(gpu, &deviceInfo, 0, &device), "cannot create device");

		if (result.success) {
			Log_nts.Info("Create vulkan device is good");
			vkCheckErrorVoid(vkGetDeviceQueue(device, queue.queueFamily.graphicsIndex, 0, &queue.graphicsQueue));
			vkCheckErrorVoid(vkGetDeviceQueue(device, queue.queueFamily.presentIndex, 0, &queue.presentQueue));
		}
		return result.success;
	}

	bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanExtension* extension)
	{
		VulkanQueueFamilyIndices indices;
		indices.FindQueueFamilies(device, surface);

		bool extensionsSupported = extension->CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			VulkanSwapchainSupportDetails swapChainSupport;
			swapChainSupport.QuerySwapChainSupport(device, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentMode.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	uint32 VulkanDevice::FindMemoryType(uint32 typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkCheckErrorVoid(vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties));

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		Log_nts.Error("failed to find suitable memory type!");
		return 0;
	}

	VkCommandBuffer VulkanDevice::BeginSingleTimeCommands(VkCommandPool commandPool)
	{
		VulkanResult result;
		bool first = true;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkCheckError(first, result, vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer), "failed to allocate command buffer");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkCheckError(first, result, vkBeginCommandBuffer(commandBuffer, &beginInfo), "failed to begin command buffer");

		return commandBuffer;
	}

	void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool)
	{
		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkEndCommandBuffer(commandBuffer), "failed to end command buffer");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkCheckError(first, result, vkQueueSubmit(queue.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "failed to submit queue");
		vkCheckError(first, result, vkQueueWaitIdle(queue.graphicsQueue), "failed to wait idle");

		vkCheckErrorVoid(vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer));
	}

	bool VulkanDevice::IsValid() {
		return gpu != nullptr && device != nullptr;
	}

}  //  nkentseu