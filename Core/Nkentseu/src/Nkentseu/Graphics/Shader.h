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
#include "Context.h"

namespace nkentseu {
    class NKENTSEU_API Shader {
        public:
            virtual Memory::Shared<Context> GetContext() = 0;
            virtual bool Destroy() = 0;
            virtual bool Bind() = 0;
            virtual bool Unbind() = 0;

            virtual bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) = 0;

            virtual bool PolygonMode(PolygonModeType::Code mode) = 0;
            virtual bool CullMode(CullModeType::Code mode) = 0;
            virtual bool FrontFaceMode(FrontFaceType::Code mode) = 0;
            virtual bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode) = 0;
            virtual bool ScissorMode(const Vector2i& offset, const Vector2u& extend) = 0;
            virtual bool ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth) = 0;

            static Memory::Shared<Shader> Create(Memory::Shared<Context> context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout);
    };

}  //  nkentseu

#endif  // __SHADER_H__!