//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:46:43 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include "ShaderInfo.h"

namespace nkentseu {
    class InternalVertexBuffer;
    
    class NKENTSEU_API VertexBuffer {
        public:
            VertexBuffer();
            ~VertexBuffer();

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout = {});
            bool Destroy();

            InternalVertexBuffer* GetInternal();
        private:
            Memory::Shared<InternalVertexBuffer> m_Internal = null_pointer;
    };

}  //  nkentseu

#endif  // __VERTEX_BUFFER_H__!