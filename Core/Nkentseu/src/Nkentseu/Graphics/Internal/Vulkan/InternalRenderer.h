//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 08:17:20 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_RENDERER_H__
#define __VULKAN_INTERNAL_RENDERER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include <System/Definitions/Memory.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <Nkentseu/Graphics/Color.h>
#include <Ntsm/Vector/Vector2.h>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace nkentseu {
    class Context;
    class Shader;
    class VertexArray;
    
    class NKENTSEU_API InternalRenderer {
        public:
            InternalRenderer();
            ~InternalRenderer();

            bool Initialize(Context* context);
            bool Deinitialize();

            bool Clear(const Color& color);
            bool Clear(uint8 r, uint8 g, uint8 b, uint8 a = 255);

            bool SetActiveShader(Memory::Shared<Shader> shader);
            bool UnsetActiveShader();

            bool DrawMode(DrawMode::Code mode, DrawContentMode::Code contentMode);
            bool Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex);

            bool Present();
            bool Swapbuffer();

            bool Prepare();
            bool Finalize();

            bool Resize(const Vector2u& size);
        private:
            Context* m_Context;
            VkCommandBuffer m_CurrentCommandBuffer;
            uint32 m_CurrentImageIndice = 0;
            bool m_IsPrepare = false;

            bool m_ClearColor = false;
            Color m_PreviousColor;

            VkExtent2D m_ViewportSize;
            bool m_ForceRecreate = false;
            Memory::Shared<Shader> m_CurrentShader = nullptr;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_RENDERER_H__!