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
        m_BufferLayout = bufferLayout;

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

    const BufferLayout& OpenglVertexBuffer::GetBufferLayaout()
    {
        return m_BufferLayout;
    }

    bool OpenglVertexBuffer::AttachToVAO()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;

        if (!Bind()) {
            return false;
        }

        for (auto& attribut : m_BufferLayout) {
            uint32 type = GLConvert::ShaderType(attribut.type);
            uint32 normalized = attribut.normalized ? GL_TRUE : GL_FALSE;
            uint32 count = attribut.GetComponentCount();
            uint32 offset = attribut.offset;
            uint32 location = attribut.location;

            glCheckError(first, result, glEnableVertexAttribArray(location), "cannot enable vertex atribut array");
            if (!result.success) {
                Log_nts.Debug();
                return false;
            }

            if (attribut.type == ShaderDataType::Float || attribut.type == ShaderDataType::Float2 || attribut.type == ShaderDataType::Float3 || attribut.type == ShaderDataType::Float4) {
                glCheckError(first, result, glVertexAttribPointer(location, count, type, normalized, m_BufferLayout.stride, (const void*)offset), "cannot set vertex attribut pointer");
            }
            else if (attribut.type == ShaderDataType::Mat3 || attribut.type == ShaderDataType::Mat4) {
                for (uint8_t i = 0; i < count; i++) {
                    glCheckError(first, result, glVertexAttribPointer(location, count, type, normalized, m_BufferLayout.stride, (const void*)(attribut.offset + sizeof(float) * count * i)), "cannot set vertex attribut pointer");
                    glCheckError(first, result, glVertexAttribDivisor(location, 1), "cannot set vertex attribut pointer");
                }
            }
            else if (attribut.type == ShaderDataType::NotDefine) {
                continue;
            }
            else {
                glCheckError(first, result, glVertexAttribIPointer(location, count, type, m_BufferLayout.stride, (const void*)offset), "cannot set vertex attribut pointer");
            }

            if (!result.success) {
                Log_nts.Debug();
                return false;
            }
        }
        return Unbind();
    }

}  //  nkentseu