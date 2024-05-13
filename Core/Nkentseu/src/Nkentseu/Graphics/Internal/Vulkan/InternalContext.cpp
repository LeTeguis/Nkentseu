//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalContext.h"


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

namespace nkentseu {

    InternalContext::InternalContext() : m_Window(nullptr) {
    }

    InternalContext::~InternalContext(){
    }

    bool InternalContext::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_Window != nullptr) return false;

        m_Window = window;

        if (m_Window == nullptr) return false;

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
        // TODO: insérer une instruction return ici
        if (m_NativeContext == nullptr) return {};
        return m_NativeContext->GetProperties();
    }
}    // namespace nkentseu

#endif