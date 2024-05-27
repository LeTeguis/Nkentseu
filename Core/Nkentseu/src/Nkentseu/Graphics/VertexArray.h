//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:10:05 AM AM.
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

namespace nkentseu {
    class InternalVertexArray;
    class InternalVertexBuffer;
    class InternalIndexBuffer;
    class Context;
    
    class NKENTSEU_API VertexArray {
        public:
            VertexArray(Context* context);
            ~VertexArray();

            Context* GetContext();
            bool SetContext(Context* context);

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

            InternalVertexArray* GetInternal();

        private:
            Context* m_Context = nullptr;
            Memory::Shared<InternalVertexArray> m_InternalVertexArray = null_pointer;
    };

}  //  nkentseu

#endif  // __VERTEX_ARRAY_H__!