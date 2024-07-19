//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 11:50:30 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __RENDER_DATA2_D_H__
#define __RENDER_DATA2_D_H__

#pragma once

#include <System/System.h>
#include "Geometry2D.h"
#include "Nkentseu/Graphics/Color.h"
#include "Nkentseu/Graphics/Texture.h"

namespace nkentseu {

    ENUMERATION(RenderMode2D, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_CONTENT(Enum::RenderColor)
        ENUM_TO_STRING_CONTENT(Enum::RenderTexture)
        ENUM_TO_STRING_END(Enum::RenderColor),
        ,
        RenderColor = 1 << 0,  // 0x01
        RenderTexture = 1 << 1,  // 0x02
        RenderBoth = RenderColor | RenderTexture,  // 0x03
    );

    ENUMERATION(BlendMode2D, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_CONTENT(Enum::AlphaBlending)
        ENUM_TO_STRING_CONTENT(Enum::AdditiveBlending)
        ENUM_TO_STRING_CONTENT(Enum::MultiplicativeBlending)
        ENUM_TO_STRING_CONTENT(Enum::SubtractiveBlending)
        ENUM_TO_STRING_CONTENT(Enum::MinBlending)
        ENUM_TO_STRING_CONTENT(Enum::MaxBlending)
        ENUM_TO_STRING_CONTENT(Enum::Zero)
        ENUM_TO_STRING_CONTENT(Enum::One)
        ENUM_TO_STRING_CONTENT(Enum::SrcColor)
        ENUM_TO_STRING_CONTENT(Enum::OneMinusSrcColor)
        ENUM_TO_STRING_CONTENT(Enum::DstColor)
        ENUM_TO_STRING_CONTENT(Enum::OneMinusDstColor)
        ENUM_TO_STRING_CONTENT(Enum::DstAlpha)
        ENUM_TO_STRING_CONTENT(Enum::OneMinusDstAlpha)
        ENUM_TO_STRING_CONTENT(Enum::SrcAlphaSaturate)
        ENUM_TO_STRING_CONTENT(Enum::ConstantColor)
        ENUM_TO_STRING_CONTENT(Enum::OneMinusConstantColor)
        ENUM_TO_STRING_CONTENT(Enum::ConstantAlpha)
        ENUM_TO_STRING_CONTENT(Enum::OneMinusConstantAlpha)
        ENUM_TO_STRING_END(Enum::None),
        ,
        None,
        AlphaBlending,
        AdditiveBlending,
        MultiplicativeBlending,
        SubtractiveBlending,
        MinBlending,
        MaxBlending,
        Zero,
        One,
        SrcColor,
        OneMinusSrcColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstColor,
        OneMinusDstColor,
        DstAlpha,
        OneMinusDstAlpha,
        SrcAlphaSaturate,
        ConstantColor,
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha
    );
    
    struct NKENTSEU_API RenderData2D {
        virtual Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const = 0;
        virtual ~RenderData2D() {}
    };

    enum class BorderType {
        None,
        Outer,
        Inner,
        Both
    };
    /*
    struct NKENTSEU_API RectangleData : public RenderData2D {
        int32 x, y, width, height;
        maths::Vector2f topLeftRadius, topRightRadius, bottomLeftRadius, bottomRightRadius;
        uint32 cornerSegmentCount;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API CircleData : public RenderData2D {
        int32 x, y, radius;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API EllipseData : public RenderData2D {
        int32 x, y, radiusX, radiusY;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API PolygonData : public RenderData2D {
        std::vector<maths::Vector2f> points;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;
        bool isClosed; // true if the polygon is closed, false if it's open

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API LineData : public RenderData2D {
        int32 x1, y1, x2, y2, lineWidth;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        maths::Vector4f lineColor; // RGBA

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API ArcData : public RenderData2D {
        int32 x, y, radius, startAngle, endAngle;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API CurveData : public RenderData2D {
        std::vector<maths::Vector2f> points;
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API BezierData : public RenderData2D {
        std::vector<maths::Vector2f> controlPoints;
        uint32 degree; // degree of the Bezier curve (e.g. 2 for quadratic, 3 for cubic)
        std::vector<maths::Vector2f> textureCoords; // coordonnees de texture pour chaque vertex
        bool autoTexCoords;
        float32 borderWidth;
        maths::Vector4f borderColor; // RGBA
        BorderType borderType;

        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };

    struct NKENTSEU_API TextData : public RenderData2D {
        const char* text;
        int32 x, y;
        Geometry2D GetGeometry(uint32 currentVertexCount, uint32 currentIndexCount) const override;
    };*/

}  //  nkentseu

#endif  // __RENDER_DATA2_D_H__!