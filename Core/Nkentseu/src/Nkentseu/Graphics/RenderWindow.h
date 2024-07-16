//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 05:28:16 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __RENDER_WINDOW_H__
#define __RENDER_WINDOW_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include "Renderer.h"

namespace nkentseu {
    
    class NKENTSEU_API RenderWindow : public Renderer {
        public:
            static Memory::Shared<RenderWindow> Create(Memory::Shared<Context> context);
            static Memory::Shared<RenderWindow> CreateInitialized(Memory::Shared<Context> context);
    };

}  //  nkentseu

#endif  // __RENDER_WINDOW_H__!