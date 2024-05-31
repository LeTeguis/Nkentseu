//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:13:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_NKENTSEU_INTERNALRENDERER_H__
#define __OPENGL_NKENTSEU_INTERNALRENDERER_H__

#pragma once

#include "System/System.h"

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/Color.h>
#include <Ntsm/Vector/Vector2.h>

#include <vector>

#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Graphics/Renderer.h"
#include "Nkentseu/Core/Events.h"

namespace nkentseu {
    class Context;
    class OpenglContext;
    class OpenglShader;
    class Shader;
    class VertexArray;

    class NKENTSEU_API OpenglRenderer : public Renderer
    {
        public:
            OpenglRenderer(Memory::Shared<Context>);
            ~OpenglRenderer();

            Memory::Shared<Context> GetContext() override;

            bool Initialize() override;
            bool Deinitialize() override;

            bool Begin(const Color& color) override;
            bool Begin(uint8 r, uint8 g, uint8 b, uint8 a = 255) override;
            bool End() override;

            bool BindShader(Memory::Shared<Shader> shader) override;
            bool UnbindShader() override;
            bool BindUniform(const std::string& name, void* data, usize size) override;
            bool UnbindUniform(const std::string& name) override;

            bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) override;

            bool PolygonMode(PolygonModeType::Code mode) override;
            bool CullMode(CullModeType::Code mode) override;
            bool FrontFaceMode(FrontFaceType::Code mode) override;
            bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode) override;
            bool ScissorMode(const Vector2i& offset, const Vector2u& extend) override;
            bool ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth) override;

            bool Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) override;
        private:
            Memory::Shared<OpenglContext> m_Context = nullptr;
            Memory::Shared<OpenglShader> m_CurrentShader = nullptr;
            bool m_IsPrepare = false;

        private:
            void OnEvent(Event& event);
            bool OnWindowResizedEvent(class WindowResizedEvent& event);
            bool CanRender();
    };
} // namespace nkentseu

#endif    // __NKENTSEU_INTERNALRENDERER_H__