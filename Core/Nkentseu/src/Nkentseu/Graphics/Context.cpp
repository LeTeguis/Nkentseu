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

    Context::Context() : m_InternalContextInfo(nullptr) {
        if (m_InternalContextInfo == nullptr) {
            m_InternalContextInfo = Memory::Alloc<InternalContextInfo>();
        }

        if (m_InternalContextInfo->internalInfo == nullptr) {
            m_InternalContextInfo->internalInfo = Memory::Alloc<InternalContext>();
        }
    }

    Context::Context(Window* window, const ContextProperties& contextProperties) : m_InternalContextInfo(nullptr)
    {
        Initialize(window, contextProperties);
    }

    Context::~Context(){
    }

    bool Context::SetWindow(Window* window)
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) {
            return false;
        }
        return m_InternalContextInfo->internalInfo->SetWindow(window);
    }

    bool Context::SetProperties(const ContextProperties& properties)
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) {
            return false;
        }
        return m_InternalContextInfo->internalInfo->SetProperties(properties);
    }

    bool Context::Initialize()
    {
        if (m_InternalContextInfo == nullptr) {
            m_InternalContextInfo = Memory::Alloc<InternalContextInfo>();
            if (m_InternalContextInfo == nullptr) return false;
        }

        if (m_InternalContextInfo->internalInfo == nullptr) {
            m_InternalContextInfo->internalInfo = Memory::Alloc<InternalContext>();
            if (m_InternalContextInfo->internalInfo == nullptr) return false;
        }
        if (m_InternalContextInfo->isLoad == false) {
            m_InternalContextInfo->isLoad = m_InternalContextInfo->internalInfo->Initialize();
        }
        return m_InternalContextInfo->isLoad;
    }

    bool Context::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_InternalContextInfo == nullptr) {
            m_InternalContextInfo = Memory::Alloc<InternalContextInfo>();
            if (m_InternalContextInfo == nullptr) return false;
        }

        if (m_InternalContextInfo->internalInfo == nullptr){
            m_InternalContextInfo->internalInfo = Memory::Alloc<InternalContext>();
            if (m_InternalContextInfo->internalInfo == nullptr) return false;
        }

        if (m_InternalContextInfo->isLoad == false) {
            m_InternalContextInfo->isLoad = m_InternalContextInfo->internalInfo->Initialize(window, contextProperties);
        }
        return m_InternalContextInfo->isLoad;
    }

    bool Context::Deinitialize()
    {
        if (m_InternalContextInfo == nullptr) return false;

        if (m_InternalContextInfo->internalInfo != nullptr) {
            bool deinitialize = m_InternalContextInfo->internalInfo->Deinitialize();

            if (!deinitialize) {
                return false;
            }

            Memory::Reset(m_InternalContextInfo->internalInfo);
        }
        m_InternalContextInfo->isLoad = false;
        Memory::Reset(m_InternalContextInfo);
        return true;
    }

    bool Context::IsInitialize()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->IsInitialize();
    }

    bool Context::MakeCurrent()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->MakeCurrent();
    }

    bool Context::UnmakeCurrent()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->UnmakeCurrent();
    }

    bool Context::IsCurrent()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->IsCurrent();
    }

    bool Context::EnableVSync()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->EnableVSync();
    }

    bool Context::DisableVSync()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->DisableVSync();
    }

    bool Context::Present()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->Present();
    }

    bool Context::Swapchaine()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return false;
        return m_InternalContextInfo->internalInfo->Swapchaine();
    }

    const GraphicsInfos& Context::GetGraphicsInfo()
    {
        static const GraphicsInfos graphicInfos = {}; 
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return graphicInfos;
        return m_InternalContextInfo->internalInfo->GetGraphicsInfo();
    }

    Window* Context::GetWindow()
    {
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return nullptr;
        return m_InternalContextInfo->internalInfo->GetWindow();
    }

    InternalContextInfo* Context::GetInternal()
    {
        if (m_InternalContextInfo == nullptr) return nullptr;
        return m_InternalContextInfo.get();
    }

    const ContextProperties& Context::GetProperties()
    {
        static const ContextProperties contextProperties = {}; 
        if (m_InternalContextInfo == nullptr || m_InternalContextInfo->internalInfo == nullptr) return contextProperties;
        return m_InternalContextInfo->internalInfo->GetProperties();
    }

}    // namespace nkentseu