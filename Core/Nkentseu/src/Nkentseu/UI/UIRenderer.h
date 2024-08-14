//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 06:16:57 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __U_I_RENDERER_H__
#define __U_I_RENDERER_H__

#pragma once

#include <System/System.h>
#include "Nkentseu/Graphics/G2d/RenderCache2D.h"
#include "UIGraphics.h"

namespace nkentseu {
    namespace ui {
        class NKENTSEU_API UIRenderer {
        public:
            UIRenderer(Memory::Shared<Context> context);
            ~UIRenderer();

            void Initialize();
            void Prepare(Memory::Shared<Renderer> renderer);
            void Present(Memory::Shared<Renderer> renderer);

            void AddGraphics(const UIGraphics& graphics);
            void AddGraphicsCollection(const UIGraphicsCollection& graphicsCollection);

        private:
            RenderCache2D renderCache;
            Memory::Shared<Context> context;
            Memory::Shared<Renderer> renderer;
        };
    }
}  //  nkentseu

#endif  // __U_I_RENDERER_H__!