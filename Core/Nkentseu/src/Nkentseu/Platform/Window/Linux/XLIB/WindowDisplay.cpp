//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowDisplay.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XLIB)

#include "WindowEventInternal.h"
#include "Nkentseu/Platform/PlatformState.h"

/* for xk-definitions */
#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#define XK_LATIN2
#define XK_LATIN3
#define XK_LATIN4
#define XK_CYRILLIC
#define XK_GREEK
#define XK_ARMENIAN
#include <X11/keysymdef.h>

namespace nkentseu {
    /* global state */
    WindowDisplay::WindowDisplay(){
    }

    WindowDisplay::~WindowDisplay(){
    }

    bool WindowDisplay::Register(bool dbclk) {
        if (m_IsRegistered) return false;

        if (PlatformState.display == nullptr || PlatformState.visual == nullptr || PlatformState.rootWindow == 0 || PlatformState.colormap == 0) {
            Log_nts.Error("Failed to get display, visual, root window, colormap");
            return false;
        }

        m_IsRegistered = true;

        return m_IsRegistered;
    }

    void WindowDisplay::SetProtocol(){
        ::Display *display = PlatformState.display;
        int32 screen = PlatformState.screen;
        ::Window rootWindow = PlatformState.rootWindow;
        ::Visual *visual = PlatformState.visual;
        ::Colormap colormap = PlatformState.colormap;

        if (display == nullptr || rootWindow == 0 || visual == nullptr || colormap == 0) {
            Log_nts.Error("Failed to get screen iterator and connexion");
            return;
        }

        XSetWMProtocols(PlatformState.display, windowHandle, &PlatformState.protocolAtom.deleteWindow, 1);
    }
}    // namespace nkentseu

#endif