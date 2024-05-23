//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:56:08 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalShader.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Logger/Formatter.h>

#include <glad/gl.h>
#include <vector>
#include <fstream>
#include <sstream>

#include <Nkentseu/Core/NkentseuLogger.h>
#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"
#include "OpenGLUtils.h"

namespace nkentseu {
    
    // Constructor
    InternalShader::InternalShader(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles) : m_Programme(0) {
        // Ajoutez votre code de constructeur ici
        SetShaderFiles(context, shaderFiles);
    }

    void InternalShader::SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles)
    {
        if (context == nullptr || context->GetInternal()) return;

        m_ShaderFiles.clear();
        m_ShaderFiles = shaderFiles;
        m_Context = context;
        // Create a deep copy of shaderFiles using the copy constructor
        // std::copy(m_ShaderFiles.begin(), m_ShaderFiles.end(), shaderFiles);
    }

    bool InternalShader::Create()
    {
        if (m_Context == nullptr || m_Context->GetInternal() || m_Programme != 0) {
            return false;
        }

        m_Modules.clear();

        for (auto [shaderType, shaderFile] : m_ShaderFiles) {
            uint32 module = MakeModule(shaderFile, shaderType);

            if (module == 0) {
                m_Modules.clear();
                return false;
            }
            m_Modules.push_back(module);
        }
        return true;
    }

    bool InternalShader::Compile()
    {
        if (m_Context == nullptr || m_Context->GetInternal() || m_Modules.size() == 0) {
            return false;
        }

        uint32 programme = MakeShader();

        if (programme == 0) {
            return false;
        }

        m_Programme = programme;
        return true;
    }

    bool InternalShader::Destroy()
    {
        if (m_Context == nullptr || m_Context->GetInternal() || m_Programme != 0) {
            if (Unbind()) {
                m_Programme = 0;
                return true;
            }
        }
        return false;
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
        if (m_Context == nullptr || m_Context->GetInternal() || m_Programme != 0) {
            glUseProgram(m_Programme);
            glCheckError();
            return true;
        }
        return false;
    }

    bool InternalShader::Unbind() const {
        if (m_Context == nullptr || m_Context->GetInternal() || m_Programme != 0) {
            glUseProgram(0);
            glCheckError();
            return true;
        }
        return false;
    }

    uint32 InternalShader::MakeModule(const std::string& filepath, ShaderType::Code code)
    {
        uint32 module_type = GLConvert::GetModuleType(code);
        if (module_type == 0) {
            return 0;
        }

        std::string shaderSource = LoadShader(filepath);
        // Log_nts.Debug("<{0}>", shaderSource);
        const char* shaderSrc = shaderSource.c_str();

        uint32 shaderModule = glCreateShader(module_type);
        glCheckError();
        glShaderSource(shaderModule, 1, &shaderSrc, NULL);
        glCheckError();
        glCompileShader(shaderModule);
        glCheckError();

        int32 success;
        glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
        glCheckError();
        if (!success) {
            char errorLog[1024];
            glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
            Log_nts.Error("Shader Module {0} compile error: {1}", ShaderType::ToString(code), std::string(errorLog));
            return 0;
        }
        return shaderModule;
    }

    uint32 InternalShader::MakeShader()
    {
        uint32 shader = glCreateProgram();
        glCheckError();

        /*
            // This step is unnecessary if you use the location specifier in your shader
            // e.g. layout (location = 0) in vec3 position;
            glBindAttribLocation(program, 0, "position"); // The index passed into glBindAttribLocation is
            glBindAttribLocation(program, 1, "texcoord"); // used by glEnableVertexAttribArray. "position"
            glBindAttribLocation(program, 2, "normal");   // "texcoord" "normal" and "color" are the names of the
            glBindAttribLocation(program, 3, "color");    // respective inputs in your fragment shader.
        */

        for (uint32 shaderModule : m_Modules) {
            glAttachShader(shader, shaderModule);
            glCheckError();
        }

        glLinkProgram(shader);
        glCheckError();

        int32 success;
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        glCheckError();

        if (!success) {
            GLsizei logLength = 0;
            GLchar errorLog[1024];
            //glGetShaderInfoLog(shader, 1024, NULL, errorLog);
            glGetProgramInfoLog(shader, 1024, &logLength, errorLog);
            Log_nts.Error("Shader Module compile error: {0}", std::string(errorLog));
            shader = 0;

            for (uint32 shaderModule : m_Modules) {
                glDeleteShader(shaderModule);
                glCheckError();
            }

            return 0;
        }

        for (uint32 shaderModule : m_Modules) {
            glDeleteShader(shaderModule);
            glCheckError();
        }

        m_Modules.clear();

        return shader;
    }

    std::string InternalShader::LoadShader(const std::string& shaderFile)
    {
        std::ifstream file;
        std::stringstream bufferedLines;
        std::string line;

        file.open(shaderFile);

        while (std::getline(file, line)) {
            bufferedLines << line << "\n";
        }

        std::string shaderSource = bufferedLines.str();
        bufferedLines.str("");

        file.close();

        return shaderSource;
    }

}  //  nkentseu

#endif