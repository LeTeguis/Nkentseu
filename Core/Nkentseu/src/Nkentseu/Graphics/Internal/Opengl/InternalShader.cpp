//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:08 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalShader.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Logger/Formatter.h>

#include <glad/gl.h>
#include <vector>
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    static uint32 MakeModule(const std::string& filepath, ShaderType::Code code);
    static uint32 MakeShader(const std::unordered_map<ShaderType::Code, std::string>& filesShaderPath);
    static uint32 GetModuleType(ShaderType::Code code);
    
    // Constructor
    InternalShader::InternalShader(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles) {
        // Ajoutez votre code de constructeur ici
        m_Programme = MakeShader(shaderFiles);
    }

    // Destructor
    InternalShader::~InternalShader() {
        // Ajoutez votre code de destructeur ici
    }

    std::string InternalShader::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const InternalShader& internalShader) {
        return internalShader.ToString();
    }

    bool InternalShader::Bind() const {
        return false;
    }

    bool InternalShader::Unbind() const {
        return false;
    }

    uint32 MakeModule(const std::string& filepath, ShaderType::Code code)
    {
        uint32 module_type = GetModuleType(code);
        if (module_type == 0) {
            return 0;
        }

        std::string shaderSource = ShaderLoader::LoadShaderToMemoryStr(filepath);
        // Log_nts.Debug("<{0}>", shaderSource);
        const char* shaderSrc = shaderSource.c_str();

        uint32 shaderModule = glCreateShader(module_type);
        glShaderSource(shaderModule, 1, &shaderSrc, NULL);
        glCompileShader(shaderModule);

        int32 success;
        glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
        if (!success) {
            char errorLog[1024];
            glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
            Log_nts.Error("Shader Module {0} compile error: {1}", ShaderType::ToString(code), std::string(errorLog));
            return 0;
        }
        return shaderModule;
    }

    uint32 MakeShader(const std::unordered_map<ShaderType::Code, std::string>& filesShaderPath)
    {
        std::vector<uint32> modules;

        for (auto [shaderType, shaderFile] : filesShaderPath) {
            modules.push_back(MakeModule(shaderFile, shaderType));
        }

        uint32 shader = glCreateProgram();

        /*
        gl_bin attribut
        */
        for (uint32 shaderModule : modules) {
            glAttachShader(shader, shaderModule);
        }

        glLinkProgram(shader);

        int32 success;
        Log_nts.Debug();
        glGetShaderiv(shader, GL_LINK_STATUS, &success);
        Log_nts.Debug();
        if (!success) {
            char errorLog[1024];
            glGetShaderInfoLog(shader, 1024, NULL, errorLog);
            Log_nts.Error("Shader Module compile error: {0}", std::string(errorLog));
            shader = 0;
        }

        for (uint32 shaderModule : modules) {
            glDeleteShader(shaderModule);
        }

        return shader;
    }

    uint32 GetModuleType(ShaderType::Code code)
    {
        if (code == ShaderType::Vertex) return GL_VERTEX_SHADER;
        if (code == ShaderType::Fragment) return GL_FRAGMENT_SHADER;
        if (code == ShaderType::Compute) return GL_COMPUTE_SHADER;
        if (code == ShaderType::Geometry) return GL_GEOMETRY_SHADER;
        //if (code == ShaderType::Pixel) return GL_PIXEL_SHADER;
        if (code == ShaderType::TesControl) return GL_TESS_CONTROL_SHADER;
        if (code == ShaderType::TesEvaluation) return GL_TESS_EVALUATION_SHADER;
        return 0;
    }

}  //  nkentseu

#endif