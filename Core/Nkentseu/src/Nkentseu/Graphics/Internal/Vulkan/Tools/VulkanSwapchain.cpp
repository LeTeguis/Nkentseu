//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:00:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanSwapchain.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
	using namespace maths;
    
	bool VulkanSwapchain::Create(VulkanDevice* gpu, VulkanSurface* surface, const Vector2u& size, const ContextProperties& contextProperties) {
		if (gpu == nullptr || surface == nullptr) return false;

		VulkanResult result;
		bool first = true;

		uint32 formatCount = 0;
		VkSurfaceFormatKHR surfaceFormats[10];
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, 0), "cannot count device surface format");
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, surfaceFormats), "cannot get device surface format");

		const std::vector<VkFormat> requestSurfaceImageFormat = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };

		for (uint32 indexFormat = 0; indexFormat < formatCount; indexFormat++) {
			VkSurfaceFormatKHR currentFormat = surfaceFormats[indexFormat];

			for (uint32 indexRequier = 0; indexRequier < requestSurfaceImageFormat.size(); indexRequier++) {
				if (currentFormat.format == requestSurfaceImageFormat[indexRequier] && currentFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
					surfaceFormat = currentFormat;
					break;
				}
			}
		}

		presentMode = SelectPresentMode(gpu, surface, contextProperties);

		VkSurfaceCapabilitiesKHR surfaceCaps = {};
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->gpu, surface->surface, &surfaceCaps), "cannot get surface capabilities khr");

		uint32 imageCount = GetMinImageCountFromPresentMode(presentMode);

		if (imageCount < surfaceCaps.minImageCount) {
			imageCount = surfaceCaps.minImageCount;
		}
		else if (surfaceCaps.maxImageCount != 0 && imageCount > surfaceCaps.maxImageCount) {
			imageCount = surfaceCaps.maxImageCount;
		}

		VkExtent2D extend = {};

		if (surfaceCaps.currentExtent.width == 0xffffffff) {
			extend.width = size.x;
			extend.height = size.y;
		}
		else {
			extend = surfaceCaps.currentExtent;
		}

		std::vector<uint32> queueFamilyIndices;

		if (gpu->queue.queueFamily.graphicsIndex >= 0)
			queueFamilyIndices.push_back(gpu->queue.queueFamily.graphicsIndex);

		if (gpu->queue.queueFamily.presentIndex >= 0 && gpu->queue.queueFamily.presentIndex != gpu->queue.queueFamily.graphicsIndex)
			queueFamilyIndices.push_back(gpu->queue.queueFamily.presentIndex);

		VkSharingMode sharingMode;
		if (queueFamilyIndices.size() > 1) {
			sharingMode = VK_SHARING_MODE_CONCURRENT;
		}
		else {
			sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.surface = surface->surface;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.preTransform = surfaceCaps.currentTransform;
		swapchainInfo.imageExtent = extend;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.queueFamilyIndexCount = queueFamilyIndices.size();
		swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		swapchainInfo.imageSharingMode = sharingMode;

		vkCheckError(first, result, vkCreateSwapchainKHR(gpu->device, &swapchainInfo, 0, &swapchain), "cannot create swapchain");

		if (result.success) {
			Log_nts.Info("Create vulkan swapchain is good");
		}
		uint32 scImageCount = 0;
		VkImage scImages[16];

		vkCheckError(first, result, vkGetSwapchainImagesKHR(gpu->device, swapchain, &scImageCount, 0), "cannot count swapchain image khr");
		vkCheckError(first, result, vkGetSwapchainImagesKHR(gpu->device, swapchain, &scImageCount, scImages), "cannot getswapchain image khr");

		imageView.resize(scImageCount);

		VkImageSubresourceRange imageRange = {};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.layerCount = 1;
		imageRange.levelCount = 1;

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.format = surfaceFormat.format;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // VK_IMAGE_VIEW_TYPE_3D, VK_IMAGE_VIEW_TYPE_CUBE_MAP
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.levelCount = 1;
		//viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		//viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		//viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		//viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.subresourceRange = imageRange;

		for (uint32 index = 0; index < scImageCount; index++) {
			swapchainImages.push_back(scImages[index]);
			viewInfo.image = swapchainImages[index];

			vkCheckError(first, result, vkCreateImageView(gpu->device, &viewInfo, 0, &imageView[index]), "cannot create image view number {0}", index);
		}

		return result.success;
	}

	bool VulkanSwapchain::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		for (usize i = 0; i < imageView.size(); i++) {
			if (imageView[i] != nullptr) {
				vkCheckErrorVoid(vkDestroyImageView(gpu->device, imageView[i], nullptr));
				imageView[i] = VK_NULL_HANDLE;
			}
		}

		imageView.clear();
		swapchainImages.clear();

		if (swapchain != nullptr) {
			vkCheckErrorVoid(vkDestroySwapchainKHR(gpu->device, swapchain, nullptr));
			swapchain = VK_NULL_HANDLE;
		}
		swapchain = nullptr;
		return true;
	}

	bool VulkanSwapchain::FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format)
	{
		for (VkFormat format_ : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device, format_, &props);

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

	uint32 VulkanSwapchain::GetMinImageCountFromPresentMode(VkPresentModeKHR present_mode)
	{
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			return 3;
		if (present_mode == VK_PRESENT_MODE_FIFO_KHR || present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
			return 2;
		if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			return 1;
		return 1;
	}

	VkPresentModeKHR VulkanSwapchain::SelectPresentMode(VulkanDevice* gpu, VulkanSurface* surface, const ContextProperties& contextProperties)
	{
		if (gpu == nullptr || surface == nullptr) return VK_PRESENT_MODE_FIFO_KHR;

		uint32 availCount = 0;
		std::vector<VkPresentModeKHR> availPresentModes;
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->gpu, surface->surface, &availCount, nullptr);
		availPresentModes.resize(availCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->gpu, surface->surface, &availCount, availPresentModes.data());

		std::vector<VkPresentModeKHR> requestPresentMode;
		if (contextProperties.pixelFormat.flags & GraphicsFlag::DoubleBuffer) {
			requestPresentMode.push_back(VK_PRESENT_MODE_FIFO_KHR);
			requestPresentMode.push_back(VK_PRESENT_MODE_FIFO_RELAXED_KHR);
		}
		else if (contextProperties.pixelFormat.flags & GraphicsFlag::TripleBuffer) {
			requestPresentMode.push_back(VK_PRESENT_MODE_MAILBOX_KHR);
		}
		else {
			requestPresentMode.push_back(VK_PRESENT_MODE_IMMEDIATE_KHR);
		}

		for (uint32 request_i = 0; request_i < requestPresentMode.size(); request_i++) {
			for (uint32 avail_i = 0; avail_i < availPresentModes.size(); avail_i++) {
				if (requestPresentMode[request_i] == availPresentModes[avail_i]) {
					return requestPresentMode[request_i];
				}
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

}  //  nkentseu