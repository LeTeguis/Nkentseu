//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglVertexBuffer.h"

#include <glad/gl.h>

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "OpenglContext.h"
#include "OpenGLUtils.h"

namespace nkentseu {
    GLfloat vertices_[] =
    {
        // Positions         // Colors
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
    };

    OpenglVertexBuffer::OpenglVertexBuffer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<OpenglContext>(context)) {
    }

    OpenglVertexBuffer::~OpenglVertexBuffer() {
    }

    bool OpenglVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout)
    {
        return Create(bufferUsage, vertices.data(), (vertices.size() / bufferLayout.componentCount), bufferLayout);
    }

    Memory::Shared<Context> OpenglVertexBuffer::GetContext()
    {
        return m_Context;
    }

    bool OpenglVertexBuffer::Destroy()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }
        return true;
    }

    bool OpenglVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout)
    {
        if (m_VertexBufferObject != 0) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glGenBuffers(1, &m_VertexBufferObject), "cannot gen vertex buffer");
        if (!result.success || m_VertexBufferObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        m_BufferUsage = bufferUsage;

        glCheckError(first, result, glBufferData(GL_ARRAY_BUFFER, bufferLayout.stride * leng, vertices, GLConvert::UsageType(m_BufferUsage)), "cannot set vertex buffer data");
        
        if (!result.success) {
            return false;
        }

        m_Leng = leng;

        if (!Unbind()) {
            return false;
        }

        return true;
    }

    bool OpenglVertexBuffer::Bind()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject), "cannot bind vertex buffer");

        return result.success;
    }

    bool OpenglVertexBuffer::Unbind()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(GL_ARRAY_BUFFER, 0), "cannot unbind vertex buffer");

        return result.success;
    }

    uint32 OpenglVertexBuffer::Leng() const
    {
        return m_Leng;
    }

    uint32 OpenglVertexBuffer::GetBuffer() const
    {
        return m_VertexBufferObject;
    }

}  //  nkentseu