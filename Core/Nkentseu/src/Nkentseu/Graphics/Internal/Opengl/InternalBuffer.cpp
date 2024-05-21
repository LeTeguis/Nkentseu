//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-16 at 10:10:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalBuffer.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Logger/Formatter.h>
#include "Nkentseu/Graphics/Buffer.h"
#include <Nkentseu/Core/NkentseuLogger.h>
#include "InternalContext.h"

namespace nkentseu {
    
    // Constructor
    InternalBuffer::InternalBuffer(const BufferAttribute& attribut) : m_Attribut(attribut) {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalBuffer::~InternalBuffer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalBuffer::SetAttibute(const BufferAttribute& attribut)
    {
        m_Attribut = attribut;
        return true;
    }

    bool InternalBuffer::Create()
    {
        return false;
    }

    bool InternalBuffer::Bind()
    {
        return false;
    }

    bool InternalBuffer::Unbind()
    {
        return false;
    }

    const BufferAttribute& InternalBuffer::GetAttribut()
    {
        return m_Attribut;
    }

    std::string InternalBuffer::ToString() const {
        return FORMATTER.Format(""); // mettez votre formatteur To string entre les guillemets
    }

    std::string ToString(const InternalBuffer& internalBuffer) {
        return internalBuffer.ToString();
    }

}  //  nkentseu

#endif