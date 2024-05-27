//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:51 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_VERTEX_BUFFER_H__
#define __OPENGL_INTERNAL_VERTEX_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Core/NkentseuLogger.h"

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalVertexBuffer {
        public:
            InternalVertexBuffer();
            ~InternalVertexBuffer();

            bool Create(Context* context, BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout = {});
            bool Create(Context* context, BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout = {});

            template <typename T>
            bool Create(Context* context, BufferDataUsage::Code bufferUsage, const std::vector<T>& vertices, const BufferLayout& bufferLayout = {}) {
                Log_nts.Debug();
                return Create(context, bufferUsage, vertices.data(), vertices.size(), bufferLayout);
            }
            bool Destroy();

            bool Bind();
            bool Unbind();

            uint32 Leng();

            uint32 GetVBO();
        private:
            uint32 m_VertexBufferObject = 0;
            BufferDataUsage::Code m_BufferUsage;

            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_VERTEX_BUFFER_H__!