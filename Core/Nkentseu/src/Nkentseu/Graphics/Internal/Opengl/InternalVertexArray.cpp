//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:11:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexArray.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "InternalVertexBuffer.h"
#include "InternalIndexBuffer.h"

#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"

#include "OpenGLUtils.h"

namespace nkentseu {
    
    // Constructor
    InternalVertexArray::InternalVertexArray(Context* context) : m_Context(context) {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexArray::~InternalVertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    Context* nkentseu::InternalVertexArray::GetContext()
    {
        return nullptr;
    }

    bool InternalVertexArray::SetContext(Context* context)
    {
        return false;
    }

    bool InternalVertexArray::Create(const BufferLayout& bufferLayout)
    {
        if (m_VertexArrayObject != 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glGenVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");

        if (!result.success || m_VertexArrayObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        if (m_VertexBuffer == nullptr) {
            return false;
        }

        if (m_VertexBuffer->GetInternal() == nullptr || !m_VertexBuffer->GetInternal()->Bind()) {
            Unbind();
            return false;
        }

        if (m_IndexBuffer != nullptr) {
            if (m_IndexBuffer->GetInternal() != nullptr) {
                m_IndexBuffer->GetInternal()->Bind();
            }
        }

        m_BufferLayout = bufferLayout;
        
        for (auto& attribut : m_BufferLayout) {
            uint32 type = GLConvert::ShaderType(attribut.type);
            uint32 normalized = attribut.normalized ? GL_TRUE : GL_FALSE;
            uint32 count = attribut.GetComponentCount();
            uint32 offset = attribut.offset;
            uint32 location = attribut.location;

            glCheckError(first, result, glVertexAttribPointer(location, count, type, normalized, m_BufferLayout.stride, (void*)offset), "cannot set vertex attribut pointer");
            if (!result.success) {
                return false;
            }

            glCheckError(first, result, glEnableVertexAttribArray(location), "cannot enable vertex atribut array");
            if (!result.success) {
                return false;
            }
        }

        if (m_IndexBuffer != nullptr) {
            if (m_IndexBuffer->GetInternal() != nullptr) {
                m_IndexBuffer->GetInternal()->Unbind();
            }
        }


        if (m_VertexBuffer == nullptr || m_VertexBuffer->GetInternal() == nullptr || !m_VertexBuffer->GetInternal()->Unbind()) {
            return false;
        }

        m_VertexNumber = m_VertexBuffer->GetInternal()->Leng();

        return Unbind();
    }

    bool InternalVertexArray::Create(uint32 vertexNumber)
    {
        if (m_VertexArrayObject != 0) return false;

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

    uint32 InternalVertexArray::GetVertexNumber() {
        return m_VertexNumber;
    }

    bool InternalVertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer)
    {
        if (vertexBuffer == nullptr) return false;
        m_VertexBuffer = vertexBuffer;
        return true;
    }

    Memory::Shared<VertexBuffer> InternalVertexArray::GetVertexBuffer()
    {
        return m_VertexBuffer;
    }

    InternalVertexBuffer* InternalVertexArray::GetInternalVertexBuffer()
    {
        if (m_VertexBuffer == nullptr) return nullptr;

        return m_VertexBuffer->GetInternal();
    }

    bool InternalVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer)
    {
        if (indexBuffer == nullptr) return false;
        m_IndexBuffer = indexBuffer;
        return true;
    }

    Memory::Shared<IndexBuffer> InternalVertexArray::GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    InternalIndexBuffer* InternalVertexArray::GetInternalIndexBuffer()
    {
        if (m_IndexBuffer == nullptr) return nullptr;

        return m_IndexBuffer->GetInternal();
    }

    bool InternalVertexArray::Destroy()
    {
        if (m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glDeleteVertexArrays(1, &m_VertexArrayObject), "cannot delete vertex array");

        if (!result.success) {
            return false;
        }

        m_VertexArrayObject = 0;
        return true;
    }

    bool InternalVertexArray::Bind()
    {
        if (m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindVertexArray(m_VertexArrayObject), "cannot bin vertes array");
        return result.success;
    }

    bool InternalVertexArray::Unbind()
    {
        if (m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindVertexArray(0), "cannot unbin vertex array");
        return result.success;
    }

    const BufferLayout& InternalVertexArray::GetBufferLayout()
    {
        return m_BufferLayout;
    }

    GLuint InternalVertexArray::GetVAO()
    {
        return m_VertexArrayObject;
    }
}  //  nkentseu

#endif