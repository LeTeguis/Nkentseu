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

    Renderer::Renderer() : m_InternalRenderer(nullptr) {
        if (m_InternalRenderer == nullptr) {
            m_InternalRenderer = Memory::Alloc<InternalRenderer>();
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
        if (m_InternalRenderer == nullptr) {
            m_InternalRenderer = Memory::Alloc<InternalRenderer>();
            if (m_InternalRenderer == nullptr) {
                return false;
            }
        }
        if (context == nullptr) {
            return false;
        }

        return m_IsInitialsed = m_InternalRenderer->Initialize(context);
    }

    bool Renderer::Deinitialize()
    {
        if (!IsValideInternal()) return false;
        return m_InternalRenderer->Deinitialize();
    }

    bool Renderer::Clear(const Color& color)
    {
        if (!IsValideInternal()) {
            return false;
        }
        return m_InternalRenderer->Clear(color);
    }

    bool Renderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a)
    {
        if (!IsValideInternal()) return false;
        return m_InternalRenderer->Clear(r, g, b, a);
    }

    bool Renderer::SetActiveShader(Memory::Shared<Shader> shader)
    {
        if (!IsValideInternal() || shader == nullptr) return false;
        return m_InternalRenderer->SetActiveShader(shader);
    }

    bool Renderer::UnsetActiveShader()
    {
        if (!IsValideInternal()) return false;
        return m_InternalRenderer->UnsetActiveShader();
    }

    bool Renderer::Present()
    {
        if (!IsValideInternal()) return false;
        return m_InternalRenderer->Present();
    }

    bool Renderer::Swapbuffer()
    {
        if (!IsValideInternal()) return false;
        return m_InternalRenderer->Swapbuffer();
    }

    bool Renderer::Resize(const Vector2u& size)
    {
        if (!IsValideInternal()) return false;
        return m_InternalRenderer->Resize(size);
    }

    bool Renderer::IsValideInternal()
    {
        return !(m_InternalRenderer == nullptr  || !m_IsInitialsed);
    }

}    // namespace nkentseu