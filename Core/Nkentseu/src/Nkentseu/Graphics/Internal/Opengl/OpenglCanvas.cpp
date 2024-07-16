//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 06:02:28 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglCanvas.h"
#include <Logger/Formatter.h>

#include "Nkentseu/Core/Window.h"

namespace nkentseu {

    using namespace maths;
    
    OpenglCanvas::OpenglCanvas(const Memory::Shared<Context>& context) : m_Context(Memory::SharedCast<OpenglContext>(context)) {
        Initialize();
    }

    OpenglCanvas::~OpenglCanvas() {
    }

    void OpenglCanvas::Destroy()
    {
        if (m_VertexArray) m_VertexArray->Destroy();
        if (m_IndexBuffer) m_IndexBuffer->Destroy();
        if (m_VertexBuffer) m_VertexBuffer->Destroy();
        if (m_Shader) m_Shader->Destroy();
    }

    maths::Vector2f OpenglCanvas::GetSize()
    {
        return m_Size;
    }

    void OpenglCanvas::Initialize() {
        CreateResources();
    }

    void OpenglCanvas::CreateResources() {
        shaderInputLayout = Memory::SharedCast<OpenglShaderInputLayout>(ShaderInputLayout::Create(m_Context));

        if (shaderInputLayout != nullptr) {
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aPos", ShaderInternalType::Enum::Float2, 0));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aColor", ShaderInternalType::Enum::Float4, 1));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aTexCoord", ShaderInternalType::Enum::Float2, 2));

            //shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CanvasTransform", ShaderStage::Enum::Vertex, BufferUsageType::StaticDraw, sizeof(CanvasTransform), 0, 10));
            shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CanvasCamera", ShaderStage::Enum::Vertex, BufferUsageType::Enum::StaticDraw, sizeof(CanvasCamera), 0, 0, 1));
            //shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CanvasMaterial", ShaderStage::Enum::Fragment, BufferUsageType::StaticDraw, sizeof(CanvasMaterial), 2, 10));

            shaderInputLayout->pushConstantInput.AddAttribute(PushConstantInputAttribute("CanvasTransform", ShaderStage::Enum::Vertex, sizeof(CanvasTransform)));
            shaderInputLayout->pushConstantInput.AddAttribute(PushConstantInputAttribute("CanvasMaterial", ShaderStage::Enum::Fragment, sizeof(CanvasMaterial)));

            //shaderInputLayout->samplerInput.AddAttribute(SamplerInputAttribute("CanvasMaterial", 0, ShaderStage::Fragment, SamplerType::CombineImage));

