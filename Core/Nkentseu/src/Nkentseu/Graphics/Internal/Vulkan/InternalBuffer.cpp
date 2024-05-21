//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:36:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    InternalBuffer::InternalBuffer(const BufferAttribute& attribut) {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalBuffer::~InternalBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalBuffer::SetAttibute(const BufferAttribute& attribut) {
        return false;
    }

    bool InternalBuffer::Create() {
        return false;
    }

    const BufferAttribute& InternalBuffer::GetAttribut() {
        static BufferAttribute tmp;
        return tmp;
    }

}  //  nkentseu

#endif