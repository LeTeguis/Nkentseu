//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:09:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Buffer.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalContext.h"
#include "Internal/Opengl/InternalBuffer.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    
    // Constructor
    Buffer::Buffer(const BufferAttribute& attribut) {
        // Ajoutez votre code de constructeur ici
        if (m_InternalBuffer == nullptr) {
            m_InternalBuffer = Memory::Alloc<InternalBuffer>(attribut);
        }
    }

    // Destructor
    Buffer::~Buffer() {
        // Ajoutez votre code de destructeur ici
    }

    InternalBuffer* Buffer::GetInternal()
    {
        if (m_InternalBuffer == nullptr) return nullptr;

        return m_InternalBuffer.get();
    }

    bool Buffer::SetAttibute(const BufferAttribute& attribut)
    {
        if (m_InternalBuffer == nullptr) return false;
        return m_InternalBuffer->SetAttibute(attribut);
    }

    bool Buffer::Create()
    {
        if (m_InternalBuffer == nullptr) return false;
        return m_InternalBuffer->Create();
    }

    const BufferAttribute& Buffer::GetAttribut()
    {
        static const BufferAttribute attribut = {};
        if (m_InternalBuffer == nullptr) return attribut;

        return m_InternalBuffer->GetAttribut();
    }

    std::string Buffer::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const Buffer& buffer) {
        return buffer.ToString();
    }

    std::string ToString(const IndexBuffer& buffer)
    {
        return std::string();
    }

    std::string ToString(const VertexBuffer& buffer)
    {
        return std::string();
    }

    // Index buffer

    IndexBuffer::IndexBuffer()
    {
        m_InternalIndexBuffer = Memory::Alloc<InternalIndexBuffer>();
    }

    IndexBuffer::IndexBuffer(const BufferAttribute& attribut)
    {
        Log_nts.Debug();
        m_InternalIndexBuffer = Memory::Alloc<InternalIndexBuffer>(attribut);
        Log_nts.Debug();
    }

    IndexBuffer::~IndexBuffer()
    {
    }

    InternalIndexBuffer* IndexBuffer::GetInternal()
    {
        if (m_InternalIndexBuffer == nullptr) return nullptr;
        return m_InternalIndexBuffer.get();
    }

    bool IndexBuffer::SetAttibute(const BufferAttribute& attribut)
    {
        if (m_InternalIndexBuffer == nullptr) return false;
        return m_InternalIndexBuffer->SetAttibute(attribut);
    }

    bool IndexBuffer::Create(uint32 indices[])
    {
        if (m_InternalIndexBuffer == nullptr) return false;
        return m_InternalIndexBuffer->Create(indices);
    }

    bool IndexBuffer::ChangeIndices(uint32 indices[])
    {
        if (m_InternalIndexBuffer == nullptr) return false;
        return m_InternalIndexBuffer->ChangeIndices(indices);
    }

    bool IndexBuffer::Destroy()
    {
        if (m_InternalIndexBuffer == nullptr) return false;

        return m_InternalIndexBuffer->Destroy();
    }

    uint32* IndexBuffer::GetIndices()
    {
        return nullptr;
    }

    const BufferAttribute& IndexBuffer::GetAttribut()
    {
        static BufferAttribute attribut;
        if (m_InternalIndexBuffer == nullptr) return attribut;
        return m_InternalIndexBuffer->GetAttribut();
    }

    std::string IndexBuffer::ToString() const
    {
        return std::string();
    }

    // Vertex Buffer

    VertexBuffer::VertexBuffer()
    {
        m_InternalVertexBuffer = Memory::Alloc<InternalVertexBuffer>();
    }

    VertexBuffer::VertexBuffer(const BufferAttribute& attribut)
    {
        Log_nts.Debug();
        m_InternalVertexBuffer = Memory::Alloc<InternalVertexBuffer>(attribut);
        Log_nts.Debug();
    }

    VertexBuffer::~VertexBuffer()
    {
    }

    InternalVertexBuffer* VertexBuffer::GetInternal()
    {
        //Log_nts.Debug();
        if (m_InternalVertexBuffer == nullptr) return nullptr;
        //Log_nts.Debug();
        return m_InternalVertexBuffer.get();
    }

    bool VertexBuffer::SetAttibute(const BufferAttribute& attribut)
    {
        if (m_InternalVertexBuffer == nullptr) return false;
        return m_InternalVertexBuffer->SetAttibute(attribut);
    }

    bool VertexBuffer::Create(float32 vertices[])
    {
        if (m_InternalVertexBuffer == nullptr) return false;
        return m_InternalVertexBuffer->Create(vertices);
    }

    bool VertexBuffer::ChangeVertices(float32 vertices[])
    {
        if (m_InternalVertexBuffer == nullptr) return false;
        return m_InternalVertexBuffer->ChangeVertices(vertices);
    }

    bool VertexBuffer::AddIndexBuffer(Memory::Shared<IndexBuffer> buff)
    {
        if (m_InternalVertexBuffer == nullptr) return false;
        return m_InternalVertexBuffer->AddIndexBuffer(buff);
    }

    bool VertexBuffer::Destroy()
    {
        if (m_InternalVertexBuffer == nullptr) return false;

        return m_InternalVertexBuffer->Destroy();
    }

    IndexBuffer* VertexBuffer::GetIndexBuffer(usize index)
    {
        if (m_InternalVertexBuffer == nullptr) return nullptr;

        return m_InternalVertexBuffer->GetIndexBuffer(index);
    }

    usize VertexBuffer::GetIndexBufferSize()
    {
        if (m_InternalVertexBuffer == nullptr) return 0;

        return m_InternalVertexBuffer->GetIndexBufferSize();
    }

    VertexArray* VertexBuffer::GetVertexArray()
    {
        if (m_InternalVertexBuffer == nullptr) return nullptr;
        return m_InternalVertexBuffer->GetVertexArray();
    }

    const BufferAttribute& VertexBuffer::GetAttribut()
    {
        static BufferAttribute attribut;
        if (m_InternalVertexBuffer == nullptr) return attribut;
        return m_InternalVertexBuffer->GetAttribut();
    }

    std::string VertexBuffer::ToString() const
    {
        return std::string();
    }

}  //  nkentseu