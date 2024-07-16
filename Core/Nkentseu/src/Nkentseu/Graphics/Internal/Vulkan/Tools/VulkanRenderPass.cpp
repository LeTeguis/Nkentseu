//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 01:12:19 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanRenderPass.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {

	bool VulkanRenderPass::Create(VulkanDevice* gpu, VulkanSwapchain* swapchain)
	{
		if (gpu == nullptr || swapchain == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.format = swapchain->surfaceFormat.format;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		VkAttachmentDescription depthAttachment = {};
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		VulkanImage::FindDepthFormat(gpu, &depthAttachment.format);
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentRef;
		subpassDescription.pDepthStencilAttachment = &depthAttachmentRef;

		std::vector<VkAttachmentDescription> attachments;
		attachments.push_back(colorAttachment);
		attachments.push_back(depthAttachment);

		std::vector<VkSubpassDescription> subpassDescriptions;
		subpassDescriptions.push_back(subpassDescription);

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = VK_ACCESS_NONE;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		std::vector<VkSubpassDependency> subpassDependencies;
		subpassDependencies.push_back(dependency);

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.attachmentCount = attachments.size();
		renderPassInfo.pSubpasses = subpassDescriptions.data();
		renderPassInfo.subpassCount = subpassDescriptions.size();
		renderPassInfo.pDependencies = subpassDependencies.data();
		renderPassInfo.dependencyCount = subpassDependencies.size();

		vkCheckError(first, result, vkCreateRenderPass(gpu->device, &renderPassInfo, 0, &renderPass), "cannot create render pass");

		if (result.success) {
			Log_nts.Info("Create render pass is good");
		}

		return result.success;
	}

	bool VulkanRenderPass::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr || renderPass == nullptr) return false;
		vkCheckErrorVoid(vkDestroyRenderPass(gpu->device, renderPass, nullptr));
		renderPass = VK_NULL_HANDLE;
		return true;
	}

}  //  nkentseu