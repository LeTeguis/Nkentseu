//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-03 at 08:33:39 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_UNIFORM_BUFFER_H__
#define __OPENGL_UNIFORM_BUFFER_H__

#pragma once

#include <System/System.h>

#include <System/System.h>
#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/UniformBuffer.h"
#include "OpenglContext.h"
#include "OpenglShader.h"
#include "OpenGLUtils.h"

namespace nkentseu {
    
    class NKENTSEU_API OpenglUniformBuffer : public UniformBuffer {
        public:
            OpenglUniformBuffer(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout);
            ~OpenglUniformBuffer();

            bool Create();
            bool Destroy() override;

            bool SetData(const std::string& name, void* data, usize size) override;
            bool SetData(const std::string& name, void* data, usize size, uint32 index) override;
            bool Bind() override;

            Memory::Shared<Context> GetContext() override;
        private:
            Memory::Shared<OpenglContext> m_Context;
            Memory::Shared<OpenglShader> m_Shader;
            UniformBufferLayout m_BufferLayout;

            std::unordered_map<std::string, OpenglBuffer> m_Buffers;
    };

}  //  nkentseu

#endif  // __OPENGL_UNIFORM_BUFFER_H__!