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

    void VulkanCanvas::Clear(const Color& color)
    {
    }

    void VulkanCanvas::Prepare()
    {
        m_Vertices.clear();
        m_Indices.clear();
        m_Commands.clear();
    }

    void VulkanCanvas::Present()
    {
        Flush();
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
        if (m_Context == nullptr) {
            return;
        }

        if (m_VertexBuffer == nullptr || m_VertexBuffer->GetBuffer() == nullptr || m_VertexBuffer->GetBuffer()->buffer == nullptr) {
            return;
        }

        if (m_IndexBuffer == nullptr || m_IndexBuffer->GetBuffer() == nullptr || m_IndexBuffer->GetBuffer()->buffer == nullptr) {
            return;
        }

        m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size());
        m_IndexBuffer->SetData(m_Indices.data(), m_Vertices.size());
        m_Shader->Bind();

        if (m_UniformBuffer != nullptr) {
            float32 distance_to_screen = 1.0f;
            CanvasCamera cameraBuffer{};
            cameraBuffer.view = matrix4f::Identity();
            cameraBuffer.proj = matrix4f::Orthogonal(distance_to_screen * m_Context->GetWindow()->GetDpiAspect(), distance_to_screen, 0.1f, 100.0f);;

            m_UniformBuffer->SetData("camera", &cameraBuffer, sizeof(CanvasCamera));
        }

        VkBuffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCheckErrorVoid(vkCmdBindVertexBuffers(m_Context->GetCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets));

        vkCheckErrorVoid(vkCmdBindIndexBuffer(m_Context->GetCurrentCommandBuffer(), m_IndexBuffer->GetBuffer()->buffer, 0, VK_INDEX_TYPE_UINT32));

        uint32 offset = 0;
        for (const auto& command : m_Commands) {
            if (m_UniformBuffer != nullptr) {
                CanvasTransform transform;
                transform.model = matrix4f();
                m_UniformBuffer->SetData("transform", &transform, sizeof(CanvasTransform));

                CanvasMaterial material;
                material.useColor = false;
                material.useTexture = false;
                m_UniformBuffer->SetData("material", &material, sizeof(CanvasMaterial));

                m_UniformBuffer->Bind();
            }
            vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_Context->GetCurrentCommandBuffer(), VulkanConvert::GetPrimitiveType(command.primitive)));
            vkCheckErrorVoid(vkCmdDrawIndexed(m_Context->GetCurrentCommandBuffer(), command.indexCount, 1, offset, 0, 0));
            offset += command.indexCount;
        }

        m_Shader->Unbind();

        m_Vertices.clear();
        m_Indices.clear();
        m_Commands.clear();
        m_IndexCount = 0;
    }

    void VulkanCanvas::DrawPoint(const Vector2f& position, const Color& color, CanvasTexture& texture)
    {
        VertexData vertex = VertexData(position, Vector4f(color), Vector2f());
        m_Vertices.push_back(vertex);

        m_Indices.push_back(m_IndexCount);
        m_IndexCount += 1;

        m_Commands.push_back({ RenderPrimitive::Points, 1, {0, 0}, {0, 0, 0, 0}, texture });
    }

    void VulkanCanvas::DrawLine(const Vector2f& start, const Vector2f& end, const Color& color, CanvasTexture& texture) {
        VertexData vertex1 = VertexData(start, Vector4f(color), Vector2f(1, 1));
        VertexData vertex2 = VertexData(start + end, Vector4f(color), Vector2f(1, 1));
        m_Vertices.push_back(vertex1);
        m_Vertices.push_back(vertex2);

        m_Indices.push_back(m_IndexCount);
        m_Indices.push_back(m_IndexCount + 1);
        m_IndexCount += 2;

        m_Commands.push_back({ RenderPrimitive::Lines, 2, {0, 0}, {0, 0, 0, 0}, texture });
    }

    void VulkanCanvas::DrawRect(const Vector2f& position, const Vector2f& size, const Color& color, bool filled, CanvasTexture& texture) {
        if (filled) {
            // Filled rectangle (four lines)
            VertexData vertex1 = VertexData(position + Vector2f(), Vector4f(color), Vector2f(1, 1));
            VertexData vertex2 = VertexData(position + Vector2f(size.x, 0), Vector4f(color), Vector2f(1, 0));
            VertexData vertex3 = VertexData(position + Vector2f(0, size.y), Vector4f(color), Vector2f(0, 0));
            VertexData vertex4 = VertexData(position + size, Vector4f(color), Vector2f(0, 1));

            m_Vertices.push_back(vertex1);
            m_Vertices.push_back(vertex2);
            m_Vertices.push_back(vertex3);
            m_Vertices.push_back(vertex4);

            m_Indices.push_back(m_IndexCount);
            m_Indices.push_back(m_IndexCount + 1);
            m_Indices.push_back(m_IndexCount + 3);
            m_Indices.push_back(m_IndexCount + 1);
            m_Indices.push_back(m_IndexCount + 2);
            m_Indices.push_back(m_IndexCount + 3);

            m_IndexCount += 4;
            m_Commands.push_back({ RenderPrimitive::Triangles, 6, {0, 0}, {0, 0, 0, 0}, texture });
        }
        else {
            // Outline rectangle (four lines)
            DrawLine(position, position + Vector2f(size.x, 0), color);
            DrawLine(position + Vector2f(size.x, 0), position + Vector2f(0, size.y), color);
            DrawLine(position + Vector2f(0, size.y), position + size, color);
            DrawLine(position + size, position, color);
        }
    }

}  //  nkentseu