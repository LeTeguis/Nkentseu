//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:51 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_VERTEX_BUFFER_H__
#define __OPENGL_INTERNAL_VERTEX_BUFFER_H__

#pragma once

#include <System/System.h>
#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Core/NkentseuLogger.h"

#include "OpenGLUtils.h"

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API OpenglVertexBuffer : public VertexBuffer{
        public:
            OpenglVertexBuffer(Memory::Shared<Context> context);
            ~OpenglVertexBuffer();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;
            uint32 Leng() const override;

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout = {});
            bool Create(BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout = {});

            template <typename T>
            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<T>& vertices, const BufferLayout& bufferLayout = {}) {
                return Create(bufferUsage, vertices.data(), vertices.size(), bufferLayout);
            }

            bool Bind() const;
            bool Unbind() const;

            virtual bool SetData(void* data, usize size) override;

            uint32 GetBuffer() const;
            const BufferLayout& GetBufferLayaout();
            bool AttachToVAO(uint32 vao, bool useDsa);
        private:
            BufferLayout m_BufferLayout;
            BufferDataUsage::Code m_BufferUsage; 
            Memory::Shared<Context> m_Context;

            uint32 m_Size = 0;
            OpenglBuffer m_Buffer;
    };

}  //  nkentseu

#endif  // __INTERNAL_VERTEX_BUFFER_H__!