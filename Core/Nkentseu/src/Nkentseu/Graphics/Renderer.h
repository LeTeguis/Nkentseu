//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:12:38 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RENDERER_H__
#define __NKENTSEU_RENDERER_H__

#pragma once

#include "System/System.h"
#include "Context.h"
#include "Shader.h"
#include "Color.h"
#include <Ntsm/Vector/Vector2.h>
#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    class VertexArray;

    class NKENTSEU_API Renderer
    {
        public:
            virtual Memory::Shared<Context> GetContext() = 0;

            virtual bool Initialize() = 0;
            virtual bool Deinitialize() = 0;

            virtual bool Begin(const Color& color) = 0;
            virtual bool Begin(uint8 r, uint8 g, uint8 b, uint8 a = 255) = 0;
            virtual bool End() = 0;

            virtual bool BindShader(Memory::Shared<Shader> shader) = 0;
            virtual bool UnbindShader() = 0;
            virtual bool BindUniform(const std::string& name, void* data, usize size) = 0;
            virtual bool UnbindUniform(const std::string& name) = 0;

            virtual bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) = 0;

            virtual bool PolygonMode(PolygonModeType::Code mode) = 0;
            virtual bool CullMode(CullModeType::Code mode) = 0;
            virtual bool FrontFaceMode(FrontFaceType::Code mode) = 0;
            virtual bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode) = 0;
            virtual bool ScissorMode(const Vector2i& offset, const Vector2u& extend) = 0;
            virtual bool ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth) = 0;

            virtual bool Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) = 0;

            static Memory::Shared<Renderer> Create(Memory::Shared<Context> context);
            static Memory::Shared<Renderer> CreateInitialized(Memory::Shared<Context> context);
    };
} // namespace nkentseu

#endif    // __NKENTSEU_RENDERER_H__