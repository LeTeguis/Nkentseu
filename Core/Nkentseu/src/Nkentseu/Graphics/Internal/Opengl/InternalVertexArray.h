//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:11:05 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INTERNAL_VERTEX_ARRAY_H__
#define __INTERNAL_VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>
#include <glad/gl.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

namespace nkentseu {
    
    class NKENTSEU_API InternalVertexArray {
        public:
            InternalVertexArray();
            ~InternalVertexArray();

            bool Create();
            bool Destroy();
            bool Bind();
            bool Unbind();

            GLuint GetVAO();

            std::string ToString() const;
            friend std::string ToString(const InternalVertexArray& internalVertexArray);
        private:
            GLuint m_VertexArrayObject;
    };

}  //  nkentseu

#endif  // __INTERNAL_VERTEX_ARRAY_H__!

#endif