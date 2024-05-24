//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:53:53 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Shader.h"

#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalShader.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalShader.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

#include "Context.h"

namespace nkentseu {
    
    // Constructor
    Shader::Shader(Context *context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout) : m_InternalShader(nullptr) {
        // Ajoutez votre code de constructeur ici
        m_InternalShader = Memory::Alloc<InternalShader>(context, shaderFiles, bufferLayout);

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

    bool Shader::Create()
    {
        if (m_InternalShader == nullptr) return false;

        bool isCreate = m_InternalShader->Create();

        if (!isCreate) {
            Log_nts.Error("Cannot create shader");
            return false;
        }

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
        if (!m_InternalShader->Compile()) {
            Log_nts.Error("Cannot compile opengl shader");
            return false;
        }
#endif

        return isCreate;
    }

    bool Shader::Destroy()
    {
        if (m_InternalShader == nullptr) return false;

        return m_InternalShader->Destroy();
    }

    void Shader::SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout)
    {
        if (m_InternalShader == nullptr) return;

        m_InternalShader->SetShaderFiles(context, shaderFiles, bufferLayout);
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