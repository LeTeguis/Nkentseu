//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:13:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_NKENTSEU_INTERNALRENDERER_H__
#define __OPENGL_NKENTSEU_INTERNALRENDERER_H__

#pragma once

#include "System/System.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/Color.h>
#include <Ntsm/Vector/Vector2.h>

#include <vector>

#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Core/Events.h"

namespace nkentseu {
    class Context;
    class Shader;
    class VertexArray;

    class NKENTSEU_API InternalRenderer
    {
        public:
            InternalRenderer();
            ~InternalRenderer();

            bool Initialize(class Context* context);
            bool Deinitialize();

            bool Clear(const Color& color);
            bool Clear(uint8 r, uint8 g, uint8 b, uint8 a = 255);

            bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode);
            bool PolygonMode(PolygonModeType::Code mode);
            bool CullMode(CullModeType::Code mode);
            bool FrontFaceMode(FrontFaceType::Code mode);
            bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode);
            bool ScissorMode(const Vector2i& offset, const Vector2u& extend);
            bool ViewMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth);

            bool Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex);

            bool Present();
            bool Swapbuffer();

            bool Prepare();
            bool Finalize();

            bool UseShader(Memory::Shared<Shader> shader);
            bool UnuseShader();

            bool Resize(const Vector2u& size);
        private:
            Context* m_Context = nullptr;
            Memory::Shared<Shader> m_CurrentShader = nullptr;
            bool m_IsPrepare = false;

        private:
            void OnEvent(Event& event);
            bool OnWindowResizedEvent(class WindowResizedEvent& event);
            bool CanRender();
    };
} // namespace nkentseu

#endif    // __NKENTSEU_INTERNALRENDERER_H__

#endif