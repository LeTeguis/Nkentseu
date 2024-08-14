//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 06:01:10 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __U_I_GRAPHICS_H__
#define __U_I_GRAPHICS_H__

#pragma once

#include <System/System.h>
#include "Nkentseu/Graphics/G2d/Vertex2D.h"
#include "Nkentseu/Graphics/Texture.h"
#include "Nkentseu/Graphics/G2d/RenderCommand.h"

#include <vector>

namespace nkentseu {
    // UIGraphics.h
    namespace ui {

        using ContainePoint = std::function<bool(const maths::Vector2f&)>;

        class NKENTSEU_API UIGraphics {
        public:
            virtual void AddVertex(const Vertex2D& vertex);
            virtual void AddIndex(unsigned int index);

            virtual bool Containe(const maths::Vector2f& point);
            virtual bool ContaineInBound(const maths::Vector2f& point);

            std::vector<Vertex2D> vertices; // Vertex de la géométrie 2D
            std::vector<uint32> indices; // Indices de la géométrie 2D
            Memory::Shared<Texture2D> texture;
            ClipRegion clipRegion;
            RenderPrimitive primitive;

            ContainePoint collision = nullptr;

            maths::Vector4f GetGlobalBounds();
            maths::Vector2f GetMinBounds();
            maths::Vector2f GetMaxBounds();

        private:
            maths::Vector2f minBounds, maxBounds;
        };

        class NKENTSEU_API UIGraphicsCollection {
        public:
            virtual bool Containe(const maths::Vector2f& point);
            virtual bool ContaineInBound(const maths::Vector2f& point);

            virtual void AddGraphics(UIGraphics graphics);
            //virtual void RemoveGraphics(UIGraphics graphics);
            virtual void RemoveGraphics(uint32 index);
            virtual uint32 Size();
            virtual void ClearGraphics();
            virtual UIGraphics GetGraphics(int32 index);
            virtual std::vector<UIGraphics> GetGraphics();

            std::vector<UIGraphics> graphics;
            ContainePoint collision = nullptr;

            maths::Vector4f GetGlobalBounds();
            maths::Vector2f GetMinBounds();
            maths::Vector2f GetMaxBounds();

        private:
            maths::Vector2f minBounds, maxBounds;
        };
    }

}  //  nkentseu

#endif  // __U_I_GRAPHICS_H__!