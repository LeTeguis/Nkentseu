//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:46:54 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include "ShaderInfo.h"

namespace nkentseu {
    class InternalIndexBuffer;
    class Context;
    
    class NKENTSEU_API IndexBuffer {
        public:
            IndexBuffer();
            ~IndexBuffer();

            bool Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices, const BufferLayout& bufferLayout = {});
            bool Destroy();

            InternalIndexBuffer* GetInternal();
        private:
            Memory::Shared<InternalIndexBuffer> m_Internal = null_pointer;
    };

}  //  nkentseu

#endif  // __INDEX_BUFFER_H__!