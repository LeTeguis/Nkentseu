//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:21:33 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalIndexBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    InternalIndexBuffer::InternalIndexBuffer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalIndexBuffer::~InternalIndexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalIndexBuffer::Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices) {
        return false;
    }

    bool InternalIndexBuffer::Destroy() {
        return false;
    }

}  //  nkentseu

#endif