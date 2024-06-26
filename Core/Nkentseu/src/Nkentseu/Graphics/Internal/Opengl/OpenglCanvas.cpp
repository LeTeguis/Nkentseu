//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-21 at 06:02:28 PM.
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
        std::unordered_map<ShaderType::Code, std::string> shaderFiles;
        shaderFiles[ShaderType::Vertex] = "Resources/shaders/canvas.vert.glsl";
        shaderFiles[ShaderType::Fragment] = "Resources/shaders/canvas.frag.glsl";

        BufferLayout bufferLayout;
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float2, "aPos", 0));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float4, "aColor", 1));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float2, "aTexCoord", 2));
        bufferLayout.CalculateOffsetsAndStride();

        UniformBufferLayout uniformLayout;
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CanvasTransform), 0, "CanvasTransform", ShaderType::Vertex, UniformBufferType::Static, MAX_QUADS));
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CanvasCamera), 1, "CanvasCamera", ShaderType::Vertex, UniformBufferType::Static, 1));
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CanvasMaterial), 2, "CanvasMaterial", ShaderType::Fragment, UniformBufferType::Static, MAX_QUADS));

        ShaderBufferLayout shaderLayout;
        shaderLayout.vertexInput = bufferLayout;
        shaderLayout.uniformBuffer = uniformLayout;
        // Define necessary layouts

        m_Shader = Memory::Alloc<OpenglShader>(m_Context);
        if (m_Shader != nullptr){
            if (!m_Shader->LoadFromFile(shaderFiles, shaderLayout)) {
                Memory::Reset(m_Shader);
            }
        }
        if (m_Shader == nullptr) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas shader");
            return;
        }

        m_UniformBuffer = Memory::SharedCast<OpenglUniformBuffer>(UniformBuffer::Create(m_Context, m_Shader, uniformLayout));
        if (m_UniformBuffer == nullptr) {
            Log_nts.Error("Cannot create uniform buffer");
        }//*/

        uint32 size = MAX_VERTICES;
            m_VertexBuffer = Memory::SharedCast<OpenglVertexBuffer>(VertexBuffer::Create(m_Context, BufferDataUsage::DynamicDraw, nullptr, size, bufferLayout));
        if (!m_VertexBuffer) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas vertex buffer");
        }

        size = MAX_INDICES;
        m_IndexBuffer = Memory::SharedCast<OpenglIndexBuffer>(IndexBuffer::Create(m_Context, BufferDataUsage::DynamicDraw, DrawIndexType::UInt32, nullptr, size));
        if (!m_IndexBuffer) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas index buffer");
        }

        m_VertexArray = Memory::SharedCast<OpenglVertexArray>(VertexArray::Create(m_Context));
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

        if (m_ScissorEnable) {
            glCheckError(first, result, glEnable(GL_SCISSOR_TEST), "cannot enable scissor test");
        }

        bool render_evalable = true;

        if (m_RenderEnable && m_Shader == nullptr || !m_Shader->Bind()) {
            render_evalable = false;
        }

        if (render_evalable) {
            if (m_UniformBuffer != nullptr) {
                float32 distance_to_screen = 1.0f;
                CanvasCamera cameraBuffer{};

                cameraBuffer.view = matrix4f::Identity();
                cameraBuffer.proj = matrix4f::Orthogonal(size.width, size.height, 0.1f, 100.0f);
                m_UniformBuffer->SetData("CanvasCamera", &cameraBuffer, sizeof(CanvasCamera));
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
                        renderCommand->texture->Bind();
                    }

                    if (m_UniformBuffer != nullptr) {
                        CanvasTransform transform;
                        transform.model = renderCommand->transform;
                        m_UniformBuffer->SetData("CanvasTransform", &transform, sizeof(CanvasTransform));

                        CanvasMaterial material;
                        material.useColor = true;
                        material.useTexture = renderCommand->texture != nullptr;
                        m_UniformBuffer->SetData("CanvasMaterial", &material, sizeof(CanvasMaterial));

                        m_UniformBuffer->Bind();
                    }

                    uint32 primitive = GLConvert::GetPrimitiveType(renderCommand->primitive);

                    glCheckError(first, result, glDrawElements(primitive, renderCommand->indexCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(uint32))), "cannot draw elements for canvas");
                    offset += renderCommand->indexCount;
                }
            }
        }

        m_VertexArray->UnbindIndex();

        if (render_evalable) {
            m_Shader->Unbind();
        }

        if (m_ScissorEnable) {
            glCheckError(first, result, glDisable(GL_SCISSOR_TEST), "cannot disable scissor test");
        }
    }

}  //  nkentseu