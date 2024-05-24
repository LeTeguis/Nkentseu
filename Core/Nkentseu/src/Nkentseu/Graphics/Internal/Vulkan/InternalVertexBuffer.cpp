//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:11:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    InternalVertexBuffer::InternalVertexBuffer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexBuffer::~InternalVertexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout) {
        return false;
    }

    bool InternalVertexBuffer::Destroy() {
        return false;
    }

}  //  nkentseu

#endif