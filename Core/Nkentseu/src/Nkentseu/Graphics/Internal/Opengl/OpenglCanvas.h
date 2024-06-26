//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 06:02:28 PM AM.
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

        void Prepare() override;
        void Present() override;
        void Destroy() override;
        maths::Vector2f GetSize() override;
    private:
        friend class OpenglRenderer;

        Memory::Shared<OpenglContext> m_Context = nullptr;
        Memory::Shared<OpenglShader> m_Shader = nullptr;

        Memory::Shared<OpenglUniformBuffer> m_UniformBuffer = nullptr;

        Memory::Shared<OpenglVertexBuffer> m_VertexBuffer = nullptr;
        Memory::Shared<OpenglIndexBuffer> m_IndexBuffer = nullptr;
        Memory::Shared<OpenglVertexArray> m_VertexArray = nullptr;

        bool m_IsPresent = true;
        maths::Vector2f m_Size;

        void Initialize();
        void CreateResources();
        void Flush();
    };

}  //  nkentseu

#endif  // __CANVAS_H__!