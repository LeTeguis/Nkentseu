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
    InternalShader::InternalShader(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout) : m_Programme(0) {
        if (context == nullptr) return;
        SetShaderFiles(context, shaderFiles, bufferLayout);
    }

    void InternalShader::SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout)
    {
        if (context == nullptr || context->GetInternal() == nullptr ) return;

        m_ShaderFiles.clear();
        m_ShaderFiles = shaderFiles;
        m_Context = context;
        m_BufferLayout = bufferLayout;
        // Create a deep copy of shaderFiles using the copy constructor
        // std::copy(m_ShaderFiles.begin(), m_ShaderFiles.end(), shaderFiles);
    }

    bool InternalShader::Create()
    {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || m_Programme != 0) {
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
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || m_Modules.size() == 0) {
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
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr  || m_Programme != 0) {
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
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr  || m_Programme != 0) {
            OpenGLResult result;
            bool first = true;
            glCheckError(first, result, glUseProgram(m_Programme), "cannot bind shader program");
            return true;
        }
        return false;
    }

    bool InternalShader::Unbind() const {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr  || m_Programme != 0) {
            OpenGLResult result;
            bool first = true;
            glCheckError(first, result, glUseProgram(0), "cannot unbind shader program");
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

        OpenGLResult result;
        bool first = true;

        std::string shaderSource = LoadShader(filepath);
        // Log_nts.Debug("<{0}>", shaderSource);
        const char* shaderSrc = shaderSource.c_str();

        glCheckError(first, result, uint32 shaderModule = glCreateShader(module_type), "cannot create shader");
        glCheckError(first, result, glShaderSource(shaderModule, 1, &shaderSrc, NULL), "cannot set shader source");
        glCheckError(first, result, glCompileShader(shaderModule), "cannot compile shader");

        int32 success;
        glCheckError(first, result, glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success), "cannot get shader iv");

        if (!success) {
            char errorLog[1024];
            glCheckError(first, result, glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog), "cannot get shader info log");
            Log_nts.Error("Shader Module {0} compile error: {1}", ShaderType::ToString(code), std::string(errorLog));
            return 0;
        }
        return shaderModule;
    }

    uint32 InternalShader::MakeShader()
    {
        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, uint32 shader = glCreateProgram(), "cannot create shader program");

        if (!m_BufferLayout.attributes.empty()) {
            for (const auto& attribute : m_BufferLayout.attributes) {
                glCheckError(first, result, glBindAttribLocation(shader, attribute.location, attribute.name.c_str()), "cannot bind attribut location");
            }

        }

        for (uint32 shaderModule : m_Modules) {
            glCheckError(first, result, glAttachShader(shader, shaderModule), "cannot attach shader");
        }

        glCheckError(first, result, glLinkProgram(shader), "cannot link program");

        int32 success;
        glCheckError(first, result, glGetProgramiv(shader, GL_LINK_STATUS, &success), "cannot get program iv");

        if (!success) {
            GLsizei logLength = 0;
            GLchar errorLog[1024];
            //glGetShaderInfoLog(shader, 1024, NULL, errorLog);
            glCheckError(first, result, glGetProgramInfoLog(shader, 1024, &logLength, errorLog), "cannot get program info");
            Log_nts.Error("Shader Module compile error: {0}", std::string(errorLog));
            shader = 0;

            for (uint32 shaderModule : m_Modules) {
                glCheckError(first, result, glDeleteShader(shaderModule), "cannot delete shader");
            }

            return 0;
        }

        for (uint32 shaderModule : m_Modules) {
            glCheckError(first, result, glDeleteShader(shaderModule), "cannot delete shader");
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