//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 06:02:35 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_CANVAS_H__
#define __VULKAN_CANVAS_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/Canvas.h"
#include "Nkentseu/Graphics/Shader.h"
#include "Nkentseu/Graphics/Context.h"

#include "VulkanContext.h"
#include "VulkanRenderer.h"
#include "VulkanShader.h"
#include "VulkanIndexBuffer.h"
#include "VulkanVertexBuffer.h"
#include "VulkanVertexArray.h"
#include "VulkanUniformBuffer.h"
#include "VulkanShaderInputLayout.h"

namespace nkentseu {
    class VulkanCanvas : public Canvas {
    public:
        VulkanCanvas(const Memory::Shared<Context>& context);
        ~VulkanCanvas();

        void Destroy() override;
        maths::Vector2f GetSize() override;

    private:
        friend class VulkanRenderer;

        Memory::Shared<VulkanContext> m_Context;
        Memory::Shared<VulkanShaderInputLayout> shaderInputLayout = nullptr;

        Memory::Shared<VulkanUniformBuffer> m_UniformBuffer = nullptr;

        Memory::Shared<VulkanShader> m_Shader;
        Memory::Shared<VulkanVertexBuffer> m_VertexBuffer;
        Memory::Shared<VulkanIndexBuffer> m_IndexBuffer;
        Memory::Shared<VulkanVertexArray> m_VertexArray;

        maths::Vector2f m_Size;

        void Initialize();
        void CreateResources();
        void Flush() override;
    };

}  //  nkentseu

#endif  // __CANVAS_H__!