//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:23:31 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglTexture.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    OpenglTexture::OpenglTexture() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    OpenglTexture::~OpenglTexture() {
        // Ajoutez votre code de destructeur ici
    }

    std::string OpenglTexture::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const OpenglTexture& openglTexture) {
        return openglTexture.ToString();
    }

}  //  nkentseu