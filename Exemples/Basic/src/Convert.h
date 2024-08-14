//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-26 at 09:09:51 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_CONVERT_H__
#define __NKENTSEU_CONVERT_H__

#pragma once

#include <NTSCore/System.h>
#include <NTSGraphics/Platform/WindowInfo.h>
#include <NTSWindow/Core/Window.h>

namespace nkentseu {
    
    struct Convert {
        static Memory::Shared<nkentseu::WindowInfo> ConvertWindowInfoFrom(Memory::Shared<nkentseu::Window> window);
    };

}  //  nkentseu

#endif  // __NKENTSEU_CONVERT_H__!