//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanContext.h"

#include "Nkentseu/Core/Window.h"
#include "VulkanUtils.h"

namespace nkentseu {
    using namespace maths;

    VulkanContext::VulkanContext(Memory::Shared<Window> window, const ContextProperties& contextProperties) : m_Window(window), m_IsInitialize(false), m_ContextProperties(contextProperties) {
    }

    VulkanContext::~VulkanContext(){
    }

    bool VulkanContext::Initialize() {
        if (m_Window == nullptr) return false;

        m_WindowSize = m_Window->ConvertPixelToDpi(Vector2f(m_Window->GetSize().width, m_Window->GetSize().height));

        m_Extension.Defined();

        if (!m_Instance.Create(m_Window.get(), m_ContextProperties, &m_Extension)) return false;
        if (!m_Surface.Create(m_Window.get(), &m_Instance)) return false;
        if (!m_Gpu.Create(&m_Instance, &m_Surface, &m_Extension)) return false;
        if (!m_Swapchain.Create(&m_Gpu, &m_Surface, m_WindowSize, m_ContextProperties)) return false;
        if (!m_CommandPool.Create(&m_Gpu)) return false;
        if (!m_Semaphore.Create(&m_Gpu)) return false;
        if (!m_RenderPass.Create(&m_Gpu, &m_Swapchain)) return false;
        if (!m_CommandBuffer.Create(&m_Gpu, &m_Swapchain, &m_CommandPool)) return false;

        if (!m_Framebuffer.Create(&m_Gpu, m_WindowSize, &m_RenderPass, &m_Swapchain)) return false;

        m_IsInitialize = true;

        return m_IsInitialize;
    }

    bool VulkanContext::IsInitialize() {
        return m_IsInitialize;
    }

    bool VulkanContext::EnableVSync() {
        return false;
    }

    bool VulkanContext::DisableVSync() {
        return false;
    }

    bool VulkanContext::Deinitialize()
    {
        m_Framebuffer.Destroy(&m_Gpu);
        m_CommandBuffer.Destroy(&m_Gpu, &m_CommandPool);
        m_RenderPass.Destroy(&m_Gpu);
        m_Semaphore.Destroy(&m_Gpu);
        m_CommandPool.Destroy(&m_Gpu);
        m_Swapchain.Destroy(&m_Gpu);
        m_Gpu.Destroy();
        m_Surface.Destroy(&m_Instance);
        m_Instance.Destroy();
        return false;
    }

    Memory::Shared<Window> VulkanContext::GetWindow() {
        return m_Window;
    }

    const ContextProperties& VulkanContext::GetProperties()
    {
        return m_ContextProperties;
    }


    const GraphicsInfos& VulkanContext::GetGraphicsInfo() {
        static GraphicsInfos tmp;
        return tmp;
    }

    VulkanInstance* VulkanContext::GetInstance()
    {
        return &m_Instance;
    }

    VulkanSurface* VulkanContext::GetSurface()
    {
        return &m_Surface;
    }

    VulkanExtension* VulkanContext::GetExtension()
    {
        return &m_Extension;
    }

    VulkanGpu* VulkanContext::GetGpu()
    {
        return &m_Gpu;
    }

    VulkanSwapchain* VulkanContext::GetSwapchain()
    {
        return &m_Swapchain;
    }

    VulkanCommandPool* VulkanContext::GetCommandPool()
    {
        return &m_CommandPool;
    }

    VulkanSemaphore* VulkanContext::GetSemaphore()
    {
        return &m_Semaphore;
    }

    VulkanRenderPass* VulkanContext::GetRenderPass()
    {
        return &m_RenderPass;
    }

    VulkanFramebuffer* VulkanContext::GetFramebuffer()
    {
        return &m_Framebuffer;
    }

    VkCommandBuffer VulkanContext::GetCurrentCommandBuffer()
    {
        return m_CommandBuffer.commandBuffers[currentImageIndex];
    }

    Vector2u VulkanContext::GetFrameBufferSize() {
        return m_Framebuffer.size;
    }

    bool VulkanContext::AddCleanUpCallback(CleanUpCallBackFn func)
    {
        return AddRecreateCallback(m_CleanList, func); // Function not found
    }

    bool VulkanContext::RemoveCleanUpCallback(CleanUpCallBackFn func)
    {
        return RemoveRecreateCallback(m_CleanList, func); // Function not found
    }

    bool VulkanContext::AddRecreateCallback(RerecreateCallBackFn func)
    {
        return AddRecreateCallback(m_RecreateList, func); // Function not found
    }

    bool VulkanContext::RemoveRecreateCallback(RerecreateCallBackFn func)
    {
        return RemoveRecreateCallback(m_RecreateList, func); // Function not found
    }

    bool VulkanContext::CanUse()
    {
        return m_CanUse;
    }

    bool VulkanContext::IsValidContext()
    {
        return m_IsInitialize;
    }

    bool VulkanContext::CleanupSwapChain() {
        CallReateCallback(m_CleanList);

        m_Framebuffer.Destroy(&m_Gpu);
        m_CommandBuffer.Destroy(&m_Gpu, &m_CommandPool);
        m_Swapchain.Destroy(&m_Gpu);
        return true;
    }

    bool VulkanContext::RecreateSwapChain() {
        if (!m_IsInitialize) return false;

		VulkanResult result;
		bool first = true;

        m_WindowSize = m_Window->ConvertPixelToDpi(Vector2f(m_Window->GetSize()));

        vkCheckError(first, result, vkDeviceWaitIdle(m_Gpu.device), "cannot wait idle");

        if (result.success == false) return false;

        if (!CleanupSwapChain()) return false;

        if (!m_Swapchain.Create(&m_Gpu, &m_Surface, m_WindowSize, m_ContextProperties)) return false;
        if (!m_CommandBuffer.Create(&m_Gpu, &m_Swapchain, &m_CommandPool)) return false;
        if (!m_Framebuffer.Create(&m_Gpu, m_WindowSize, &m_RenderPass, &m_Swapchain)) return false;
        currentImageIndex = 0;

        CallReateCallback(m_RecreateList);

        return true;
    }

    bool VulkanContext::AddRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe, RerecreateCallBackFn func)
    {
        if (func == nullptr) return false;

        auto it = std::find_if(callbackListe.begin(), callbackListe.end(),
            [&func](const RerecreateCallBackFn& registeredFunc) {
                // Compare target of std::function objects
                return registeredFunc.target<bool(bool)>() == func.target<bool(bool)>();
            });

        if (it == callbackListe.end()) {
            callbackListe.push_back(func);
            return true;
        }
        return false; // Function already exists
    }

    bool VulkanContext::RemoveRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe, RerecreateCallBackFn func)
    {
        if (func == nullptr) return false;

        auto it = std::find_if(callbackListe.begin(), callbackListe.end(),
            [&func](const RerecreateCallBackFn& registeredFunc) {
                // Compare target of std::function objects
                return registeredFunc.target<bool(bool)>() == func.target<bool(bool)>();
            });

        if (it != callbackListe.end()) {
            callbackListe.erase(it);
            return true;
        }
        return false; // Function not found
    }

    bool VulkanContext::CallReateCallback(std::vector<RerecreateCallBackFn>& callbackListe)
    {
        // Iterate over m_RecreateList and call non-null callbacks, remove null callbacks
        auto it = callbackListe.begin();
        while (it != callbackListe.end()) {
            if (*it) {
                // Call the callback with the forceRecreate parameter
                (*it)(true);
                ++it;
            }
            else {
                // Remove null callbacks
                it = callbackListe.erase(it);
            }
        }
        return true;
    }
}    // namespace nkentseu