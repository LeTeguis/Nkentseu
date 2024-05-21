//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 08:06:06 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "PushConstant.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    
    // Constructor
    PushConstant::PushConstant() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    PushConstant::~PushConstant() {
        // Ajoutez votre code de destructeur ici
    }

    bool PushConstant::Create(const PushConstantLayout& pushConstantLayout)
    {
        return false;
    }

    bool PushConstant::Destroy()
    {
        return false;
    }

    std::string PushConstant::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const PushConstant& pushConstant) {
        return pushConstant.ToString();
    }

}  //  nkentseu