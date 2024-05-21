//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:46:54 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "IndexBuffer.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalIndexBuffer.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalIndexBuffer.h"
#endif

namespace nkentseu {
    
    // Constructor
    IndexBuffer::IndexBuffer() {
        // Ajoutez votre code de constructeur ici
        m_Internal = Memory::Alloc<InternalIndexBuffer>();
    }

    // Destructor
    IndexBuffer::~IndexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool IndexBuffer::Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices)
    {
        if (m_Internal == nullptr) {
            m_Internal = Memory::Alloc<InternalIndexBuffer>();
            if (m_Internal == nullptr) {
                return false;
            }
        }
        return m_Internal->Create(bufferUsage, indexType, indices);
    }

    bool IndexBuffer::Destroy()
    {
        if (m_Internal == nullptr) return false;
        return m_Internal->Destroy();
    }

    InternalIndexBuffer* IndexBuffer::GetInternal()
    {
        if (m_Internal == nullptr) return nullptr;

        return m_Internal.get();
    }

}  //  nkentseu