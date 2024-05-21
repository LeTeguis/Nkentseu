//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:09:21 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __BUFFER_H__
#define __BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include "ShaderInfo.h"
#include "VertexArray.h"

namespace nkentseu {
    class InternalBuffer;
    
    class NKENTSEU_API Buffer {
        public:
            Buffer(const BufferAttribute& attribut);
            ~Buffer();

            InternalBuffer* GetInternal();

            bool SetAttibute(const BufferAttribute& attribut);
            bool Create();

            const BufferAttribute& GetAttribut();

            std::string ToString() const;
            friend std::string ToString(const Buffer& buffer);
        private:
            Memory::Shared<InternalBuffer> m_InternalBuffer = null_pointer;
    };

}  //  nkentseu

#endif  // __BUFFER_H__!