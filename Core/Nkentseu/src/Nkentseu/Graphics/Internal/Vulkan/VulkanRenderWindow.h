//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 05:28:49 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_RENDER_WINDOW_H__
#define __VULKAN_RENDER_WINDOW_H__

#pragma once

#include <System/System.h>
#include "Nkentseu/Graphics/RenderWindow.h"
#include "VulkanContext.h"

namespace nkentseu {
    
    class NKENTSEU_API VulkanRenderWindow : public RenderWindow {
        public:
            VulkanRenderWindow(Memory::Shared<Context> context);
            ~VulkanRenderWindow();

            Memory::Shared<Context> GetContext() override;

            bool Initialize() override;
            bool Deinitialize() override;

            bool Begin(const Color& color) override;
            bool Begin(uint8 r, uint8 g, uint8 b, uint8 a = 255) override;
            bool End() override;

            bool SetViewport(const maths::Vector4f& viewport) override;
            bool SetViewport(float32 x, float32 y, float32 width, float32 height) override;
            bool ResetViewport() override;

            bool EnableDepthTest(bool enabled) override;
            bool EnableScissorTest(bool enabled) override;

            bool SetScissor(const maths::Vector4f& scissor) override;
            bool SetScissor(float32 x, float32 y, float32 width, float32 height) override;
            bool ResetScissor() override;

            bool SetPolygonMode(PolygonModeType mode) override;
            bool SetCullMode(CullModeType mode) override;
            bool SetFrontFaceMode(FrontFaceType mode) override;
            bool SetRenderPrimitive(RenderPrimitive mode) override;

            Memory::Shared<Canvas> GetCanvas() override;
        private:
            Memory::Shared<VulkanContext> m_Context;
            Color m_PreviousColor;
            vk::Extent2D m_ViewportSize;

            bool Recreate(bool force);
            bool CleanUp(bool force);

            // frame buffer
            bool CreateFrameBuffer(const maths::Vector2u& size);
            bool DestroyFrameBuffer();

            std::vector<vk::Framebuffer> framebuffer = {};
            maths::Vector2u size = {};
            
            // depth image
            vk::Image depthImage;
            vk::ImageView depthImageView;
            vk::DeviceMemory depthMemory;

            vk::Format format;
            uint32 mipLevels = 1;
            vk::ImageLayout layout;

            bool CreateDepth(const maths::Vector2u& size, vk::Format format = vk::Format::eR32G32B32A32Sfloat, vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1);
            bool DestroyDepth();
            bool CreateImage(vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspectMask);
            bool CreateMemory();
            bool CreateImageView(vk::ImageAspectFlags aspectMask);

            // Private members
    };

}  //  nkentseu

#endif  // __VULKAN_RENDER_WINDOW_H__!