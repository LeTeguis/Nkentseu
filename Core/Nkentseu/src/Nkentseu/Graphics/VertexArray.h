//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:05 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

namespace nkentseu {
    class InternalVertexArray;
    
    class NKENTSEU_API VertexArray {
        public:
            VertexArray();
            ~VertexArray();

            bool Create();
            bool Destroy();

            InternalVertexArray* GetInternal();

            std::string ToString() const;
            friend std::string ToString(const VertexArray& vertexArray);
        private:
            Memory::Shared<InternalVertexArray> m_InternalVertexArray = null_pointer;
    };

}  //  nkentseu

#endif  // __VERTEX_ARRAY_H__!