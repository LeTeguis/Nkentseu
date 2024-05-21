//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:21:33 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_INDEX_BUFFER_H__
#define __VULKAN_INTERNAL_INDEX_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    
    class NKENTSEU_API InternalIndexBuffer {
        public:
            InternalIndexBuffer();
            ~InternalIndexBuffer();

            bool Create(BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const std::vector<uint32>& indices);
            bool Destroy();
        private:
        protected:
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_INDEX_BUFFER_H__!