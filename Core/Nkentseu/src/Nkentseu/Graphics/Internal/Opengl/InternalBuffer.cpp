//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:10:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Buffer.h"
#include "Nkentseu/Graphics/VertexArray.h"
#include "InternalVertexArray.h"
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    
    // Constructor
    InternalBuffer::InternalBuffer(const BufferAttribute& attribut) : m_Attribut(attribut) {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalBuffer::~InternalBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalBuffer::SetAttibute(const BufferAttribute& attribut)
    {
        m_Attribut = attribut;
        return true;
    }

    bool InternalBuffer::Create()
    {
        return false;
    }

    bool InternalBuffer::Bind()
    {
        return false;
    }

    bool InternalBuffer::Unbind()
    {
        return false;
    }

    const BufferAttribute& InternalBuffer::GetAttribut()
    {
        return m_Attribut;
    }

    std::string InternalBuffer::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const InternalBuffer& internalBuffer) {
        return internalBuffer.ToString();
    }

    std::string ToString(const InternalIndexBuffer& buffer)
    {
        return std::string();
    }

    // Index buffer

    InternalIndexBuffer::InternalIndexBuffer(const BufferAttribute& attribut)
    {
    }

    InternalIndexBuffer::~InternalIndexBuffer()
    {
    }

    bool InternalIndexBuffer::SetAttibute(const BufferAttribute& attribut)
    {
        m_Attribut = attribut;
        return true;
    }

    bool InternalIndexBuffer::Create(uint32 indices[])
    {
        Log_nts.Debug();
        m_Buffer = 0;

        glGenBuffers(1, &m_Buffer);
        Log_nts.Debug();

        if (m_Buffer == 0) return false;
        Log_nts.Debug();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        Log_nts.Debug();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
        Log_nts.Debug();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        Log_nts.Debug();

        m_Attribut.size = sizeof(indices);
        Log_nts.Debug();

        m_Length = (sizeof(indices) / sizeof(indices[0])) + 1;
        Log_nts.Debug();
        for (usize i = 0; i < sizeof(indices) / sizeof(uint32); ++i) {
            m_Indices.push_back(indices[i]);
        }
        Log_nts.Debug();
        return true;
    }

    bool InternalIndexBuffer::Create(const std::vector<uint32>& indices)
    {
        m_Buffer = 0;

        glGenBuffers(1, &m_Buffer);

        if (m_Buffer == 0) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32), indices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_Attribut.size = sizeof(indices);

        m_Length = (sizeof(indices) / sizeof(indices[0])) + 1;
        for (usize i = 0; i < indices.size(); ++i) {
            m_Indices.push_back(indices[i]);
        }
        return true;
    }

    bool InternalIndexBuffer::ChangeIndices(uint32 indices[])
    {
        if (m_Buffer == 0) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_Attribut.size = sizeof(indices);
        for (usize i = 0; i < sizeof(indices) / sizeof(uint32); ++i) {
            m_Indices.push_back(indices[i]);
        }
        return true;
    }

    bool InternalIndexBuffer::ChangeIndices(const std::vector<uint32>& indices)
    {
        if (m_Buffer == 0) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(uint32), indices.data());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_Indices.clear();

        m_Attribut.size = indices.size();
        for (usize i = 0; i < indices.size(); ++i) {
            m_Indices.push_back(indices[i]);
        }
        return true;
    }

    bool InternalIndexBuffer::Destroy()
    {
        if (m_Buffer != 0) return false;

        glDeleteBuffers(1, &m_Buffer);
        return true;
    }

    int32 InternalIndexBuffer::GetLength()
    {
        return m_Length;
    }

    bool InternalIndexBuffer::Bind()
    {
        if (m_Buffer == 0) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
        return true;
    }

    bool InternalIndexBuffer::Unbind()
    {
        if (m_Buffer == 0) return false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return true;
    }

    std::vector<uint32> InternalIndexBuffer::GetIndices()
    {
        return m_Indices;
    }

    GLuint InternalIndexBuffer::GetBuffer()
    {
        return m_Buffer;
    }

    const BufferAttribute& InternalIndexBuffer::GetAttribut()
    {
        return m_Attribut;
    }

    std::string InternalIndexBuffer::ToString() const
    {
        return std::string();
    }

    // Vertex buffer

    InternalVertexBuffer::InternalVertexBuffer(const BufferAttribute& attribut) : m_Attribut(attribut)
    {
    }

    InternalVertexBuffer::~InternalVertexBuffer()
    {
    }

    bool InternalVertexBuffer::SetAttibute(const BufferAttribute& attribut)
    {
        m_Attribut = attribut;
        return true;
    }

    bool InternalVertexBuffer::Create(float32 vertices[])
    {
        if (m_VertexArray == nullptr) {
            m_VertexArray = Memory::Alloc<VertexArray>();

            if (m_VertexArray == nullptr) {
                return false;
            }
        }

        if (!m_VertexArray->Create()) {
            return false;
        }

        glGenBuffers(1, &m_Buffer);

        if (m_Buffer != 0) {
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);

        if (m_Buffer == 0) {
            return false;
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        int32 bsize = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        if (bsize == 0)
        {
            return false;
        }

        std::copy(m_CurrentVertices.begin(), m_CurrentVertices.end(), vertices);
        return false;
    }

    bool InternalVertexBuffer::ChangeVertices(float32 vertices[])
    {
        if (m_Buffer == 0) {
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        int32 bsize = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        if (bsize == 0)
        {
            return false;
        }

        std::copy(m_CurrentVertices.begin(), m_CurrentVertices.end(), vertices);
        return true;
    }

    bool InternalVertexBuffer::AddIndexBuffer(Memory::Shared<IndexBuffer> buff)
    {
        m_IndexBuffers.push_back(buff);
        
        return SetIndexBuffer();;
    }

    bool InternalVertexBuffer::Destroy()
    {
        if (m_VertexArray == nullptr || m_Buffer == 0) return false;

        bool delete_it = true;

        for (Memory::Shared<IndexBuffer> indexBuffer : m_IndexBuffers)
        {
            static GLuint buffer = 0;
            buffer = indexBuffer->GetInternal()->GetBuffer();

            if (buffer == 0) {
                delete_it = false;
            }

            glDeleteBuffers(1, &buffer);
        }
        glDeleteBuffers(1, &m_Buffer);
        return delete_it;
    }

    bool InternalVertexBuffer::Bind()
    {
        if (m_VertexArray == nullptr || m_Buffer == 0) return false;
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
        return true;
    }

    bool InternalVertexBuffer::Unbind()
    {
        if (m_VertexArray == nullptr || m_Buffer == 0) return false;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return true;
    }

    GLuint InternalVertexBuffer::GetBuffer()
    {
        return m_Buffer;
    }

    IndexBuffer* InternalVertexBuffer::GetIndexBuffer(usize index)
    {
        if (m_VertexArray == nullptr || m_Buffer == 0 || index >= m_IndexBuffers.size()) return nullptr;
        return m_IndexBuffers[index].get();
    }

    usize InternalVertexBuffer::GetIndexBufferSize()
    {
        if (m_VertexArray == nullptr || m_Buffer == 0) return 0;
        return m_IndexBuffers.size();
    }

    VertexArray* InternalVertexBuffer::GetVertexArray()
    {
        if (m_VertexArray == nullptr || m_Buffer == 0) return nullptr;
        return m_VertexArray.get();
    }

    const BufferAttribute& InternalVertexBuffer::GetAttribut()
    {
        return m_Attribut;
    }

    std::string InternalVertexBuffer::ToString() const
    {
        return std::string();
    }

    bool InternalVertexBuffer::SetIndexBuffer()
    {
        if (m_VertexArray == nullptr || m_Buffer == 0) return false;

        m_VertexArray->GetInternal()->Bind();
        Bind();
        for (int i = 0; i < m_IndexBuffers.size(); i++) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        }
        Unbind();
        m_VertexArray->GetInternal()->Unbind();
        return true;
    }

    std::string ToString(const InternalVertexBuffer& buffer) {
        return "";
    }

}  //  nkentseu

#endif