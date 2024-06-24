//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:38 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglIndexBuffer.h"

#include <glad/gl.h>

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "OpenglContext.h"
#include "OpenGLUtils.h"
#include "OpenglIndexBuffer.h"
#include "OpenglIndexBuffer.h"

namespace nkentseu {
    
    // Constructor  (m_ElementBufferObject)
    OpenglIndexBuffer::OpenglIndexBuffer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<OpenglContext>(context)){
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    OpenglIndexBuffer::~OpenglIndexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool OpenglIndexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<uint32>& indices)
    {
        return Create(bufferUsage, DrawIndexType::UInt32, indices.data(), indices.size());
    }

    bool OpenglIndexBuffer::Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* indices, uint32 leng)
    {
        if (m_Buffer.buffer != 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;
        m_Size = leng;

        if (m_Buffer.buffer != 0) {
            return false;
        }

        m_Size = leng;
        m_IndexType = indexType;
        usize size = m_Size * sizeof(uint32);

        return m_Buffer.Create("index buffer", GL_ELEMENT_ARRAY_BUFFER, GLConvert::UsageType(m_BufferUsage), indices, size, 0, 0, 1);
    }

    Memory::Shared<Context> OpenglIndexBuffer::GetContext()
    {
        return m_Context;
    }

    bool OpenglIndexBuffer::Destroy()
    {
        if (m_Context == nullptr) {
            return false;
        }
        return m_Buffer.Destroy();
    }

    bool OpenglIndexBuffer::Bind() const
    {
        if (m_Context == nullptr) {
            return false;
        }
        return m_Buffer.Bind();
    }

    bool OpenglIndexBuffer::Unbind() const
    {
        if (m_Context == nullptr) {
            return false;
        }
        return m_Buffer.Unbind();
    }

    bool OpenglIndexBuffer::SetData(void* data, usize size)
    {
        if (m_Context == nullptr) {
            return false;
        }
        return m_Buffer.WriteToBuffer(data, size);
    }

    uint32 OpenglIndexBuffer::Leng() const
    {
        return m_Size;
    }

    DrawIndexType::Code OpenglIndexBuffer::GetIndexType() const
    {
        return m_IndexType;
    }

    uint32 OpenglIndexBuffer::GetBuffer() const
    {
        return m_Buffer.buffer;
    }

}  //  nkentseu