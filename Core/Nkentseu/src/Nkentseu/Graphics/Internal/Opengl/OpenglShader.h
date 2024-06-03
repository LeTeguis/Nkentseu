//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:56:08 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_SHADER_H__
#define __OPENGL_INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <Nkentseu/Graphics/Shader.h>
#include <Nkentseu/Graphics/Context.h>
#include "OpenglContext.h"

namespace nkentseu {
    class NKENTSEU_API OpenglShader : public Shader {
        public:
            OpenglShader(Memory::Shared<Context> context);
            ~OpenglShader();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;
            bool LoadFromFile(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout);

            bool Compile();

            bool Bind() const;
            bool Unbind() const;
        private:
            uint32 m_Programme = 0;
            Memory::Shared<OpenglContext> m_Context = nullptr;
            ShaderBufferLayout m_Layout;

            std::vector<uint32> m_Modules;

        private:
            uint32 MakeModule(const std::string& filepath, ShaderType::Code code);
            uint32 MakeShader();

            std::string LoadShader(const std::string& shaderFile);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!