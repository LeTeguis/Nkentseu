//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:11:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "OpenglVertexArray.h"

#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "OpenglVertexBuffer.h"
#include "OpenglIndexBuffer.h"

#include "Nkentseu/Graphics/Context.h"
#include "OpenglContext.h"

#include "OpenGLUtils.h"

namespace nkentseu {

    OpenglVertexArray::OpenglVertexArray(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<OpenglContext>(context)) {
        m_OglSil = Memory::SharedCast<OpenglShaderInputLayout>(sil);
    }

    OpenglVertexArray::~OpenglVertexArray() {
    }

    Memory::Shared<Context> nkentseu::OpenglVertexArray::GetContext()
    {
        return m_Context;
    }

    bool OpenglVertexArray::Create()
    {
        if (m_VertexArrayObject != 0 || m_Context == nullptr) {
            return false;
        }

        OpenGLResult result;
        bool first = true;

        if (m_UseDsa) {
            glCheckError(first, result, glCreateVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");
        }
        else {
            glCheckError(first, result, glGenVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");
        }

        return true;
    }

    bool OpenglVertexArray::Create(uint32 vertexNumber)
    {
        if (m_VertexArrayObject != 0 || m_Context == nullptr) return false;

        OpenGLResult result;
        bool first = true;

        if (m_UseDsa) {
            glCheckError(first, result, glCreateVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");
        }
        else {
            glCheckError(first, result, glGenVertexArrays(1, &m_VertexArrayObject), "cannot gen vertex array");
        }

        if (!result.success || m_VertexArrayObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        m_VertexNumber = vertexNumber;
        return Unbind();
    }

    uint32 OpenglVertexArray::GetVertexLeng() {
        if (m_VertexBuffer == nullptr) return 0;
        return m_VertexBuffer->Leng();
    }

    bool OpenglVertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer)
    {
        if (m_OglSil == nullptr) return false;
        m_VertexBuffer = Memory::SharedCast<OpenglVertexBuffer>(vertexBuffer);
        return ActualizeVertexBuffer();
    }

    Memory::Shared<VertexBuffer> OpenglVertexArray::GetVertexBuffer()
    {
        return m_VertexBuffer;
    }

    bool OpenglVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer)
    {
        if (m_OglSil == nullptr) return false;

        m_IndexBuffer = Memory::SharedCast<OpenglIndexBuffer>(indexBuffer);

        if (m_UseDsa && m_IndexBuffer != nullptr) {
            glVertexArrayElementBuffer(m_VertexArrayObject, m_IndexBuffer->GetBuffer());
        }
        return m_IndexBuffer == nullptr;
    }

    Memory::Shared<IndexBuffer> OpenglVertexArray::GetIndexBuffer()
    {
        return m_IndexBuffer;
    }

    uint32 OpenglVertexArray::GetIndexLeng()
    {
        if (m_IndexBuffer == nullptr) return 0;
        return m_IndexBuffer->Leng();
    }

    uint32 OpenglVertexArray::Leng()
    {
        if (m_Context == nullptr || m_VertexBuffer == nullptr) return m_VertexNumber;
        return m_VertexBuffer->Leng() == 0 ? m_VertexNumber : m_VertexBuffer->Leng();
    }

    bool OpenglVertexArray::Destroy()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glDeleteVertexArrays(1, &m_VertexArrayObject), "cannot delete vertex array");

        if (!result.success) {
            return false;
        }

        m_VertexArrayObject = 0;
        return true;
    }

    bool OpenglVertexArray::Draw(DrawVertexType::Code drawVertex)
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        if (!Bind()) {
            return false;
        }

        if (m_VertexBuffer == nullptr || (m_VertexBuffer != nullptr && !m_VertexBuffer->Bind() && Leng() == 0)) {
            Unbind();
            return false;
        }

        if (m_IndexBuffer != nullptr && !m_IndexBuffer->Bind()) {
            m_VertexBuffer->Unbind();
            Unbind();
            return false;
        }

        uint32 vertexType = GLConvert::VertexType(drawVertex);
        uint32 vertices_per_type = GLConvert::VerticesPerType(vertexType);

        OpenGLResult result;
        bool first = true;

        if (m_IndexBuffer != nullptr) {
            uint32 indexType = GLConvert::IndexType(m_IndexBuffer->GetIndexType());
            glCheckError(first, result, glDrawElements(vertexType, m_IndexBuffer->Leng(), indexType, 0), "cannot draw elements");
        }
        else {
            glCheckError(first, result, glDrawArrays(vertexType, 0, Leng()), "cannot draw arrays");
        }

