//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-03 at 08:33:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanUniformBuffer.h"
#include <Logger/Formatter.h>
#include "VulkanUtils.h"

namespace nkentseu {
    
    VulkanUniformBuffer::VulkanUniformBuffer(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout) : m_Context(Memory::SharedCast<VulkanContext>(context)), m_BufferLayout(uniformLayout), m_Shader(Memory::SharedCast<VulkanShader>(shader)){
        if (m_Context != nullptr) {
            m_Context->AddRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanUniformBuffer::Recreate));
            m_Context->AddCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanUniformBuffer::CleanUp));
        }
    }

    VulkanUniformBuffer::~VulkanUniformBuffer() {
    }

    bool VulkanUniformBuffer::Create()
    {
        if (m_Context == nullptr) return false;

        if (m_BufferLayout.attributes.size() > 0) {
            std::vector<UniformBufferType::Code> dTypesList = {};
            for (auto& attribut : m_BufferLayout.attributes) {
                if (std::find(dTypesList.begin(), dTypesList.end(), attribut.uType) == dTypesList.end()) {
                    dTypesList.push_back(attribut.uType);

                    VkDescriptorType dtype = (attribut.uType == UniformBufferType::Static) ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

                    m_DescriptorPool.Add(dtype, m_Context->GetSwapchain()->swapchainImages.size());
                }
            }

            if (!m_DescriptorPool.Create(m_Context->GetGpu(), m_Context->GetSwapchain())) {
                return false;
            }

            if (!CreateDescriptorSets()) {
                return false;
            }

            VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            uint32 imageCount = m_Context->GetSwapchain()->swapchainImages.size();
            std::vector<VkWriteDescriptorSet> descriptorWrites;
            bool success = true;

            for (auto& attribut : m_BufferLayout.attributes) {
                m_UniformBuffers[attribut.name] = {};
                VkDescriptorType descriptorType = attribut.uType == UniformBufferType::Static ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER :
                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

                if (!m_UniformBuffers[attribut.name].Create(m_Context->GetGpu(), attribut, usage, m_DescriptorSets, descriptorType)) {
                    Log_nts.Error("Cannot create uniforme buffer : name = {0}", attribut.name);
                    success = false;
                }
                else {
                    for (auto wds : m_UniformBuffers[attribut.name].writeDescriptorSets) {
                        descriptorWrites.push_back(wds);
                    }
                }
            }

            if (descriptorWrites.size() > 0) {
                vkUpdateDescriptorSets(m_Context->GetGpu()->device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
            }
        }
    }

    Memory::Shared<Context> VulkanUniformBuffer::GetContext()
    {
        return m_Context;
    }

    bool VulkanUniformBuffer::Bind(const std::string& name, void* data, usize size)
    {
        if (m_Context == nullptr || m_Context->GetCurrentCommandBuffer() == nullptr || data == nullptr || size == 0) return false;

        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            return false;
        }

        //usize offset = index * size;
        usize offset = 0;
        bool success = true;
        auto& uniforms = it->second;

        for (auto& uniform : uniforms.uniformBuffers) {
            uniform.Mapped(m_Context->GetGpu(), size, offset);
            success = uniform.WriteToBuffer(data, size, offset);
            success = uniform.Flush(m_Context->GetGpu(), size, offset);
            uniform.UnMapped(m_Context->GetGpu());
        }

        VkDescriptorSet descriptorSet = m_DescriptorSets[m_Context->currentImageIndex];
        VkPipelineLayout pipelineLayout = m_Shader->GetPipelineLayout()->pipelineLayout;
        VkCommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
        vkCheckErrorVoid(vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, 0));
        return false;
    }

    bool VulkanUniformBuffer::Bind(const std::string& name, void* data, usize size, usize offset)
    {
        if (m_Context == nullptr || m_Context->GetCurrentCommandBuffer() == nullptr || data == nullptr || size == 0) return false;

        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            return false;
        }

        //usize offset = index * size;
        bool success = true;
        auto& uniforms = it->second;

        for (auto& uniform : uniforms.uniformBuffers) {
            uniform.Mapped(m_Context->GetGpu(), size, offset);
            success = uniform.WriteToBuffer(data, size, offset);
            success = uniform.Flush(m_Context->GetGpu(), size, offset);
            uniform.UnMapped(m_Context->GetGpu());
        }

        VkDescriptorSet descriptorSet = m_DescriptorSets[m_Context->currentImageIndex];
        VkPipelineLayout pipelineLayout = m_Shader->GetPipelineLayout()->pipelineLayout;
        VkCommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
        vkCheckErrorVoid(vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, 0));
        return false;
    }

    bool VulkanUniformBuffer::Destroy()
    {
        if (FreeData()) {
            m_Context->RemoveRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanUniformBuffer::Recreate));
            m_Context->RemoveCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanUniformBuffer::CleanUp));
            return true;
        }
        return false;
    }

    bool VulkanUniformBuffer::CreateDescriptorSets()
    {
        if (m_Context == nullptr || m_DescriptorPool.descriptorPool == nullptr) return false;

        VulkanResult result;
        bool first = true;

        VkDescriptorSetLayout descriptorLayout = m_Shader->GetPipelineLayout()->descriptorSetLayout;
        std::vector<VkDescriptorSetLayout> layouts(m_Context->GetSwapchain()->swapchainImages.size(), descriptorLayout);
        VkDescriptorSetAllocateInfo allocInfo{};

        Log_nts.Debug("size = {0}", m_Context->GetSwapchain()->swapchainImages.size());

        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_DescriptorPool.descriptorPool;
        allocInfo.descriptorSetCount = m_Context->GetSwapchain()->swapchainImages.size();
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets.resize(m_Context->GetSwapchain()->swapchainImages.size());
        vkCheckError(first, result, vkAllocateDescriptorSets(m_Context->GetGpu()->device, &allocInfo, m_DescriptorSets.data()), "cannot create descriptor sets");

        return result.success;
    }

    bool VulkanUniformBuffer::Recreate(bool force)
    {
        return Create();
    }

    bool VulkanUniformBuffer::CleanUp(bool force)
    {
        return FreeData();
    }

    bool VulkanUniformBuffer::FreeData()
    {
        if (m_Context == nullptr) return false;

        if (m_BufferLayout.attributes.size() > 0) {
            for (auto& attribut : m_BufferLayout.attributes) {
                if (!m_UniformBuffers[attribut.name].Destroy(m_Context->GetGpu())) {
                    Log_nts.Error("Cannot destroy uniforme buffer : name = {0}", attribut.name);
                }
            }
            m_UniformBuffers.clear();
        }

        VulkanResult result;
        bool first = true;

        if (m_DescriptorSets.size() > 0) {

            vkCheckError(first, result, vkFreeDescriptorSets(m_Context->GetGpu()->device, m_DescriptorPool.descriptorPool, m_DescriptorSets.size(), m_DescriptorSets.data()), "cannot free descriptor sets");
            m_DescriptorSets.clear();
        }

        m_DescriptorPool.Destroy(m_Context->GetGpu());
        return result.success;
    }

}  //  nkentseu