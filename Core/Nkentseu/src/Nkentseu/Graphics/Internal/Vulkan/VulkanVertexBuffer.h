//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:11:16 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_VERTEX_BUFFER_H__
#define __VULKAN_INTERNAL_VERTEX_BUFFER_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "VulkanInternal.h"

#include <vulkan/vulkan.hpp>

namespace nkentseu {
    class VulkanContext;
    
    class NKENTSEU_API VulkanVertexBuffer : public VertexBuffer {
        public:
            VulkanVertexBuffer(Memory::Shared<Context> context);
            ~VulkanVertexBuffer();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout = {});
            bool Create(BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout = {});

            template <typename T>
            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<T>& vertices, const BufferLayout& bufferLayout = {}) {
                return Create(bufferUsage, vertices.data(), vertices.size(), bufferLayout);
            }

            virtual bool SetData(void* data, usize size) override;

            VulkanBuffer* GetBuffer();

            uint32 Leng() const override;
        private:
            VulkanBuffer m_VertexBufferObject;
            Memory::Shared<VulkanContext> m_Context = nullptr;
            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif  // __INTERNAL_VERTEX_BUFFER_H__!