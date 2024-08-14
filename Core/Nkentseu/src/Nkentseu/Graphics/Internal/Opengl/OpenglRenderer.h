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
    class OpenglCanvas;
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

            bool SetViewport(const maths::Vector4f& viewport) override;
            bool SetViewport(float32 x, float32 y, float32 width, float32 height) override;
            bool ResetViewport() override;

            bool SetScissor(const maths::Vector4f& scissor) override;
            bool SetScissor(float32 x, float32 y, float32 width, float32 height) override;
            bool ResetScissor() override;
        private:
            Memory::Shared<OpenglContext> m_Context = nullptr;
            bool m_IsPrepare = false;

        private:
            void OnEvent(Event& event);
            bool OnWindowResizedEvent(class WindowResizedEvent& event);
            bool CanRender();
    };
} // namespace nkentseu

#endif    // __NKENTSEU_INTERNALRENDERER_H__