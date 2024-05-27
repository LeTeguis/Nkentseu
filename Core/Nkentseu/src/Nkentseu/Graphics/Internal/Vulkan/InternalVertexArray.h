//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:22:16 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_VERTEX_ARRAY_H__
#define __VULKAN_INTERNAL_VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <System/Definitions/Memory.h>
#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    class VertexBuffer;
    class InternalVertexBuffer;
    class IndexBuffer;
    class InternalIndexBuffer;
    class Context;
    
    class NKENTSEU_API InternalVertexArray {
        public:
            InternalVertexArray(Context* context);
            ~InternalVertexArray();

            bool Create(const BufferLayout& bufferLayout);
            bool Create(uint32 vertexNumber);
            bool Destroy();

            bool SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer);
            Memory::Shared<VertexBuffer> GetVertexBuffer();
            InternalVertexBuffer* GetInternalVertexBuffer();
            uint32 GetVertexNumber();

            bool SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer);
            Memory::Shared<IndexBuffer> GetIndexBuffer();
            InternalIndexBuffer* GetInternalIndexBuffer();
        private:
            uint32 m_VertexNumber = 0;
            Context* m_Context = nullptr;

            Memory::Shared<VertexBuffer> m_VertexBuffer = nullptr;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_VERTEX_ARRAY_H__!