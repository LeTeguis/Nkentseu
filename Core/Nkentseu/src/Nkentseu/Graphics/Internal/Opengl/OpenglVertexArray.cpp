//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:11:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglVertexArray.h"

#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

#include "Nkentseu/Graphics/Context.h"
#include "OpenglContext.h"

#include "OpenGLUtils.h"

namespace nkentseu {

    OpenglVertexArray::OpenglVertexArray(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<OpenglContext>(context)) {
    }

    OpenglVertexArray::~OpenglVertexArray() {
    }

    Memory::Shared<Context> nkentseu::OpenglVertexArray::GetContext()
    {
        return m_Context;
    }

    bool OpenglVertexArray::Create()
    {
        if (m_VertexArrayObject != 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glGenVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");
        return true;
    }

    bool OpenglVertexArray::Create(uint32 vertexNumber)
    {
        if (m_VertexArrayObject != 0 || m_Context == nullptr) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glGenVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");

        if (!result.success || m_VertexArrayObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        m_VertexNumber = vertexNumber;
        return Unbind();
    }

    uint32 OpenglVertexArray::GetVertexLeng() {
        if (m_VertexBuffer == nullptr) return 0;
        return m_VertexBuffer->Leng();
    }

    bool OpenglVertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer)
    {
        m_VertexBuffer = Memory::SharedCast<OpenglVertexBuffer>(vertexBuffer);
        return ActualizeVertexBuffer();
    }

    Memory::Shared<VertexBuffer> OpenglVertexArray::GetVertexBuffer()
    {
        return m_VertexBuffer;
    }

    bool OpenglVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer)
    {
        m_IndexBuffer = Memory::SharedCast<OpenglIndexBuffer>(indexBuffer);
        return m_IndexBuffer == nullptr;
    }

    Memory::Shared<IndexBuffer> OpenglVertexArray::GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    uint32 OpenglVertexArray::GetIndexLeng()
    {
        if (m_IndexBuffer == nullptr) return 0;
        return m_IndexBuffer->Leng();
    }

    uint32 OpenglVertexArray::Leng()
    {
        if (m_Context == nullptr || m_VertexBuffer == nullptr) return m_VertexNumber;
        return m_VertexBuffer->Leng() == 0 ? m_VertexNumber : m_VertexBuffer->Leng();
    }

    bool OpenglVertexArray::Destroy()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glDeleteVertexArrays(1, &m_VertexArrayObject), "cannot delete vertex array");

        if (!result.success) {
            return false;
        }

        m_VertexArrayObject = 0;
        return true;
    }

    bool OpenglVertexArray::Bind()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindVertexArray(m_VertexArrayObject), "cannot bin vertes array");
        return result.success;
    }

    bool OpenglVertexArray::Unbind()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindVertexArray(0), "cannot unbin vertex array");
        return result.success;
    }

    GLuint OpenglVertexArray::GetBuffer()
    {
        return m_VertexArrayObject;
    }

    bool OpenglVertexArray::ActualizeVertexBuffer()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        bool success = true;

        if (m_VertexBuffer != nullptr) {
            success = m_VertexBuffer->AttachToVAO();
        }

        return Unbind() && success;
    }
}  //  nkentseu