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
    
    class NKENTSEU_API InternalVertexArray {
        public:
            InternalVertexArray();
            ~InternalVertexArray();

            bool Create(const BufferLayout& bufferLayout);
            bool Destroy();

            bool SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer);
            Memory::Shared<VertexBuffer> GetVertexBuffer();
            InternalVertexBuffer* GetInternalVertexBuffer();

            bool SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer);
            Memory::Shared<IndexBuffer> GetIndexBuffer();
            InternalIndexBuffer* GetInternalIndexBuffer();
        private:
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_VERTEX_ARRAY_H__!