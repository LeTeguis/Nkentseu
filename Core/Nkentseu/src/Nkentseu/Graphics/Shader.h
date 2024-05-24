//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:53:53 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __SHADER_H__
#define __SHADER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include <unordered_map>

#include "ShaderInfo.h"

namespace nkentseu {
    class InternalShader;
    class Context;
    
    class NKENTSEU_API Shader {
        public:
            Shader(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout = {});
            ~Shader();

            bool Create();
            bool Destroy();
            void SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout = {});

            InternalShader* GetInternal();

            std::string ToString() const;
            friend std::string ToString(const Shader& shader);
        private:
            Memory::Shared<InternalShader> m_InternalShader = null_pointer;
    };

}  //  nkentseu

#endif  // __SHADER_H__!