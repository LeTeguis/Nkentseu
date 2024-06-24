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

    void OpenglCanvas::Clear(const Color& color)
    {
    }

    void OpenglCanvas::Prepare()
    {
        m_IsPresent = false;
    }

    void OpenglCanvas::Present()
    {
        Flush();
        m_IsPresent = true;
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

        m_Shader = Memory::SharedCast<OpenglShader>(Shader::Create(m_Context, shaderFiles, shaderLayout));
        if (!m_Shader) {
            Log_nts.Error("Failed to create OpenGL OpenglCanvas shader");
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
            //cameraBuffer.proj = matrix4f::Orthogonal(distance_to_screen * m_Context->GetWindow()->GetDpiAspect(), distance_to_screen, 0.1f, 100.0f);
            cameraBuffer.proj = matrix4f::PerspectiveFov(Angle(45), m_Context->GetWindow()->GetDpiAspect(), 0.1, 100.0f);

            m_UniformBuffer->SetData("CanvasCamera", &cameraBuffer, sizeof(CanvasCamera));
        }

        if (m_VertexArray == nullptr || !m_VertexArray->Bind()) {
            return;
        }

        if (m_VertexBuffer == nullptr || !m_VertexBuffer->Bind()) {
            return;
        }
        m_VertexArray->GetVertexBuffer()->SetData(m_Vertices.data(), m_Vertices.size());

        if (m_IndexBuffer == nullptr || !m_IndexBuffer->Bind()) {
            return;
        }
        m_VertexArray->GetIndexBuffer()->SetData(m_Indices.data(), m_Indices.size());

        uint32 offset = 0;
        for (const auto& command : m_Commands) {
            if (m_UniformBuffer != nullptr) {
                CanvasTransform transform;
                transform.model = matrix4f();
                m_UniformBuffer->SetData("CanvasTransform", &transform, sizeof(CanvasTransform));

                CanvasMaterial material;
                material.useColor = false;
                material.useTexture = false;
                m_UniformBuffer->SetData("CanvasMaterial", &material, sizeof(CanvasMaterial));

                m_UniformBuffer->Bind();
                //Log_nts.Debug();
            }
            m_VertexArray->DrawIndex(command.primitive);
            //m_VertexArray->DrawIndex(command.primitive, offset * sizeof(uint32), command.indexCount);
            //glDrawElements(GLConvert::GetPrimitiveType(command.primitive), command.indexCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(uint32)));
            offset += command.indexCount;
        }
        m_VertexArray->DrawVertex(RenderPrimitive::Triangles);

        m_IndexBuffer->Unbind();
        m_VertexBuffer->Unbind();
        m_VertexArray->Unbind();
        m_Shader->Unbind();

        m_Vertices.clear();
        m_Indices.clear();
        m_Commands.clear();
        m_IndexCount = 0;
    }

    void OpenglCanvas::DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture& texture)
    {
        VertexData vertex = VertexData(position, Vector4f(color), Vector2f());
        m_Vertices.push_back(vertex);

        m_Indices.push_back(m_IndexCount);
        m_IndexCount += 1;

        m_Commands.push_back({ RenderPrimitive::Points, 1, {0, 0}, {0, 0, 0, 0}, texture });
    }

    void OpenglCanvas::DrawLine(const Vector2f& start, const Vector2f& end, const Color& color, CanvasTexture& texture) {
        VertexData vertex1 = VertexData(start, Vector4f(color), Vector2f(1, 1));
        VertexData vertex2 = VertexData(start + end, Vector4f(color), Vector2f(1, 1));
        m_Vertices.push_back(vertex1);
        m_Vertices.push_back(vertex2);

        m_Indices.push_back(m_IndexCount);
        m_Indices.push_back(m_IndexCount + 1);
        m_IndexCount += 2;

        m_Commands.push_back({ RenderPrimitive::Lines, 2, {0, 0}, {0, 0, 0, 0}, texture});
    }

    void OpenglCanvas::DrawRect(const Vector2f& position, const Vector2f& size, const Color& color, bool filled, CanvasTexture& texture) {
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