//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-19 at 10:46:54 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include "ShaderInfo.h"
#include "Context.h"

namespace nkentseu {
    class InternalIndexBuffer;
    
    class NKENTSEU_API IndexBuffer {
        public:
            virtual Memory::Shared<Context> GetContext() = 0;
            virtual bool Destroy() = 0;
            virtual uint32 Leng() const = 0;
            virtual bool SetData(void* data, usize size) = 0;

            static Memory::Shared<IndexBuffer> Create(Memory::Shared<Context> context, BufferDataUsage::Code bufferUsage, const std::vector<uint32>& indices);
            static Memory::Shared<IndexBuffer> Create(Memory::Shared<Context> context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* indices, uint32 leng);
    };

}  //  nkentseu

#endif  // __INDEX_BUFFER_H__!