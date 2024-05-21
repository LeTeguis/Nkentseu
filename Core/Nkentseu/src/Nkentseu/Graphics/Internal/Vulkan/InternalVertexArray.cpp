//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:22:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexArray.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"

#include "InternalVertexBuffer.h"
#include "InternalIndexBuffer.h"

namespace nkentseu {
    
    // Constructor
    InternalVertexArray::InternalVertexArray() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexArray::~InternalVertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexArray::Create(const BufferLayout& bufferLayout) {
        return false;
    }

    bool InternalVertexArray::Destroy() {
        return false;
    }

    bool InternalVertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer) {
        return false;
    }

    Memory::Shared<VertexBuffer> InternalVertexArray::GetVertexBuffer() {
        return nullptr;
    }

    InternalVertexBuffer* InternalVertexArray::GetInternalVertexBuffer() {
        return nullptr;
    }

    bool InternalVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) {
        return false;
    }

    Memory::Shared<IndexBuffer> InternalVertexArray::GetIndexBuffer() {
        return nullptr;
    }

    InternalIndexBuffer* InternalVertexArray::GetInternalIndexBuffer() {
        return nullptr;
    }

}  //  nkentseu

#endif