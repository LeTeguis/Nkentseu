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
        if (m_ElementBufferObject != 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glGenBuffers(1, &m_ElementBufferObject), "cannot gen buffer for index buffer");
        if (!result.success || m_ElementBufferObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        m_BufferUsage = bufferUsage;

        glCheckError(first, result, glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices.size(), indices.data(), GLConvert::UsageType(m_BufferUsage)), "cannot set buffer data for index buffer");

        if (!result.success) {
            return false;
        }

        m_Leng = indices.size();

        m_IndexType = DrawIndexType::UInt32;
        return Unbind();
    }

    bool OpenglIndexBuffer::Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* indices, uint32 leng)
    {
        if (m_ElementBufferObject != 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glGenBuffers(1, &m_ElementBufferObject), "cannot gen buffer for index buffer");
        if (!result.success || m_ElementBufferObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        m_BufferUsage = bufferUsage;

        glCheckError(first, result, glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * leng, indices, GLConvert::UsageType(m_BufferUsage)), "cannot set buffer data for index buffer");

        if (!result.success) {
            return false;
        }

        m_Leng = leng;

        m_IndexType = indexType;
        return Unbind();
    }

    Memory::Shared<Context> OpenglIndexBuffer::GetContext()
    {
        return m_Context;
    }

    bool OpenglIndexBuffer::Destroy()
    {
        if (m_ElementBufferObject == 0 || m_Context == nullptr) {
            return false;
        }
        return true;
    }

    bool OpenglIndexBuffer::Bind()
    {
        if (m_ElementBufferObject == 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObject), "cannot bin index buffer");
        return result.success;
    }

    bool OpenglIndexBuffer::Unbind()
    {
        if (m_ElementBufferObject == 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0), "cannot unbin index buffer");
        return result.success;
    }

    uint32 OpenglIndexBuffer::Leng() const
    {
        return m_Leng;
    }

    DrawIndexType::Code OpenglIndexBuffer::GetIndexType() const
    {
        return m_IndexType;
    }

}  //  nkentseu