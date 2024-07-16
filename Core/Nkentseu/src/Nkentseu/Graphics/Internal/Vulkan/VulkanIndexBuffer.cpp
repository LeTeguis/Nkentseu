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

    bool VulkanIndexBuffer::Create(BufferUsageType bufferUsage, const std::vector<uint32>& indices) {
        return Create(bufferUsage, IndexBufferType::Enum::UInt32, indices.data(), indices.size());
    }

    bool VulkanIndexBuffer::Destroy() {
        if (m_Context == nullptr) return false;
        bool success = m_IndexBufferObject.Destroy(m_Context.get());
        return success;
    }

    bool VulkanIndexBuffer::SetData(void* data, usize size)
    {
        if (m_Context == nullptr) return false;
        m_IndexBufferObject.Mapped(m_Context.get(), size, 0);
        bool success = m_IndexBufferObject.WriteToBuffer(data, size, 0);
        success = m_IndexBufferObject.Flush(m_Context.get(), size, 0);
        m_IndexBufferObject.UnMapped(m_Context.get());
        return success;
    }

    bool VulkanIndexBuffer::Create(BufferUsageType bufferUsage, IndexBufferType indexType, const void* indices, uint32 leng)
    {
        if (m_Context == nullptr) return false;

        VulkanResult result;
        bool first = true;
        m_IndexType = indexType;

        vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
        vk::SharingMode sharingMode = vk::SharingMode::eExclusive;

        vk::BufferUsageFlags usage_t = vk::BufferUsageFlagBits::eTransferSrc;
        vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
        vk::MemoryPropertyFlags propertyFlags2 = vk::MemoryPropertyFlagBits::eDeviceLocal;

        m_IndexBufferObject.size = IndexBufferType::SizeOf(indexType);
        usize size = m_IndexBufferObject.size * leng;

        VkBufferInternal stanging;

        if (!VkBufferInternal::CreateBuffer(m_Context.get(), size, usage_t, sharingMode, propertyFlags, stanging.buffer, stanging.bufferMemory)) {
            return false;
        }

        if (!stanging.Mapped(m_Context.get(), size, 0, {})) {
            return false;
        }

        if (!stanging.WriteToBuffer(indices, size, 0)) {
            return false;
        }

        if (!stanging.UnMapped(m_Context.get())) {
            return false;
        }

        if (!VkBufferInternal::CreateBuffer(m_Context.get(), size, usage, sharingMode, propertyFlags2, m_IndexBufferObject.buffer, m_IndexBufferObject.bufferMemory)) {
            return false;
        }

        if (!VkBufferInternal::CopyBuffer(m_Context.get(), stanging.buffer, m_IndexBufferObject.buffer, size)) {
            return false;
        }

        bool success = stanging.Destroy(m_Context.get());//*/

        m_Leng = leng;
        Log_nts.Info("Create index buffer is good");

        return true;
    }

    VkBufferInternal* VulkanIndexBuffer::GetBuffer()
    {
        if (m_Context == nullptr) return nullptr;
        return &m_IndexBufferObject;
    }

    IndexBufferType VulkanIndexBuffer::GetIndexType() const {
        return m_IndexType;
    }

    uint32 VulkanIndexBuffer::Leng() const
    {
        return m_Leng;
    }
}  //  nkentseu