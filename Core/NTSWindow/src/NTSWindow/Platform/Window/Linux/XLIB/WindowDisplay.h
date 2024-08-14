//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "NTSCore/System.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XLIB)


#ifndef __NKENTSEU_WINDOW_DISPLAY_H__
#define __NKENTSEU_WINDOW_DISPLAY_H__

#include "Nkentseu/Core/WindowProperties.h"
#include <unordered_map>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace nkentseu {
    class WindowEventInternal;

    class NKENTSEU_API WindowDisplay
    {
        friend class WindowEventInternal;
        public:

            WindowDisplay();
            ~WindowDisplay();

            unsigned long windowHandle;
            ::XSetWindowAttributes attributes;
            bool isMouseInside = true;
            
            friend class WindowEventInternal;

            bool Register(bool dbclk);
            void SetProtocol();
        private:
            bool m_IsRegistered = false;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_DISPLAY_H__

#endif    // NKENTSEU_PLATFORM_LINUX