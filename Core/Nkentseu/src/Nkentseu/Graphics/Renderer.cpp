//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:12:38 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Renderer.h"

#ifdef NKENTSEU_GRAPHICS_API_SOFTWARE
#include "Internal/Software/InternalRenderer.h"
#elif defined(NKENTSEU_GRAPHICS_API_OPENGL)
#include "Internal/Opengl/InternalRenderer.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalRenderer.h"
#elif defined(NKENTSEU_GRAPHICS_API_DIRECTX11)
#include "Internal/DirectX11/InternalRenderer.h"
#elif defined(NKENTSEU_GRAPHICS_API_DIRECTX12)
#include "Internal/DirectX12/InternalRenderer.h"
#elif defined(NKENTSEU_GRAPHICS_API_METAL)
#include "Internal/Metal/InternalRenderer.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported this graphics context"
#endif

#include <Nkentseu/Platform/Internal/InternalMemory.h>
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

    Renderer::Renderer() : m_InternalRendererInfo(nullptr) {
        if (m_InternalRendererInfo == nullptr) {
            m_InternalRendererInfo = Memory::Alloc<InternalRendererInfo>();
        }

        if (m_InternalRendererInfo->internalInfo == nullptr) {
            m_InternalRendererInfo->internalInfo = Memory::Alloc<InternalRenderer>();
        }
    }

    Renderer::Renderer(Context* context)
    {
        Initialize(context);
    }

    Renderer::~Renderer(){
    }

    bool Renderer::Initialize(Context* context)
    {
        if (m_InternalRendererInfo == nullptr) {
            m_InternalRendererInfo = Memory::Alloc<InternalRendererInfo>();
            if (m_InternalRendererInfo == nullptr) {
                return false;
            }
        }

        if (m_InternalRendererInfo->internalInfo == nullptr) {
            m_InternalRendererInfo->internalInfo = Memory::Alloc<InternalRenderer>();
            if (m_InternalRendererInfo->internalInfo == nullptr) {
                return false;
            }
        }
        if (context == nullptr) {
            return false;
        }

        if (m_InternalRendererInfo->isLoad == false) {
            m_InternalRendererInfo->isLoad = m_InternalRendererInfo->internalInfo->Initialize(context);
        }
        return m_InternalRendererInfo->isLoad;
    }

    bool Renderer::Deinitialize()
    {
        if (m_InternalRendererInfo == nullptr || m_InternalRendererInfo->internalInfo == nullptr || !m_InternalRendererInfo->isLoad) return false;
        return m_InternalRendererInfo->internalInfo->Deinitialize();
    }

    bool Renderer::Clear(const Color& color)
    {
        if (m_InternalRendererInfo == nullptr || m_InternalRendererInfo->internalInfo == nullptr || !m_InternalRendererInfo->isLoad) {
            return false;
        }
        return m_InternalRendererInfo->internalInfo->Clear(color);
    }

    bool Renderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a)
    {
        if (m_InternalRendererInfo == nullptr || m_InternalRendererInfo->internalInfo == nullptr || !m_InternalRendererInfo->isLoad) return false;
        return m_InternalRendererInfo->internalInfo->Clear(r, g, b, a);
    }

    bool Renderer::Present()
    {
        if (m_InternalRendererInfo == nullptr || m_InternalRendererInfo->internalInfo == nullptr || !m_InternalRendererInfo->isLoad) return false;
        return m_InternalRendererInfo->internalInfo->Present();
    }

    bool Renderer::Swapbuffer()
    {
        if (m_InternalRendererInfo == nullptr || m_InternalRendererInfo->internalInfo == nullptr || !m_InternalRendererInfo->isLoad) return false;
        return m_InternalRendererInfo->internalInfo->Swapbuffer();
    }

    bool Renderer::Resize(const Vector2u& size)
    {
        if (m_InternalRendererInfo == nullptr || m_InternalRendererInfo->internalInfo == nullptr || !m_InternalRendererInfo->isLoad) return false;
        return m_InternalRendererInfo->internalInfo->Resize(size);
    }

}    // namespace nkentseu