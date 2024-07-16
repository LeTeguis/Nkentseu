//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:19:32 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanImage.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
	using namespace maths;
    
	bool VulkanImage::Create(VulkanDevice* gpu, const Vector2u& size, ImageType imageType, VkFormat format, VkSampleCountFlagBits samples) {
		this->format = format;
		this->size = size;
		this->mipLevels = 1;
		this->layout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImageUsageFlags usage;
		VkImageAspectFlags aspectMask;

		if (imageType == ImageType::DEPTH) {
			usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			VkFormat format_ind;
			FindDepthFormat(gpu, &format_ind);

			if (format_ind != this->format) {
				this->format = format_ind;
			}
		}
		else {
			usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		usage = usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		if (this->format >= VK_FORMAT_D16_UNORM_S8_UINT) {
			aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
		}

		CreateImage(gpu, imageType, samples, usage, aspectMask);
		CreateMemory(gpu);
		CreateImageView(gpu, aspectMask);

		layout = VK_IMAGE_LAYOUT_UNDEFINED;
		if (imageType == ImageType::DEPTH) {
			//TransitionImageLayout(gpu, pool->commandPool, gpu->queue.graphicsQueue, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}
		return true;
	}

	// Destructor to clean up Vulkan resources
	bool VulkanImage::Destroy(VulkanDevice* gpu) {

		if (gpu == nullptr) return false;
		vkCheckErrorVoid(vkDestroyImageView(gpu->device, imageView, nullptr));
		vkCheckErrorVoid(vkDestroyImage(gpu->device, image, nullptr));
		vkCheckErrorVoid(vkFreeMemory(gpu->device, memory, nullptr));
		return true;
	}

	// Initialize Vulkan image resources
	bool VulkanImage::CreateImage(VulkanDevice* gpu, ImageType imageType, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspectMask) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = size.width;
		imageInfo.extent.height = size.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = samples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		vkCheckError(first, result, vkCreateImage(gpu->device, &imageInfo, nullptr, &image), "failed to create image!");
		return result.success;
	}

	bool VulkanImage::CreateMemory(VulkanDevice* gpu) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkMemoryRequirements memRequirements;
		vkCheckErrorVoid(vkGetImageMemoryRequirements(gpu->device, image, &memRequirements));

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(gpu->gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vkCheckError(first, result, vkAllocateMemory(gpu->device, &allocInfo, nullptr, &memory), "failed to allocate image memory!");

		vkCheckError(first, result, vkBindImageMemory(gpu->device, image, memory, 0), "failed to bind image memory");
		return result.success;
	}

	bool VulkanImage::CreateImageView(VulkanDevice* gpu, VkImageAspectFlags aspectMask) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		vkCheckError(first, result, vkCreateImageView(gpu->device, &viewInfo, nullptr, &imageView), "failed to create image view!");
		return result.success;
	}

	bool VulkanImage::FindSupportedFormat(VulkanDevice* gpu, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format)
	{
		if (gpu == nullptr) return false;

		for (VkFormat format_ : candidates) {
			VkFormatProperties props;
			vkCheckErrorVoid(vkGetPhysicalDeviceFormatProperties(gpu->gpu, format_, &props));

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				*format = format_;
				return true;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				*format = format_;
				return true;
			}
		}
		Log_nts.Error("failed to find supported format!");
		return false;
	}

	bool VulkanImage::FindDepthFormat(VulkanDevice* gpu, VkFormat* format)
	{
		return FindSupportedFormat(gpu, { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, format);
	}

	// Transition image layout to a specific stage
	void VulkanImage::TransitionImageLayout(VulkanDevice* gpu, VkCommandPool commandPool, VkQueue graphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout) {
		if (gpu == nullptr) return;

		VkCommandBuffer commandBuffer = gpu->BeginSingleTimeCommands(commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (HasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			//barrier.srcAccessMask = VK_IMAGE_LAYOUT_GENERAL;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCheckErrorVoid(vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		));

		gpu->EndSingleTimeCommands(commandBuffer, commandPool);

		layout = newLayout;
	}

	// Helper methods
	uint32 VulkanImage::FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkCheckErrorVoid(vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties));

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		Log_nts.Error("failed to find suitable memory type!");
		return 0;
	}

	bool VulkanImage::HasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	VkSampler VulkanImage::CreateTextureSampler(VulkanDevice* gpu, bool repeate, bool smooth)
	{
		Log_nts.Warning();
		VkSamplerCreateInfo samplerInfo;
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		{
			if (smooth) {
				samplerInfo.magFilter = VK_FILTER_LINEAR;// vk::Filter::eLinear;
				samplerInfo.minFilter = VK_FILTER_LINEAR;// vk::Filter::eLinear;
				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;// vk::SamplerMipmapMode::eLinear;
			}
			else {
				samplerInfo.magFilter = VK_FILTER_NEAREST;// vk::Filter::eNearest;
				samplerInfo.minFilter = VK_FILTER_NEAREST; // vk::Filter::eNearest;
				samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST; // vk::SamplerMipmapMode::eNearest;
			}
			if (repeate) {
				samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; // vk::SamplerAddressMode::eRepeat;
				samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; // vk::SamplerAddressMode::eRepeat;
				samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT; // vk::SamplerAddressMode::eRepeat;
			}
			else {
				samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // vk::SamplerAddressMode::eClampToEdge;
				samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // vk::SamplerAddressMode::eClampToEdge;
				samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // vk::SamplerAddressMode::eClampToEdge;
			}

			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 16.0f;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;// vk::CompareOp::eAlways;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;// vk::BorderColor::eIntOpaqueBlack;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
		}
		VkSampler sampler;
		VulkanResult result;
		bool first = true;
		Log_nts.Warning();
		vkCheckError(first, result, vkCreateSampler(gpu->device, &samplerInfo, nullptr, &sampler), "cannote create sampler texture");
		Log_nts.Warning();
		return result.success ? sampler : nullptr;
	}

}  //  nkentseu