//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:12:38 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RENDERER_H__
#define __NKENTSEU_RENDERER_H__

#pragma once

#include "System/System.h"
#include "System/Definitions/Memory.h"
#include "Context.h"
#include "Shader.h"
#include "Color.h"
#include "Canvas.h"

#include <Ntsm/Vector/Vector2.h>
#include "Nkentseu/Graphics/ShaderInfo.h"
#include "UniformBuffer.h"

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

            virtual Memory::Shared<Canvas> GetCanvas() = 0;

            static Memory::Shared<Renderer> Create(Memory::Shared<Context> context);
            static Memory::Shared<Renderer> CreateInitialized(Memory::Shared<Context> context);
    };
} // namespace nkentseu

#endif    // __NKENTSEU_RENDERER_H__