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
#include "Vertex2D.h"
#include "Shape2D.h"

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

    using CanvasTexture = Memory::Shared<Texture2D>;

    struct NKENTSEU_API RenderCommand {
        RenderPrimitive::Enum primitive;
        uint32 indexCount;
        maths::matrix4f transform = maths::matrix4f::Identity();
        CanvasTexture texture;

        RenderCommand(RenderPrimitive::Enum prim, uint32 count, const maths::matrix4f& trans, CanvasTexture tex = nullptr)
            : primitive(prim), indexCount(count), transform(trans), texture(tex) {}

        RenderCommand() = default;

        RenderCommand(const RenderCommand& other)
            : primitive(other.primitive), indexCount(other.indexCount), transform(other.transform), texture(other.texture) {}

        RenderCommand& operator=(const RenderCommand& other) {
            if (this != &other) {
                primitive = other.primitive;
                indexCount = other.indexCount;
                transform = other.transform;
                texture = other.texture;
            }
            return *this;
        }
    };

    class NKENTSEU_API Canvas {
    public:
        virtual void Prepare() = 0;
        virtual void Present() = 0;
        virtual void Destroy() = 0;
        virtual maths::Vector2f GetSize() = 0;

        virtual void DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawLine(const maths::Vector2f& start, const maths::Vector2f& end, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, bool filled = true, CanvasTexture texture = nullptr);

        virtual void Draw(RenderPrimitive::Enum primitive, const std::vector<Vertex2D>& vertices, const std::vector<uint32>& indices, CanvasTexture texture = nullptr, const maths::matrix4f& transform = maths::matrix4f::Identity());
        virtual void Draw(RenderPrimitive::Enum primitive, const Vertex2D* vertices, usize verticesNumber, const uint32* indices, usize indicesNumber, CanvasTexture texture = nullptr, const maths::matrix4f& transform = maths::matrix4f::Identity());
        virtual void Draw(RenderPrimitive::Enum primitive, const Shape2D& shape, CanvasTexture texture = nullptr, const maths::matrix4f& transform = maths::matrix4f::Identity());

        static Memory::Shared<Canvas> Create(const Memory::Shared<Context>& context);

    protected:
        std::vector<Vertex2D> m_Vertices;
        std::vector<uint32> m_Indices;
        std::vector<RenderCommand> m_Commands;
        uint32 m_IndexCount = 0;
    };

}  //  nkentseu

#endif  // __CANVAS_H__!