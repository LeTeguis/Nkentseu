//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 06:16:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "UIRenderer.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    namespace ui {
        UIRenderer::UIRenderer(Memory::Shared<Context> context) : context(context), renderCache(context) {}

        UIRenderer::~UIRenderer() {}

        void UIRenderer::Initialize() {
            renderCache.Initialize();
        }

        void UIRenderer::Prepare(Memory::Shared<Renderer> renderer) {
            this->renderer = renderer;
            renderCache.Prepare(renderer);
        }

        void UIRenderer::Present(Memory::Shared<Renderer> renderer) {
            renderCache.Present(renderer);
        }

        void UIRenderer::AddGraphics(const UIGraphics& graphics) {
            renderCache.BeginCommand(graphics.primitive);

            for (const auto& vertex : graphics.vertices) {
                renderCache.AddVertex(vertex);
            }

            for (const auto& index : graphics.indices) {
                renderCache.AddIndex(index);
            }

            renderCache.SetCommandTextureId(graphics.texture);
            renderCache.SetCommandClipRegion(graphics.clipRegion);

            renderCache.EndCommand(true);
        }

        void UIRenderer::AddGraphicsCollection(const UIGraphicsCollection& graphicsCollection) {
            for (const auto& graphics : graphicsCollection.graphics) {
                AddGraphics(graphics);
            }
        }
    }
}  //  nkentseu  //  nkentseu