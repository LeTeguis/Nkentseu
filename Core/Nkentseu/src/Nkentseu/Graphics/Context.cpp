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

namespace nkentseu {

    Context::Context() : m_InternalContextInfo(nullptr) {
        m_InternalContextInfo = Memory::Alloc<InternalContextInfo>();

        if (m_InternalContextInfo != nullptr) {
            m_InternalContextInfo->internalInfo = Memory::Alloc<InternalContext>();
            m_InternalContextInfo->isLoad = false;
        }
    }

    Context::Context(Window* window, const ContextProperties& contextProperties) : m_InternalContextInfo(nullptr)
    {
        m_InternalContextInfo = Memory::Alloc<InternalContextInfo>();

        Initialize(window, contextProperties);

        if (m_InternalContextInfo != nullptr) {
            m_InternalContextInfo->isLoad = false;
        }
    }

    Context::~Context(){
    }

    bool Context::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        if (m_InternalContextInfo == nullptr) return false;
        m_InternalContextInfo->isLoad = m_InternalContextInfo->internalInfo->Initialize(window, contextProperties);
        return m_InternalContextInfo->isLoad;
    }

    InternalContextInfo* Context::GetInternal()
    {
        return m_InternalContextInfo.get();
    }

}    // namespace nkentseu