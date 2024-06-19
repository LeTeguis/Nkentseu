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
    using namespace maths;

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
            m_Modules[GLConvert::GetModernModuleType(type_)] = module;
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
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        if (Unbind()) {
            OpenGLResult result;
            bool first = true;
            glCheckError(first, result, glDeleteProgramPipelines(1, &m_Programme), "cannot delete pipeline");
            m_Programme = 0;
            return true;
        }
        return false;
    }

    OpenglShader::~OpenglShader() {
    }

    bool OpenglShader::Bind() {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;
        glCheckError(first, result, glBindProgramPipeline(m_Programme), "cannot bind shader program");
        return result.success;
    }

    bool OpenglShader::Unbind() {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;
        glCheckError(first, result, glBindProgramPipeline(0), "cannot unbind shader program");
        return result.success;
    }

    bool OpenglShader::DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode)
    {
        return PolygonMode(contentMode) && CullMode(mode);
    }

    bool OpenglShader::PolygonMode(PolygonModeType::Code mode)
    {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;
        glCheckError(first, result, glPolygonMode(GLConvert::CullModeType(CullModeType::FrontBack), GLConvert::PolygonModeType(mode)), "cannot change polygon mode");
        return result.success;
    }

    bool OpenglShader::CullMode(CullModeType::Code mode)
    {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }

        static bool cullFaceIsEnable = false;
        OpenGLResult result;
        bool first = true;

        if (mode != CullModeType::NoCull) {
            if (!cullFaceIsEnable) {
                glCheckError(first, result, glEnable(GL_CULL_FACE), "cannot enable cull face mode");
                cullFaceIsEnable = true;
            }
            glCheckError(first, result, glCullFace(GLConvert::CullModeType(mode)), "cannot change  cull face mode");
        }
        else {
            if (cullFaceIsEnable) {
                cullFaceIsEnable = false;
            }
        }
        return result.success;
    }

    bool OpenglShader::FrontFaceMode(FrontFaceType::Code mode)
    {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        OpenGLResult result;
        bool first = true;

        if (mode == FrontFaceType::Clockwise) {
            glCheckError(first, result, glFrontFace(GL_CW), "cannot set front face clock wise");
        }
        else if (mode == FrontFaceType::CounterClockwise) {
            glCheckError(first, result, glFrontFace(GL_CCW), "cannot set front face cunter clock wise");
        }
        return result.success;
    }

    bool OpenglShader::PrimitiveTopologyMode(PrimitiveTopologyType::Code mode)
    {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        return true;
    }

    bool OpenglShader::ScissorMode(const Vector2i& offset, const Vector2u& extend)
    {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        return true;
    }

    bool OpenglShader::ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth)
    {
        if (m_Context == nullptr || m_Programme == 0) {
            return false;
        }
        return true;
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
        const char* shaderSrc = shaderSource.c_str(); 
        
        glCheckError(first, result, uint32 shaderModule = glCreateShaderProgramv(module_type, 1, &shaderSrc), "cannot create shader");

        if (!result.success && shaderModule != 0) {
            glCheckError(first, result, glDeleteProgram(shaderModule), "cannot delete shader");
            shaderModule = 0;
        }
        return shaderModule;
    }

    uint32 OpenglShader::MakeShader()
    {
        OpenGLResult result;
        bool first = true;
        uint32 pipeline = 0;
        glCheckError(first, result, glCreateProgramPipelines(1, &pipeline); , "cannot create shader program");

        if (!m_Layout.vertexInput.attributes.empty()) {
            for (const auto& attribute : m_Layout.vertexInput.attributes) {
                glCheckError(first, result, glBindAttribLocation(pipeline, attribute.location, attribute.name.c_str()), "cannot bind attribut location");
            }
        }

        for (auto [type, shaderModule] : m_Modules) {
            glCheckError(first, result, glUseProgramStages(pipeline, type, shaderModule), "cannot attach shader");
        }

        glCheckError(first, result, glBindProgramPipeline(pipeline), "cannot link program");

        for (auto [type, shaderModule] : m_Modules) {
            glCheckError(first, result, glDeleteProgram(shaderModule), "cannot delete shader");
        }

        if (!result.success && pipeline != 0) {
            glCheckError(first, result, glDeleteProgramPipelines(1, &pipeline), "cannot delete pipeline");
        }

        m_Modules.clear();

        return pipeline;
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