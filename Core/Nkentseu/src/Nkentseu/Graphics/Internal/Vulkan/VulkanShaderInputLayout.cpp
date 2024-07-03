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

        if (uniformInput.attributes.size() > 0) {
            for (auto& attribut : uniformInput.attributes) {
                VkDescriptorType dtype;
                if (attribut.usage == BufferUsageType::StaticDraw) {
                    dtype = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                }
                else if (attribut.usage == BufferUsageType::DynamicDraw) {
                    dtype = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
                }
                else if (attribut.usage == BufferUsageType::StreamDraw) {
                    dtype = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                }

                m_PipelineLayout.Add(attribut.binding, dtype, VulkanConvert::ShaderStageToVkShaderStage(attribut.stage));
            }
        }

        if (samplerInput.attributes.size() > 0) {
            for (auto& attribut : samplerInput.attributes) {
                VkDescriptorType dtype;
                if (attribut.type == SamplerType::CombineImage) {
                    dtype = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                }
                else if (attribut.type == SamplerType::SamplerImage) {
                    dtype = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                }
                else if (attribut.type == SamplerType::StorageImage) {
                    dtype = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                }
                m_PipelineLayout.Add(attribut.binding, dtype, VulkanConvert::ShaderStageToVkShaderStage(attribut.stage));
            }
        }

        if (pushConstantInput.attributes.size() > 0) {
            for (const auto& attribut : pushConstantInput.attributes) {
                VkPushConstantRange pushConstantRange{};
                pushConstantRange.stageFlags = VulkanConvert::ShaderStageToVkShaderStage(attribut.stage);
                pushConstantRange.offset = attribut.offset;
                pushConstantRange.size = attribut.size;

                m_PipelineLayout.AddPushConstantRange(pushConstantRange);
            }
        }

        if (!m_PipelineLayout.Create(m_Context->GetGpu(), m_Context->GetSwapchain())) {
            Log_nts.Debug("Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
            return false;
        }

        return true;
    }

    bool VulkanShaderInputLayout::Release()
    {
        // Release Vulkan resources
        m_PipelineLayout.Destroy(m_Context->GetGpu());
        return ShaderInputLayout::Release();
    }

    bool VulkanShaderInputLayout::UpdatePushConstant(const std::string& name, void* data, usize size, Memory::Shared<Shader> shader) {
        // Validate Vulkan context and pipeline layout
        if (!m_Context || !m_PipelineLayout.IsValid() || !m_Context->GetCurrentCommandBuffer()) {
            return false;
        }

        // Find the push constant attribute by name
        const PushConstantInputAttribute* attribute = nullptr;
        for (const auto& attr : pushConstantInput.attributes) {
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

        VulkanResult result;
        bool first = true;

        vkCheckErrorVoid(vkCmdPushConstants(
            m_Context->GetCurrentCommandBuffer(),
            m_PipelineLayout.pipelineLayout,
            VulkanConvert::ShaderStageToVkShaderStage(attribute->stage),
            0, //attribute->offset,
            size,
            data
        ));
        return true;
    }

}  //  nkentseu