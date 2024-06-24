//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-03 at 08:33:39 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglUniformBuffer.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    OpenglUniformBuffer::OpenglUniformBuffer(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout) : m_Context(Memory::SharedCast<OpenglContext>(context)), m_BufferLayout(uniformLayout), m_Shader(Memory::SharedCast<OpenglShader>(shader))
    {
    }

    // Destructor
    OpenglUniformBuffer::~OpenglUniformBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool OpenglUniformBuffer::Create()
    {
        if (m_Buffers.size() != 0 || m_Context == nullptr || m_Shader == nullptr) {
            return false;
        }
        bool success = true;
        m_Shader->Bind();
        for (auto& [name, ubo] : m_BufferLayout.attributes) {
            uint32 instance = 1;

            if (ubo.uType == UniformBufferType::Dynamic) {
                instance = ubo.instance;
            }

            m_Buffers[name].Create(name, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, nullptr, ubo.size, ubo.binding, 0, instance, !true);
            m_Buffers[name].uType = ubo.uType;

            OpenGLResult result;
            bool first = true;

            glCheckError(first, result, GLuint blockIndex = glGetUniformBlockIndex(m_Shader->GetProgramme(), name.c_str()), "");
            if (blockIndex == GL_INVALID_INDEX) {
                Log_nts.Error("Error: Uniform block index is invalid : {0}-{1}", name, m_Shader->GetProgramme());
            }
            else {
                Log_nts.Trace("block index is valid name : {0}-{1}-{2}", name, blockIndex, m_Shader->GetProgramme());
                glCheckError(first, result, glUniformBlockBinding(m_Shader->GetProgramme(), blockIndex, ubo.binding), "");
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
        if (m_Context == nullptr || data == nullptr || size == 0) return false;

        auto it = m_Buffers.find(name);
        if (it == m_Buffers.end()) {
            return false;
        }

        auto& uniformBuffer = it->second;

        return uniformBuffer.WriteToBuffer(data, size, 0);
        //return uniformBuffer.WriteToBuffer(data, size, 0);
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