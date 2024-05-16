//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:08 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INTERNAL_SHADER_H__
#define __INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <System/Definitions/Memory.h>

#include <Nkentseu/Graphics/ShaderInfo.h>

namespace nkentseu {
    
    class NKENTSEU_API InternalShader {
        public:
            InternalShader(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
            ~InternalShader();

            void SetShaderFiles(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
            bool CreateShader();
            bool CompileShader();
            bool Destroy();

            std::string ToString() const;
            friend std::string ToString(const InternalShader& internalShader);

            bool Bind() const;
            bool Unbind() const;
        private:
            uint32 m_Programme = 0;

            std::unordered_map<ShaderType::Code, std::string> m_ShaderFiles;
            std::vector<uint32> m_Modules;

        private:
            uint32 MakeModule(const std::string& filepath, ShaderType::Code code);
            uint32 MakeShader();
            uint32 GetModuleType(ShaderType::Code code);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!

#endif