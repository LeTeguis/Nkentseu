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
#include "VulkanVertexArray.h"

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

}  //  nkentseu