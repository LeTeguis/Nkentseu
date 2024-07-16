//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:17:32 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanBuffer.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {

	bool VulkanBuffer::WriteToBuffer(const void* data, usize size, usize offset)
	{
		if (mappedData == nullptr) return false;

		if (size == VK_WHOLE_SIZE) {
			memcpy(mappedData, data, this->size);
		}
		else {
			char* memOffset = (char*)mappedData;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
		return true;
	}

	bool VulkanBuffer::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		if (buffer != nullptr) {
			vkCheckErrorVoid(vkDestroyBuffer(gpu->device, buffer, nullptr));
			buffer = VK_NULL_HANDLE;
		}

		bool success = VulkanStaticDebugInfo::Result();

		if (bufferMemory != nullptr) {
			vkCheckErrorVoid(vkFreeMemory(gpu->device, bufferMemory, nullptr));
			bufferMemory = VK_NULL_HANDLE;
		}

		return success && VulkanStaticDebugInfo::Result();
	}

	bool VulkanBuffer::Mapped(VulkanDevice* gpu, usize size, usize offset, VkMemoryMapFlags flag)
	{
		if (gpu == nullptr || bufferMemory == nullptr || buffer == nullptr || mappedData != nullptr) return false;

		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkMapMemory(gpu->device, bufferMemory, offset, size, flag, (void**)&mappedData), "cannot map buffer memory");
		return result.success;
	}

	bool VulkanBuffer::UnMapped(VulkanDevice* gpu)
	{
		if (gpu == nullptr || bufferMemory == nullptr || buffer == nullptr || mappedData == nullptr || mappedData == nullptr) return false;
		vkCheckErrorVoid(vkUnmapMemory(gpu->device, bufferMemory));
		mappedData = nullptr;
		return VulkanStaticDebugInfo::Result();
	}

	bool VulkanBuffer::Flush(VulkanDevice* gpu, usize size, usize offset)
	{
		if (gpu == nullptr || bufferMemory == nullptr || buffer == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = bufferMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		vkCheckError(first, result, vkFlushMappedMemoryRanges(gpu->device, 1, &mappedRange), "cannot flush buffer memory");
		return result.success;
	}

	int64 VulkanBuffer::FindMemoryType(VulkanDevice* gpu, uint32 typeFilter, VkMemoryPropertyFlags properties)
	{
		if (gpu == nullptr) return 0;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(gpu->gpu, &memProperties);

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		return -1;
	}

	bool VulkanBuffer::CreateBuffer(VulkanDevice* gpu, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		if (gpu == nullptr) {
			return false;
		}

		VulkanResult result;
		bool first = true;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = sharingMode;

		vkCheckError(first, result, vkCreateBuffer(gpu->device, &bufferInfo, nullptr, &buffer), "cannot create buffer");

		if (!result.success || buffer == nullptr) {
			Log_nts.Error("Cannot create buffer");
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkCheckErrorVoid(vkGetBufferMemoryRequirements(gpu->device, buffer, &memRequirements));

		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot get memory buffer requirements");
			return false;
		}

		int64 index = FindMemoryType(gpu, memRequirements.memoryTypeBits, properties);

		if (index < 0) {
			Log_nts.Error("Cannot find correct memory type");
			return false;
		}

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = index;

		vkCheckError(first, result, vkAllocateMemory(gpu->device, &allocInfo, nullptr, &bufferMemory), "cannot allocat memory buffer");
		vkCheckError(first, result, vkBindBufferMemory(gpu->device, buffer, bufferMemory, 0), "cannot bind buffer memory");

		return result.success;
	}

	bool VulkanBuffer::CopyBuffer(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		if (gpu == nullptr || commandPool == nullptr) {
			return false;
		}

		VulkanResult result;
		bool first = true;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool->commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;

		vkCheckError(first, result, vkAllocateCommandBuffers(gpu->device, &allocInfo, &commandBuffer), "cannot allocate command buffer");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkCheckError(first, result, vkBeginCommandBuffer(commandBuffer, &beginInfo), "cannot begin command buffer");

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optionnel
		copyRegion.dstOffset = 0; // Optionnel
		copyRegion.size = size;

		if (result.success) {
			vkCheckErrorVoid(vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion));
			result.success = VulkanStaticDebugInfo::Result();
		}
		vkCheckError(first, result, vkEndCommandBuffer(commandBuffer), "cannot end command buffer");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkCheckError(first, result, vkQueueSubmit(gpu->queue.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "cannot submit queue");
		vkCheckError(first, result, vkQueueWaitIdle(gpu->queue.graphicsQueue), "cannot wait idle queue");

		if (result.success) {
			vkCheckErrorVoid(vkFreeCommandBuffers(gpu->device, commandPool->commandPool, 1, &commandBuffer));
			result.success = VulkanStaticDebugInfo::Result();
		}
		return result.success;
	}

	bool VulkanBuffer::CopyBufferToImage(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkBuffer srcBuffer, VkImage dstImage, const maths::Vector2u& size)
	{
		VkCommandBuffer commandBuffer = gpu->BeginSingleTimeCommands(commandPool->commandPool);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { size.x, size.y, 1 };
		vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		gpu->EndSingleTimeCommands(commandBuffer, commandPool->commandPool);
		return true;
	}

}  //  nkentseu