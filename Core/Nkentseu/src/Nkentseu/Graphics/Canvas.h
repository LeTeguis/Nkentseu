//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 05:50:37 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __SYSTEM_CANVAS_H__
#define __SYSTEM_CANVAS_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include <Ntsm/Vector/Vector2.h>
#include <Ntsm/Vector/Vector3.h>
#include <Ntsm/Vector/Vector4.h>
#include <Ntsm/Matrix/Matrix4.h>

#include "Context.h"
#include "Texture.h"
#include "Color.h"
#include "RenderPrimitive.h"

#include <vector>

namespace nkentseu {

    #define MAX_QUADS 50000

    #define MAX_VERTICES MAX_QUADS * 4
    #define MAX_INDICES MAX_QUADS * 6

    struct NKENTSEU_API VertexData {
        maths::Vector2f position;
        maths::Vector4f color;
        maths::Vector2f uv;

        VertexData() = default;

        VertexData(const maths::Vector2f& pos, const maths::Vector4f& col, const maths::Vector2f& uv)
            : position(pos), color(col), uv(uv) {}
    };

    struct CanvasTransform {
        maths::matrix4f model = maths::matrix4f::Identity();
    };

    struct CanvasCamera {
        maths::matrix4f view = maths::matrix4f::Identity();
        maths::matrix4f proj = maths::matrix4f::Identity();
    };

    struct CanvasMaterial {
        int32 useTexture = false;
        int32 useColor = true;
    };

    using CanvasTexture = const Memory::Shared<Texture2D>;

    struct NKENTSEU_API RenderCommand {
        RenderPrimitive::Enum primitive;
        uint32 indexCount;

        maths::Vector2f scissor;
        maths::Vector4f viewport;

        CanvasTexture texture;
    };

    class NKENTSEU_API Canvas {
    public:
        virtual void Clear(const Color& color) = 0;
        virtual void Prepare() = 0;
        virtual void Present() = 0;
        virtual void Destroy() = 0;
        virtual maths::Vector2f GetSize() = 0;

        virtual void DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture& texture = nullptr) = 0;
        virtual void DrawLine(const maths::Vector2f& start, const maths::Vector2f& end, const Color& color, CanvasTexture& texture = nullptr) = 0;
        virtual void DrawRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, bool filled = true, CanvasTexture& texture = nullptr) = 0;

        static Memory::Shared<Canvas> Create(const Memory::Shared<Context>& context);

    protected:
    };

}  //  nkentseu

#endif  // __CANVAS_H__!