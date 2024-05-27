//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:10:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VertexArray.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalVertexArray.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalVertexArray.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>
#include "Context.h"

namespace nkentseu {
    
    // Constructor
    VertexArray::VertexArray(Context* context) : m_Context(context){
        m_InternalVertexArray = Memory::Alloc<InternalVertexArray>(m_Context);
    }

    // Destructor
    VertexArray::~VertexArray() {
        // Ajoutez votre code de destructeur ici
    }

    Context* VertexArray::GetContext()
    {
        return nullptr;
    }

    bool VertexArray::SetContext(Context* context)
    {
        return false;
    }

    bool VertexArray::Create(const BufferLayout& bufferLayout)
    {
        if (m_InternalVertexArray == nullptr) {
            m_InternalVertexArray = Memory::Alloc<InternalVertexArray>(m_Context);

            if (m_InternalVertexArray == nullptr) {
                return false;
            }
        }
        return m_InternalVertexArray->Create(bufferLayout);
    }

    bool VertexArray::Create(uint32 vertexNumber)
    {
        if (m_InternalVertexArray == nullptr) {
            m_InternalVertexArray = Memory::Alloc<InternalVertexArray>(m_Context);

            if (m_InternalVertexArray == nullptr) {
                return false;
            }
        }
        return m_InternalVertexArray->Create(vertexNumber);
    }

    bool VertexArray::Destroy()
    {
        if (m_InternalVertexArray == nullptr) return false;
        return m_InternalVertexArray->Destroy();
    }

    bool VertexArray::SetVertexBuffer(Memory::Shared<VertexBuffer> vertexBuffer)
    {
        if (m_InternalVertexArray == nullptr) return false;
        return m_InternalVertexArray->SetVertexBuffer(vertexBuffer);
    }

    Memory::Shared<VertexBuffer> VertexArray::GetVertexBuffer()
    {
        if (m_InternalVertexArray == nullptr || m_InternalVertexArray->GetVertexBuffer() == nullptr) return nullptr;
        return m_InternalVertexArray->GetVertexBuffer();
    }

    InternalVertexBuffer* VertexArray::GetInternalVertexBuffer()
    {
        if (m_InternalVertexArray == nullptr || m_InternalVertexArray->GetInternalVertexBuffer() == nullptr) return nullptr;
        return m_InternalVertexArray->GetInternalVertexBuffer();
    }

    uint32 VertexArray::GetVertexNumber()
    {
        if (m_InternalVertexArray == nullptr) {
            return 0;
        }
        return m_InternalVertexArray->GetVertexNumber();
    }

    bool VertexArray::SetIndexBuffer(Memory::Shared<IndexBuffer> indexBuffer)
    {
        if (m_InternalVertexArray == nullptr) return false;
        return m_InternalVertexArray->SetIndexBuffer(indexBuffer);
    }

    Memory::Shared<IndexBuffer> VertexArray::GetIndexBuffer()
    {
        if (m_InternalVertexArray == nullptr || m_InternalVertexArray->GetIndexBuffer() == nullptr) return nullptr;
        return m_InternalVertexArray->GetIndexBuffer();
    }

    InternalIndexBuffer* VertexArray::GetInternalIndexBuffer()
    {
        if (m_InternalVertexArray == nullptr || m_InternalVertexArray->GetInternalIndexBuffer() == nullptr) return nullptr;
        return m_InternalVertexArray->GetInternalIndexBuffer();
    }

    InternalVertexArray* VertexArray::GetInternal()
    {
        if (m_InternalVertexArray == nullptr) return nullptr;

        return m_InternalVertexArray.get();
    }
}  //  nkentseu