            if (!shaderInputLayout->Initialize()) {
                Log_nts.Error("linitialisation des input shader ont echouer");
            }
        }

        ShaderFilePathLayout shaderFilesLayout({
            {"Resources/shaders/canvas.vert.glsl", ShaderStage::Enum::Vertex},
            {"Resources/shaders/canvas.frag.glsl", ShaderStage::Enum::Fragment},
        });

        m_Shader = Memory::Alloc<OpenglShader>(m_Context);
        if (m_Shader != nullptr){
            if (!m_Shader->LoadFromFile(shaderFilesLayout, shaderInputLayout)) {
                Memory::Reset(m_Shader);
            }
        }
        if (m_Shader == nullptr) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas shader");
            return;
        }

        m_UniformBuffer = Memory::SharedCast<OpenglUniformBuffer>(UniformBuffer::Create(m_Context, shaderInputLayout, m_Shader, {"CanvasCamera"}));
        if (m_UniformBuffer == nullptr) {
            Log_nts.Error("Cannot create uniform buffer");
        }//*/

        uint32 size = MAX_VERTICES;
        m_VertexBuffer = Memory::SharedCast<OpenglVertexBuffer>(VertexBuffer::Create(m_Context, shaderInputLayout, BufferDataUsage::DynamicDraw, nullptr, size));
        if (!m_VertexBuffer) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas vertex buffer");
        }

        size = MAX_INDICES;
        m_IndexBuffer = Memory::SharedCast<OpenglIndexBuffer>(IndexBuffer::Create(m_Context, BufferDataUsage::DynamicDraw, DrawIndexType::UInt32, nullptr, size));
        if (!m_IndexBuffer) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas index buffer");
        }

        m_VertexArray = Memory::SharedCast<OpenglVertexArray>(VertexArray::Create(m_Context, shaderInputLayout));
        if (!m_VertexArray) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas vertex array");
        }
        else {
            m_VertexArray->SetVertexBuffer(m_VertexBuffer);
            m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        }
    }

    void OpenglCanvas::Flush()
    {
        if (m_Vertices.empty() || m_Indices.empty() || m_CanvasCommands.empty()) {
            return;
        }
        OpenGLResult result;
        bool first = true;
        Vector2f size = m_Context->GetWindow()->GetSize();

        glCheckError(first, result, glDisable(GL_DEPTH_TEST), "cannot disable depth test");

        if (m_ScissorEnable) {
            glCheckError(first, result, glEnable(GL_SCISSOR_TEST), "cannot enable scissor test");
        }

        bool render_evalable = true;

        if ((!m_RenderEnable && m_Shader == nullptr) || !m_Shader->Bind()) {
            render_evalable = false;
        }

        if (render_evalable) {
            if (m_UniformBuffer != nullptr) {
                float32 distance_to_screen = 1.0f;
                CanvasCamera cameraBuffer{};

                cameraBuffer.view = matrix4f::Identity();
                //cameraBuffer.proj = matrix4f::Orthogonal(size.width, size.height, 0.1f, 100.0f);
                //cameraBuffer.proj = matrix4f::Orthogonal(size, -1.f, 1.0f);
                cameraBuffer.proj = matrix4f::Orthogonal(Vector2f(0, size.height), Vector2f(size.width, 0), -1.f, 1.0f);
                m_UniformBuffer->SetData("CanvasCamera", &cameraBuffer, sizeof(CanvasCamera));
                m_UniformBuffer->Bind();
                //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
            }

            if (m_VertexArray == nullptr || m_VertexArray->GetVertexBuffer() == nullptr || m_VertexArray->GetIndexBuffer() == nullptr) {
                return;
            }

            m_VertexArray->GetVertexBuffer()->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex2D));
            m_VertexArray->GetIndexBuffer()->SetData(m_Indices.data(), m_Indices.size() * sizeof(uint32));

            m_VertexArray->BindIndex();
        }

        uint32 offset = 0;
        for (auto it = m_CanvasCommands.begin(); it != m_CanvasCommands.end(); ++it) {
            const auto command = *it;

            if (auto viewportCommand = Memory::SharedCast<CanvasViewportCommand>(command)) {
                // Apply viewport
                Viewport viewport = viewportCommand->viewport;
                glCheckError(first, result, glViewport(viewport.x, viewport.y, viewport.width, viewport.height), "cannot set command viewport in canvas fulsh");
            } else if (auto scissorCommand = Memory::SharedCast<CanvasScissorCommand>(command)) {
                // Apply scissort
                Scissor scissor = scissorCommand->scissor;
                glCheckError(first, result, glScissor(scissor.x, scissor.y, scissor.width, scissor.height), "cannot set command scissor in canvas fulsh");
            } else if (render_evalable){
                if (auto renderCommand = Memory::SharedCast<CanvasRenderCommand>(command)) {
                    if (renderCommand->texture) {
                        renderCommand->texture->Bind(0);
                    }

                    if (shaderInputLayout != nullptr) {
                        CanvasTransform transform;
                        transform.model = renderCommand->transform;
                        shaderInputLayout->UpdatePushConstant("CanvasTransform", &transform, sizeof(CanvasTransform), m_Shader);

                        CanvasMaterial material;
                        material.useColor = 1 << 1;
                        material.useTexture |= renderCommand->texture != nullptr ? 1 << 2 : 0;
                        shaderInputLayout->UpdatePushConstant("CanvasMaterial", &material, sizeof(CanvasMaterial), m_Shader);
                    }
                    m_VertexArray->DrawIndex(renderCommand->primitive, offset, renderCommand->indexCount);
                    offset += renderCommand->indexCount;
                }
            }
        }

        if (!m_VertexArray->UnbindIndex()) {
        }

        if (render_evalable && !m_Shader->Unbind()) {
        }

        if (m_ScissorEnable) {
            glCheckError(first, result, glDisable(GL_SCISSOR_TEST), "cannot disable scissor test");

            if (!result.success) {
            }
        }

        glCheckError(first, result, glEnable(GL_DEPTH_TEST), "cannot enable depth test");
        if (!result.success) {
        }
    }

}  //  nkentseu