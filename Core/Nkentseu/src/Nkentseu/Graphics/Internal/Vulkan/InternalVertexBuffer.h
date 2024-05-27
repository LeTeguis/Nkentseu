//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:11:16 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_VERTEX_BUFFER_H__
#define __VULKAN_INTERNAL_VERTEX_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include "Nkentseu/Graphics/ShaderInfo.h"
#include "VulkanInternal.h"

#include <vulkan/vulkan.hpp>

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalVertexBuffer {
        public:
            InternalVertexBuffer();
            ~InternalVertexBuffer();

            bool Create(Context* context, BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout = {});
            bool Create(Context* context, BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout = {});

            template <typename T>
            bool Create(Context* context, BufferDataUsage::Code bufferUsage, const std::vector<T>& vertices, const BufferLayout& bufferLayout = {}) {
                return Create(context, bufferUsage, vertices.data(), vertices.size(), bufferLayout);
            }

            VulkanBuffer* GetBuffer();

            bool Destroy();
            uint32 Leng();
        private:
            VulkanBuffer m_VertexBufferObject;
            Context* m_Context = nullptr;

            uint32 m_Leng = 0;
    };

}  //  nkentseu
#endif

#endif  // __INTERNAL_VERTEX_BUFFER_H__!