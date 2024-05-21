//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:55:46 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_UTILS_H__
#define __OPENGL_UTILS_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include <Nkentseu/Core/NkentseuLogger.h>
#include "InternalContext.h"
#include "Nkentseu/Graphics/ShaderInfo.h"

namespace nkentseu {
    class NKENTSEU_API GLConvert {
        public:
            static uint32 ShaderType(ShaderDataType::Code type);
            static uint32 UsageType(BufferDataUsage::Code type);
            static uint32 DrawModeType(DrawMode::Code mode);
            static uint32 DrawContentModeType(DrawContentMode::Code contentMode);
            static uint32 VerticesPerType(uint32 vertexType);
            static uint32 VertexType(DrawVertexType::Code drawVertex);
            static uint32 IndexType(DrawIndexType::Code drawIndex);
    };
}  //  nkentseu

#endif

#endif  // __OPEN_G_L_UTILS_H__!