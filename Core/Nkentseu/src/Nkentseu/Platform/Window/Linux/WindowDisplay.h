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
    class WindowInternal;
    class WindowEventInternal;
    class Window;

    class NKENTSEU_API WindowDisplay
    {
        friend class WindowEventInternal;
        public:

            WindowDisplay();
            ~WindowDisplay();

            xcb_screen_t* screenHandle;
            xcb_window_t windowHandle;
            xcb_intern_atom_cookie_t internAtomCookie;
            xcb_atom_t windowManagerProtocolsProperty;
            xcb_atom_t windowManagerWindowDeleteProtocol;

            WindowProperties windowProperties;

            class Window* windowSuper;
            friend class WindowEventInternal;

            bool Register(bool dbclk);
            void StaticNative(class Window*);
        private:
            bool m_IsRegistered = false;
            static WindowDisplay* GetCurrent(xcb_window_t window);
            void InterneAtomReply(bool type, bool onlyIfExist, const std::string& name, xcb_generic_error_t** generic);
    };

    static thread_local WindowDisplay* currentWindowDisplay = nullptr;
    static thread_local std::unordered_map<xcb_window_t, WindowDisplay*> windowHandleMap;
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_DISPLAY_H__

#endif    // NKENTSEU_PLATFORM_LINUX