//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
#include "NkentseuPch/ntspch.h"
#include "InternalContext.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include <Nkentseu/Platform/Window/Windows/WGLContext.h>
#elif defined NKENTSEU_PLATFORM_LINUX
    #if defined(NKENTSEU_PLATFORM_LINUX_XCB)
    #include <Nkentseu/Platform/Window/Linux/XCB/XGLContext.h>
    #elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
    #include <Nkentseu/Platform/Window/Linux/XLIB/XGLContext.h>
    #endif
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

    InternalContext::InternalContext() : m_Window(nullptr){
        if (m_NativeContext == nullptr) {
            m_NativeContext = Memory::Alloc<NativeContext>();
        }
    }

    InternalContext::~InternalContext(){
    }

    bool nkentseu::InternalContext::SetWindow(Window* window)
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->SetWindow(window);
    }

    bool InternalContext::SetProperties(const ContextProperties& properties)
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->SetProperties(properties);
    }

    bool InternalContext::Initialize()
    {
        if (m_NativeContext == nullptr) {
            m_NativeContext = Memory::Alloc<NativeContext>();
            if (m_NativeContext == nullptr) {
                return false;
            }
        }
        return m_NativeContext->Initialize();
    }

    bool InternalContext::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_NativeContext == nullptr) {
            m_NativeContext = Memory::Alloc<NativeContext>();
            if (m_NativeContext == nullptr) {
                return false;
            }
        }
        if (!m_NativeContext->SetWindow(window)){
            Log_nts.Error();
            return false;
        }
        if (!m_NativeContext->SetProperties(contextProperties)){
            Log_nts.Error();
            return false;
        }
        return m_NativeContext->Initialize();
    }

    bool InternalContext::Deinitialize()
    {
        if (m_NativeContext == nullptr) return false;
        bool deinit = m_NativeContext->Deinitialize();
        if (deinit) {
            Memory::Reset(m_NativeContext);
        }
        return deinit;
    }

    bool InternalContext::IsInitialize()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->IsInitialize();
    }

    bool InternalContext::MakeCurrent()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->MakeCurrent();
    }

    bool InternalContext::UnmakeCurrent()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->UnmakeCurrent();
    }

    bool InternalContext::IsCurrent()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->IsCurrent();
    }

    bool InternalContext::EnableVSync()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->EnableVSync();
    }

    bool InternalContext::DisableVSync()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->DisableVSync();
    }

    bool InternalContext::Present()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->Present();
    }

    bool InternalContext::Swapchaine()
    {
        if (m_NativeContext == nullptr) return false;
        return m_NativeContext->Swapchaine();
    }

    const GraphicsInfos& InternalContext::GetGraphicsInfo()
    {
        static const GraphicsInfos graphicsInfos = {}; 
        if (m_NativeContext == nullptr) return graphicsInfos;
        return m_NativeContext->GetGraphicsInfo();
    }

    Memory::Shared<NativeContext> InternalContext::GetNativeContext() {
        return m_NativeContext;
    }

    Window* InternalContext::GetWindow() {
        if (m_NativeContext == nullptr) return nullptr;
        return m_NativeContext->GetWindow();
    }

    const ContextProperties& InternalContext::GetProperties()
    {
        static const ContextProperties contextProperties = {}; 
        if (m_NativeContext == nullptr) return contextProperties;
        return m_NativeContext->GetProperties();
    }
}    // namespace nkentseu

#endif