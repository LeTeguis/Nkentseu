//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "WindowDisplay.h"

#include "NTSWindow/Core/NkentseuLogger.h"

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

#define MWM_HINTS_FUNCTIONS (1L << 0)

#define MWM_HINTS_DECORATIONS   (1L << 1)

#define MWM_HINTS_INPUT_MODE    (1L << 2)

#define MWM_HINTS_ALL           (MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS|MWM_HINTS_INPUT_MODE) 

#define MWM_FUNC_ALL        (1L << 0)

#define MWM_FUNC_RESIZE     (1L << 1)

#define MWM_FUNC_MOVE       (1L << 2)

#define MWM_FUNC_MINIMIZE   (1L << 3)

#define MWM_FUNC_MAXIMIZE   (1L << 4)

#define MWM_FUNC_CLOSE      (1L << 5) 

#define MWM_DECOR_ALL       (1L << 0)

#define MWM_DECOR_BORDER    (1L << 1)

#define MWM_DECOR_RESIZEH   (1L << 2)

#define MWM_DECOR_TITLE     (1L << 3)

#define MWM_DECOR_MENU      (1L << 4)

#define MWM_DECOR_MINIMIZE  (1L << 5)

#define MWM_DECOR_MAXIMIZE  (1L << 6) 

#define MWM_INPUT_MODELESS          0

#define MWM_INPUT_PRIMARY_APPLICATION_MODAL 1

#define MWM_INPUT_SYSTEM_MODAL          2

#define MWM_INPUT_FULL_APPLICATION_MODAL    3

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

        struct 
        {
            long flags;
            long functions;
            long decorations;
            long inputmode;
        } prop;
  
        prop.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS | MWM_HINTS_INPUT_MODE;
        prop.decorations = 0;

        prop.inputmode = MWM_INPUT_PRIMARY_APPLICATION_MODAL;
        
        prop.functions = MWM_FUNC_MOVE;
        prop.functions |= MWM_FUNC_RESIZE;
        
        prop.decorations |= MWM_DECOR_TITLE;
        prop.decorations |= MWM_DECOR_BORDER;
        prop.decorations |= MWM_DECOR_RESIZEH;
            
        prop.decorations |= MWM_DECOR_MINIMIZE;
        prop.functions |= MWM_FUNC_MINIMIZE;
        
        prop.decorations |= MWM_DECOR_MAXIMIZE;
        prop.functions |= MWM_FUNC_MAXIMIZE;
        
        prop.functions |= MWM_FUNC_CLOSE;

        XSetWMProtocols(display, windowHandle, &PlatformState.protocolAtom.deleteWindow, 1);
        XSetWMProtocols(display, windowHandle, &PlatformState.protocolAtom.windowCreatedAtom, 1);
        XChangeProperty(display, windowHandle, PlatformState.protocolAtom.windowCreatedAtom, PlatformState.protocolAtom.windowCreatedAtom, 32, PropModeReplace, (unsigned char*) &PlatformState.protocolAtom.windowCreatedAtom, 4);
        //wmChangeState;
        //wmMinimize;
        //wmMaximizeHorz;
        //wmMaximizeVert;
	    XSync(display, False);
    }
}    // namespace nkentseu

#endif