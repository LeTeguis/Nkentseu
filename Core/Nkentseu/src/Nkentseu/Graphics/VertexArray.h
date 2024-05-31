//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:05 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include "ShaderInfo.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Context.h"

namespace nkentseu {
    class NKENTSEU_API VertexArray {
        public:
            virtual Memory::Shared<Context> GetContext() = 0;
            virtual bool SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer) = 0;
            virtual Memory::Shared<VertexBuffer> GetVertexBuffer() = 0;
            virtual uint32 GetVertexLeng() = 0;
            virtual bool SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) = 0;
            virtual Memory::Shared<IndexBuffer> GetIndexBuffer() = 0;
            virtual uint32 GetIndexLeng() = 0;
            virtual uint32 Leng() = 0;
            virtual bool Destroy() = 0;

            static Memory::Shared<VertexArray> Create(Memory::Shared<Context> context, const BufferLayout& bufferLayout);
            static Memory::Shared<VertexArray> Create(Memory::Shared<Context> context, uint32 vertexNumber);
    };

}  //  nkentseu

#endif  // __VERTEX_ARRAY_H__!