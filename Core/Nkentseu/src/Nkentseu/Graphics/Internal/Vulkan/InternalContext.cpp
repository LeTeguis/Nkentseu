//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalContext.h"


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "Nkentseu/Core/Window.h"
#include "VulkanUtils.h"

namespace nkentseu {

    InternalContext::InternalContext() : m_Window(nullptr), m_IsInitialize(false) {
    }

    InternalContext::~InternalContext(){
    }

    bool InternalContext::SetWindow(Window* window) {
        if (window == nullptr) return false;
        m_Window = window;
        m_WindowSize = m_Window->ConvertPixelToDpi(Vector2f(m_Window->GetSize().width, m_Window->GetSize().height));
        return true;
    }
    bool InternalContext::SetProperties(const ContextProperties& properties) {
        m_ContextProperties = properties;
        return true;
    }

    bool InternalContext::Initialize() {
        if (m_Window == nullptr) return false;

        m_WindowSize = m_Window->ConvertPixelToDpi(Vector2f(m_Window->GetSize().width, m_Window->GetSize().height));

        m_Extension.Defined();

        if (!m_Instance.Create(m_Window, m_ContextProperties, &m_Extension)) return false;
        if (!m_Surface.Create(m_Window, &m_Instance)) return false;
        if (!m_Gpu.GetDevice(&m_Instance, &m_Surface, &m_Extension)) return false;
        if (!m_Swapchain.Create(&m_Gpu, &m_Surface, m_WindowSize, m_ContextProperties)) return false;
        if (!m_CommandPool.Create(&m_Gpu)) return false;
        if (!m_Semaphore.Create(&m_Gpu)) return false;
        if (!m_RenderPass.Create(&m_Gpu, &m_Swapchain)) return false;
        if (!m_PipelineLayout.Create(&m_Gpu)) return false;

        if (!m_Framebuffer.Create(&m_Gpu, m_WindowSize, &m_RenderPass, &m_Swapchain)) return false;

        m_IsInitialize = true;

        return m_IsInitialize;
    }

    bool InternalContext::IsInitialize() {
        return m_IsInitialize;
    }

    bool InternalContext::EnableVSync() {
        return false;
    }

    bool InternalContext::DisableVSync() {
        return false;
    }

    bool InternalContext::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_Window != nullptr) return false;

        m_Window = window;

        if (m_Window == nullptr) return false;

        m_ContextProperties = contextProperties;

        return Initialize();
    }

    bool InternalContext::Deinitialize()
    {
        // m_Instance.Destroy();
        return false;
    }

    Window* InternalContext::GetWindow() {
        return m_Window;
    }

    const ContextProperties& InternalContext::GetProperties()
    {
        return m_ContextProperties;
    }


    const GraphicsInfos& InternalContext::GetGraphicsInfo() {
        static GraphicsInfos tmp;
        return tmp;
    }

    VulkanInstance* nkentseu::InternalContext::GetInstance()
    {
        return &m_Instance;
    }

    VulkanSurface* nkentseu::InternalContext::GetSurface()
    {
        return &m_Surface;
    }

    VulkanExtension* nkentseu::InternalContext::GetExtension()
    {
        return &m_Extension;
    }

    VulkanGpu* nkentseu::InternalContext::GetGpu()
    {
        return &m_Gpu;
    }

    VulkanSwapchain* nkentseu::InternalContext::GetSwapchain()
    {
        return &m_Swapchain;
    }

    VulkanCommandPool* nkentseu::InternalContext::GetCommandPool()
    {
        return &m_CommandPool;
    }

    VulkanSemaphore* nkentseu::InternalContext::GetSemaphore()
    {
        return &m_Semaphore;
    }

    VulkanRenderPass* nkentseu::InternalContext::GetRenderPass()
    {
        return &m_RenderPass;
    }

    VulkanFramebuffer* nkentseu::InternalContext::GetFramebuffer()
    {
        return &m_Framebuffer;
    }

    Vector2u InternalContext::GetFrameBufferSize() {
        return m_Framebuffer.size;
    }

    bool InternalContext::AddRecreateCallback(RerecreateCallBackFn func) {
        if (func == nullptr) return false;

        auto it = std::find_if(m_RecreateList.begin(), m_RecreateList.end(),
            [&func](const RerecreateCallBackFn& registeredFunc) {
                // Compare target of std::function objects
                return registeredFunc.target<bool(bool)>() == func.target<bool(bool)>();
            });

        if (it == m_RecreateList.end()) {
            m_RecreateList.push_back(func);
            return true;
        }
        return false; // Function already exists
    }

    bool InternalContext::RemoveRecreateCallback(RerecreateCallBackFn func) {
        if (func == nullptr) return false;

        auto it = std::find_if(m_RecreateList.begin(), m_RecreateList.end(),
            [&func](const RerecreateCallBackFn& registeredFunc) {
                // Compare target of std::function objects
                return registeredFunc.target<bool(bool)>() == func.target<bool(bool)>();
            });

        if (it != m_RecreateList.end()) {
            m_RecreateList.erase(it);
            return true;
        }
        return false; // Function not found
    }

    bool InternalContext::IsValidContext()
    {
        return m_IsInitialize;
    }

    bool InternalContext::CleanupSwapChain() {
        m_Framebuffer.Destroy(&m_Gpu);
        m_Swapchain.Destroy(&m_Gpu);
        return true;
    }

    bool InternalContext::RecreateSwapChain() {
        if (!m_IsInitialize) return false;

		VulkanResult result;
		bool first = true;

        m_WindowSize = m_Window->ConvertPixelToDpi(Vector2f(m_Window->GetSize()));

        vkCheckError(first, result, vkDeviceWaitIdle(m_Gpu.device), "cannot wait idle");

        if (result.success == false) return false;

        if (!CleanupSwapChain()) return false;

        if (!m_Swapchain.Create(&m_Gpu, &m_Surface, m_WindowSize, m_ContextProperties)) return false;
        if (!m_Framebuffer.Create(&m_Gpu, m_WindowSize, &m_RenderPass, &m_Swapchain)) return false;

        // Iterate over m_RecreateList and call non-null callbacks, remove null callbacks
        auto it = m_RecreateList.begin();
        while (it != m_RecreateList.end()) {
            if (*it) {
                // Call the callback with the forceRecreate parameter
                (*it)(true);
                ++it;
            }
            else {
                // Remove null callbacks
                it = m_RecreateList.erase(it);
            }
        }

        return true;
    }
}    // namespace nkentseu

#endif