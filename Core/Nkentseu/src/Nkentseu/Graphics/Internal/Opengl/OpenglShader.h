//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:08 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_SHADER_H__
#define __OPENGL_INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>
#include <unordered_map>

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <Nkentseu/Graphics/Shader.h>
#include <Nkentseu/Graphics/Context.h>
#include "OpenglContext.h"
#include "OpenGLUtils.h"

namespace nkentseu {
    class NKENTSEU_API OpenglShader : public Shader {
        public:
            OpenglShader(Memory::Shared<Context> context);
            ~OpenglShader();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;
            bool LoadFromFile(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout);

            bool Compile();

            bool Bind() override;
            bool Unbind() override;

            bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) override;

            bool PolygonMode(PolygonModeType::Code mode) override;
            bool CullMode(CullModeType::Code mode) override;
            bool FrontFaceMode(FrontFaceType::Code mode) override;
            bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode) override;
            bool ScissorMode(const Vector2i& offset, const Vector2u& extend) override;
            bool ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth) override;
        private:
            uint32 m_Programme = 0;
            Memory::Shared<OpenglContext> m_Context = nullptr;
            ShaderBufferLayout m_Layout;

            std::vector<uint32> m_Modules;
            //std::unordered_map<std::string, OpenglBuffer> m_UniformBuffers;

        private:
            uint32 MakeModule(const std::string& filepath, ShaderType::Code code);
            uint32 MakeShader();

            std::string LoadShader(const std::string& shaderFile);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!