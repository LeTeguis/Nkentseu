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
    class InternalVertexBuffer;
    class InternalIndexBuffer;
    
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

    class NKENTSEU_API IndexBuffer {
    public:
        IndexBuffer();
        IndexBuffer(const BufferAttribute& attribut);
        ~IndexBuffer();

        InternalIndexBuffer* GetInternal();

        bool SetAttibute(const BufferAttribute& attribut);

        bool Create(uint32 indices[]);
        bool ChangeIndices(uint32 indices[]);
        bool Destroy();

        uint32* GetIndices();

        const BufferAttribute& GetAttribut();

        std::string ToString() const;
        friend std::string ToString(const IndexBuffer& buffer);
    private:
        Memory::Shared<InternalIndexBuffer> m_InternalIndexBuffer = null_pointer;
    };

    class NKENTSEU_API VertexBuffer {
        public:
            VertexBuffer();
            VertexBuffer(const BufferAttribute& attribut);
            ~VertexBuffer();

            InternalVertexBuffer* GetInternal();

            bool SetAttibute(const BufferAttribute& attribut);
            bool Create(float32 vertices[]);
            bool ChangeVertices(float32 vertices[]);
            bool AddIndexBuffer(Memory::Shared<IndexBuffer> buff);
            bool Destroy();

            IndexBuffer* GetIndexBuffer(usize index);
            usize GetIndexBufferSize();
            VertexArray* GetVertexArray();

            const BufferAttribute& GetAttribut();

            std::string ToString() const;
            friend std::string ToString(const VertexBuffer& buffer);
        private:
            Memory::Shared<InternalVertexBuffer> m_InternalVertexBuffer = null_pointer;
    };

}  //  nkentseu

#endif  // __BUFFER_H__!