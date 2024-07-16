//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:13:31 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_PIPELINE_LAYOUT_H__
#define __VULKAN_PIPELINE_LAYOUT_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <Nkentseu/Graphics/ShaderInputLayout.h>

#include <unordered_map>

namespace nkentseu {
    
    struct NKENTSEU_API VulkanPipelineLayout {
        bool Create(VulkanDevice* gpu, VulkanSwapchain* swapchain);
        bool Destroy(VulkanDevice* gpu);

        bool CreateDescriptorSetLayout(VulkanDevice* gpu, VulkanSwapchain* swapchain);

        void Add(BufferSpecificationType::Enum bst, uint32 binding, VkDescriptorType type, VkShaderStageFlags shaderStage);
        void AddPushConstantRange(const VkPushConstantRange& pushConstantRange);

        bool IsValid() const;

        std::unordered_map<BufferSpecificationType::Enum, std::vector<VkDescriptorSetLayoutBinding>> layoutBindings{};
        std::vector<VkPushConstantRange> pushConstantRanges{};

        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        std::unordered_map<BufferSpecificationType::Enum, VkDescriptorSetLayout> descriptorSetLayouts{};
    };

}  //  nkentseu

#endif  // __VULKAN_PIPELINE_LAYOUT_H__!