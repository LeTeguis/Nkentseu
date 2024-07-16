//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-25 at 06:35:53 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __SHAPE2_D_H__
#define __SHAPE2_D_H__

#pragma once

#include <System/System.h>
#include "Color.h"
#include <Ntsm/Vector/Vector4.h>
#include "G2d/Vertex2D.h"

namespace nkentseu {
    
    class NKENTSEU_API Shape2D {
        public:
            virtual void GenerateVertices() = 0;
            virtual void GenerateIndices() = 0;

            const std::vector<Vertex2D>& GetVertices() const { return m_Vertices; }
            const std::vector<uint32>& GetIndices() const { return m_Indices; }

        protected:
            std::vector<Vertex2D> m_Vertices;
            std::vector<uint32> m_Indices;
    };

    class NKENTSEU_API CircleShape : public Shape2D {
    public:
        CircleShape(float32 radiusX, float32 radiusY, int32 segments, const Color& color);

        void GenerateVertices() override;
        void GenerateIndices() override;

    private:
        float32 m_RadiusX; // Radius along X-axis
        float32 m_RadiusY; // Radius along Y-axis
        int32 m_Segments;
        Color m_Color;
    };

    class NKENTSEU_API RectangleShape : public Shape2D {
    public:
        RectangleShape(float32 width, float32 height, float32 cornerRadius, int32 cornerSegments, const Color& color);

        void GenerateVertices() override;
        void GenerateIndices() override;

    private:
        float32 m_Width;
        float32 m_Height;
        float32 m_CornerRadius;
        int32 m_CornerSegments;
        Color m_Color;
    };

}  //  nkentseu

#endif  // __SHAPE2_D_H__!