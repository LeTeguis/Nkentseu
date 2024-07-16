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

    VulkanVertexBuffer::VulkanVertexBuffer(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
        m_Vksil = Memory::SharedCast<VulkanShaderInputLayout>(sil);
    }

    Memory::Shared<Context> VulkanVertexBuffer::GetContext()
    {
        return m_Context;
    }

    VulkanVertexBuffer::~VulkanVertexBuffer() {
    }

    bool VulkanVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices) {
        if (m_Vksil == nullptr) return false;

        return Create(bufferUsage, vertices.data(), vertices.size() / m_Vksil->vertexInput.componentCount);
    }

    bool VulkanVertexBuffer::Create(BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng) {
        if (m_Context == nullptr || m_Vksil == nullptr) return false;

        usize size = leng * m_Vksil->vertexInput.stride;
        m_VertexBufferObject.size = size;

        vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
        vk::SharingMode sharingMode = vk::SharingMode::eExclusive;

        vk::BufferUsageFlags usage_t = vk::BufferUsageFlagBits::eTransferSrc;
        vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
        vk::MemoryPropertyFlags propertyFlags2 = vk::MemoryPropertyFlagBits::eDeviceLocal;

        VkBufferInternal stanging;

        if (!VkBufferInternal::CreateBuffer(m_Context.get(), size, usage_t, sharingMode, propertyFlags, stanging.buffer, stanging.bufferMemory)) {
            return false;
        }

        if (!stanging.Mapped(m_Context.get(), size, 0, {})) {
            return false;
        }

        if (!stanging.WriteToBuffer(vertices, size, 0)) {
            return false;
        }

        if (!stanging.UnMapped(m_Context.get())) {
            return false;
        }

        if (!VkBufferInternal::CreateBuffer(m_Context.get(), size, usage, sharingMode, propertyFlags2, m_VertexBufferObject.buffer, m_VertexBufferObject.bufferMemory)) {
            return false;
        }

        if (!VkBufferInternal::CopyBuffer(m_Context.get(), stanging.buffer, m_VertexBufferObject.buffer, size)) {
            return false;
        }

        bool success = stanging.Destroy(m_Context.get());
        if (!success)
            Log_nts.Error("success : {0}", ((success) ? "True" : "False"));
       
        m_Leng = leng;
        if (success)
            Log_nts.Info("Create vertex buffer is good");

        return success;
    }

    bool VulkanVertexBuffer::SetData(void* data, usize size)
    {
        if (m_Context == nullptr) return false;
        m_VertexBufferObject.Mapped(m_Context.get(), size, 0);
        bool success = m_VertexBufferObject.WriteToBuffer(data, size, 0);
        success = m_VertexBufferObject.Flush(m_Context.get(), size, 0);
        m_VertexBufferObject.UnMapped(m_Context.get());
        return success;
    }

    VkBufferInternal* VulkanVertexBuffer::GetBuffer()
    {
        if (m_Context == nullptr) return nullptr;
        return &m_VertexBufferObject;
    }

    uint32 VulkanVertexBuffer::Leng() const {
        return m_Leng;
    }

    bool VulkanVertexBuffer::Destroy() {
        if (m_Context == nullptr) return false;
        bool success = m_VertexBufferObject.Destroy(m_Context.get());
        return success;
    }

}  //  nkentseu