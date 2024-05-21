//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:51 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_BUFFER_H__
#define __OPENGL_INTERNAL_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <System/Definitions/Memory.h>
#include <glad/gl.h>
#include <vector>

#include "InternalVertexArray.h"

namespace nkentseu {

    class IndexBuffer;
    
    class NKENTSEU_API InternalBuffer {
        public:
            InternalBuffer(const BufferAttribute& attribut);
            ~InternalBuffer();

            bool SetAttibute(const BufferAttribute& attribut);
            bool Create();
            bool Bind();
            bool Unbind();

            const BufferAttribute& GetAttribut();

            std::string ToString() const;
            friend std::string ToString(const InternalBuffer& internalBuffer);
        private:
            BufferAttribute m_Attribut;
    };

}  //  nkentseu

#endif  // __INTERNAL_BUFFER_H__!

#endif