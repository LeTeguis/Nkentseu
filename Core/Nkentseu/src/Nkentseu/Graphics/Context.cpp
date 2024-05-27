//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:48:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Context.h"

#ifdef NKENTSEU_GRAPHICS_API_SOFTWARE
#include "Internal/Software/InternalContext.h"
#elif defined(NKENTSEU_GRAPHICS_API_OPENGL)
#include "Internal/Opengl/InternalContext.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalContext.h"
#elif defined(NKENTSEU_GRAPHICS_API_DIRECTX11)
#include "Internal/DirectX11/InternalContext.h"
#elif defined(NKENTSEU_GRAPHICS_API_DIRECTX12)
#include "Internal/DirectX12/InternalContext.h"
#elif defined(NKENTSEU_GRAPHICS_API_METAL)
#include "Internal/Metal/InternalContext.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported this graphics context"
#endif

#include <Nkentseu/Platform/Internal/InternalMemory.h>
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

    Context::Context() : m_InternalContext(nullptr) {
        if (m_InternalContext == nullptr) {
            m_InternalContext = Memory::Alloc<InternalContext>();
        }
    }

    Context::Context(Window* window, const ContextProperties& contextProperties) : m_InternalContext(nullptr)
    {
        Initialize(window, contextProperties);
    }

    Context::~Context(){
    }

    bool Context::SetWindow(Window* window)
    {
        if (m_InternalContext == nullptr) {
            return false;
        }
        return m_InternalContext->SetWindow(window);
    }

    bool Context::SetProperties(const ContextProperties& properties)
    {
        if (m_InternalContext == nullptr) {
            return false;
        }
        return m_InternalContext->SetProperties(properties);
    }

    bool Context::Initialize()
    {
        if (m_InternalContext == nullptr) {
            m_InternalContext = Memory::Alloc<InternalContext>();
            if (m_InternalContext == nullptr) return false;
        }
        
        if (m_IsInitialized == false) {
            m_IsInitialized = m_InternalContext->Initialize();
        }
        return m_IsInitialized;
    }

    bool Context::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_InternalContext == nullptr){
            m_InternalContext = Memory::Alloc<InternalContext>();
            if (m_InternalContext == nullptr) return false;
        }

        if (m_IsInitialized == false) {
            m_IsInitialized = m_InternalContext->Initialize(window, contextProperties);
        }
        return m_IsInitialized;
    }

    bool Context::Deinitialize()
    {
        if (m_InternalContext != nullptr) {
            bool deinitialize = m_InternalContext->Deinitialize();

            if (!deinitialize) {
                return false;
            }

            Memory::Reset(m_InternalContext);
        }
        m_IsInitialized = false;
        Memory::Reset(m_InternalContext);
        return true;
    }

    bool Context::IsInitialize()
    {
        if (m_InternalContext == nullptr) return false;
        return m_InternalContext->IsInitialize();
    }

    bool Context::EnableVSync()
    {
        if (m_InternalContext == nullptr) return false;
        return m_InternalContext->EnableVSync();
    }

    bool Context::DisableVSync()
    {
        if (m_InternalContext == nullptr) return false;
        return m_InternalContext->DisableVSync();
    }

    const GraphicsInfos& Context::GetGraphicsInfo()
    {
        static const GraphicsInfos graphicInfos = {}; 
        if (m_InternalContext == nullptr) return graphicInfos;
        return m_InternalContext->GetGraphicsInfo();
    }

    Window* Context::GetWindow()
    {
        if (m_InternalContext == nullptr) return nullptr;
        return m_InternalContext->GetWindow();
    }

    InternalContext* Context::GetInternal()
    {
        if (m_InternalContext == nullptr) return nullptr;
        return m_InternalContext.get();
    }

    const ContextProperties& Context::GetProperties()
    {
        static const ContextProperties contextProperties = {}; 
        if (m_InternalContext == nullptr) return contextProperties;
        return m_InternalContext->GetProperties();
    }

    bool Context::IsValidContext()
    {
        if (m_InternalContext == nullptr || !m_InternalContext->IsValidContext()) {
            Log_nts.Error("Invalid Internal context");
            return false;
        }
        return true;
    }

}    // namespace nkentseu