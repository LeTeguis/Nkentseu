//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:52 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_SHADER_H__
#define __VULKAN_INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <System/Definitions/Memory.h>
#include <unordered_map>
#include "Nkentseu/Graphics/ShaderInfo.h"
#include <string>

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalShader {
        public:
            InternalShader(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
            ~InternalShader();

            bool Create();
            bool CreateShader();
            bool Destroy();
            void SetShaderFiles(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
        private:
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!

#endif