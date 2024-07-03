//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-03 at 08:33:39 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglUniformBuffer.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    OpenglUniformBuffer::OpenglUniformBuffer(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<OpenglContext>(context))
    {
        m_Buffers.clear();
        m_Oglsil = Memory::SharedCast<OpenglShaderInputLayout>(sil);
    }

    OpenglUniformBuffer::~OpenglUniformBuffer() {
    }

    bool OpenglUniformBuffer::Create(Memory::Shared<Shader> shader, const std::vector<std::string> uniformsLoader)
    {
        m_Shader = Memory::SharedCast<OpenglShader>(shader);

        if (m_Buffers.size() != 0 || m_Context == nullptr || m_Shader == nullptr || m_Oglsil == nullptr || uniformsLoader.empty()) {
            return false;
        }
        bool success = true;
        m_Shader->Bind();
        for (auto& attribut : m_Oglsil->uniformInput.attributes) {
            bool loadit = false;
            for (auto name : uniformsLoader) {
                if (name == attribut.name) {
                    loadit = true;
                }
            }

            if (!loadit) {
                continue;
            }

            if (attribut.usage != BufferUsageType::Enum::DynamicDraw) {
                attribut.instance = 1;
            }

            m_Buffers[attribut.name].Create(attribut.name, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, nullptr, attribut.size, attribut.binding, 0, attribut.instance);
            m_Buffers[attribut.name].uType = attribut.usage;

            OpenGLResult result;
            bool first = true;

            glCheckError(first, result, GLuint blockIndex = glGetUniformBlockIndex(m_Shader->GetProgramme(), attribut.name.c_str()), "");
            if (blockIndex == GL_INVALID_INDEX) {
                Log_nts.Error("Error: Uniform block index is invalid : {0}-{1}", attribut.name, m_Shader->GetProgramme());
            }
            else {
                glCheckError(first, result, glUniformBlockBinding(m_Shader->GetProgramme(), blockIndex, attribut.binding), "");
            }
        }

        m_Shader->Unbind();
        return success;
    }

    bool OpenglUniformBuffer::Destroy()
    {
        bool success = true;
        for (auto& [name, ubo] : m_Buffers) {
            success = success && ubo.Destroy();
        }
        m_Buffers.clear();
        return success;
    }

    bool OpenglUniformBuffer::SetData(const std::string& name, void* data, usize size)
    {
        return SetData(name, data, size, 0);
    }

    bool OpenglUniformBuffer::SetData(const std::string& name, void* data, usize size, uint32 index)
    {
        if (m_Context == nullptr || data == nullptr || size == 0) {
            return false;
        }

        auto it = m_Buffers.find(name);
        if (it == m_Buffers.end()) {
            return false;
        }

        auto& uniformBuffer = it->second;

        return uniformBuffer.WriteToBuffer(data, size, 0);
    }

    bool OpenglUniformBuffer::Bind()
    {
        return true;
    }

    Memory::Shared<Context> OpenglUniformBuffer::GetContext()
    {
        return m_Context;
    }

}  //  nkentseu