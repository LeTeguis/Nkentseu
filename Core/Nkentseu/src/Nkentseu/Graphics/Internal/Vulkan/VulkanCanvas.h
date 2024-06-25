//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 06:02:35 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_CANVAS_H__
#define __VULKAN_CANVAS_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/Canvas.h"
#include "Nkentseu/Graphics/Context.h"

#include "VulkanContext.h"
#include "VulkanRenderer.h"
#include "VulkanShader.h"
#include "VulkanIndexBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanVertexArray.h"
#include "VulkanUniformBuffer.h"

namespace nkentseu {
    class VulkanCanvas : public Canvas {
    public:
        VulkanCanvas(const Memory::Shared<Context>& context);
        ~VulkanCanvas();

        void Clear(const Color& color) override;
        void Prepare() override;
        void Present() override;
        void Destroy() override;
        maths::Vector2f GetSize() override;

        void DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture texture = nullptr) override;
        void DrawLine(const maths::Vector2f& start, const maths::Vector2f& end, const Color& color, CanvasTexture texture = nullptr) override;
        void DrawRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, bool filled = true, CanvasTexture texture = nullptr) override;

    private:
        friend class VulkanRenderer;

        Memory::Shared<VulkanContext> m_Context;
        Memory::Shared<VulkanUniformBuffer> m_UniformBuffer = nullptr;

        Memory::Shared<VulkanShader> m_Shader;
        Memory::Shared<VulkanVertexBuffer> m_VertexBuffer;
        Memory::Shared<VulkanIndexBuffer> m_IndexBuffer;
        Memory::Shared<VulkanVertexArray> m_VertexArray;

        std::vector<VertexData> m_Vertices;
        std::vector<uint32> m_Indices;
        std::vector<RenderCommand> m_Commands;
        uint32 m_IndexCount = 0;

        maths::Vector2f m_Size;

        void Initialize();
        void CreateResources();
        void Flush();
    };

}  //  nkentseu

#endif  // __CANVAS_H__!