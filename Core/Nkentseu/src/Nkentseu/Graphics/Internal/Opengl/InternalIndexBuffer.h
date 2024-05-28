//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-19 at 10:47:38 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_INDEX_BUFFER_H__
#define __OPENGL_INTERNAL_INDEX_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalIndexBuffer {
        public:
            InternalIndexBuffer();
            ~InternalIndexBuffer();

            bool Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices, const BufferLayout& bufferLayout = {});
            bool Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* vertices, uint32 leng, const BufferLayout& bufferLayout = {});
            bool Destroy();

            bool Bind();
            bool Unbind();

            uint32 Leng() const;
            DrawIndexType::Code GetIndexType() const;
        private:
            uint32 m_ElementBufferObject = 0;
            BufferDataUsage::Code m_BufferUsage = BufferDataUsage::StaticDraw;
            DrawIndexType::Code m_IndexType = DrawIndexType::UInt32;

            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_INDEX_BUFFER_H__!