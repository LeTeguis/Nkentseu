//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalContext.h"


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "Nkentseu/Core/Window.h"

namespace nkentseu {

    InternalContext::InternalContext() : m_Window(nullptr) {
    }

    InternalContext::~InternalContext(){
    }

    bool InternalContext::SetWindow(Window* window) {
        return false;
    }
    bool InternalContext::SetProperties(const ContextProperties& properties) {
        return false;
    }

    bool InternalContext::Initialize() {
        return false;
    }
    bool InternalContext::IsInitialize() {
        return false;
    }

    bool InternalContext::EnableVSync() {
        return false;
    }

    bool InternalContext::DisableVSync() {
        return false;
    }

    bool InternalContext::Present() {
        return false;
    }

    bool InternalContext::Swapchaine() {
        return false;
    }

    bool InternalContext::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_Window != nullptr) return false;

        m_Window = window;

        if (m_Window == nullptr) return false;

        m_Extension.Defined();

        if (!m_Instance.Create(m_Window, contextProperties, &m_Extension)) return false;
        if (!m_Surface.Create(m_Window, &m_Instance)) return false;
        if (!m_Gpu.GetDevice(&m_Instance, &m_Surface, &m_Extension)) return false;
        if (!m_Swapchain.Create(&m_Gpu, &m_Surface)) return false;

        return false;
    }

    bool InternalContext::Deinitialize()
    {
        return false;
    }

    bool InternalContext::MakeCurrent()
    {
        return false;
    }

    bool InternalContext::UnmakeCurrent()
    {
        return false;
    }

    bool InternalContext::IsCurrent()
    {
        return false;
    }

    Window* InternalContext::GetWindow() {
        return m_Window;
    }

    const ContextProperties& InternalContext::GetProperties()
    {
        static ContextProperties tmp;
        return tmp;
    }


    const GraphicsInfos& InternalContext::GetGraphicsInfo() {
        static GraphicsInfos tmp;
        return tmp;
    }
}    // namespace nkentseu

#endif