//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:13:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalRenderer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Nkentseu/Graphics/Context.h>
#include <Nkentseu/Graphics/Shader.h>
#include "InternalShader.h"

#include <Nkentseu/Graphics/Buffer.h>
#include <Nkentseu/Graphics/VertexArray.h>

#include "InternalBuffer.h"
#include "InternalVertexArray.h"
#include "InternalVertexBuffer.h"
#include "InternalIndexBuffer.h"
#include "OpenGLUtils.h"

#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Core/NkentseuLogger.h>

#include <glad/gl.h>
#include "InternalContext.h"
#include "InternalRenderer.h"
#include <Nkentseu/Event/EventBroker.h>
#include <Nkentseu/Event/EventFilter.h>



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
        bool makecurrent = true;
        if (!m_Context->GetInternal()->IsCurrent()) {
            makecurrent = m_Context->GetInternal()->MakeCurrent();
        }
        EventTrack.AddObserver(REGISTER_CLIENT_EVENT(InternalRenderer::OnEvent));
        return makecurrent;
    }

    bool InternalRenderer::Deinitialize()
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        bool makecurrent = false;
        if (m_Context->GetInternal()->IsCurrent()) {
            makecurrent = m_Context->GetInternal()->UnmakeCurrent();
        }
        EventTrack.RemoveObserver(REGISTER_CLIENT_EVENT(InternalRenderer::OnEvent));
        return false;
    }

    void InternalRenderer::OnEvent(Event& event) {
        if (m_Context == nullptr || !m_Context->IsInitialize()) return;

        EventBroker broker(event);

        broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(InternalRenderer::OnWindowResizedEvent));
    }

    bool InternalRenderer::OnWindowResizedEvent(WindowResizedEvent& event)
    {
        if (m_Context == nullptr || m_Context->GetWindow() == nullptr) {
            return false;
        }
        OpenGLResult result;
        bool first = true;

        Vector2u size = m_Context->GetWindow()->ConvertPixelToDpi(event.GetSize());
        glCheckError(first, result, glViewport(0, 0, size.width, size.height), "cannot change viewport");

        return true;
    }

    bool InternalRenderer::Clear(const Color& color)
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        if (m_Context->GetInternal()->IsCurrent()) {

            OpenGLResult result;
            bool first = true;

            glCheckError(first, result, glClearColor(color.Rf(), color.Gf(), color.Bf(), color.Af()), "cannot clear color");
            if (result.success) {
                glCheckError(first, result, glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT), "cannot clear");
                return result.success;
            }
            return false;
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

    bool InternalRenderer::DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode)
    {
        if (!CanRender()) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glPolygonMode(GLConvert::CullModeType(CullModeType::FrontBack), GLConvert::PolygonModeType(contentMode)), "cannot change polygon mode");

        glCullFace(GLConvert::CullModeType(mode));
        //glFrontFace(GL_CCW); // GL_CW
        return result.success;
    }

    bool InternalRenderer::PolygonMode(PolygonModeType::Code mode)
    {
        return false;
    }

    bool InternalRenderer::CullMode(CullModeType::Code mode)
    {
        return false;
    }

    bool InternalRenderer::FrontFaceMode(FrontFaceType::Code mode)
    {
        return false;
    }

    bool InternalRenderer::PrimitiveTopologyMode(PrimitiveTopologyType::Code mode)
    {
        return false;
    }

    bool InternalRenderer::ScissorMode(const Vector2i& offset, const Vector2u& extend)
    {
        return false;
    }

    bool InternalRenderer::ViewMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth)
    {
        return false;
    }

    bool InternalRenderer::Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex)
    {
        if (!CanRender() || m_CurrentShader == nullptr || drawVertex == DrawVertexType::NotDefine) {
            return false;
        }
        if (m_CurrentShader->GetInternal() == nullptr || !m_CurrentShader->GetInternal()->Bind()) {
            return false;
        }

        if (vertexArray == nullptr) {
            return false;
        }

        InternalVertexArray *internalVertexArray = vertexArray->GetInternal();

        if (internalVertexArray == nullptr) {
            return false;
        }

        InternalVertexBuffer* vertexBuffer = vertexArray->GetInternal()->GetInternalVertexBuffer();
        InternalIndexBuffer* indexBuffer = vertexArray->GetInternal()->GetInternalIndexBuffer();

        if (vertexBuffer == nullptr && internalVertexArray->GetVertexNumber() == 0) {
            return false;
        }

        if (!internalVertexArray->Bind()) {
            return false;
        }


        uint32 vertexType = GLConvert::VertexType(drawVertex);
        uint32 vertices_per_type = GLConvert::VerticesPerType(vertexType);

        if (indexBuffer == nullptr) {
            uint32 count = internalVertexArray->GetVertexNumber();

            if (vertexBuffer != nullptr) {
                if (!vertexBuffer->Bind()) {
                    return false;
                }
                count = vertexBuffer->Leng();
            }

            OpenGLResult result;
            bool first = true;

            glCheckError(first, result, glDrawArrays(vertexType, 0, count), "cannot draw arrays");

            if (!result.success) {
                return false;
            }
        }
        else {
            if (indexBuffer->Bind()) {
                uint32 count = indexBuffer->Leng();

                OpenGLResult result;
                bool first = true;

                glCheckError(first, result, glDrawElements(vertexType, count, GLConvert::IndexType(indexBuffer->GetIndexType()), 0), "cannot draw elements");

                if (!result.success) {
                    return false;
                }
            }
        }

        return internalVertexArray->Unbind();
    }

    bool InternalRenderer::Present()
    {
        if (!CanRender()) return false;
        if (m_Context->GetInternal()->IsCurrent()) {
            bool swap = m_Context->GetInternal()->Present();

            if (swap) {

                OpenGLResult result;
                bool first = true;

                glCheckError(first, result, glFlush(), "cannot flus");
                return result.success;
            }
        }
        return false;
    }

    bool InternalRenderer::Swapbuffer()
    {
        return Present();
    }

    bool InternalRenderer::Prepare()
    {
        m_IsPrepare = false;
        if (!CanRender()) return false;
        m_IsPrepare = true;

        return true;
    }

    bool InternalRenderer::Finalize()
    {
        return Present();
    }

    bool InternalRenderer::UseShader(Memory::Shared<Shader> shader)
    {
        if (!CanRender() || shader == nullptr || shader->GetInternal() == nullptr) {
            return false;
        }

        if (m_CurrentShader != shader) {
            if (m_CurrentShader != nullptr) {
                bool unbind = m_CurrentShader->GetInternal()->Unbind();

                if (!unbind) {
                    return false;
                }
            }
            m_CurrentShader = shader;
        }
        return m_CurrentShader->GetInternal()->Bind();
    }

    bool InternalRenderer::UnuseShader()
    {
        if (!CanRender() || m_CurrentShader == nullptr) return false;

        bool unbind = m_CurrentShader->GetInternal()->Unbind();

        if (!unbind) {
            return false;
        }
        m_CurrentShader = nullptr;
        return true;
    }

    bool InternalRenderer::Resize(const Vector2u& size)
    {
        if (!CanRender() || !m_IsPrepare) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glViewport(0, 0, size.width, size.height), "cannot change viewport");
        return result.success;
    }

    bool InternalRenderer::CanRender()
    {
        if (m_Context == nullptr) {
            return false;
        }
        if (m_Context->GetInternal() == nullptr) {
            return false;
        }
        if (!m_Context->IsInitialize()) {
            return false;
        }
        InternalContext* context = m_Context->GetInternal();
        if (context->GetWindow()->GetSize() == Vector2u()) {
            return false;
        }
        return true;
    }
}    // namespace nkentseu

#endif