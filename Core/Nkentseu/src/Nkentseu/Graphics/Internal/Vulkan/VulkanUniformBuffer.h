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

namespace nkentseu {
    
    class NKENTSEU_API VulkanUniformBuffer : public UniformBuffer {
        public:
            VulkanUniformBuffer(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout);
            ~VulkanUniformBuffer();

            bool Create();
            //bool SetCommandBuffer(VkCommandBuffer commandBuffer);
            Memory::Shared<Context> GetContext() override;
            bool Bind(const std::string& name, void* data, usize size) override;
            bool Bind(const std::string& name, void* data, usize size, uint32 index) override;
            bool Flush() override;
            bool Destroy() override;
        private:
            Memory::Shared<VulkanContext> m_Context;
            Memory::Shared<VulkanShader> m_Shader;
            UniformBufferLayout m_BufferLayout;
            VulkanDescriptorPool m_DescriptorPool;

            std::unordered_map<std::string, VulkanUBO> m_UniformBuffers;
            std::vector<VkDescriptorSet> m_DescriptorSets = {};

            uint32 m_OffsetDynamicCount = 0;
            usize m_DynamicAlignment = 0;

            bool CreateDescriptorSets();
            bool Recreate(bool force);
            bool CleanUp(bool force);
            bool FreeData();
    };

}  //  nkentseu

#endif  // __VULKAN_UNIFORM_BUFFER_H__!