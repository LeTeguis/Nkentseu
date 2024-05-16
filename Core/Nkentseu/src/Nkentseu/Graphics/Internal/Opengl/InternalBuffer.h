//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:51 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INTERNAL_BUFFER_H__
#define __INTERNAL_BUFFER_H__

#pragma once

#include <System/System.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <System/Definitions/Memory.h>
#include <glad/gl.h>
#include <vector>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include "InternalVertexArray.h"

namespace nkentseu {

    class IndexBuffer;
    class VertexBuffer;
    class VertexArray;
    
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

    class NKENTSEU_API InternalIndexBuffer {
    public:
        InternalIndexBuffer() {}
        InternalIndexBuffer(const BufferAttribute& attribut);
        ~InternalIndexBuffer();

        bool SetAttibute(const BufferAttribute& attribut);

        bool Create(uint32 indices[]);
        bool Create(const std::vector<uint32>& indices);
        bool ChangeIndices(uint32 indices[]);
        bool ChangeIndices(const std::vector<uint32>& indices);
        bool Destroy();

        int32 GetLength();

        bool Bind();
        bool Unbind();

        std::vector<uint32> GetIndices();

        GLuint GetBuffer();

        const BufferAttribute& GetAttribut();

        std::string ToString() const;
        friend std::string ToString(const InternalIndexBuffer& buffer);
    private:
        BufferAttribute m_Attribut;
        GLuint m_Buffer;
        int32 m_Length;

        std::vector<uint32> m_Indices;
    };

    class NKENTSEU_API InternalVertexBuffer {
    public:
        InternalVertexBuffer() {}
        InternalVertexBuffer(const BufferAttribute& attribut);
        ~InternalVertexBuffer();

        bool SetAttibute(const BufferAttribute& attribut);
        bool Create(float32 vertices[]);
        bool ChangeVertices(float32 vertices[]);
        bool AddIndexBuffer(Memory::Shared<IndexBuffer> buff);
        bool Destroy();

        bool Bind();
        bool Unbind();

        GLuint GetBuffer();

        IndexBuffer* GetIndexBuffer(usize index);
        usize GetIndexBufferSize();
        VertexArray* GetVertexArray();

        InternalIndexBuffer* GetInternalIndexBuffer(usize index);
        InternalVertexArray* GetInternalVertexArray();

        const BufferAttribute& GetAttribut();

        std::string ToString() const;
        friend std::string ToString(const InternalVertexBuffer& buffer);
    private:
        bool SetIndexBuffer();

        std::vector< Memory::Shared<IndexBuffer> > m_IndexBuffers;
        Memory::Shared<VertexArray> m_VertexArray;

        GLuint m_Buffer;
        std::vector<float32> m_CurrentVertices;
        BufferAttribute m_Attribut;
    };

}  //  nkentseu

#endif  // __INTERNAL_BUFFER_H__!

#endif