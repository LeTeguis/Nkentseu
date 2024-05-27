//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:38 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalIndexBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <glad/gl.h>

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"
#include "OpenGLUtils.h"
#include "InternalIndexBuffer.h"

namespace nkentseu {
    
    // Constructor  (m_ElementBufferObject)
    InternalIndexBuffer::InternalIndexBuffer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalIndexBuffer::~InternalIndexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalIndexBuffer::Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices, const BufferLayout& bufferLayout)
    {
        if (m_ElementBufferObject != 0) {
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
        m_Indices.clear();
        for (uint32 index = 0; index < indices.size(); index++) {
            m_Indices.push_back(indices[index]);
        }

        m_IndexType = indexType;
        return Unbind();
    }

    bool InternalIndexBuffer::Destroy()
    {
        if (m_ElementBufferObject == 0) {
            return false;
        }
        return true;
    }

    bool InternalIndexBuffer::Bind()
    {
        if (m_ElementBufferObject == 0) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObject), "cannot bin index buffer");
        return result.success;
    }

    bool InternalIndexBuffer::Unbind()
    {
        if (m_ElementBufferObject == 0) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0), "cannot unbin index buffer");
        return result.success;
    }

    uint32 InternalIndexBuffer::Leng() const
    {
        return m_Leng;
    }

    DrawIndexType::Code InternalIndexBuffer::GetIndexType() const
    {
        return m_IndexType;
    }

}  //  nkentseu

#endif