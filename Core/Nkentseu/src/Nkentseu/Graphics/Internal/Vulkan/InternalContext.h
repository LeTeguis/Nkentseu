//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNA_LCONTEXT_H__
#define __VULKAN_INTERNA_LCONTEXT_H__

#pragma once


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "System/System.h"
#include "System/Nature/Base.h"
#include "Nkentseu/Graphics/Context.h"
#include "VulkanInternal.h"

#include <functional>
#include <vector>

namespace nkentseu {
    class Window;

    using RerecreateCallBackFn = std::function<bool(bool)>;
    #define RECREATE_CALLBACK_FN(method) std::bind(&method, this, STDPH(1))

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

            VulkanInstance* GetInstance();
            VulkanSurface* GetSurface();
            VulkanExtension* GetExtension();
            VulkanGpu* GetGpu();
            VulkanSwapchain* GetSwapchain();
            VulkanCommandPool* GetCommandPool();
            VulkanSemaphore* GetSemaphore();
            VulkanRenderPass* GetRenderPass();
            VulkanFramebuffer* GetFramebuffer();

            Vector2u GetWindowSize(bool forceRecreate);

            bool AddRecreateCallback(RerecreateCallBackFn func);
            bool RemoveRecreateCallback(RerecreateCallBackFn func);
        public:
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
            VulkanRenderPass m_RenderPass;
            VulkanFramebuffer m_Framebuffer;
            VulkanPipelineLayout m_PipelineLayout;

            bool m_IsInitialize = false;
            
            Vector2u m_WindowSize = {};

            std::vector<RerecreateCallBackFn> m_RecreateList;

        public:
            bool CleanupSwapChain();
            bool RecreateSwapChain();
    };
} // namespace nkentseu


#endif


#endif    // __NKENTSEU_INTERNA_LCONTEXT_H__