//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:08 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_SHADER_H__
#define __OPENGL_INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <System/Definitions/Memory.h>

#include <Nkentseu/Graphics/ShaderInfo.h>

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalShader {
        public:
            InternalShader(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
            ~InternalShader();

            void SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
            bool Create();
            bool Compile();
            bool Destroy();

            std::string ToString() const;
            friend std::string ToString(const InternalShader& internalShader);

            bool Bind() const;
            bool Unbind() const;
        private:
            uint32 m_Programme = 0;
            Context* m_Context = nullptr;

            std::unordered_map<ShaderType::Code, std::string> m_ShaderFiles;
            std::vector<uint32> m_Modules;

        private:
            uint32 MakeModule(const std::string& filepath, ShaderType::Code code);
            uint32 MakeShader();

            std::string LoadShader(const std::string& shaderFile);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!

#endif