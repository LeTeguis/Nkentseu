//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:12:38 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RENDERER_H__
#define __NKENTSEU_RENDERER_H__

#pragma once

#include "System/System.h"
#include "Context.h"
#include "Color.h"
#include <Ntsm/Vector/Vector2.h>
#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    class InternalRenderer;
    class VertexArray;
    class Shader;

    class NKENTSEU_API Renderer
    {
        public:
            Renderer();
            Renderer(Context* context);
            ~Renderer();

            bool Initialize(Context* context);
            bool Deinitialize();

            bool Clear(const Color& color);
            bool Clear(uint8 r, uint8 g, uint8 b, uint8 a = 255);

            bool UseShader(Memory::Shared<Shader> shader);
            bool UnuseShader();

            bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode);
            bool Draw(Memory::Shared<class VertexArray> vertexArray, DrawVertexType::Code drawVertex);

            bool Prepare();
            bool Finalize();

            bool Present();
            bool Swapbuffer();

            bool Resize(const Vector2u& size);
        private:
            Memory::Shared<class InternalRenderer> m_InternalRenderer = null_pointer;

            bool IsValideInternal();
            bool m_IsInitialsed = false;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_RENDERER_H__