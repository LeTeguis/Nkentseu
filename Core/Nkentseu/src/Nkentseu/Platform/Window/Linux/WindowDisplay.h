//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_LINUX


#ifndef __NKENTSEU_WINDOW_DISPLAY_H__
#define __NKENTSEU_WINDOW_DISPLAY_H__

#include "Nkentseu/Core/WindowProperties.h"
#include <unordered_map>

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_icccm.h>

namespace nkentseu {
    class WindowEventInternal;

    class NKENTSEU_API WindowDisplay
    {
        friend class WindowEventInternal;
        public:

            WindowDisplay();
            ~WindowDisplay();

            xcb_screen_t* screenHandle;
            xcb_window_t windowHandle;
            bool isMouseInside = true;
            
            friend class WindowEventInternal;

            bool Register(bool dbclk);
        private:
            bool m_IsRegistered = false;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_DISPLAY_H__

#endif    // NKENTSEU_PLATFORM_LINUX