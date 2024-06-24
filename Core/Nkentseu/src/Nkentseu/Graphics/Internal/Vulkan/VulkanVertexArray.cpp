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
    VulkanVertexArray::VulkanVertexArray(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
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
        if (m_Context == nullptr) return false;

        m_VertexBuffer = Memory::SharedCast<VulkanVertexBuffer>(vertexBuffer);

        return m_VertexBuffer != nullptr;
    }

    Memory::Shared<VertexBuffer> VulkanVertexArray::GetVertexBuffer() {
        if (m_Context == nullptr) return nullptr;
        return m_VertexBuffer;
    }

    bool VulkanVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) {
        if (m_Context == nullptr) return false;

        m_IndexBuffer = Memory::SharedCast<VulkanIndexBuffer>(indexBuffer);

        return m_IndexBuffer != nullptr;
    }

    Memory::Shared<IndexBuffer> VulkanVertexArray::GetIndexBuffer() {
        if (m_Context == nullptr) return nullptr;
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

        if (m_VertexBuffer != nullptr && m_VertexBuffer->GetBuffer() != nullptr) {
            VkBuffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
            VkDeviceSize offsets[] = { 0 };
            vkCheckErrorVoid(vkCmdBindVertexBuffers(m_Context->GetCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets));
        }
        else if (Leng() == 0) {
            return false;
        }

        if (m_IndexBuffer == nullptr) {
            vkCheckErrorVoid(vkCmdDraw(m_Context->GetCurrentCommandBuffer(), Leng(), 1, 0, 0));
        }
        else {
            vkCmdBindIndexBuffer(m_Context->GetCurrentCommandBuffer(), m_IndexBuffer->GetBuffer()->buffer, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(m_Context->GetCurrentCommandBuffer(), m_IndexBuffer->Leng(), 1, 0, 0, 0);
        }

        return true;
    }

    bool VulkanVertexArray::DrawVertex(RenderPrimitive::Enum primitive)
    {
        if (m_VertexBuffer == nullptr) {
            return false;
        }

        return DrawVertex(primitive, 0, m_VertexBuffer->Leng());
    }

    bool VulkanVertexArray::DrawVertex(RenderPrimitive::Enum primitive, uint32 firstVertex, uint32 vertexCount)
    {
        if (m_Context == nullptr) {
            return false;
        }

        if (m_VertexBuffer == nullptr || m_VertexBuffer->GetBuffer() == nullptr || m_VertexBuffer->GetBuffer()->buffer == nullptr) {
            return false;
        }

        vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_Context->GetCurrentCommandBuffer(), VulkanConvert::GetPrimitiveType(primitive)));

        uint32 first = m_VertexBuffer->Leng() < firstVertex ? 0 : firstVertex;
        uint32 count = first + vertexCount > m_VertexBuffer->Leng() ? m_VertexBuffer->Leng() : vertexCount;

        VkBuffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCheckErrorVoid(vkCmdBindVertexBuffers(m_Context->GetCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets));

        vkCheckErrorVoid(vkCmdDraw(m_Context->GetCurrentCommandBuffer(), count, 1, first, 0));

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
        if (m_Context == nullptr) {
            return false;
        }

        if (m_VertexBuffer == nullptr || m_VertexBuffer->GetBuffer() == nullptr || m_VertexBuffer->GetBuffer()->buffer == nullptr) {
            return false;
        }

        if (m_IndexBuffer == nullptr || m_IndexBuffer->GetBuffer() == nullptr || m_IndexBuffer->GetBuffer()->buffer == nullptr) {
            return false;
        }

        vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_Context->GetCurrentCommandBuffer(), VulkanConvert::GetPrimitiveType(primitive)));

        uint32 first = m_IndexBuffer->Leng() < firstIndex ? 0 : firstIndex;
        uint32 count = first + indexCount > m_IndexBuffer->Leng() ? m_IndexBuffer->Leng() : indexCount;

        VkBuffer vertexBuffers[] = { m_VertexBuffer->GetBuffer()->buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCheckErrorVoid(vkCmdBindVertexBuffers(m_Context->GetCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets));

        vkCheckErrorVoid(vkCmdBindIndexBuffer(m_Context->GetCurrentCommandBuffer(), m_IndexBuffer->GetBuffer()->buffer, 0, VK_INDEX_TYPE_UINT32));
        vkCheckErrorVoid(vkCmdDrawIndexed(m_Context->GetCurrentCommandBuffer(), count, 1, first, 0, 0));

        return true;
    }

}  //  nkentseu