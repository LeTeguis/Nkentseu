//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalContext.h"


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "Nkentseu/Core/Window.h"

namespace nkentseu {

    InternalContext::InternalContext() : m_Window(nullptr), m_IsInitialize(false) {
    }

    InternalContext::~InternalContext(){
    }

    bool InternalContext::SetWindow(Window* window) {
        if (window == nullptr) return false;
        m_Window = window;
        return true;
    }
    bool InternalContext::SetProperties(const ContextProperties& properties) {
        m_ContextProperties = properties;
        return true;
    }

    bool InternalContext::Initialize() {
        if (m_Window == nullptr) return false;

        m_Extension.Defined();

        if (!m_Instance.Create(m_Window, m_ContextProperties, &m_Extension)) return false;
        if (!m_Surface.Create(m_Window, &m_Instance)) return false;
        if (!m_Gpu.GetDevice(&m_Instance, &m_Surface, &m_Extension)) return false;
        if (!m_Swapchain.Create(&m_Gpu, &m_Surface)) return false;
        if (!m_CommandPool.Create(&m_Gpu)) return false;
        if (!m_Semaphore.Create(&m_Gpu)) return false;

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
}    // namespace nkentseu

#endif