//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_CONTEXT_H__
#define __VULKAN_INTERNAL_CONTEXT_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"
#include "Nkentseu/Graphics/Context.h"
#include "VulkanInternal.h"

#include <functional>
#include <vector>

namespace nkentseu {
    class Window;

    using RerecreateCallBackFn = std::function<bool(bool)>;
    using CleanUpCallBackFn = RerecreateCallBackFn;
    #define SWAPCHAIN_CALLBACK_FN(method) std::bind(&method, this, STDPH(1))

    class NKENTSEU_API VulkanContext : public Context
    {
        public:
            VulkanContext(Memory::Shared<Window> window, const ContextProperties& contextProperties);
            ~VulkanContext();

            bool Initialize() override;
            bool Deinitialize() override;
            bool IsInitialize() override;

            bool EnableVSync() override;
            bool DisableVSync() override;

            const GraphicsInfos& GetGraphicsInfo() override;
            const ContextProperties& GetProperties() override;
            Memory::Shared<Window> GetWindow() override;

            VulkanInstance* GetInstance();
            VulkanSurface* GetSurface();
            VulkanExtension* GetExtension();
            VulkanGpu* GetGpu();
            VulkanSwapchain* GetSwapchain();
            VulkanCommandPool* GetCommandPool();
            VulkanSemaphore* GetSemaphore();
            VulkanRenderPass* GetRenderPass();
            VulkanFramebuffer* GetFramebuffer();
            VkCommandBuffer GetCurrentCommandBuffer();

            maths::Vector2u GetFrameBufferSize();

            bool AddCleanUpCallback(CleanUpCallBackFn func);
            bool RemoveCleanUpCallback(CleanUpCallBackFn func);

            bool AddRecreateCallback(RerecreateCallBackFn func);
            bool RemoveRecreateCallback(RerecreateCallBackFn func);

            bool IsValidContext();
            bool CanUse();

            uint32 currentImageIndex = 0;
            bool renderThisFrame = false;
        private:

            Memory::Shared<Window> m_Window;
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
            VulkanCommandBuffer m_CommandBuffer;

            bool m_IsInitialize = false;
            bool m_CanUse = false;
            
            maths::Vector2u m_WindowSize = {};

            std::vector<RerecreateCallBackFn> m_RecreateList;
            std::vector<CleanUpCallBackFn> m_CleanList;

        public:
            bool CleanupSwapChain();
            bool RecreateSwapChain();

            bool AddRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe, RerecreateCallBackFn func);
            bool RemoveRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe, RerecreateCallBackFn func);
            bool CallReateCallback(std::vector<RerecreateCallBackFn>& callbackListe);
    };
} // namespace nkentseu

#endif    // __NKENTSEU_INTERNA_LCONTEXT_H__