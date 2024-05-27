//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:21:33 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalIndexBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"

namespace nkentseu {
    
    // Constructor
    InternalIndexBuffer::InternalIndexBuffer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalIndexBuffer::~InternalIndexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalIndexBuffer::Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices, const BufferLayout& bufferLayout) {
        if (context == nullptr || context->GetInternal() == nullptr) return false;
        m_Context = context;
        InternalContext* ct = m_Context->GetInternal();

        Log_nts.Debug("size = {0} - {1} - {2}", indices.size(), bufferLayout.stride);

        if (!m_IndexBufferObject.Create(&ct->m_Gpu, &ct->m_CommandPool, indices.data(), indices.size(), sizeof(uint32), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)) {
            return false;
        }

        m_Leng = indices.size();
        m_IndexType = indexType;

        return true;
    }

    bool InternalIndexBuffer::Destroy() {
        return false;
    }

    bool InternalIndexBuffer::Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* indices, uint32 leng, const BufferLayout& bufferLayout)
    {
        if (context == nullptr || context->GetInternal() == nullptr) return false;
        m_Context = context;
        InternalContext* ct = m_Context->GetInternal();

        if (!m_IndexBufferObject.Create(&ct->m_Gpu, &ct->m_CommandPool, indices, leng, sizeof(uint32), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)) {
            return false;
        }

        m_Leng = leng;
        m_IndexType = indexType;

        return true;
    }

    VulkanBuffer* InternalIndexBuffer::GetBuffer()
    {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return nullptr;
        return &m_IndexBufferObject;
    }

    DrawIndexType::Code InternalIndexBuffer::GetIndexType() const {
        return m_IndexType;
    }

    uint32 InternalIndexBuffer::Leng()
    {
        return m_Leng;
    }
}  //  nkentseu

#endif