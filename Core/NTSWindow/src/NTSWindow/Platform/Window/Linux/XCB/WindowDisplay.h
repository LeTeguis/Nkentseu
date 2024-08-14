//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "NTSCore/System.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XCB)


#ifndef __NKENTSEU_WINDOW_DISPLAY_H__
#define __NKENTSEU_WINDOW_DISPLAY_H__

#include "Nkentseu/Core/WindowProperties.h"
#include <unordered_map>

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_icccm.h>

namespace nkentseu {
    class WindowEventInternal;

    typedef uint16 NtsWindowState;
    typedef enum NtsWindowStateMask : NtsWindowState {
        NKENTSEU_WINDOW_STATE_MASK_CLEAR          = 0,            /* state is empty, nothing is set */
        NKENTSEU_WINDOW_STATE_MASK_MODAL          = (1 << 0),     /* make modal */
        NKENTSEU_WINDOW_STATE_MASK_STICKY         = (1 << 1),     /* make it stick to top */
        NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_VERT = (1 << 2),     /* maximize it vertically */
        NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_HORZ = (1 << 3),     /* maximize it horizontally */
        NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED = NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_VERT | NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_HORZ,
        NKENTSEU_WINDOW_STATE_MASK_SHADED            = (1 << 4),  /* shade (just show titlebar) */
        NKENTSEU_WINDOW_STATE_MASK_SKIP_TASKBAR      = (1 << 5),  /* don't show on tasbar? */
        NKENTSEU_WINDOW_STATE_MASK_SKIP_PAGER        = (1 << 6),  /* don't show in ALT+TAB? */
        NKENTSEU_WINDOW_STATE_MASK_HIDDEN            = (1 << 7),  /* set window in hidden state */
        NKENTSEU_WINDOW_STATE_MASK_FULLSCREEN        = (1 << 8),  /* make fullscreen */
        NKENTSEU_WINDOW_STATE_MASK_ABOVE             = (1 << 9),  /* reorder to above */
        NKENTSEU_WINDOW_STATE_MASK_BELOW             = (1 << 10), /* reorder to below */
        NKENTSEU_WINDOW_STATE_MASK_DEMANDS_ATTENTION = (1 << 11), /* needs attention (blinking window) */
        NKENTSEU_WINDOW_STATE_MASK_FOCUSED           = (1 << 12)  /* set focused */
    } NtsWindowStateMask;

    typedef uint16 NtsWindowActionPermissions;
    typedef enum NtsWindowActionPermissionMask : NtsWindowActionPermissions {
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_CLEAR         = 0,          /* no actions allowed! */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MOVE          = (1 << 0),   /* is movable */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_RESIZE        = (1 << 1),   /* is resizable */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MINIMIZE      = (1 << 2),   /* is minimizable */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_SHADE         = (1 << 3),   /* can be shaded */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_STICK         = (1 << 4),   /* can be sticky to top */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE_HORZ = (1 << 5),   /* can be maximized horizontally */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE_VERT = (1 << 6),   /* can be maximized vertically */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE = NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE_VERT |
                                                    NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE_HORZ,
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_FULLSCREEN     = (1 << 7),  /* can go fullscreen */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_CHANGE_DESKTOP = (1 << 8),  /* can change desktop */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_CLOSE          = (1 << 9),  /* is closable */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_ABOVE          = (1 << 10), /* can reorder to above */
        NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_BELOW          = (1 << 11), /* can reorder to below */
    } NtsWindowActionMask;

    class NKENTSEU_API WindowDisplay
    {
        friend class WindowEventInternal;
        public:

            WindowDisplay();
            ~WindowDisplay();

            xcb_window_t windowHandle = (xcb_window_t)-1;
            bool isMouseInside = true;
            NtsWindowState state = NKENTSEU_WINDOW_STATE_MASK_CLEAR;
            NtsWindowActionPermissions permissions = NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_CLEAR;

            void SetState(NtsWindowState state);
            void SetPermission(NtsWindowActionPermissions permissions);
            
            friend class WindowEventInternal;

            bool Register(bool dbclk);
        private:
            bool m_IsRegistered = false;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_DISPLAY_H__

#endif    // NKENTSEU_PLATFORM_LINUX