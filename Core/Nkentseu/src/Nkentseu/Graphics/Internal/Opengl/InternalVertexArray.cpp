//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:11:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexArray.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    InternalVertexArray::InternalVertexArray() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexArray::~InternalVertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexArray::Create()
    {
        if (m_VertexArrayObject != 0) return false;
        glGenVertexArrays(1, &m_VertexArrayObject);
        return m_VertexArrayObject != 0;
    }

    bool InternalVertexArray::Destroy()
    {
        if (m_VertexArrayObject == 0) return false;

        glDeleteVertexArrays(1, &m_VertexArrayObject);
        m_VertexArrayObject = 0;
        return true;
    }

    bool InternalVertexArray::Bind()
    {
        if (m_VertexArrayObject == 0) return false;
        glBindVertexArray(m_VertexArrayObject);
        return true;
    }

    bool InternalVertexArray::Unbind()
    {
        if (m_VertexArrayObject == 0) return false;
        glBindVertexArray(0);
        return true;
    }

    GLuint InternalVertexArray::GetVAO()
    {
        return m_VertexArrayObject;
    }

    std::string InternalVertexArray::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const InternalVertexArray& internalVertexArray) {
        return internalVertexArray.ToString();
    }

}  //  nkentseu

#endif