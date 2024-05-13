//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:13:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalRenderer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Nkentseu/Graphics/Context.h>
#include "Nkentseu/Graphics/Color.h"
#include <glad/gl.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS
//#include "Nkentseu/Graphics/Internal/Opengl/InternalContext.h"
//#include <glad/wgl.h>
#elif defined NKENTSEU_PLATFORM_LINUX
//#include <glad/glx.h>
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported this graphics context"
#endif

#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

    InternalRenderer::InternalRenderer() : m_Context(nullptr){
    }

    InternalRenderer::~InternalRenderer(){
    }

    bool InternalRenderer::Initialize(Context* context)
    {
        if (context == nullptr) {
            return false;
        }
        m_Context = context;
        if (!m_Context->IsInitialize()) {
            return m_Context->Initialize();
        }
        if (!m_Context->IsCurrent()) {
            return m_Context->MakeCurrent();
        }
        return true;
    }

    bool InternalRenderer::Deinitialize()
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        if (m_Context->IsCurrent()) {
            return m_Context->UnmakeCurrent();
        }
        return false;
    }

    bool InternalRenderer::Clear(const Color& color)
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        if (m_Context->IsCurrent()) {
            glClearColor(color.Rf(), color.Gf(), color.Bf(), color.Af());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // Log_nts.Debug();
            return true;
        }
        return false;
    }

    bool InternalRenderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a)
    {
        return Clear(Color(r, g, b, a));
    }

    bool InternalRenderer::Present()
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        if (m_Context->IsCurrent()) {
            bool swap = m_Context->Present();

            if (swap) {
                glFlush();
                return true;
            }
        }
        return false;
    }

    bool InternalRenderer::Swapbuffer()
    {
        return Present();
    }

    bool InternalRenderer::Resize(const Vector2u& size)
    {
        glViewport(0, 0, size.width, size.height);
        return true;
    }

}    // namespace nkentseu

#endif