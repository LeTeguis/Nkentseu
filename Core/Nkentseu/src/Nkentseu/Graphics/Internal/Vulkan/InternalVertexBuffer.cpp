//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:11:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"

#include <vulkan/vulkan.hpp>
#include "VulkanUtils.h"

namespace nkentseu {
    
    // Constructor
    InternalVertexBuffer::InternalVertexBuffer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalVertexBuffer::~InternalVertexBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexBuffer::Create(Context* context, BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout) {
        return Create(context, bufferUsage, vertices.data(), vertices.size() / bufferLayout.componentCount, bufferLayout);
    }

    bool InternalVertexBuffer::Create(Context* context, BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout) {
        if (context == nullptr || context->GetInternal() == nullptr) return false;
        m_Context = context;
        InternalContext* ct = m_Context->GetInternal();

        Log_nts.Debug("size = {0} - {1} - {2}", leng, bufferLayout.stride, ((vertices == nullptr) ? "True" : "False"));

        if (!m_VertexBufferObject.Create(&ct->m_Gpu, &ct->m_CommandPool, vertices, leng, bufferLayout.stride, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)) {
            return false;
        }

        m_Leng = leng;

        return true;
    }

    VulkanBuffer* InternalVertexBuffer::GetBuffer()
    {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return nullptr;
        return &m_VertexBufferObject;
    }

    uint32 InternalVertexBuffer::Leng() {
        return m_Leng;
    }

    bool InternalVertexBuffer::Destroy() {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return false;
        return m_VertexBufferObject.Destroy(&m_Context->GetInternal()->m_Gpu);
    }

}  //  nkentseu

#endif