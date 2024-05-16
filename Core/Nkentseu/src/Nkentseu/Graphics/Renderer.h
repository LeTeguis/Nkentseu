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
#include "Shader.h"

namespace nkentseu {
    class InternalRenderer;

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

            bool SetActiveShader(Memory::Shared<Shader> shader);
            bool UnsetActiveShader();

            bool Present();
            bool Swapbuffer();

            bool Resize(const Vector2u& size);
        private:
            Memory::Shared<InternalRenderer> m_InternalRenderer = null_pointer;

            bool IsValideInternal();
            bool m_IsInitialsed = false;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_RENDERER_H__