//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:47:38 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_INDEX_BUFFER_H__
#define __OPENGL_INTERNAL_INDEX_BUFFER_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/ShaderInfo.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "OpenglContext.h"

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API OpenglIndexBuffer : public IndexBuffer{
        public:
            OpenglIndexBuffer(Memory::Shared<Context> context);
            ~OpenglIndexBuffer();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;
            uint32 Leng() const override;

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<uint32>& indices);
            bool Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* vertices, uint32 leng);
            
            bool Bind();
            bool Unbind();

            DrawIndexType::Code GetIndexType() const;
        private:
            Memory::Shared<OpenglContext> m_Context = nullptr;

            uint32 m_ElementBufferObject = 0;
            BufferDataUsage::Code m_BufferUsage = BufferDataUsage::StaticDraw;
            DrawIndexType::Code m_IndexType = DrawIndexType::UInt32;

            uint32 m_Leng = 0;
    };

}  //  nkentseu

#endif  // __INTERNAL_INDEX_BUFFER_H__!