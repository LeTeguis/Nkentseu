//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 06:02:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanCanvas.h"
#include <Logger/Formatter.h>

#include "Nkentseu/Core/Window.h"

namespace nkentseu {
    using namespace maths;

    VulkanCanvas::VulkanCanvas(const Memory::Shared<Context>& context)
        : m_Context(Memory::SharedCast<VulkanContext>(context)) {
        Initialize();
    }

    VulkanCanvas::~VulkanCanvas() {
    }

    void VulkanCanvas::Destroy()
    {
        if (m_VertexArray) m_VertexArray->Destroy();
        if (m_IndexBuffer) m_IndexBuffer->Destroy();
        if (m_VertexBuffer) m_VertexBuffer->Destroy();
        if (m_Shader) m_Shader->Destroy();
    }

    maths::Vector2f VulkanCanvas::GetSize()
    {
        return m_Size;
    }

    void VulkanCanvas::Initialize() {
        CreateResources();
    }

    void VulkanCanvas::CreateResources() {
        shaderInputLayout = Memory::SharedCast<VulkanShaderInputLayout>(ShaderInputLayout::Create(m_Context));

        if (shaderInputLayout != nullptr) {
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aPos", ShaderInternalType::Float2, 0));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aColor", ShaderInternalType::Float4, 1));
            shaderInputLayout->vertexInput.AddAttribute(VertexInputAttribute("aTexCoord", ShaderInternalType::Float2, 2));

            //shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CanvasTransform", ShaderStage::Enum::Vertex, BufferUsageType::StaticDraw, sizeof(CanvasTransform), 0, 10));
            shaderInputLayout->uniformInput.AddAttribute(UniformInputAttribute("CanvasCamera", ShaderStage::Enum::Vertex, BufferUsageType::StaticDraw, sizeof(CanvasCamera), 0, 1));
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

        m_Shader = Memory::Alloc<VulkanShader>(m_Context);
        if (m_Shader != nullptr) {
            if (!m_Shader->LoadFromFile(shaderFilesLayout, shaderInputLayout)) {
                Memory::Reset(m_Shader);
            }
        }
        if (m_Shader == nullptr) {
            Log_nts.Error("Failed to create Vulkan VulkanCanvas shader");
            return;
        }

        m_UniformBuffer = Memory::SharedCast<VulkanUniformBuffer>(UniformBuffer::Create(m_Context, shaderInputLayout, m_Shader, { "CanvasCamera" }));
        if (m_UniformBuffer == nullptr) {
            Log_nts.Error("Cannot create uniform buffer");
        }//*/

        uint32 size = MAX_VERTICES;
        m_VertexBuffer = Memory::SharedCast<VulkanVertexBuffer>(VertexBuffer::Create(m_Context, shaderInputLayout, BufferDataUsage::DynamicDraw, nullptr, size));
        if (!m_VertexBuffer) {
            Log_nts.Error("Failed to create Vulkan VulkanCanvas vertex buffer");
        }

        size = MAX_INDICES;
        m_IndexBuffer = Memory::SharedCast<VulkanIndexBuffer>(IndexBuffer::Create(m_Context, BufferDataUsage::DynamicDraw, DrawIndexType::UInt32, nullptr, size));
        if (!m_IndexBuffer) {
            Log_nts.Error("Failed to create Vulkan VulkanCanvas index buffer");
        }

        m_VertexArray = Memory::SharedCast<VulkanVertexArray>(VertexArray::Create(m_Context, shaderInputLayout));
        if (!m_VertexArray) {
            Log_nts.Error("Failed to create Vulkan VulkanCanvas vertex array");
        }
        else {
            m_VertexArray->SetVertexBuffer(m_VertexBuffer);
            m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        }
    }

    void VulkanCanvas::Flush()
    {
        if (m_Vertices.empty() || m_Indices.empty() || m_CanvasCommands.empty()) {
            return;
        }
        Vector2f size = m_Context->GetWindow()->GetSize();

        if (m_ScissorEnable) {
            // a implementer
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
                VkViewport viewport = {};
                viewport.width = viewportCommand->viewport.width;
                viewport.height = viewportCommand->viewport.height;
                viewport.maxDepth = 1.0f;
                viewport.x = viewportCommand->viewport.x;
                viewport.y = viewportCommand->viewport.y;
                vkCheckErrorVoid(vkCmdSetViewport(m_Context->GetCurrentCommandBuffer(), 0, 1, &viewport));
                // a implementer
            }
            else if (auto scissorCommand = Memory::SharedCast<CanvasScissorCommand>(command)) {
                // Apply scissort
                VkRect2D scissor = {};
                scissor.offset = { scissorCommand->scissor.x, scissorCommand->scissor.y };
                scissor.extent = { (uint32)scissorCommand->scissor.width, (uint32)scissorCommand->scissor.height };
                vkCheckErrorVoid(vkCmdSetScissor(m_Context->GetCurrentCommandBuffer(), 0, 1, &scissor));
                // a implementer
            }
            else if (render_evalable) {
                if (auto renderCommand = Memory::SharedCast<CanvasRenderCommand>(command)) {
                    if (renderCommand->texture) {
                        renderCommand->texture->Bind();
                    }

                    if (shaderInputLayout != nullptr) {
                        CanvasTransform transform;
                        transform.model = renderCommand->transform;
                        shaderInputLayout->UpdatePushConstant("CanvasTransform", &transform, sizeof(CanvasTransform), m_Shader);

                        CanvasMaterial material;
                        material.useColor = true;
                        material.useTexture = renderCommand->texture != nullptr;
                        shaderInputLayout->UpdatePushConstant("CanvasMaterial", &material, sizeof(CanvasMaterial), m_Shader);

                        //shaderInputLayout->Bind();
                    }

                    VkPrimitiveTopology primitive = VulkanConvert::GetPrimitiveType(renderCommand->primitive);

                    vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_Context->GetCurrentCommandBuffer(), primitive));
                    vkCheckErrorVoid(vkCmdDrawIndexed(m_Context->GetCurrentCommandBuffer(), renderCommand->indexCount, 1, offset, 0, 0));
                    offset += renderCommand->indexCount;
                }
            }
        }

        m_VertexArray->UnbindIndex();

        if (render_evalable) {
            m_Shader->Unbind();
        }

        if (m_ScissorEnable) {
            // a implementer
        }
    }

}  //  nkentseu