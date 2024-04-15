//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_LINUX


#ifndef __NKENTSEU_WINDOW_DISPLAY_H__
#define __NKENTSEU_WINDOW_DISPLAY_H__

extern "C" {
#include <X11/Xlib.h>
}

#include "Nkentseu/Core/WindowProperties.h"
#include <unordered_map>

namespace nkentseu {
    class WindowInternal;
    class WindowEventInternal;

    class NKENTSEU_API WindowDisplay
    {
        friend class WindowEventInternal;
        public:
            WindowDisplay();
            ~WindowDisplay();

            WindowProperties windowProperties;
            bool isCursorVisible;
            bool isCursorGrabbed;
            bool isMouseInside = true;
            Screen* screen;
            int screenId;

            ::Display* windowHandle;
            const Window windowRoot;

            class nkentseu::Window* windowSuper;
            friend class WindowEventInternal;

            bool Register(bool dbclk);
            bool UnRegister(bool dbclk);
            void StaticNative(class nkentseu::Window*);
        private:
            bool m_IsRegistered = false;
            static WindowDisplay* GetCurrent(::Display display);
    };

    static thread_local WindowDisplay* currentWindowDisplay = nullptr;
    static thread_local std::unordered_map<::Display, WindowDisplay*> windowHandleMap;
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_DISPLAY_H__

#endif    // NKENTSEU_PLATFORM_LINUX