//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:38 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_INDEX_BUFFER_H__
#define __OPENGL_INTERNAL_INDEX_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    
    class NKENTSEU_API InternalIndexBuffer {
        public:
            InternalIndexBuffer();
            ~InternalIndexBuffer();

            bool Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices, const BufferLayout& bufferLayout = {});
            bool Destroy();

            bool Bind();
            bool Unbind();

            uint32 Leng() const;
            DrawIndexType::Code GetIndexType() const;
        private:
            uint32 m_ElementBufferObject = 0;
            BufferDataUsage::Code m_BufferUsage = BufferDataUsage::StaticDraw;
            DrawIndexType::Code m_IndexType = DrawIndexType::UnsignedInt;
            std::vector<uint32> m_Indices;

            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_INDEX_BUFFER_H__!