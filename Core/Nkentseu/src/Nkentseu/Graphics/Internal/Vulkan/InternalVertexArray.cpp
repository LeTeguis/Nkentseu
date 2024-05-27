//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:22:16 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalVertexArray.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"

#include "InternalContext.h"
#include "InternalVertexBuffer.h"
#include "InternalIndexBuffer.h"
#include "InternalVertexArray.h"
#include "InternalVertexArray.h"

namespace nkentseu {
    
    // Constructor
    InternalVertexArray::InternalVertexArray(Context* context) {
        if (context == nullptr) return;
        m_Context = context;
    }

    // Destructor
    InternalVertexArray::~InternalVertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalVertexArray::Create(const BufferLayout& bufferLayout) {
        if (m_Context == nullptr) return false;
        return true;
    }

    bool InternalVertexArray::Create(uint32 vertexNumber)
    {
        if (m_Context == nullptr) return false;
        m_VertexNumber = vertexNumber;
        return true;
    }

    uint32 InternalVertexArray::GetVertexNumber() {
        return m_VertexNumber;
    }

    bool InternalVertexArray::Destroy() {
        return false;
    }

    bool InternalVertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer) {
        if (m_Context == nullptr || vertexBuffer == nullptr || vertexBuffer->GetInternal() == nullptr) return false;
        m_VertexBuffer = vertexBuffer;

        m_VertexNumber = m_VertexBuffer->GetInternal()->Leng();
        return true;
    }

    Memory::Shared<VertexBuffer> InternalVertexArray::GetVertexBuffer() {
        return m_VertexBuffer;
    }

    InternalVertexBuffer* InternalVertexArray::GetInternalVertexBuffer() {
        //Log_nts.Debug();
        if (m_VertexBuffer == nullptr) return nullptr;
        //Log_nts.Debug();

        return m_VertexBuffer->GetInternal();
    }

    bool InternalVertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer) {
        if (m_Context == nullptr || indexBuffer == nullptr || indexBuffer->GetInternal() == nullptr) return false;
        m_IndexBuffer = indexBuffer;

        m_VertexNumber = m_IndexBuffer->GetInternal()->Leng();
        return true;
    }

    Memory::Shared<IndexBuffer> InternalVertexArray::GetIndexBuffer() {
        return m_IndexBuffer;
    }

    InternalIndexBuffer* InternalVertexArray::GetInternalIndexBuffer() {
        //Log_nts.Debug();
        if (m_IndexBuffer == nullptr) return nullptr;
        //Log_nts.Debug();

        return m_IndexBuffer->GetInternal();
    }

}  //  nkentseu

#endif