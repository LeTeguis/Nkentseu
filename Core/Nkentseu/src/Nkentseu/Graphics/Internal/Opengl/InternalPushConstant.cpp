//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 08:06:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalPushConstant.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    InternalPushConstant::InternalPushConstant() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalPushConstant::~InternalPushConstant() {
        // Ajoutez votre code de destructeur ici
    }

    std::string InternalPushConstant::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const InternalPushConstant& internalPushConstant) {
        return internalPushConstant.ToString();
    }

}  //  nkentseu