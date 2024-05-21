//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 09:36:16 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_BUFFER_H__
#define __VULKAN_INTERNAL_BUFFER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Nkentseu/Graphics/ShaderInfo.h>

namespace nkentseu {
    
    class NKENTSEU_API InternalBuffer {
        public:
            InternalBuffer(const BufferAttribute& attribut);
            ~InternalBuffer();

            bool SetAttibute(const BufferAttribute& attribut);
            bool Create();

            const BufferAttribute& GetAttribut();
        private:
    };

}  //  nkentseu

#endif

#endif  // __INTERNAL_BUFFER_H__!