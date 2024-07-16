//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:31 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanPipelineLayout.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

namespace nkentseu {
    
    bool VulkanPipelineLayout::Create(VulkanDevice* gpu, VulkanSwapchain* swapchain)
    {
        if (!gpu) return false;
        VulkanResult result;
        bool first = true;

        if (!CreateDescriptorSetLayout(gpu, swapchain)) {
            return false;
        }

        std::vector<VkDescriptorSetLayout> layouts;
        for (const auto& [bst, layout] : descriptorSetLayouts) {
            layouts.push_back(layout);
            Log_nts.Debug();
        }

        VkPipelineLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
        layoutInfo.pSetLayouts = layouts.data();

        if (!pushConstantRanges.empty()) {
            layoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
            layoutInfo.pPushConstantRanges = pushConstantRanges.data();
        }

        vkCheckError(first, result, vkCreatePipelineLayout(gpu->device, &layoutInfo, nullptr, &pipelineLayout), "cannot create pipeline layout");

        if (result.success) {
            Log_nts.Info("Pipeline layout created successfully");
        }

        return result.success;
    }

    bool VulkanPipelineLayout::Destroy(VulkanDevice* gpu)
    {
        if (!gpu || !gpu->device) return false;

        for (auto& [bst, layout] : descriptorSetLayouts) {
            if (layout != VK_NULL_HANDLE) {
                vkDestroyDescriptorSetLayout(gpu->device, layout, nullptr);
                layout = VK_NULL_HANDLE;
            }
        }

        if (pipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(gpu->device, pipelineLayout, nullptr);
            pipelineLayout = VK_NULL_HANDLE;
        }

        return true;
    }

    bool VulkanPipelineLayout::CreateDescriptorSetLayout(VulkanDevice* gpu, VulkanSwapchain* swapchain)
    {
        if (!gpu || !swapchain) return false;

        VulkanResult result;
        bool first = true;

        for (const auto& [bst, bindings] : layoutBindings) {
            if (bindings.empty()) continue;

            VkDescriptorSetLayoutCreateInfo layoutDescriptorInfo{};
            layoutDescriptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutDescriptorInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutDescriptorInfo.pBindings = bindings.data();

            //VkDescriptorSetLayout layout;
            descriptorSetLayouts[bst] = nullptr;
            vkCheckError(first, result, vkCreateDescriptorSetLayout(gpu->device, &layoutDescriptorInfo, nullptr, &descriptorSetLayouts[bst]), "cannot create descriptor set layout");

            if (!result.success) return false;

            //descriptorSetLayouts[bst] = layout;
        }

        return true;
    }

    void VulkanPipelineLayout::Add(BufferSpecificationType::Enum bst, uint32 binding, VkDescriptorType type, VkShaderStageFlags shaderStage)
    {
        VkDescriptorSetLayoutBinding layoutBinding = {};
        layoutBinding.binding = binding;
        layoutBinding.descriptorType = type;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = shaderStage;
        layoutBinding.pImmutableSamplers = nullptr;

        layoutBindings[bst].push_back(layoutBinding);
        //layoutBindings[bst].push_back({ binding, type, 1, shaderStage, nullptr });
    }

    void VulkanPipelineLayout::AddPushConstantRange(const VkPushConstantRange& pushConstantRange)
    {
        pushConstantRanges.push_back(pushConstantRange);
    }

    bool VulkanPipelineLayout::IsValid() const
    {
        return pipelineLayout != VK_NULL_HANDLE;
    }

}  //  nkentseu