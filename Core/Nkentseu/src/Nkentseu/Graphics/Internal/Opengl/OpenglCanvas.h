//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-21 at 06:02:28 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_CANVAS_H__
#define __OPENGL_CANVAS_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/Canvas.h"
#include "Nkentseu/Graphics/Context.h"

#include "OpenglContext.h"
#include "OpenglRenderer.h"
#include "OpenglShader.h"
#include "OpenglIndexBuffer.h"
#include "OpenglVertexBuffer.h"
#include "OpenglVertexArray.h"
#include "OpenglUniformBuffer.h"

namespace nkentseu {

    class OpenglCanvas : public Canvas {
    public:
        OpenglCanvas(const Memory::Shared<Context>& context);
        ~OpenglCanvas();

        void Clear(const Color& color) override;
        void Prepare() override;
        void Present() override;
        void Destroy() override;
        maths::Vector2f GetSize() override;

        void DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture texture = nullptr) override;
        void DrawLine(const maths::Vector2f& start, const maths::Vector2f& end, const Color& color, CanvasTexture texture = nullptr) override;
        void DrawRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, bool filled = true, CanvasTexture texture = nullptr) override;

    private:
        friend class OpenglRenderer;

        Memory::Shared<OpenglContext> m_Context = nullptr;
        Memory::Shared<OpenglShader> m_Shader = nullptr;

        Memory::Shared<OpenglUniformBuffer> m_UniformBuffer = nullptr;

        Memory::Shared<OpenglVertexBuffer> m_VertexBuffer = nullptr;
        Memory::Shared<OpenglIndexBuffer> m_IndexBuffer = nullptr;
        Memory::Shared<OpenglVertexArray> m_VertexArray = nullptr;

        std::vector<VertexData> m_Vertices;
        std::vector<uint32> m_Indices;
        std::vector<RenderCommand> m_Commands;

        uint32 m_IndexCount = 0;

        bool m_IsPresent = true;

        maths::Vector2f m_Size;

        void Initialize();
        void CreateResources();
        void Flush();
    };

}  //  nkentseu

#endif  // __CANVAS_H__!