//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:21:33 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_INDEX_BUFFER_H__
#define __VULKAN_INTERNAL_INDEX_BUFFER_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "VulkanInternal.h"

namespace nkentseu {
    class VulkanContext;
    
    class NKENTSEU_API VulkanIndexBuffer : public IndexBuffer {
        public:
            VulkanIndexBuffer(Memory::Shared<Context> context);
            ~VulkanIndexBuffer();

            Memory::Shared<Context> GetContext() override;

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<uint32>& indices);
            bool Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* vertices, uint32 leng);
            bool Destroy() override;

            VulkanBuffer* GetBuffer();
            DrawIndexType::Code GetIndexType() const;
            uint32 Leng() const override;
        private:
            Memory::Shared<VulkanContext> m_Context = nullptr;
            VulkanBuffer m_IndexBufferObject;
            DrawIndexType::Code m_IndexType = DrawIndexType::UInt32;

            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif  // __INTERNAL_INDEX_BUFFER_H__!