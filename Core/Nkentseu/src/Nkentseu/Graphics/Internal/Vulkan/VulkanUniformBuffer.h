//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-03 at 08:33:11 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_UNIFORM_BUFFER_H__
#define __VULKAN_UNIFORM_BUFFER_H__

#pragma once

#include <System/System.h>
#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/Shader.h"
#include "Nkentseu/Graphics/UniformBuffer.h"
#include "VulkanContext.h"
#include "VulkanShader.h"
#include "Tools/VulkanUBO.h"
#include "Tools/VulkanDescriptorPool.h"

namespace nkentseu {
    
    class NKENTSEU_API VulkanUniformBuffer : public UniformBuffer {
        public:
            VulkanUniformBuffer(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);
            ~VulkanUniformBuffer();

            bool Create(Memory::Shared<Shader> shader, const std::vector<std::string> uniformsLoader) override;

            bool SetData(const std::string& name, void* data, usize size) override;
            bool SetData(const std::string& name, void* data, usize size, uint32 index) override;
            bool Bind() override;

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;
        private:
            Memory::Shared<VulkanContext> m_Context;
            Memory::Shared<VulkanShader> m_Shader;
            Memory::Shared<VulkanShaderInputLayout> m_Vksil = nullptr;

            std::unordered_map<std::string, VkUniformBufferInternal> m_UniformBuffers;
            std::vector<vk::DescriptorSet> m_DescriptorSets = {};

            uint32 m_OffsetDynamicCount = 0;
            usize m_DynamicAlignment = 0;

            std::vector<std::string> m_UniformLoaders;

            bool CreateDescriptorSets();
            bool Recreate(bool force);
            bool CleanUp(bool force);
            bool FreeData();

            bool Create(const std::vector<std::string> uniformsLoader);

            // descriptor pool
            vk::DescriptorPool descriptorPool = nullptr;
            std::vector<vk::DescriptorPoolSize> poolSizes;
            bool CreateDescriptorPool();
            bool DestroyDescriptorPool();
            void AddInDescriptorPool(vk::DescriptorType type, uint32 count);
    };

}  //  nkentseu

#endif  // __VULKAN_UNIFORM_BUFFER_H__!