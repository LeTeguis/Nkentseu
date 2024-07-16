//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:30:26 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanInternal.h"

/*
#if defined(XWIN_WIN32)
#elif defined(XWIN_COCOA)
#define VK_USE_PLATFORM_MACOS_MVK
#elif defined(XWIN_UIKIT)
#define VK_USE_PLATFORM_IOS_MVK
#elif defined(XWIN_XCB)
#elif defined(XWIN_ANDROID)
#define VK_USE_PLATFORM_ANDROID_KHR
#endif
*/

#ifdef NKENTSEU_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include "Nkentseu/Platform/Window/Windows/Win32Window.h"
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan_xlib.h>
#include "Nkentseu/Platform/Window/Linux/XLIB/WindowInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan_xcb.h>
#include "Nkentseu/Platform/Window/Linux/XCB/WindowInternal.h"
#endif

#include <Logger/Formatter.h>

#include "Nkentseu/Core/Window.h"
#include "Nkentseu/Platform/PlatformState.h"
#include "VulkanUtils.h"

namespace nkentseu {
	using namespace maths;
	
	/*/ VulkanSemaphore
	

	// VulkanPipelineConfig
	void VulkanPipelineConfig::DefaultPipelineConfig(VulkanPipelineConfig* configInfo){
		if (configInfo == nullptr) return;
		configInfo->inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo->inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo->inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		configInfo->inputAssemblyInfo.pNext = nullptr;
		configInfo->inputAssemblyInfo.flags = 0;

		configInfo->viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo->viewportInfo.viewportCount = 1;
		configInfo->viewportInfo.pViewports = nullptr;
		configInfo->viewportInfo.scissorCount = 1;
		configInfo->viewportInfo.pScissors = nullptr;

		configInfo->rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo->rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo->rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo->rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo->rasterizationInfo.lineWidth = 1.0f;
		configInfo->rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo->rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo->rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo->rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo->rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo->rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		configInfo->multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo->multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo->multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo->multisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo->multisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo->multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo->multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		configInfo->colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo->colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo->colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo->colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo->colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo->colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo->colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo->colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo->colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo->colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo->colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo->colorBlendInfo.attachmentCount = 1;
		configInfo->colorBlendInfo.pAttachments = &configInfo->colorBlendAttachment;
		configInfo->colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo->colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo->colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo->colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		configInfo->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo->depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo->depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo->depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo->depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo->depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo->depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo->depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo->depthStencilInfo.front = {};  // Optional
		configInfo->depthStencilInfo.back = {};   // Optional

		configInfo->dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo->dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo->dynamicStateInfo.pDynamicStates = configInfo->dynamicStateEnables.data();
		configInfo->dynamicStateInfo.dynamicStateCount = static_cast<uint32>(configInfo->dynamicStateEnables.size());
		configInfo->dynamicStateInfo.flags = 0;
		configInfo->dynamicStateInfo.pNext = nullptr; // a chercher

		configInfo->bindingDescriptions = VulkanDefaultVertex::GetBindingDescriptions();
		configInfo->attributeDescriptions = VulkanDefaultVertex::GetAttributeDescriptions();
	}

	void VulkanPipelineConfig::EnableAlphaBlending(VulkanPipelineConfig* configInfo){
		if (configInfo == nullptr) return;

		configInfo->colorBlendAttachment.blendEnable = VK_TRUE;
		configInfo->colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo->colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		configInfo->colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		configInfo->colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo->colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo->colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo->colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	}

	// VulkanDefaultVertex
	std::vector<VkVertexInputBindingDescription> VulkanDefaultVertex::GetBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(VulkanDefaultVertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanDefaultVertex::GetAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanDefaultVertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanDefaultVertex, color) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanDefaultVertex, normal) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VulkanDefaultVertex, uv) });

		return attributeDescriptions;
	}

	// VulkanRenderPass
	

	// VulkanFramebuffer
	

	// vulkan descriptor pool

	

	// VulkanPipelineLayout
	

	// vulkan buffer


	// uniform buffer

	

	// command buffer
	

	// vulkan texture
	bool VulkanTexture::Create(VulkanDevice* gpu, VulkanCommandPool* commandPool, const maths::Vector2u& size, TextureFormat::Code format) {
		if (gpu == nullptr) return false;

		mipLevels = static_cast<uint32_t>(std::floor(std::log2(maths::Max(size.x, size.y))) + 1);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = size.x;
		imageInfo.extent.height = size.y;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = GetVulkanFormat(format);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(gpu->device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(gpu->device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = gpu->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(gpu->device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate image memory!");
		}

		vkBindImageMemory(gpu->device, image, imageMemory, 0);

		CreateImageView(gpu, format);

		return true;
	}

	void VulkanTexture::Destroy(VulkanDevice* gpu) {
		if (gpu == nullptr) return;

		if (imageView != VK_NULL_HANDLE) {
			vkDestroyImageView(gpu->device, imageView, nullptr);
			imageView = VK_NULL_HANDLE;
		}

		if (image != VK_NULL_HANDLE) {
			vkDestroyImage(gpu->device, image, nullptr);
			image = VK_NULL_HANDLE;
		}

		if (imageMemory != VK_NULL_HANDLE) {
			vkFreeMemory(gpu->device, imageMemory, nullptr);
			imageMemory = VK_NULL_HANDLE;
		}
	}

	void VulkanTexture::Update(VulkanDevice* gpu, VulkanCommandPool* commandPool, const uint8_t* pixels, const maths::Vector2u& size) {
		Update(gpu, commandPool, pixels, size, { 0, 0 });
	}

	void VulkanTexture::Update(VulkanDevice* gpu, VulkanCommandPool* commandPool, const uint8_t* pixels, const maths::Vector2u& size, const maths::Vector2i& offset) {

		VkDeviceSize imageSize = size.x * size.y * 4;
		if (gpu == nullptr || pixels == nullptr || imageSize == 0) {
			if (pixels == nullptr) {
				Log_nts.Fatal();
			}
			else if (imageSize == 0) {
				Log_nts.Fatal();
			}
			return;
		}

		VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VulkanBuffer stanging;

		stanging.CreateBuffer(gpu, imageSize, usage, sharingMode, propertyFlags, stanging.buffer, stanging.bufferMemory);

		void* data;
		vkMapMemory(gpu->device, stanging.bufferMemory, 0, imageSize, 0, &data);
		std::memcpy(data, pixels, static_cast<usize>(imageSize));
		vkUnmapMemory(gpu->device, stanging.bufferMemory);

		TransitionImageLayout(gpu, commandPool, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(gpu, commandPool, stanging.buffer, size, offset);

		vkDestroyBuffer(gpu->device, stanging.buffer, nullptr);
		vkFreeMemory(gpu->device, stanging.bufferMemory, nullptr);

		TransitionImageLayout(gpu, commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void VulkanTexture::TransitionImageLayout(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkImageLayout oldLayout, VkImageLayout newLayout) {
		if (gpu == nullptr || commandPool == nullptr) return;

		VkCommandBuffer commandBuffer = gpu->BeginSingleTimeCommands(commandPool->commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
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
			throw std::invalid_argument("Unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		gpu->EndSingleTimeCommands(commandBuffer, commandPool->commandPool);
	}

	void VulkanTexture::CopyBufferToImage(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkBuffer buffer, const maths::Vector2u& size, const maths::Vector2i& offset) {

		if (gpu == nullptr || commandPool == nullptr) return;

		VkCommandBuffer commandBuffer = gpu->BeginSingleTimeCommands(commandPool->commandPool);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { offset.x, offset.y, 0 };
		region.imageExtent = { size.x, size.y, 1 };

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		gpu->EndSingleTimeCommands(commandBuffer, commandPool->commandPool);
	}

	void VulkanTexture::GenerateMipmap(VulkanDevice* gpu, VulkanCommandPool* commandPool, const maths::Vector2u& size) {

		if (gpu == nullptr || commandPool == nullptr) return;
		VkCommandBuffer commandBuffer = gpu->BeginSingleTimeCommands(commandPool->commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32 mipWidth = size.x;
		int32 mipHeight = size.y;

		for (uint32 i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		gpu->EndSingleTimeCommands(commandBuffer, commandPool->commandPool);
	}

	VkFormat VulkanTexture::GetVulkanFormat(TextureFormat::Code format) {
		switch (format) {
		case TextureFormat::RGBA8:
			return VK_FORMAT_R8G8B8A8_UNORM;//  ::eR8G8B8A8Unorm;
		case TextureFormat::RGB8:
			return VK_FORMAT_R8G8B8_UNORM; // vk::Format::eR8G8B8Unorm;
		case TextureFormat::SRGB8_ALPHA8:
			return VK_FORMAT_R8G8B8A8_SRGB; // vk::Format::eR8G8B8A8Srgb;
		case TextureFormat::RED_INTEGER:
			return VK_FORMAT_R8_UINT; // vk::Format::eR8Uint;
		case TextureFormat::DEPTH_COMPONENT16:
			return VK_FORMAT_D16_UNORM; // vk::Format::eD16Unorm;
		case TextureFormat::DEPTH_COMPONENT24:
			return VK_FORMAT_X8_D24_UNORM_PACK32; // vk::Format::eX8D24UnormPack32;
		case TextureFormat::DEPTH_COMPONENT32F:
			return VK_FORMAT_D32_SFLOAT; // vk::Format::eD32Sfloat;
		case TextureFormat::STENCIL_INDEX8:
			return VK_FORMAT_S8_UINT; // vk::Format::eS8Uint;
		case TextureFormat::DEPTH24_STENCIL8:
			return VK_FORMAT_D24_UNORM_S8_UINT;// vk::Format::eD24UnormS8Uint;
		case TextureFormat::DEPTH32F_STENCIL8:
			return VK_FORMAT_D32_SFLOAT_S8_UINT; // vk::Format::eD32SfloatS8Uint;
		default:
			return VK_FORMAT_UNDEFINED;
		}
	}

	void VulkanTexture::CreateImageView(VulkanDevice* gpu, TextureFormat::Code format) {
		if (gpu == nullptr) return;

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = GetVulkanFormat(format);
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(gpu->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create texture image view!");
		}
	}*/
}  //  nkentseu