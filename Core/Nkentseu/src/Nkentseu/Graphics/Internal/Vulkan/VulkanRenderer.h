//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 08:17:20 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_RENDERER_H__
#define __VULKAN_INTERNAL_RENDERER_H__

#pragma once

#include <System/System.h>

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Graphics/Renderer.h>
#include <Ntsm/Vector/Vector2.h>
#include "Nkentseu/Core/Events.h"

#include <vector>

#include <vulkan/vulkan.hpp>
#include "VulkanInternal.h"

namespace nkentseu {
    class VulkanContext;
    class VulkanShader;
    class VulkanVertexArray;
    
    class NKENTSEU_API VulkanRenderer : public Renderer {
        public:
            VulkanRenderer(Memory::Shared<Context> context);
            ~VulkanRenderer();

            Memory::Shared<Context> GetContext() override;

            bool Initialize() override;
            bool Deinitialize() override;

            bool Begin(const Color& color) override;
            bool Begin(uint8 r, uint8 g, uint8 b, uint8 a = 255) override;
            bool End() override;

            bool Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) override;

            bool BindShader(Memory::Shared<Shader> shader) override;
            bool UnbindShader() override;
            bool BindUniform(const std::string& name, void* data, usize size) override;
            bool UnbindUniform(const std::string& name) override;

            bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) override;

            bool PolygonMode(PolygonModeType::Code mode) override;
            bool CullMode(CullModeType::Code mode) override;
            bool FrontFaceMode(FrontFaceType::Code mode) override;
            bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode) override;
            bool ScissorMode(const Vector2i& offset, const Vector2u& extend) override;
            bool ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth) override;
        private:
            Memory::Shared<VulkanContext> m_Context;
            VkCommandBuffer m_CurrentCommandBuffer;

            bool m_IsPrepare = false;

            VulkanDynamicMode m_DynamicMode;

            bool m_ClearColor = false;
            Color m_PreviousColor;

            VkExtent2D m_ViewportSize;
            Memory::Shared<VulkanShader> m_CurrentShader = nullptr;

        private:
            void OnEvent(Event& event);
            bool OnWindowResizedEvent(class WindowResizedEvent& event);
            bool CanRender();
    };

}  //  nkentseu

#endif  // __INTERNAL_RENDERER_H__!