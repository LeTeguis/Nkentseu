//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-29 at 10:08:47 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanShaderInputLayout.h"
#include <Logger/Formatter.h>

#include "VulkanUtils.h"

namespace nkentseu {
    
    VulkanShaderInputLayout::VulkanShaderInputLayout(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
    }

    VulkanShaderInputLayout::~VulkanShaderInputLayout() {
    }
    bool VulkanShaderInputLayout::Initialize()
    {
        if (m_Context == nullptr || !ShaderInputLayout::Initialize()) {
            return false;
        }

        // Créer les bindings de descripteur pour les uniform inputs
        for (auto& attribut : uniformInput.attributes) {
            vk::DescriptorSetLayoutBinding layoutBinding = {};
            layoutBinding.binding = attribut.binding;
            layoutBinding.descriptorType = VulkanConvert::BufferUsageAttributType(attribut.usage);
            layoutBinding.descriptorCount = 1;
            layoutBinding.stageFlags = VulkanConvert::ShaderStageToVkShaderStage(attribut.stage);
            layoutBinding.pImmutableSamplers = nullptr;

            layoutBindings[BufferSpecificationType::Enum::Uniform].push_back(layoutBinding);
            sets[BufferSpecificationType::Enum::Uniform] = attribut.set;
        }

        // Créer les bindings de descripteur pour les sampler inputs
        for (auto& attribut : samplerInput.attributes) {
            vk::DescriptorSetLayoutBinding layoutBinding = {};
            layoutBinding.binding = attribut.binding;
            layoutBinding.descriptorType = VulkanConvert::SamplerInputAttributType(attribut.type);
            layoutBinding.descriptorCount = 1;
            layoutBinding.stageFlags = VulkanConvert::ShaderStageToVkShaderStage(attribut.stage);
            layoutBinding.pImmutableSamplers = nullptr;
            
            layoutBindings[BufferSpecificationType::Enum::Texture].push_back(layoutBinding);
            sets[BufferSpecificationType::Enum::Texture] = attribut.set;
        }

        // Créer les push constant ranges
        for (auto& attribut : pushConstantInput.attributes) {
            vk::PushConstantRange pushConstantRange = {};
            pushConstantRange.stageFlags = VulkanConvert::ShaderStageToVkShaderStage(attribut.stage);
            pushConstantRange.offset = attribut.offset;
            pushConstantRange.size = attribut.size;

            pushConstantRanges.push_back(pushConstantRange);
        }

        // Créer le pipeline layout
        if (!CreatePipelineLayout()) {
            Log_nts.Error("cannot create pipeline layout");
            return false;
        }

        return true;
    }

    bool VulkanShaderInputLayout::Release()
    {
        // Release Vulkan resources
        DestroyPipelineLayout();
        return ShaderInputLayout::Release();
    }

    bool VulkanShaderInputLayout::UpdatePushConstant(const std::string& name, void* data, usize size, Memory::Shared<Shader> shader) {
        // Valider le contexte Vulkan et le pipeline layout
        if (m_Context == nullptr || pipelineLayout == nullptr) {
            return false;
        }

        // Find the push constant attribute by name
        const PushConstantInputAttribute* attribute = nullptr;
        for (auto& attr : pushConstantInput.attributes) {
            if (attr.name == name) {
                attribute = &attr;
                break;
            }
        }

        if (!attribute) {
            // Push constant attribute with the given name not found
            return false;
        }

        // Validate the size of data matches the expected size
        if (size != attribute->size) {
            // Size mismatch
            return false;
        }

        vk::CommandBuffer currentCommandBuffer = m_Context->GetCommandBuffer();

        vkCheckErrorVoid(currentCommandBuffer.pushConstants(
            pipelineLayout,
            VulkanConvert::ShaderStageToVkShaderStage(attribute->stage),
            0, //attribute->offset,
            size,
            data
        ));
        return true;
    }

