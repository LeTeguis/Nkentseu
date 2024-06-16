//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/17/2024 at 9:38:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once
#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#ifndef __NKENTSEU_WIN32_EVENT_CODE_H__
#define __NKENTSEU_WIN32_EVENT_CODE_H__

#pragma once

#include "Nkentseu/Event/Keyboard.h"
#include "Nkentseu/Event/Mouse.h"
#include "Nkentseu/Event/GenericInput.h"
#include <Nkentseu/Event/EventState.h>

namespace nkentseu {
    class NKENTSEU_API Win32ECode
    {
        public:
            static uint64 ConvertKeycodeWinkey(uint64 code, bool keycode_to_vkey = false);
            // static Keyboard::Code WinkeyToKeycode(uint64 winkeycode);

            static Keyboard::Code WinkeyToKeycodeSpecial(uint64 winkeycode, bool shifDown);

            static uint64 ConvertScancodeWinkey(uint64 code, bool scancode_to_vkey = false);
            //static Keyboard::Code WinkeyToScancode(uint64 winkeycode);

            static Keyboard::Code WinkeyToScancodeSpecial(uint64 winkeycode, bool shifDown);

            static ModifierState ModifierStateToWinkey();
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_EVENT_CODE_H__

#endif