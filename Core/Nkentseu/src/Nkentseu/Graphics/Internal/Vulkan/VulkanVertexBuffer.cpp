//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:11:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanVertexBuffer.h"

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "VulkanContext.h"

#include <vulkan/vulkan.hpp>
#include "VulkanUtils.h"

namespace nkentseu {

    VulkanVertexBuffer::VulkanVertexBuffer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
    }

    Memory::Shared<Context> VulkanVertexBuffer::GetContext()
    {
        return m_Context;
    }

    VulkanVertexBuffer::~VulkanVertexBuffer() {
    }

    bool VulkanVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout) {
        return Create(bufferUsage, vertices.data(), vertices.size() / bufferLayout.componentCount, bufferLayout);
    }

    bool VulkanVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout) {
        if (m_Context == nullptr) return false;

        VulkanResult result;
        bool first = true;

        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBufferUsageFlagBits usage_t = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryPropertyFlags propertyFlags2 = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        usize size = leng * bufferLayout.stride;
        m_VertexBufferObject.size = size;

        VulkanBuffer stanging;

        if (!VulkanBuffer::CreateBuffer(m_Context->GetGpu(), size, usage_t, sharingMode, propertyFlags, stanging.buffer, stanging.bufferMemory)) {
            return false;
        }

        vkCheckError(first, result, vkMapMemory(m_Context->GetGpu()->device, stanging.bufferMemory, 0, size, 0, &m_VertexBufferObject.mappedData), "cannot map buffer memory");

        if (!result.success) {
            return false;
        }

        memcpy(m_VertexBufferObject.mappedData, vertices, size);

        vkCheckErrorVoid(vkUnmapMemory(m_Context->GetGpu()->device, stanging.bufferMemory));

        if (!VulkanStaticDebugInfo::success) {
            return false;
        }

        if (!VulkanBuffer::CreateBuffer(m_Context->GetGpu(), size, usage, sharingMode, propertyFlags2, m_VertexBufferObject.buffer, m_VertexBufferObject.bufferMemory)) {
            return false;
        }

        if (!VulkanBuffer::CopyBuffer(m_Context->GetGpu(), m_Context->GetCommandPool(), stanging.buffer, m_VertexBufferObject.buffer, size)) {
            return false;
        }

        bool success = stanging.Destroy(m_Context->GetGpu());

        m_Leng = leng;
        Log_nts.Info("Create vertex buffer is good");

        return success;
    }

    VulkanBuffer* VulkanVertexBuffer::GetBuffer()
    {
        if (m_Context == nullptr) return nullptr;
        return &m_VertexBufferObject;
    }

    uint32 VulkanVertexBuffer::Leng() const {
        return m_Leng;
    }

    bool VulkanVertexBuffer::Destroy() {
        if (m_Context == nullptr) return false;
        bool success = m_VertexBufferObject.Destroy(m_Context->GetGpu());
        return success;
    }

}  //  nkentseu