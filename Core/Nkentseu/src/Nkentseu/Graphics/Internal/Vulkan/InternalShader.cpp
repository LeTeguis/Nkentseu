//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:52 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalShader.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"

namespace nkentseu {
    
    // Constructor
    InternalShader::InternalShader(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles) {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalShader::~InternalShader() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalShader::Create() {
        return false;
    }

    bool InternalShader::CreateShader() {
        return false;
    }

    bool InternalShader::Destroy() {
        return false;
    }

    void InternalShader::SetShaderFiles(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles) {
    }


}  //  nkentseu

#endif