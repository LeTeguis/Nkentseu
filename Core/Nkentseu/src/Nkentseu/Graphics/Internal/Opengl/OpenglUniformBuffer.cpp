//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-03 at 08:33:39 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglUniformBuffer.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    OpenglUniformBuffer::OpenglUniformBuffer(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout) : m_Context(Memory::SharedCast<OpenglContext>(context)), m_BufferLayout(uniformLayout)
    {
    }

    // Destructor
    OpenglUniformBuffer::~OpenglUniformBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool OpenglUniformBuffer::Create()
    {
        bool success = true;
        for (auto& [name, ubo] : m_BufferLayout.attributes) {
            m_UniformBuffers[name] = {};
            if (!m_UniformBuffers[name].Create(name, ubo.size, ubo.usage, ubo.binding, 0)) {
                success = false;
            }
            //m_UniformBuffers[ubo.name].Create(shader, ubo.name, ubo.size, ubo.usage, ubo.binding, ubo.offset);
        }
        return success;
    }

    bool OpenglUniformBuffer::Destroy()
    {
        bool success = true;
        for (auto& [name, ubo] : m_BufferLayout.attributes) {
            m_UniformBuffers[name] = {};
            if (!m_UniformBuffers[name].Destroy()) {
                success = false;
            }
        }
        m_UniformBuffers.clear();
        return success;
    }

    Memory::Shared<Context> OpenglUniformBuffer::GetContext()
    {
        return m_Context;
    }

    bool OpenglUniformBuffer::Bind(const std::string& name, void* data, usize size)
    {
        if (m_Context == nullptr || data == nullptr || size == 0) return false;

        // Rechercher le tampon uniforme correspondant au nom donné
        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            // Le tampon uniforme avec ce nom n'a pas été trouvé
            return false;
        }

        OpenglBuffer& uniformBuffer = it->second;

        if (uniformBuffer.uniform != 0) {
            uniformBuffer.WriteToBuffer(data, size, 0);
        }
    }

    bool OpenglUniformBuffer::Bind(const std::string& name, void* data, usize size, uint32 index)
    {
        if (m_Context == nullptr || data == nullptr || size == 0) return false;

        // Rechercher le tampon uniforme correspondant au nom donné
        auto it = m_UniformBuffers.find(name);
        if (it == m_UniformBuffers.end()) {
            // Le tampon uniforme avec ce nom n'a pas été trouvé
            return false;
        }

        OpenglBuffer& uniformBuffer = it->second;

        if (uniformBuffer.uniform != 0) {
            uniformBuffer.WriteToBuffer(data, size, 0);
        }
    }

    bool OpenglUniformBuffer::Send(uint32 index)
    {
        if (m_Context == nullptr) return false;
        return true;
    }

}  //  nkentseu