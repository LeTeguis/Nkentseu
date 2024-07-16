//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:22:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanVertexArray.h"

#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"

#include "VulkanContext.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanVertexArray.h"
#include "VulkanUtils.h"

namespace nkentseu {
    VulkanVertexArray::VulkanVertexArray(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
        m_Vksil = Memory::SharedCast<VulkanShaderInputLayout>(sil);
    }

    // Destructor
    VulkanVertexArray::~VulkanVertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    Memory::Shared<Context> VulkanVertexArray::GetContext() {
        return m_Context;
    }

    bool VulkanVertexArray::Create() {
        if (m_Context == nullptr) return false;
        return true;
    }

    bool VulkanVertexArray::Create(uint32 vertexNumber)
    {
        if (m_Context == nullptr) return false;
        m_VertexNumber = vertexNumber;
        return true;
    }

    uint32 VulkanVertexArray::GetVertexLeng() {
        if (m_Context == nullptr || m_VertexBuffer == nullptr) return 0;
        return m_VertexBuffer->Leng();
    }

    bool VulkanVertexArray::Destroy() {
        if (m_Context == nullptr) return false;
        return true;
    }

    bool VulkanVertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer) {
        if (m_Context == nullptr || m_Vksil == nullptr) return false;

        m_VertexBuffer = Memory::SharedCast<VulkanVertexBuffer>(vertexBuffer);

        return m_VertexBuffer != nullptr;
    }

    Memory::Shared<VertexBuffer> VulkanVertexArray::GetVertexBuffer() {
        if (m_Context == nullptr || m_Vksil == nullptr) return nullptr;
        return m_VertexBuffer;
    }

    bool VulkanVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) {
        if (m_Context == nullptr || m_Vksil == nullptr) return false;

        m_IndexBuffer = Memory::SharedCast<VulkanIndexBuffer>(indexBuffer);

        return m_IndexBuffer != nullptr;
    }

    Memory::Shared<IndexBuffer> VulkanVertexArray::GetIndexBuffer() {
        if (m_Context == nullptr || m_Vksil == nullptr) return nullptr;
        return m_IndexBuffer;
    }

    uint32 VulkanVertexArray::GetIndexLeng()
    {
        if (m_Context == nullptr || m_IndexBuffer == nullptr) return 0;
        return m_IndexBuffer->Leng();
    }

    uint32 VulkanVertexArray::Leng()
    {
        if (m_Context == nullptr || m_VertexBuffer == nullptr) return m_VertexNumber;
        return m_VertexBuffer->Leng() == 0 ? m_VertexNumber : m_VertexBuffer->Leng();
    }

    bool VulkanVertexArray::Draw(DrawVertexType::Code drawVertex) {
        if (m_Context == nullptr || drawVertex == DrawVertexType::NotDefine) {
            return false;
        }

        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();
        
        if (m_VertexBuffer != nullptr && m_VertexBuffer->GetBuffer() != nullptr) {
            vk::Buffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
            vk::DeviceSize offsets[] = { 0 };
            vkCheckErrorVoid(commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets));
        }
        else if (Leng() == 0) {
            return false;
        }

        if (m_IndexBuffer == nullptr) {
            vkCheckErrorVoid(commandBuffer.draw(Leng(), 1, 0, 0));
        }
        else {
            vkCheckErrorVoid(commandBuffer.bindIndexBuffer(m_IndexBuffer->GetBuffer()->buffer, 0, vk::IndexType::eUint32));
            vkCheckErrorVoid(commandBuffer.drawIndexed(m_IndexBuffer->Leng(), 1, 0, 0, 0));
        }
        
        return true;
    }

    bool VulkanVertexArray::BindVertex()
    {
        if (m_Context == nullptr || m_BindInfo == BindInfo::VERTEX_BIND) return false;

        if (m_VertexBuffer == nullptr || m_VertexBuffer->GetBuffer() == nullptr || m_VertexBuffer->GetBuffer()->buffer == nullptr) {
            if (Leng() == 0) {
                m_BindInfo = BindInfo::NO_BIND;
                return false;
            }
            m_BindInfo = BindInfo::VERTEX_BIND;
            return true;
        }

        m_BindInfo = BindInfo::VERTEX_BIND;
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        vk::Buffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
        vk::DeviceSize offsets[] = { 0 };
        vkCheckErrorVoid(commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot bind vertex buffer");
        }
        return true;
    }

    bool VulkanVertexArray::UnbindVertex()
    {
        if (m_Context == nullptr || m_BindInfo != BindInfo::VERTEX_BIND) return false;
        m_BindInfo = BindInfo::NO_BIND;
        return true;
    }

    bool VulkanVertexArray::DrawVertex(RenderPrimitive::Enum primitive)
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

    bool VulkanVertexArray::DrawVertex(RenderPrimitive::Enum primitive, uint32 firstVertex, uint32 vertexCount)
    {
        if (m_Context == nullptr || vertexCount == 0 || m_BindInfo != BindInfo::VERTEX_BIND) {
            return false;
        }
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        uint32 first = Leng() < firstVertex ? 0 : firstVertex;
        uint32 count = first + vertexCount > Leng() ? Leng() : vertexCount;

        vkCheckErrorVoid(commandBuffer.setPrimitiveTopology(VulkanConvert::GetPrimitiveType(primitive)));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot set primitive topology");
        }
        vkCheckErrorVoid(commandBuffer.draw(Leng(), 1, first, 0));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot draw vertex buffer");
        }

        return true;
    }

    bool VulkanVertexArray::BindIndex()
    {
        if (m_Context == nullptr || m_BindInfo == BindInfo::INDEX_BIND) return false;

        if (m_VertexBuffer == nullptr || m_VertexBuffer->GetBuffer() == nullptr || m_VertexBuffer->GetBuffer()->buffer == nullptr) {
            m_BindInfo = BindInfo::NO_BIND;
            return false;
        }

        if (m_IndexBuffer == nullptr || m_IndexBuffer->GetBuffer() == nullptr || m_IndexBuffer->GetBuffer()->buffer == nullptr) {
            m_BindInfo = BindInfo::NO_BIND;
            return false;
        }

        m_BindInfo = BindInfo::INDEX_BIND;
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        vk::Buffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
        vk::DeviceSize offsets[] = { 0 };

        vkCheckErrorVoid(commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot bind vertex buffer");
        }
        vkCheckErrorVoid(commandBuffer.bindIndexBuffer(m_IndexBuffer->GetBuffer()->buffer, 0, vk::IndexType::eUint32));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot bind index buffer");
        }

        return true;
    }

    bool VulkanVertexArray::UnbindIndex()
    {
        if (m_Context == nullptr || m_BindInfo != BindInfo::INDEX_BIND) return false;
        m_BindInfo = BindInfo::NO_BIND;
        return true;
    }

    bool VulkanVertexArray::DrawIndex(RenderPrimitive::Enum primitive)
    {
        if (m_IndexBuffer == nullptr) {
            return false;
        }
        return DrawIndex(primitive, 0, m_IndexBuffer->Leng());
    }

    bool VulkanVertexArray::DrawIndex(RenderPrimitive::Enum primitive, uint32 firstIndex, uint32 indexCount)
    {
        if (m_Context == nullptr || m_IndexBuffer == nullptr || m_BindInfo != BindInfo::INDEX_BIND) {
            return false;
        }
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        uint32 first = m_IndexBuffer->Leng() < firstIndex ? 0 : firstIndex;
        uint32 count = first + indexCount > m_IndexBuffer->Leng() ? m_IndexBuffer->Leng() : indexCount;

        vkCheckErrorVoid(commandBuffer.setPrimitiveTopology(VulkanConvert::GetPrimitiveType(primitive))); 
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot set primitive topology");
        }
        vkCheckErrorVoid(commandBuffer.drawIndexed(count, 1, first, 0, 0));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot draw index buffer");
            return false;
        }
        return true;
    }

}  //  nkentseu