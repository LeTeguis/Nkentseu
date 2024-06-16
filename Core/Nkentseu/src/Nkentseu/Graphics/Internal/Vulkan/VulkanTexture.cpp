//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:23:53 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanTexture.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    VulkanTexture::VulkanTexture() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    VulkanTexture::~VulkanTexture() {
        // Ajoutez votre code de destructeur ici
    }

    std::string VulkanTexture::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const VulkanTexture& vulkanTexture) {
        return vulkanTexture.ToString();
    }

}  //  nkentseu