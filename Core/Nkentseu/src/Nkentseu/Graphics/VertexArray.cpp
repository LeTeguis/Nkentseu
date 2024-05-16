//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VertexArray.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalVertexArray.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    
    // Constructor
    VertexArray::VertexArray() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    VertexArray::~VertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    bool VertexArray::Create()
    {
        if (m_InternalVertexArray == nullptr) {
            m_InternalVertexArray = Memory::Alloc<InternalVertexArray>();

            if (m_InternalVertexArray == nullptr) {
                return false;
            }
        }
        return m_InternalVertexArray->Create();
    }

    bool VertexArray::Destroy()
    {
        if (m_InternalVertexArray == nullptr) return false;
        return m_InternalVertexArray->Destroy();
    }

    InternalVertexArray* VertexArray::GetInternal()
    {
        if (m_InternalVertexArray == nullptr) return nullptr;

        return m_InternalVertexArray.get();
    }

    std::string VertexArray::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const VertexArray& vertexArray) {
        return vertexArray.ToString();
    }

}  //  nkentseu