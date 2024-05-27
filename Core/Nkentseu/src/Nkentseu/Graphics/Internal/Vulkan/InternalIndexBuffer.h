//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:21:33 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_INDEX_BUFFER_H__
#define __VULKAN_INTERNAL_INDEX_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalIndexBuffer {
        public:
            InternalIndexBuffer();
            ~InternalIndexBuffer();

            bool Create(Context* context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices, const BufferLayout& bufferLayout = {});
            bool Destroy();
        private:
            Context* m_Context = nullptr;
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_INDEX_BUFFER_H__!