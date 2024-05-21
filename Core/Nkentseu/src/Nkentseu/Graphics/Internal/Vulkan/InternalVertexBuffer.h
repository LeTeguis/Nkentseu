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

namespace nkentseu {
    
    class NKENTSEU_API InternalVertexBuffer {
        public:
            InternalVertexBuffer();
            ~InternalVertexBuffer();

            bool Create(BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, uint32 numberElements);
            bool Destroy();
        private:
        protected:
    };

}  //  nkentseu
#endif

#endif  // __INTERNAL_VERTEX_BUFFER_H__!