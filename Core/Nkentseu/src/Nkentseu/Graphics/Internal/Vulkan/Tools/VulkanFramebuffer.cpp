//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:08:56 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanFramebuffer.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
	using namespace maths;
    
	bool VulkanFramebuffer::Create(VulkanDevice* gpu, const Vector2u& size, VulkanRenderPass* renderPass, VulkanSwapchain* swapchain, bool depth)
	{
		if (gpu == nullptr || !gpu->IsValid() || renderPass == nullptr || swapchain == nullptr) return false;

		VulkanResult result;
		bool first = true;

		useDepth = depth;

		this->size = size;

		if (useDepth) {
			if (depthImage.Create(gpu, size, VulkanImage::ImageType::DEPTH)) {
				Log_nts.Info("Create swapchain is good");
			}
		}

		framebuffer.resize(swapchain->swapchainImages.size());
		//Log_nts.Warning("{0}", framebuffer.size());

		std::vector<VkImageView> attachments = {};
		if (useDepth) {
			attachments.resize(2);
			attachments[1] = depthImage.imageView;
		}
		else {
			attachments.resize(1);
		}

		if (renderPass->renderPass == nullptr) {
			Log_nts.Fatal();
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.width = this->size.width;
		framebufferInfo.height = this->size.height;
		framebufferInfo.renderPass = renderPass->renderPass;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = attachments.size();

		for (uint32 index = 0; index < swapchain->swapchainImages.size(); index++) {
			attachments[0] = swapchain->imageView[index];
			framebufferInfo.pAttachments = attachments.data();

			vkCheckError(first, result, vkCreateFramebuffer(gpu->device, &framebufferInfo, 0, &framebuffer[index]), "cannot create frame buffer number {0}", index);
		}

		if (result.success) {
			Log_nts.Info("Create framebuffer is good");
		}

		return result.success;
	}

	bool VulkanFramebuffer::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		for (usize i = 0; i < framebuffer.size(); i++) {
			vkCheckErrorVoid(vkDestroyFramebuffer(gpu->device, framebuffer[i], nullptr));
			framebuffer[i] = VK_NULL_HANDLE;
		}

		if (useDepth) {
			depthImage.Destroy(gpu);
		}

		framebuffer.clear();
		size = Vector2u();
		return true;
	}

}  //  nkentseu