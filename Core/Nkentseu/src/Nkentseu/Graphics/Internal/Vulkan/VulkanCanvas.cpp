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

    void VulkanCanvas::Prepare()
    {
        m_IsPresent = false;
        m_Vertices.clear();
        m_Indices.clear();
        m_Commands.clear();
        m_IndexCount = 0;
    }

    void VulkanCanvas::Present()
    {
        Flush();
        m_IsPresent = true;
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
        std::unordered_map<ShaderType::Code, std::string> shaderFiles;
        shaderFiles[ShaderType::Vertex] = "Resources/shaders/canvas.vert.glsl";
        shaderFiles[ShaderType::Fragment] = "Resources/shaders/canvas.frag.glsl";

        BufferLayout bufferLayout;
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float2, "aPos", 0));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float4, "aColor", 1));
        bufferLayout.attributes.push_back(BufferAttribute(ShaderDataType::Float2, "aTexCoord", 2));
        bufferLayout.CalculateOffsetsAndStride();

        UniformBufferLayout uniformLayout;
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CanvasTransform), 0, "transform", ShaderType::Vertex, UniformBufferType::Dynamic, 10));
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CanvasCamera), 1, "camera", ShaderType::Vertex, UniformBufferType::Static, 1));
        uniformLayout.AddAttribut(UniformBufferAttribut(sizeof(CanvasMaterial), 2, "material", ShaderType::Fragment, UniformBufferType::Dynamic, 10));

        ShaderBufferLayout shaderLayout;
        shaderLayout.vertexInput = bufferLayout;
        shaderLayout.uniformBuffer = uniformLayout;
        // Define necessary layouts

        m_Shader = Memory::SharedCast<VulkanShader>(Shader::Create(m_Context, shaderFiles, shaderLayout));
        if (!m_Shader) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas shader");
        }

        m_UniformBuffer = Memory::SharedCast<VulkanUniformBuffer>(UniformBuffer::Create(m_Context, m_Shader, uniformLayout));
        if (m_UniformBuffer == nullptr) {
            Log_nts.Error("Cannot create uniform buffer");
        }
        Log_nts.Trace();

        m_VertexBuffer = Memory::SharedCast<VulkanVertexBuffer>(VertexBuffer::Create(m_Context, BufferDataUsage::DynamicDraw, nullptr, sizeof(VertexData), bufferLayout));
        if (!m_VertexBuffer) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas vertex buffer");
        }
        Log_nts.Trace();

        m_IndexBuffer = Memory::SharedCast<VulkanIndexBuffer>(IndexBuffer::Create(m_Context, BufferDataUsage::DynamicDraw, DrawIndexType::UInt32, nullptr, sizeof(uint32)));
        if (!m_IndexBuffer) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas index buffer");
        }

        m_VertexArray = Memory::SharedCast<VulkanVertexArray>(VertexArray::Create(m_Context));
        if (!m_VertexArray) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas vertex array");
        }
        else {
            m_VertexArray->SetVertexBuffer(m_VertexBuffer);
            m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        }
    }

    void VulkanCanvas::Flush()
    {
        if (m_Vertices.empty() || m_Indices.empty() || m_Commands.empty()) {
            return;
        }

        if (m_Shader == nullptr || !m_Shader->Bind()) {
            return;
        }

        if (m_UniformBuffer != nullptr) {
            float32 distance_to_screen = 1.0f;
            CanvasCamera cameraBuffer{};

            cameraBuffer.view = matrix4f::Identity();
            cameraBuffer.proj = matrix4f::Identity();
            //cameraBuffer.proj = matrix4f::Orthogonal(distance_to_screen * m_Context->GetWindow()->GetDpiAspect(), distance_to_screen, 0.1f, 100.0f);
            m_UniformBuffer->SetData("CanvasCamera", &cameraBuffer, sizeof(CanvasCamera));
        }

        if (m_VertexArray == nullptr || m_VertexArray->GetVertexBuffer() == nullptr || m_VertexArray->GetIndexBuffer() == nullptr) {
            return;
        }

        m_VertexArray->GetVertexBuffer()->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex2D));
        m_VertexArray->GetIndexBuffer()->SetData(m_Indices.data(), m_Indices.size() * sizeof(uint32));

        m_VertexArray->BindIndex();

        uint32 offset = 0;
        for (auto it = m_Commands.begin(); it != m_Commands.end(); ++it) {
            const auto& command = *it;

            if (command.texture) {
                command.texture->Bind();
            }

            if (m_UniformBuffer != nullptr) {
                CanvasTransform transform;
                transform.model = command.transform;
                m_UniformBuffer->SetData("CanvasTransform", &transform, sizeof(CanvasTransform));

                CanvasMaterial material;
                material.useColor = true;
                material.useTexture = command.texture != nullptr;
                m_UniformBuffer->SetData("CanvasMaterial", &material, sizeof(CanvasMaterial));

                m_UniformBuffer->Bind();
            }

            vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_Context->GetCurrentCommandBuffer(), VulkanConvert::GetPrimitiveType(command.primitive)));
            vkCheckErrorVoid(vkCmdDrawIndexed(m_Context->GetCurrentCommandBuffer(), command.indexCount, 1, offset, 0, 0));
            offset += command.indexCount;
        }

        m_VertexArray->UnbindIndex();

        m_Shader->Unbind();

        m_Vertices.clear();
        m_Indices.clear();
        m_Commands.clear();
        m_IndexCount = 0;
    }

}  //  nkentseu