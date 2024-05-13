//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/23/2024 at 1:01:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "System/System.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XCB)

#ifndef __NKENTSEU_WINDOWEVENTCODE_H__
#define __NKENTSEU_WINDOWEVENTCODE_H__

#pragma once

#include "Nkentseu/Event/Keyboard.h"
#include "Nkentseu/Event/Mouse.h"
#include "Nkentseu/Event/GenericInput.h"
#include "Nkentseu/Event/EventState.h"

namespace nkentseu {
    class NKENTSEU_API WindowEventCode
    {
        public:
            static uint64 KeycodeToWinkey(Keyboard::Code code);
            static Keyboard::Code WinkeyToKeycode(uint64 winkeycode);

            static Keyboard::Code WinkeyToKeycodeSpecial(uint64 winkeycode, bool shifDown);

            static uint64 ScancodeToWinkey(Keyboard::Code code);
            static Keyboard::Code WinkeyToScancode(uint64 winkeycode);

            static Keyboard::Code WinkeyToScancodeSpecial(uint64 winkeycode, bool shifDown);

            static ModifierState ModifierStateToWinkey();
        private:
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOWEVENTCODE_H__

#endif