        m_IndexBuffer->Unbind();
        m_VertexBuffer->Unbind();
        return Unbind() && result.success;
    }

    bool OpenglVertexArray::BindVertex()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0 || m_BindInfo == BindInfo::VERTEX_BIND) {
            return false;
        }

        if (!Bind()) {
            m_BindInfo = BindInfo::NO_BIND;
            return false;
        }

        if (m_VertexBuffer != nullptr && !m_VertexBuffer->Bind()) {
            Unbind();
            m_BindInfo = BindInfo::NO_BIND;
            return false;
        }
        m_BindInfo = BindInfo::VERTEX_BIND;
        return true;
    }

    bool OpenglVertexArray::UnbindVertex()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0 || m_BindInfo != BindInfo::VERTEX_BIND) {
            return false;
        }
        m_BindInfo = BindInfo::NO_BIND;
        if (m_VertexBuffer != nullptr && !m_VertexBuffer->Unbind()){
            return Unbind() && false;
        }
        return Unbind();
    }

    bool OpenglVertexArray::DrawVertex(RenderPrimitive::Enum primitive)
    {
        if (m_BindInfo != BindInfo::VERTEX_BIND) return false;

        if (m_VertexBuffer != nullptr && m_VertexBuffer->Leng() != 0) {
            return DrawVertex(primitive, 0, m_VertexBuffer->Leng());
        }

        if (Leng() != 0) {
            return DrawVertex(primitive, 0, Leng());
        }

        return false;
    }

    bool OpenglVertexArray::DrawVertex(RenderPrimitive::Enum primitive, uint32 firstVertex, uint32 vertexCount)
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0 || vertexCount == 0 || m_BindInfo != BindInfo::VERTEX_BIND) return false;
        uint32 firstElement = Leng() < firstVertex ? 0 : firstVertex;
        uint32 count = firstElement + vertexCount > Leng() ? Leng() : vertexCount;

        OpenGLResult result;
        bool first = true;
        glCheckError(first, result, glDrawArrays(GLConvert::GetPrimitiveType(primitive), firstElement, count), "cannot draw arrays");
        return result.success;
    }

    bool OpenglVertexArray::BindIndex()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0 || m_BindInfo == BindInfo::INDEX_BIND) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        if (m_VertexBuffer == nullptr || (m_VertexBuffer != nullptr && !m_VertexBuffer->Bind() && Leng() == 0)) {
            Unbind();
            m_BindInfo = BindInfo::NO_BIND;
            return false;
        }

        if (m_IndexBuffer == nullptr || (m_IndexBuffer != nullptr && !m_IndexBuffer->Bind())) {
            m_VertexBuffer->Unbind();
            Unbind();
            m_BindInfo = BindInfo::NO_BIND;
            return false;
        }
        m_BindInfo = BindInfo::INDEX_BIND;

        return true;
    }

    bool OpenglVertexArray::UnbindIndex()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0 || m_BindInfo != BindInfo::INDEX_BIND
            || m_IndexBuffer == nullptr || m_VertexBuffer == nullptr) return false;
        m_BindInfo = BindInfo::NO_BIND;
        if (!m_IndexBuffer->Unbind()) {
        }
        if (!m_VertexBuffer->Unbind()) {
        }
        return Unbind();
    }

    bool OpenglVertexArray::DrawIndex(RenderPrimitive::Enum primitive)
    {
        if (m_IndexBuffer == nullptr) {
            return false;
        }
        return DrawIndex(primitive, 0, m_IndexBuffer->Leng());
    }

    bool OpenglVertexArray::DrawIndex(RenderPrimitive::Enum primitive, uint32 firstIndex, uint32 indexCount)
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0 || m_BindInfo != BindInfo::INDEX_BIND) return false;

        OpenGLResult result;
        bool first = true;

        uint32 firstElement = m_IndexBuffer->Leng() < firstIndex ? 0 : firstIndex;
        uint32 count = firstElement + indexCount > m_IndexBuffer->Leng() ? m_IndexBuffer->Leng() : indexCount;

        uint32 indexType = GLConvert::IndexType(m_IndexBuffer->GetIndexType());
        uint32 offset = (firstElement) * sizeof(uint32);
        uint32 primitiveType = GLConvert::GetPrimitiveType(primitive);
        
        //glCheckError(first, result, glDrawElements(primitiveType, count, indexType, 0), "cannot draw elements");
        glCheckError(first, result, glDrawElements(primitiveType, count, indexType, (void*)(offset)), "cannot draw elements");

        return result.success;
    }

    bool OpenglVertexArray::Bind()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindVertexArray(m_VertexArrayObject), "cannot bin vertes array");
        return result.success;
    }

    bool OpenglVertexArray::Unbind()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) return false;

        OpenGLResult result;
        bool first = true;

        glCheckError(first, result, glBindVertexArray(0), "cannot unbin vertex array");
        return result.success;
    }

    GLuint OpenglVertexArray::GetBuffer()
    {
        return m_VertexArrayObject;
    }

    bool OpenglVertexArray::ActualizeVertexBuffer()
    {
        if (m_Context == nullptr || m_VertexArrayObject == 0) {
            return false;
        }

        if (!Bind()) {
            return false;
        }

        bool success = true;

        if (m_VertexBuffer != nullptr) {
            success = m_VertexBuffer->AttachToVAO(m_VertexArrayObject, m_UseDsa);
        }

        return Unbind() && success;
    }
}  //  nkentseu