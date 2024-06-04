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
#include "OpenGLUtils.h"

namespace nkentseu {
    
    class NKENTSEU_API OpenglUniformBuffer : public UniformBuffer {
        public:
            OpenglUniformBuffer(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout);
            ~OpenglUniformBuffer();

            bool Create();
            bool Destroy() override;

            Memory::Shared<Context> GetContext() override;
            bool Bind(const std::string& name, void* data, usize size) override;
            bool Bind(const std::string& name, void* data, usize size, usize offset) override;
        private:
            Memory::Shared<OpenglContext> m_Context;
            UniformBufferLayout m_BufferLayout;
            std::unordered_map<std::string, OpenglBuffer> m_UniformBuffers;
    };

}  //  nkentseu

#endif  // __OPENGL_UNIFORM_BUFFER_H__!