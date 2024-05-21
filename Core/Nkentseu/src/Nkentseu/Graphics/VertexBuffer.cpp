//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:46:43 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VertexBuffer.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalVertexBuffer.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalVertexBuffer.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    
    // Constructor
    VertexBuffer::VertexBuffer() {
        // Ajoutez votre code de constructeur ici
        m_Internal = Memory::Alloc<InternalVertexBuffer>();
    }

    // Destructor
    VertexBuffer::~VertexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool VertexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, uint32 numberElements)
    {
        if (m_Internal == nullptr) {
            m_Internal = Memory::Alloc<InternalVertexBuffer>();
            if (m_Internal == nullptr) {
                return false;
            }
        }
        return m_Internal->Create(bufferUsage, vertices, numberElements);
    }

    bool VertexBuffer::Destroy()
    {
        if (m_Internal == nullptr) return false;
        
        return m_Internal->Destroy();
    }

    InternalVertexBuffer* VertexBuffer::GetInternal()
    {
        if (m_Internal == nullptr) return nullptr;
        return m_Internal.get();
    }
}  //  nkentseu