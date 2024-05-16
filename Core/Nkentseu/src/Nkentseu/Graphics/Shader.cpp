//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:53:53 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Shader.h"

#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalShader.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    
    // Constructor
    Shader::Shader(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles) : m_InternalShader(nullptr) {
        // Ajoutez votre code de constructeur ici
        m_InternalShader = Memory::Alloc<InternalShader>(shaderFiles);

        if (m_InternalShader == nullptr) {
            Log_nts.Error("unload shader");
        }
    }

    // Destructor
    Shader::~Shader() {
        // Ajoutez votre code de destructeur ici
        if (m_InternalShader == nullptr) return;

        Memory::Reset(m_InternalShader);
    }

    InternalShader* Shader::GetInternal()
    {
        if (m_InternalShader == nullptr) return nullptr;

        return m_InternalShader.get();
    }

    std::string Shader::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const Shader& shader) {
        return shader.ToString();
    }

}  //  nkentseu