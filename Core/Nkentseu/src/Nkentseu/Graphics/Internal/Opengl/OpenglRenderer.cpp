//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:13:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglRenderer.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include <Nkentseu/Platform/Window/Windows/WGLContext.h>
#elif defined NKENTSEU_PLATFORM_LINUX
#include <GL/glext.h>
#if defined(NKENTSEU_PLATFORM_LINUX_XCB)
#include <Nkentseu/Platform/Window/Linux/XCB/XGLContext.h>
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
#include <Nkentseu/Platform/Window/Linux/XLIB/XGLContext.h>
#endif
#endif

#include <Nkentseu/Core/Window.h>
#include <Nkentseu/Graphics/Context.h>
#include <Nkentseu/Graphics/Shader.h>
#include <Nkentseu/Graphics/VertexArray.h>
#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Core/NkentseuLogger.h>
#include <Nkentseu/Event/EventBroker.h>
#include <Nkentseu/Event/EventFilter.h>

#include "OpenglShader.h"
#include "OpenglContext.h"
#include "OpenglRenderer.h"
#include "OpenglVertexArray.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"
#include "OpenGLUtils.h"

#include <glad/gl.h>


namespace nkentseu {
    OpenglRenderer::OpenglRenderer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<OpenglContext>(context)) {
    }

    OpenglRenderer::~OpenglRenderer(){
    }

    Memory::Shared<Context> OpenglRenderer::GetContext()
    {
        return m_Context;
    }

    bool OpenglRenderer::Initialize()
    {
        if (m_Context == nullptr) {
            return false;
        }

        if (!m_Context->IsInitialize()) {
            return m_Context->Initialize();
        }

        bool makecurrent = true;

        if (!m_Context->GetNative()->IsCurrent()) {
            makecurrent = m_Context->GetNative()->MakeCurrent();
        }

        EventTrack.AddObserver(REGISTER_CLIENT_EVENT(OpenglRenderer::OnEvent));
        return makecurrent;
    }

    bool OpenglRenderer::Deinitialize()
    {
        if (m_Context == nullptr || !m_Context->IsInitialize()) {
            return false;
        }
        bool makecurrent = false;
        if (m_Context->GetNative()->IsCurrent()) {
            makecurrent = m_Context->GetNative()->UnmakeCurrent();
        }
        EventTrack.RemoveObserver(REGISTER_CLIENT_EVENT(OpenglRenderer::OnEvent));
        return false;
    }

    void OpenglRenderer::OnEvent(Event& event) {
        if (m_Context == nullptr || !m_Context->IsInitialize()) return;

        EventBroker broker(event);

        broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(OpenglRenderer::OnWindowResizedEvent));
    }

    bool OpenglRenderer::OnWindowResizedEvent(WindowResizedEvent& event)
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

    bool OpenglRenderer::Begin(const Color& color)
    {
        m_IsPrepare = false;
        if (!CanRender() || !m_Context->GetNative()->IsCurrent()) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glClearColor(color.Rf(), color.Gf(), color.Bf(), color.Af()), "cannot clear color");
        if (result.success) {
            static bool enableDepth = false;
            static uint32 clear_infos = 0;

            if (!enableDepth) {
                glEnable(GL_DEPTH_TEST);
                enableDepth = true;
            }

            if (enableDepth) clear_infos |= GL_DEPTH_BUFFER_BIT;
            clear_infos |= GL_COLOR_BUFFER_BIT;
            clear_infos |= GL_STENCIL_BUFFER_BIT;

            glCheckError(first, result, glClear(clear_infos), "cannot clear");
            if (result.success) {
                m_IsPrepare = true;
            }
        }
        return result.success;
    }

    bool OpenglRenderer::Begin(uint8 r, uint8 g, uint8 b, uint8 a)
    {
        return Begin(Color(r, g, b, a));
    }

    bool OpenglRenderer::DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode)
    {
        return PolygonMode(contentMode) && CullMode(mode);
    }

    bool OpenglRenderer::PolygonMode(PolygonModeType::Code mode)
    {
        if (!CanRender() || !m_IsPrepare) {
            return false;
        }
        OpenGLResult result;
        bool first = true;
        glCheckError(first, result, glPolygonMode(GLConvert::CullModeType(CullModeType::FrontBack), GLConvert::PolygonModeType(mode)), "cannot change polygon mode");
        return result.success;
    }

    bool OpenglRenderer::CullMode(CullModeType::Code mode)
    {
        if (!CanRender() || !m_IsPrepare) {
            return false;
        }

        static bool cullFaceIsEnable = false;
        OpenGLResult result;
        bool first = true;

        if (mode != CullModeType::NoCull) {
            if (!cullFaceIsEnable) {
                glCheckError(first, result, glEnable(GL_CULL_FACE), "cannot enable cull face mode");
                cullFaceIsEnable = true;
            }
            glCheckError(first, result, glCullFace(GLConvert::CullModeType(mode)), "cannot change  cull face mode");
        }
        else {
            if (cullFaceIsEnable) {
                glCheckError(first, result, glDisable(GL_CULL_FACE), "cannot disable cull face mode");
                cullFaceIsEnable = false;
            }
        }
        return result.success;
    }

    bool OpenglRenderer::FrontFaceMode(FrontFaceType::Code mode)
    {
        if (!CanRender() || !m_IsPrepare) {
            return false;
        }
        OpenGLResult result;
        bool first = true;

        if (mode == FrontFaceType::Clockwise) {
            glCheckError(first, result, glFrontFace(GL_CW), "cannot set front face clock wise");
        }
        else if (mode == FrontFaceType::CounterClockwise) {
            glCheckError(first, result, glFrontFace(GL_CCW), "cannot set front face cunter clock wise");
        }
        return result.success;
    }

    bool OpenglRenderer::PrimitiveTopologyMode(PrimitiveTopologyType::Code mode)
    {
        if (!CanRender() || !m_IsPrepare) {
            return false;
        }
        return true;
    }

    bool OpenglRenderer::ScissorMode(const Vector2i& offset, const Vector2u& extend)
    {
        if (!CanRender() || !m_IsPrepare) {
            return false;
        }
        return true;
    }

    bool OpenglRenderer::ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth)
    {
        if (!CanRender() || !m_IsPrepare) {
            return false;
        }
        return true;
    }

    bool OpenglRenderer::Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex)
    {
        if (!CanRender() || m_CurrentShader == nullptr || drawVertex == DrawVertexType::NotDefine) {
            return false;
        }

        if (vertexArray == nullptr) {
            return false;
        }

        Memory::Shared<OpenglVertexArray> vVertexArray = Memory::SharedCast<OpenglVertexArray>(vertexArray);

        if (vVertexArray == nullptr || !vVertexArray->Bind()) {
            return false;
        }

        Memory::Shared<OpenglVertexBuffer> vertexBuffer = Memory::SharedCast<OpenglVertexBuffer>(vVertexArray->GetVertexBuffer());
        Memory::Shared<OpenglIndexBuffer> indexBuffer = Memory::SharedCast<OpenglIndexBuffer>(vVertexArray->GetIndexBuffer());
        
        if (vertexBuffer != nullptr && !vertexBuffer->Bind() && vVertexArray->Leng() == 0) {
            vVertexArray->Unbind();
            return false;
        }

        if (indexBuffer != nullptr && !indexBuffer->Bind()) {
            vVertexArray->Unbind();
            return false;
        }

        uint32  vertexCount = vVertexArray->Leng();

        uint32 vertexType = GLConvert::VertexType(drawVertex);
        uint32 vertices_per_type = GLConvert::VerticesPerType(vertexType);

        OpenGLResult result;
        bool first = true;

        if (indexBuffer != nullptr) {
            glCheckError(first, result, glDrawElements(vertexType, indexBuffer->Leng(), GLConvert::IndexType(indexBuffer->GetIndexType()), 0), "cannot draw elements");
        }
        else {
            Log_nts.Debug();
            glCheckError(first, result, glDrawArrays(vertexType, 0, vertexCount), "cannot draw arrays");
        }

        return vVertexArray->Unbind() && result.success;
    }

    bool OpenglRenderer::End()
    {
        if (!CanRender() || !m_Context->IsCurrent()) return false;
        bool swap = m_Context->Swapchaine();

        if (swap) {
            OpenGLResult result;
            bool first = true;

            glCheckError(first, result, glFlush(), "cannot flus");
            swap = result.success;
        }
        return swap;
    }

    bool OpenglRenderer::BindShader(Memory::Shared<Shader> shader)
    {
        if (!CanRender() || shader == nullptr) {
            return false;
        }

        Memory::Shared<OpenglShader> sh = Memory::SharedCast<OpenglShader>(shader);

        if (m_CurrentShader != sh) {
            if (m_CurrentShader != nullptr) {
                bool unbind = m_CurrentShader->Unbind();

                if (!unbind) {
                    return false;
                }
            }
            m_CurrentShader = sh;
        }
        return m_CurrentShader->Bind();
    }

    bool OpenglRenderer::UnbindShader()
    {
        if (!CanRender() || m_CurrentShader == nullptr) return false;

        bool unbind = m_CurrentShader->Unbind();
        m_CurrentShader = nullptr;

        if (!unbind) {
            return false;
        }
        return true;
    }

    bool OpenglRenderer::BindUniform(const std::string& name, void* data, usize size)
    {
        if (!CanRender() || m_CurrentShader == nullptr) return false;
        return m_CurrentShader->UseUniform(name, data, size, 0);
    }

    bool OpenglRenderer::UnbindUniform(const std::string& name)
    {
        if (!CanRender() || m_CurrentShader == nullptr) return false;
        return false;
    }

    bool OpenglRenderer::CanRender()
    {
        if (m_Context == nullptr) {
            return false;
        }
        if (m_Context->GetNative() == nullptr) {
            return false;
        }
        if (!m_Context->IsInitialize()) {
            return false;
        }
        if (m_Context->GetWindow()->GetSize() == Vector2u()) {
            return false;
        }
        return true;
    }
}    // namespace nkentseu