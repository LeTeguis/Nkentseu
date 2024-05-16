//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:13:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalRenderer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Nkentseu/Graphics/Context.h>
#include <Nkentseu/Graphics/Shader.h>
#include <Nkentseu/Graphics/Buffer.h>
#include <Nkentseu/Graphics/VertexArray.h>
#include "InternalBuffer.h"
#include "InternalShader.h"
#include "InternalVertexArray.h"

#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Core/NkentseuLogger.h>

#include <glad/gl.h>



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
            return true;
        }
        return false;
    }

    bool InternalRenderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a)
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        return Clear(Color(r, g, b, a));
    }

    bool InternalRenderer::DrawVertexBuffer(Memory::Shared<VertexBuffer> vertex)
    {
        if (m_Context == nullptr || !m_Context->IsInitialize() || m_CurrentShader == nullptr) {
            return false;
        }
        if (!m_CurrentShader->GetInternal()->Bind() || !vertex->GetInternal()->GetVertexArray()->GetInternal()->Bind() || !vertex->GetInternal()->GetIndexBuffer(0)->GetInternal()->Bind()) {
            return false;
        }
        glDrawElements(GL_TRIANGLES, vertex->GetInternal()->GetIndexBuffer(0)->GetInternal()->GetLength(), GL_UNSIGNED_INT, vertex->GetInternal()->GetIndexBuffer(0)->GetInternal()->GetIndices().data());
        return true;
    }

    bool InternalRenderer::DrawIndexBuffer(Memory::Shared<IndexBuffer> index)
    {
        if (m_Context == nullptr || !m_Context->IsInitialize() || m_CurrentShader == nullptr) {
            return false;
        }
        return true;
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

    bool InternalRenderer::SetActiveShader(Memory::Shared<Shader> shader)
    {
        if (shader == nullptr || shader->GetInternal() == nullptr) {
            return false;
        }

        if (m_CurrentShader != nullptr) {
            bool unbind = m_CurrentShader->GetInternal()->Unbind();

            if (!unbind) {
                return false;
            }
        }
        m_CurrentShader = shader;
        return m_CurrentShader->GetInternal()->Bind();
    }

    bool InternalRenderer::UnsetActiveShader()
    {
        if (m_CurrentShader != nullptr) {
            bool unbind = m_CurrentShader->GetInternal()->Unbind();

            if (!unbind) {
                return false;
            }
            m_CurrentShader = nullptr;
            return true;
        }
        return false;
    }

    bool InternalRenderer::Resize(const Vector2u& size)
    {
        glViewport(0, 0, size.width, size.height);
        return true;
    }

}    // namespace nkentseu

#endif