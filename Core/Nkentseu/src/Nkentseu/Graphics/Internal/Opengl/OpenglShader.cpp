//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:56:08 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglShader.h"

#include <Logger/Formatter.h>

#include <glad/gl.h>
#include <vector>
#include <fstream>
#include <sstream>

#include <Nkentseu/Core/NkentseuLogger.h>
#include "OpenGLUtils.h"

namespace nkentseu {
    
    // Constructor
    OpenglShader::OpenglShader(Memory::Shared<Context> context) : m_Programme(0), m_Context(Memory::SharedCast<OpenglContext>(context)), m_Layout({}) {
    }

    bool OpenglShader::LoadFromFile(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout) {
        if (m_Context == nullptr || m_Programme != 0) {
            return false;
        }

        m_Modules.clear();
        m_Layout = shaderLayout;

        for (auto [type_, file_] : shaderFiles) {
            uint32 module = MakeModule(file_, type_);

            if (module == 0) {
                m_Modules.clear();
                return false;
            }
            m_Modules.push_back(module);
        }
        return Compile();
    }

    bool OpenglShader::Compile()
    {
        if (m_Context == nullptr || m_Modules.size() == 0) {
            return false;
        }

        m_Programme = MakeShader();

        if (m_Programme == 0) {
            return false;
        }
        return true;
    }

    Memory::Shared<Context> OpenglShader::GetContext()
    {
        return m_Context;
    }

    bool OpenglShader::Destroy()
    {
        if (m_Context == nullptr || m_Programme != 0) {
            if (Unbind()) {
                m_Programme = 0;
                return true;
            }
        }
        return false;
    }

    // Destructor
    OpenglShader::~OpenglShader() {
        // Ajoutez votre code de destructeur ici
    }

    bool OpenglShader::Bind() const {
        if (m_Context == nullptr || m_Programme != 0) {
            OpenGLResult result;
            bool first = true;
            glCheckError(first, result, glUseProgram(m_Programme), "cannot bind shader program");
            return true;
        }
        return false;
    }

    bool OpenglShader::Unbind() const {
        if (m_Context == nullptr || m_Programme != 0) {
            OpenGLResult result;
            bool first = true;
            glCheckError(first, result, glUseProgram(0), "cannot unbind shader program");
            return true;
        }
        return false;
    }
    
    bool OpenglShader::UseUniform(const std::string& name, void* data, usize size, uint32 index)
    {// Vérifier si le contexte est valide
        if (m_Context == nullptr || data == nullptr || size == 0) return false;

        // Rechercher le tampon uniforme correspondant au nom donné
        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            // Le tampon uniforme avec ce nom n'a pas été trouvé
            return false;
        }

        OpenglBuffer& uniformBuffer = it->second;

        if (uniformBuffer.uniform != 0) {
            uniformBuffer.WriteToBuffer(data, size, index);
        }
    }

    uint32 OpenglShader::MakeModule(const std::string& filepath, ShaderType::Code code)
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

    uint32 OpenglShader::MakeShader()
    {
        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, uint32 shader = glCreateProgram(), "cannot create shader program");

        if (!m_Layout.vertexInput.attributes.empty()) {
            for (const auto& attribute : m_Layout.vertexInput.attributes) {
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

        for (auto& ubo : m_Layout.uniformBuffer.attributes) {
            m_UniformBuffers[ubo.name] = {};
            m_UniformBuffers[ubo.name].Create(shader, ubo.name, ubo.size, ubo.usage, ubo.binding, 0);
            //m_UniformBuffers[ubo.name].Create(shader, ubo.name, ubo.size, ubo.usage, ubo.binding, ubo.offset);
        }

        return shader;
    }

    std::string OpenglShader::LoadShader(const std::string& shaderFile)
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