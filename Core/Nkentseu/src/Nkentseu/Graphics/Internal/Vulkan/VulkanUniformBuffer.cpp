//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-03 at 08:33:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanUniformBuffer.h"
#include <Logger/Formatter.h>
#include "VulkanUtils.h"

namespace nkentseu {
    
    VulkanUniformBuffer::VulkanUniformBuffer(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<VulkanContext>(context)){
        m_Vksil = Memory::SharedCast<VulkanShaderInputLayout>(sil);
        if (m_Context != nullptr && m_Vksil != nullptr) {
            m_Context->AddRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanUniformBuffer::Recreate));
            m_Context->AddCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanUniformBuffer::CleanUp));
        }
    }

    VulkanUniformBuffer::~VulkanUniformBuffer() {
    }

    bool VulkanUniformBuffer::Create(Memory::Shared<Shader> shader, const std::vector<std::string> uniformsLoader)
    {
        m_Shader = Memory::SharedCast<VulkanShader>(shader);
        m_UniformLoaders.clear();
        m_UniformLoaders.insert(m_UniformLoaders.begin(), uniformsLoader.begin(), uniformsLoader.end());
        return Create(m_UniformLoaders);
    }

    Memory::Shared<Context> VulkanUniformBuffer::GetContext()
    {
        return m_Context;
    }

    bool VulkanUniformBuffer::SetData(const std::string& name, void* data, usize size)
    {
        if (m_Context == nullptr || m_Context->GetCommandBuffer() == nullptr || data == nullptr || size == 0) return false;

        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            return false;
        }

        auto& uniforms = it->second;

        uniforms.Binds(m_Context.get(), data, size, 0);
        return false;
    }

    bool VulkanUniformBuffer::SetData(const std::string& name, void* data, usize size, uint32 index)
    {
        if (m_Context == nullptr || m_Context->GetCommandBuffer() == nullptr || data == nullptr || size == 0) return false;

        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            return false;
        }

        bool success = true;
        auto& uniforms = it->second;
        success = uniforms.Binds(m_Context.get(), data, size, index);
        //success = uniforms.Bind(m_Context->GetDevice(), data, size, m_Context->currentImageIndex, index);
        return success;
    }

    bool VulkanUniformBuffer::Bind()
    {
        if (m_Context == nullptr || m_Context->GetCommandBuffer() == nullptr || m_Vksil == nullptr) return false;

        uint32 odc = m_OffsetDynamicCount;
        vk::PipelineBindPoint pbp = vk::PipelineBindPoint::eGraphics;
        vk::DescriptorSet descriptorSet = m_DescriptorSets[m_Context->currentImageIndex];
        vk::PipelineLayout pipelineLayout = m_Vksil->pipelineLayout;
        uint32 set = m_Vksil->sets[BufferSpecificationType::Enum::Uniform];
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();
        std::vector<uint32> dynamicOffsets = {};

        for (auto& [nom, uniform] : m_UniformBuffers) {
            if (uniform.dynamicAlignment != 0) {
                dynamicOffsets.push_back(uniform.currentOffset);
            }
        }

        if (dynamicOffsets.size() == 0) {
            vkCheckErrorVoid(commandBuffer.bindDescriptorSets(pbp, pipelineLayout, set, 1, &descriptorSet, 0, nullptr));
            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("Cannot bin descriptor set");
                return false;
            }
        }
        else {
            vkCheckErrorVoid(commandBuffer.bindDescriptorSets(pbp, pipelineLayout, set, 1, &descriptorSet, dynamicOffsets.size(), dynamicOffsets.data()));
            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("Cannot bin descriptor set");
                return false;
            }
        }
        return true;
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
        if (m_Context == nullptr || descriptorPool == nullptr || m_Vksil == nullptr) return false;

        VulkanResult result;
        bool first = true;

        // Recupere le layout du descriptor set pour les uniform buffers
        auto it = m_Vksil->descriptorSetLayouts.find(BufferSpecificationType::Enum::Uniform);
        if (it == m_Vksil->descriptorSetLayouts.end()) {
            Log_nts.Error("Uniform buffer descriptor set layout not found.");
            return false;
        }

        std::vector<vk::DescriptorSetLayout> layouts(m_Context->swapchainImages.size(), it->second);
        vk::DescriptorSetAllocateInfo allocInfo{};
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32>(layouts.size());
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets.resize(layouts.size());
        vkCheckError(first, result, m_Context->device.allocateDescriptorSets(&allocInfo, m_DescriptorSets.data()), "cannot create descriptor sets");

        return result.success;
    }

    bool VulkanUniformBuffer::Recreate(bool force)
    {
        return Create(m_UniformLoaders);
    }

    bool VulkanUniformBuffer::CleanUp(bool force)
    {
        return FreeData();
    }

    bool VulkanUniformBuffer::FreeData()
    {
        if (m_Context == nullptr || m_Vksil == nullptr) return false;
        if (m_Vksil->uniformInput.attributes.size() > 0) {
            for (auto& attribut : m_Vksil->uniformInput.attributes) {
                if (!m_UniformBuffers[attribut.name].Destroy(m_Context.get())) {
                  Log_nts.Error("Cannot destroy uniforme buffer : name = {0}", attribut.name);
                }
            }
            m_UniformBuffers.clear();
        }

        VulkanResult result;
        bool first = true;

        if (m_DescriptorSets.size() > 0) {
            vkCheckError(first, result, m_Context->device.freeDescriptorSets(descriptorPool, m_DescriptorSets.size(), m_DescriptorSets.data()), "cannot free descriptor sets");
            m_DescriptorSets.clear();
        }

        DestroyDescriptorPool();
        m_OffsetDynamicCount = 0;
        return result.success;
    }

    bool VulkanUniformBuffer::Create(const std::vector<std::string> uniformsLoader)
    {
        if (m_Context == nullptr || m_Vksil == nullptr) return false;

        if (m_Vksil->uniformInput.attributes.size() > 0) {
            std::vector<BufferUsageType::Enum> dTypesList = {};
            for (auto& attribut : m_Vksil->uniformInput.attributes) {

                bool loadit = false;
                for (auto name : uniformsLoader) {
                    if (name == attribut.name) {
                        loadit = true;
                    }
                }

                if (!loadit) {
                    continue;
                }

                if (std::find(dTypesList.begin(), dTypesList.end(), attribut.usage) == dTypesList.end()) {
                    dTypesList.push_back(attribut.usage);

                    vk::DescriptorType dtype = vk::DescriptorType::eUniformBuffer;

                    if (attribut.usage == BufferUsageType::Enum::DynamicDraw) {
                        dtype = vk::DescriptorType::eUniformBufferDynamic;
                    }

                    AddInDescriptorPool(dtype, m_Context->swapchainImages.size());
                }
            }

            if (!CreateDescriptorPool()) {
                return false;
            }

            if (!CreateDescriptorSets()) {
                return false;
            }

            vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eUniformBuffer;
            uint32 imageCount = m_Context->swapchainImages.size();
            std::vector<vk::WriteDescriptorSet> descriptorWrites;
            bool success = true;

            for (auto& attribut : m_Vksil->uniformInput.attributes) {
                bool loadit = false;
                for (auto name : uniformsLoader) {
                    if (name == attribut.name) {
                        loadit = true;
                    }
                }

                if (!loadit) {
                    continue;
                }
                m_UniformBuffers[attribut.name] = {};
                vk::DescriptorType descriptorType = vk::DescriptorType::eUniformBuffer;
                if (attribut.usage == BufferUsageType::Enum::DynamicDraw) {
                    descriptorType = vk::DescriptorType::eUniformBufferDynamic;
                }

                if (!m_UniformBuffers[attribut.name].Create(m_Context.get(), attribut, usage, m_DescriptorSets, descriptorType)) {
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
                vkCheckErrorVoid(m_Context->device.updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data(), 0, nullptr));
                if (!VulkanStaticDebugInfo::Result()) {
                    Log_nts.Error("Cannot update descriptor set");
                    return false;
                }
            }
            return success;
        }
        return false;
    }

    bool VulkanUniformBuffer::CreateDescriptorPool()
    {
        VulkanResult result;
        bool first = true;

        vk::DescriptorPoolCreateInfo poolInfo{};
        poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        if (poolSizes.size() > 0) {
            poolInfo.poolSizeCount = poolSizes.size();
            poolInfo.pPoolSizes = poolSizes.data();
        }
        poolInfo.maxSets = static_cast<uint32_t>(m_Context->swapchainImages.size());

        vkCheckError(first, result, m_Context->device.createDescriptorPool(&poolInfo, m_Context->allocator, &descriptorPool), "cannot create descriptor pool");

        return result.success;
    }

    bool VulkanUniformBuffer::DestroyDescriptorPool()
    {
        if (descriptorPool != nullptr) {
            vkCheckErrorVoid(m_Context->device.destroyDescriptorPool(descriptorPool, m_Context->allocator));
            descriptorPool = nullptr;

            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("Cannot destroy descriptor pool");
            }
        }
        return true;
    }

    void VulkanUniformBuffer::AddInDescriptorPool(vk::DescriptorType type, uint32 count)
    {
        vk::DescriptorPoolSize poolSize{};
        poolSize.type = type;
        poolSize.descriptorCount = count;
        poolSizes.push_back(poolSize);
    }

}  //  nkentseu