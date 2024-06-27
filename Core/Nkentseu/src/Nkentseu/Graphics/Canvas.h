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

    struct Scissor {
        bool enabled = false;
        union {
            struct { int32 x; int32 y; int32 width; int32 height; };
            maths::Vector4i extend;
        };

        Scissor() = default;
        Scissor(bool isEnabled, const maths::Vector4i& ext)
            : enabled(isEnabled), extend(ext) {}
        Scissor(bool isEnabled, int32 x, int32 y, int32 width, int32 height)
            : enabled(isEnabled), x(x), y(y), width(width), height(height) {}

        // Constructeur de copie
        Scissor(const Scissor& other)
            : enabled(other.enabled), extend(other.extend) {}

        // Opérateur d'affectation
        Scissor& operator=(const Scissor& other) {
            if (this != &other) {
                enabled = other.enabled;
                extend = other.extend;
            }
            return *this;
        }
    };

    struct Viewport {
        bool enabled = false;
        union {
            struct { float32 x; float32 y; float32 width; float32 height; };
            maths::Vector4f extend;
        };

        Viewport() = default;
        Viewport(bool isEnabled, const maths::Vector4f& ext)
            : enabled(isEnabled), extend(ext) {}
        Viewport(bool isEnabled, float32 x, float32 y, float32 width, float32 height)
            : enabled(isEnabled), x(x), y(y), width(width), height(height) {}

        // Constructeur de copie
        Viewport(const Viewport& other)
            : enabled(other.enabled), extend(other.extend) {}

        // Opérateur d'affectation
        Viewport& operator=(const Viewport& other) {
            if (this != &other) {
                enabled = other.enabled;
                extend = other.extend;
            }
            return *this;
        }
    };

    class CanvasCommand {
    public:
        virtual void Execute() {}
    };

    class CanvasRenderCommand : public CanvasCommand {
    public:
        RenderPrimitive::Enum primitive;
        uint32 indexCount;
        maths::matrix4f transform;
        CanvasTexture texture;

        CanvasRenderCommand(RenderPrimitive::Enum prim, uint32 count, const maths::matrix4f& trans, CanvasTexture tex = nullptr)
            : primitive(prim), indexCount(count), transform(trans), texture(tex) {}

        virtual void Execute() override {
        }
    };

    class CanvasScissorCommand : public CanvasCommand {
    public:
        Scissor scissor;

        CanvasScissorCommand(const Scissor& sc) : scissor(sc) {}

        virtual void Execute() override {
        }
    };

    class CanvasViewportCommand : public CanvasCommand {
    public:
        Viewport viewport;

        CanvasViewportCommand(const Viewport& vp) : viewport(vp) {}

        void Execute() override {
        }
    };

    class NKENTSEU_API Canvas {
    public:
        virtual void Prepare();
        virtual void Present();
        virtual void Destroy() = 0;
        virtual maths::Vector2f GetSize() = 0;

        virtual void SetScissor(const Scissor& scissor);
        virtual void SetViewport(const Viewport& viewport);
        virtual void SetScissor(int32 x, int32 y, int32 width, int32 height);
        virtual void SetViewport(float32 x, float32 y, float32 width, float32 height);

        virtual void DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawLine(const maths::Vector2f& start, const maths::Vector2f& end, const Color& color, CanvasTexture texture = nullptr);

        virtual void DrawRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, bool filled = true, CanvasTexture texture = nullptr);
        virtual void DrawFilledRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawHollowRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawThickOutlineRect(const maths::Vector2f& position, const maths::Vector2f& size, float32 thickness, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawRoundedRect(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawRoundedRect(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& cornerRadii, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawFilledRoundRect(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawFilledRoundRect(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& cornerRadii, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawHollowRoundRect(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& cornerRadii, float32 thickness, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawHollowRoundRect(const maths::Vector2f& position, const maths::Vector2f& size, float32 cornerRadii, float32 thickness, const Color& color, CanvasTexture texture = nullptr);
        void DrawThickOutlineRoundRect(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& cornerRadii, float32 thickness, const Color& color, CanvasTexture texture = nullptr);
        void DrawThickOutlineRoundRect(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, float32 thickness, const Color& color, CanvasTexture texture = nullptr);

        virtual void DrawFilledCircle(const maths::Vector2f& center, float32 radius, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawHollowCircle(const maths::Vector2f& center, float32 radius, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawThickOutlineCircle(const maths::Vector2f& center, float32 radius, float32 thickness, const Color& color, CanvasTexture texture = nullptr);

        virtual void DrawFilledEllipse(const maths::Vector2f& center, float32 radiusX, float32 radiusY, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawHollowEllipse(const maths::Vector2f& center, float32 radiusX, float32 radiusY, const Color& color, CanvasTexture texture = nullptr);
        virtual void DrawThickOutlineEllipse(const maths::Vector2f& center, float32 radiusX, float32 radiusY, float32 thickness, const Color& color, CanvasTexture texture = nullptr);

        virtual void Draw(RenderPrimitive::Enum primitive, const std::vector<Vertex2D>& vertices, const std::vector<uint32>& indices, CanvasTexture texture = nullptr, const maths::matrix4f& transform = maths::matrix4f::Identity());
        virtual void Draw(RenderPrimitive::Enum primitive, const Vertex2D* vertices, usize verticesNumber, const uint32* indices, usize indicesNumber, CanvasTexture texture = nullptr, const maths::matrix4f& transform = maths::matrix4f::Identity());
        virtual void Draw(RenderPrimitive::Enum primitive, const Shape2D& shape, CanvasTexture texture = nullptr, const maths::matrix4f& transform = maths::matrix4f::Identity());

        static Memory::Shared<Canvas> Create(const Memory::Shared<Context>& context);

    protected:
        using CanvasCommandShared = Memory::Shared<CanvasCommand>;

        std::vector<Vertex2D> m_Vertices;
        std::vector<uint32> m_Indices;
        std::vector<CanvasCommandShared> m_CanvasCommands;
        uint32 m_IndexCount = 0;
        bool m_ScissorEnable = false;
        bool m_RenderEnable = false;
        bool m_IsPresent = true;

        virtual void Flush() = 0;
        virtual void AddCommand(CanvasCommandShared command);
    };

}  //  nkentseu

#endif  // __CANVAS_H__!