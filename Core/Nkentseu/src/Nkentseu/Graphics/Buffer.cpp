//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:09:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Buffer.h"
#include <Logger/Formatter.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include "Internal/Opengl/InternalContext.h"
#include "Internal/Opengl/InternalBuffer.h"
#elif defined(NKENTSEU_GRAPHICS_API_VULKAN)
#include "Internal/Vulkan/InternalContext.h"
#include "Internal/Vulkan/InternalBuffer.h"
#endif
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    
    // Constructor
    Buffer::Buffer(const BufferAttribute& attribut) {
        // Ajoutez votre code de constructeur ici
        if (m_InternalBuffer == nullptr) {
            m_InternalBuffer = Memory::Alloc<InternalBuffer>(attribut);
        }
    }

    // Destructor
    Buffer::~Buffer() {
        // Ajoutez votre code de destructeur ici
    }

    InternalBuffer* Buffer::GetInternal()
    {
        if (m_InternalBuffer == nullptr) return nullptr;

        return m_InternalBuffer.get();
    }

    bool Buffer::SetAttibute(const BufferAttribute& attribut)
    {
        if (m_InternalBuffer == nullptr) return false;
        return m_InternalBuffer->SetAttibute(attribut);
    }

    bool Buffer::Create()
    {
        if (m_InternalBuffer == nullptr) return false;
        return m_InternalBuffer->Create();
    }

    const BufferAttribute& Buffer::GetAttribut()
    {
        static const BufferAttribute attribut = {};
        if (m_InternalBuffer == nullptr) return attribut;

        return m_InternalBuffer->GetAttribut();
    }

    std::string Buffer::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const Buffer& buffer) {
        return buffer.ToString();
    }

    std::string ToString(const IndexBuffer& buffer)
    {
        return std::string();
    }

    std::string ToString(const VertexBuffer& buffer)
    {
        return std::string();
    }

}  //  nkentseu