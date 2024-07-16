//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 12:47:17 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __RENDER_PRIMITIVE_H__
#define __RENDER_PRIMITIVE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>

namespace nkentseu {
    
    class NKENTSEU_API RenderPrimitive1 {
        public:
            enum Enum
            {
                Points,        
                Lines,         
                LineStrip,     
                Triangles,     
                TriangleStrip,
                TriangleFan,
                Paths
            };
    };

}  //  nkentseu

#endif  // __PRIMITIVE_TYPE_H__!