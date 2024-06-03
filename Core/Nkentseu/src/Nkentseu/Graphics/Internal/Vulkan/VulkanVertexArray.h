//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:22:16 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_VERTEX_ARRAY_H__
#define __VULKAN_INTERNAL_VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>

#include <System/Definitions/Memory.h>
#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Graphics/VertexArray.h"

namespace nkentseu {
    class VulkanVertexBuffer;
    class VulkanIndexBuffer;
    class VulkanContext;
    
    class NKENTSEU_API VulkanVertexArray : public VertexArray {
        public:
            VulkanVertexArray(Memory::Shared<Context> context);
            ~VulkanVertexArray();

            Memory::Shared<Context> GetContext() override;

            bool Create();
            bool Create(uint32 vertexNumber);

            bool Destroy() override;

            bool SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer) override;
            Memory::Shared<VertexBuffer> GetVertexBuffer() override;
            uint32 GetVertexLeng() override;

            bool SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) override;
            Memory::Shared<IndexBuffer> GetIndexBuffer() override;
            uint32 GetIndexLeng() override;

            uint32 Leng() override;
        private:
            uint32 m_VertexNumber = 0;
            Memory::Shared<Context> m_Context = nullptr;

            Memory::Shared<VulkanVertexBuffer> m_VertexBuffer = nullptr;
            Memory::Shared<VulkanIndexBuffer> m_IndexBuffer = nullptr;
    };

}  //  nkentseu

#endif  // __INTERNAL_VERTEX_ARRAY_H__!