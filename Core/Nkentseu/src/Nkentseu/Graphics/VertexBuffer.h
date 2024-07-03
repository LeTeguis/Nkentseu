//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-19 at 10:46:43 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include "ShaderInfo.h"
#include "Context.h"
#include "ShaderInputLayout.h"

namespace nkentseu {
    class InternalVertexBuffer;
    
    class NKENTSEU_API VertexBuffer {
        public:
            virtual Memory::Shared<Context> GetContext() = 0;
            virtual bool Destroy() = 0;
            virtual uint32 Leng() const = 0;
            virtual bool SetData(void* data, usize size) = 0;

            virtual bool Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices) = 0;
            virtual bool Create(BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng) = 0;

            static Memory::Shared<VertexBuffer> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);

            static Memory::Shared<VertexBuffer> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices);

            static Memory::Shared<VertexBuffer> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng);

            template <typename T>
            static Memory::Shared<VertexBuffer> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, BufferDataUsage::Code bufferUsage, const std::vector<T>& vertices) {
                return Create(context, sil, bufferUsage, vertices.data(), vertices.size());
            }
    };

}  //  nkentseu

#endif  // __VERTEX_BUFFER_H__!