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

#include "InternalContext.h"

#include "OpenGLUtils.h"

namespace nkentseu {
    
    // Constructor
    InternalVertexArray::InternalVertexArray() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexArray::~InternalVertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexArray::Create(const BufferLayout& bufferLayout)
    {
        if (m_VertexArrayObject != 0) return false;

        glGenVertexArrays(1, &m_VertexArrayObject);
        if (glCheckError() != GL_NO_ERROR || m_VertexArrayObject == 0) {
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
        
        uint32 location = 0;
        for (auto& attribut : m_BufferLayout) {
            uint32 type = GLConvert::ShaderType(attribut.type);
            uint32 normalized = attribut.normalized ? GL_TRUE : GL_FALSE;
            uint32 count = attribut.GetComponentCount();
            uint32 offset = attribut.offset;

            glVertexAttribPointer(location, count, type, normalized, m_BufferLayout.stride, (void*)offset);
            if (glCheckError() != GL_NO_ERROR) {
                return false;
            }

            glEnableVertexAttribArray(location);
            if (glCheckError() != GL_NO_ERROR) {
                return false;
            }
            location++;
        }

        if (m_IndexBuffer != nullptr) {
            if (m_IndexBuffer->GetInternal() != nullptr) {
                m_IndexBuffer->GetInternal()->Unbind();
            }
        }

        if (!m_VertexBuffer->GetInternal()->Unbind()) {
            return false;
        }

        return Unbind();
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

        glDeleteVertexArrays(1, &m_VertexArrayObject);

        if (glCheckError() != GL_NO_ERROR) {
            return false;
        }

        m_VertexArrayObject = 0;
        return true;
    }

    bool InternalVertexArray::Bind()
    {
        if (m_VertexArrayObject == 0) return false;
        glBindVertexArray(m_VertexArrayObject);
        return glCheckError() == GL_NO_ERROR;
    }

    bool InternalVertexArray::Unbind()
    {
        if (m_VertexArrayObject == 0) return false;
        glBindVertexArray(0);
        return glCheckError() == GL_NO_ERROR;
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