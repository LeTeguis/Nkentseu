//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNA_LCONTEXT_H__
#define __VULKAN_INTERNA_LCONTEXT_H__

#pragma once


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "System/System.h"
#include "Nkentseu/Graphics/Context.h"
#include "VulkanInternal.h"

namespace nkentseu {
    class Window;

    class NKENTSEU_API InternalContext
    {
        public:
            InternalContext();
            ~InternalContext();

            bool SetWindow(Window* window);
            bool SetProperties(const ContextProperties& properties);

            bool Initialize();
            bool Initialize(class Window* window, const ContextProperties& contextProperties);
            bool Deinitialize();
            bool IsInitialize();

            bool EnableVSync();
            bool DisableVSync();

            const GraphicsInfos& GetGraphicsInfo();

            Window* GetWindow();
            const ContextProperties& GetProperties();
        private:
            friend class InternalRenderer;

            Window* m_Window;
            ContextProperties m_ContextProperties;

            VulkanInstance m_Instance;
            VulkanSurface m_Surface;
            VulkanExtension m_Extension;
            VulkanGpu m_Gpu;
            VulkanSwapchain m_Swapchain;
            VulkanCommandPool m_CommandPool;
            VulkanSemaphore m_Semaphore;

            bool m_IsInitialize = false;
    };
} // namespace nkentseu


#endif


#endif    // __NKENTSEU_INTERNA_LCONTEXT_H__