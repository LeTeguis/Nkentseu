//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:11:05 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_VERTEX_ARRAY_H__
#define __OPENGL_INTERNAL_VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>
#include <glad/gl.h>

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <Nkentseu/Graphics/VertexArray.h>

namespace nkentseu {
    class Context;
    class OpenglVertexBuffer;
    class OpenglIndexBuffer;
    
    class NKENTSEU_API OpenglVertexArray : public VertexArray{
        public:
            OpenglVertexArray(Memory::Shared<Context> context);
            ~OpenglVertexArray();

            Memory::Shared<Context> GetContext() override;
            bool SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer) override;
            Memory::Shared<VertexBuffer> GetVertexBuffer() override;
            uint32 GetVertexLeng() override;
            bool SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) override;
            Memory::Shared<IndexBuffer> GetIndexBuffer() override;
            uint32 GetIndexLeng() override;
            uint32 Leng() override;
            bool Destroy() override;
            bool Draw(DrawVertexType::Code drawVertex) override;

            bool Create();
            bool Create(uint32 vertexNumber);

            bool Bind();
            bool Unbind();

            GLuint GetBuffer();
            //uint32 
        private:
            GLuint m_VertexArrayObject = 0; // VAO
            Memory::Shared<OpenglVertexBuffer> m_VertexBuffer;
            Memory::Shared<OpenglIndexBuffer> m_IndexBuffer;
            uint32 m_VertexNumber = 0;

            Memory::Shared<Context> m_Context;
    private:
        bool ActualizeVertexBuffer();
    };

}  //  nkentseu

#endif  // __INTERNAL_VERTEX_ARRAY_H__!