    bool VulkanShaderInputLayout::CreatePipelineLayout() {
        if (m_Context == nullptr) return false;
        VulkanResult result;
        bool first = true;

        if (!CreateDescriptorSetLayout()) {
            Log_nts.Error("cannot create descriptor set layout");
            return false;
        }
        uint32 index = 0;
        std::vector<vk::DescriptorSetLayout> layouts;
        layouts.resize(descriptorSetLayouts.size());
        for (auto& [bst, layout] : descriptorSetLayouts) {
            if (layout != nullptr) {
                layouts[sets[bst]] = layout;
                Log_nts.Debug("indice = {0}", sets[bst]);
                /*layouts.push_back(layout);
                Log_nts.Info("DescriptorSetLayout added to PipelineLayout for BufferSpecificationType : {0}", BufferSpecificationType(bst));

                if (bst == BufferSpecificationType::Enum::Uniform) {
                    layouts[]
                    uniformInputIndex = index;
                } else if (bst == BufferSpecificationType::Enum::Texture) {
                    samplerInputIndex = index;
                }
                index++;*/
            }
        }
        vk::PipelineLayoutCreateInfo layoutInfo = {};

        if (layouts.size() > 0) {
            layoutInfo.setLayoutCount = (uint32)layouts.size();
            layoutInfo.pSetLayouts = layouts.data();
        }
        else {
            Log_nts.Fatal();
        }

        if (!pushConstantRanges.empty()) {
            layoutInfo.pushConstantRangeCount = static_cast<uint32>(pushConstantRanges.size());
            layoutInfo.pPushConstantRanges = pushConstantRanges.data();
        }

        vk::PipelineLayout pipelineLayout = nullptr;
        vkCheckError(first, result, m_Context->device.createPipelineLayout(&layoutInfo, m_Context->allocator, &pipelineLayout), "cannot create pipeline layout");

        if (result.success && pipelineLayout != VK_NULL_HANDLE) {
            this->pipelineLayout = pipelineLayout;
            Log_nts.Info("Pipeline layout created successfully");
            return true;
        }

        return false;
    }

    bool VulkanShaderInputLayout::DestroyPipelineLayout()
    {
        if (m_Context == nullptr) return false;

        for (auto& [bst, layout] : descriptorSetLayouts) {
            if (layout != VK_NULL_HANDLE) {
                vkCheckErrorVoid(m_Context->device.destroyDescriptorSetLayout(layout, m_Context->allocator));
                layout = VK_NULL_HANDLE;
            }
        }

        if (pipelineLayout != VK_NULL_HANDLE) {
            vkCheckErrorVoid(m_Context->device.destroyPipelineLayout(pipelineLayout, m_Context->allocator));
            pipelineLayout = VK_NULL_HANDLE;
        }

        return true;
    }

    bool VulkanShaderInputLayout::CreateDescriptorSetLayout() {
        if (m_Context == nullptr) return false;

        VulkanResult result;
        bool first = true;

        for (const auto& [bst, bindings] : layoutBindings) {
            if (bindings.empty()) continue;

            vk::DescriptorSetLayoutCreateInfo layoutDescriptorInfo{};
            layoutDescriptorInfo.bindingCount = static_cast<uint32>(bindings.size());
            layoutDescriptorInfo.pBindings = bindings.data();

            vk::DescriptorSetLayout descriptorSetLayout = nullptr;
            vkCheckError(first, result, m_Context->device.createDescriptorSetLayout(&layoutDescriptorInfo, m_Context->allocator, &descriptorSetLayout), "cannot create descriptor set layout");

            if (result.success && descriptorSetLayout != VK_NULL_HANDLE) {
                descriptorSetLayouts[bst] = descriptorSetLayout;
                Log_nts.Info("DescriptorSetLayout created successfully for BufferSpecificationType {0}", BufferSpecificationType(bst));
            }
        }
        return true;
    }

    bool VulkanShaderInputLayout::IsValid() const
    {
        return pipelineLayout != VK_NULL_HANDLE;
    }

}  //  nkentseu
