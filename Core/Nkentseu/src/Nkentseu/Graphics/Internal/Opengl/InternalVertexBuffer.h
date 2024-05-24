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

namespace nkentseu {
    
    class NKENTSEU_API InternalVertexBuffer {
        public:
            InternalVertexBuffer();
            ~InternalVertexBuffer();

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout = {});
            bool Destroy();

            bool Bind();
            bool Unbind();

            std::vector<float32> GetVertices();
            uint32 Leng();

            uint32 GetVBO();
        private:
            uint32 m_VertexBufferObject = 0;
            BufferDataUsage::Code m_BufferUsage;
            std::vector<float32> m_Vertices;

            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_VERTEX_BUFFER_H__!