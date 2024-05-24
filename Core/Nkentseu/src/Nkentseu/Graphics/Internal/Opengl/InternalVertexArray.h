//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:11:05 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_VERTEX_ARRAY_H__
#define __OPENGL_INTERNAL_VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <glad/gl.h>

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/ShaderInfo.h>

namespace nkentseu {
    class VertexBuffer;
    class IndexBuffer;
    class InternalVertexBuffer;
    class InternalIndexBuffer;
    
    class NKENTSEU_API InternalVertexArray {
        public:
            InternalVertexArray();
            ~InternalVertexArray();

            bool Create(const BufferLayout& bufferLayout);
            bool Create(uint32 vertexNumber);

            bool SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer);
            Memory::Shared<VertexBuffer> GetVertexBuffer();
            InternalVertexBuffer* GetInternalVertexBuffer();
            uint32 GetVertexNumber();

            bool SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer);
            Memory::Shared<IndexBuffer> GetIndexBuffer();
            InternalIndexBuffer* GetInternalIndexBuffer();

            bool Destroy();
            bool Bind();
            bool Unbind();

            const BufferLayout& GetBufferLayout();

            GLuint GetVAO();
            //uint32 
        private:
            GLuint m_VertexArrayObject = 0; // VAO
            Memory::Shared<VertexBuffer> m_VertexBuffer;
            Memory::Shared<IndexBuffer> m_IndexBuffer;
            BufferLayout m_BufferLayout;
            uint32 m_VertexNumber = 0;
    };

}  //  nkentseu

#endif  // __INTERNAL_VERTEX_ARRAY_H__!

#endif