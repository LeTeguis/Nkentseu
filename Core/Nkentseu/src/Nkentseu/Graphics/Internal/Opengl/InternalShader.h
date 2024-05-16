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

            std::string ToString() const;
            friend std::string ToString(const InternalShader& internalShader);

            bool Bind() const;
            bool Unbind() const;
        private:
            uint32 m_Programme;
        protected:
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!

#endif