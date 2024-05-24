//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <glad/gl.h>

#include <Logger/Formatter.h>
#include "InternalContext.h"
#include "OpenGLUtils.h"

namespace nkentseu {
    GLfloat vertices_[] =
    {
        // Positions         // Colors
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
    };
    // Constructor
    InternalVertexBuffer::InternalVertexBuffer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexBuffer::~InternalVertexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout)
    {
        if (m_VertexBufferObject != 0) {
            return false;
        }

        glGenBuffers(1, &m_VertexBufferObject);
        if (glCheckError() != GL_NO_ERROR || m_VertexBufferObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        m_BufferUsage = bufferUsage;

        glBufferData(GL_ARRAY_BUFFER, sizeof(float32) * vertices.size(), vertices.data(), GLConvert::UsageType(m_BufferUsage));

        if (glCheckError() != GL_NO_ERROR) {
            return false;
        }

        m_Leng = vertices.size() / bufferLayout.componentCount;

        if (Unbind()) {
            return true;
        }

        m_Vertices.clear();
        for (uint32 index = 0; index < vertices.size(); index++) {
            m_Vertices.push_back(vertices[index]);
        }
        return true;
    }

    bool InternalVertexBuffer::Destroy()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }
        return true;
    }

    bool InternalVertexBuffer::Bind()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

        return glCheckError() == GL_NO_ERROR;
    }

    bool InternalVertexBuffer::Unbind()
    {
        if (m_VertexBufferObject == 0) {
            return false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        bool success = glCheckError() == GL_NO_ERROR;

        return success;
    }

    std::vector<float32> InternalVertexBuffer::GetVertices()
    {
        return m_Vertices;
    }

    uint32 InternalVertexBuffer::Leng()
    {
        return m_Leng;
    }

    uint32 InternalVertexBuffer::GetVBO()
    {
        return m_VertexBufferObject;
    }

}  //  nkentseu

#endif