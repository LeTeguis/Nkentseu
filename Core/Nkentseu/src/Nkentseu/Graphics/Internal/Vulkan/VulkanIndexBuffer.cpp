//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:21:33 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanIndexBuffer.h"

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "VulkanContext.h"
#include "VulkanUtils.h"

namespace nkentseu {

    VulkanIndexBuffer::VulkanIndexBuffer(Memory::Shared<Context>  context) : m_Context(Memory::SharedCast<VulkanContext>(context))
    {
    }

    Memory::Shared<Context>  VulkanIndexBuffer::GetContext()
    {
        return m_Context;
    }

    // Destructor
    VulkanIndexBuffer::~VulkanIndexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool VulkanIndexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<uint32>& indices) {
        return Create(bufferUsage, DrawIndexType::UInt32, indices.data(), indices.size());
    }

    bool VulkanIndexBuffer::Destroy() {
        if (m_Context == nullptr) return false;
        bool success = m_IndexBufferObject.Destroy(m_Context->GetGpu());
        return success;
    }

    bool VulkanIndexBuffer::SetData(void* data, usize size)
    {
        if (m_Context == nullptr) return false;
        m_IndexBufferObject.Mapped(m_Context->GetGpu(), size, 0);
        bool success = m_IndexBufferObject.WriteToBuffer(data, size, 0);
        success = m_IndexBufferObject.Flush(m_Context->GetGpu(), size, 0);
        m_IndexBufferObject.UnMapped(m_Context->GetGpu());
        return success;
    }

    bool VulkanIndexBuffer::Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* indices, uint32 leng)
    {
        if (m_Context == nullptr) return false;

        VulkanResult result;
        bool first = true;

        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBufferUsageFlagBits usage_t = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryPropertyFlags propertyFlags2 = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        m_IndexBufferObject.size = DrawIndexType::SizeOf(indexType);
        usize size = m_IndexBufferObject.size * leng;

        /*if (!VulkanBuffer::CreateBuffer(m_Context->GetGpu(), size, usage, sharingMode, propertyFlags, m_IndexBufferObject.buffer, m_IndexBufferObject.bufferMemory)) {
            Log_nts.Error("Cannot create uniforme buffer : name = {0} at index = {1}");
            return false;
        }*/

        VulkanBuffer stanging;

        if (!VulkanBuffer::CreateBuffer(m_Context->GetGpu(), size, usage_t, sharingMode, propertyFlags, stanging.buffer, stanging.bufferMemory)) {
            return false;
        }

        vkCheckError(first, result, vkMapMemory(m_Context->GetGpu()->device, stanging.bufferMemory, 0, size, 0, &m_IndexBufferObject.mappedData), "cannot map buffer memory");

        if (!result.success) {
            return false;
        }

        memcpy(m_IndexBufferObject.mappedData, indices, size);

        vkCheckErrorVoid(vkUnmapMemory(m_Context->GetGpu()->device, stanging.bufferMemory));

        if (!VulkanStaticDebugInfo::success) {
            return false;
        }

        if (!VulkanBuffer::CreateBuffer(m_Context->GetGpu(), size, usage, sharingMode, propertyFlags2, m_IndexBufferObject.buffer, m_IndexBufferObject.bufferMemory)) {
            return false;
        }

        if (!VulkanBuffer::CopyBuffer(m_Context->GetGpu(), m_Context->GetCommandPool(), stanging.buffer, m_IndexBufferObject.buffer, size)) {
            return false;
        }

        bool success = stanging.Destroy(m_Context->GetGpu());//*/

        m_Leng = leng;
        Log_nts.Info("Create index buffer is good");

        return true;
    }

    VulkanBuffer* VulkanIndexBuffer::GetBuffer()
    {
        if (m_Context == nullptr) return nullptr;
        return &m_IndexBufferObject;
    }

    DrawIndexType::Code VulkanIndexBuffer::GetIndexType() const {
        return m_IndexType;
    }

    uint32 VulkanIndexBuffer::Leng() const
    {
        return m_Leng;
    }
}  //  nkentseu