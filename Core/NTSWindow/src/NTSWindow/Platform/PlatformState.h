//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 10:00:48 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_PLATFORM_STATE_H__
#define __NKENTSEU_PLATFORM_STATE_H__

#pragma once

#include <NTSCore/System.h>
#include <NTSCore/Nature/Base.h>

#include <vector>

#if defined(NKENTSEU_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

#if defined(NKENTSEU_PLATFORM_LINUX)

#include <X11/Xlib.h>
#include <X11/Xutil.h>

    #ifdef NKENTSEU_LINUX_WIN_API_XCB
        #include <X11/Xlib-xcb.h>
        #include <xcb/xcb.h>
    #elif NKENTSEU_LINUX_WIN_API_XLIB
        #include <X11/Xatom.h>
    #endif
#endif

#if defined(NKENTSEU_PLATFORM_ANDROID)
#endif

#if defined(NKENTSEU_PLATFORM_IOS)
#endif

#if defined(NKENTSEU_PLATFORM_MACOS)
#endif

#if defined(NKENTSEU_PLATFORM_NDS)
#endif

namespace nkentseu {

    #ifdef NKENTSEU_PLATFORM_LINUX
        #ifdef NKENTSEU_LINUX_WIN_API_XCB
            struct NKENTSEU_API XCBStateTools {
                xcb_connection_t     *connection;
                xcb_screen_iterator_t screen_iterator;

                /** @b Atom to be used to set other created atoms. */
                xcb_atom_t WM_PROTOCOLS;
                /** @b Atom we receive in client message events to recognize for close window events. */
                xcb_atom_t WM_DELETE_WINDOW;

                /**< @b WM_STATE : https://x.org/releases/X11R7.6/doc/xorg-docs/specs/ICCCM/icccm.html */
                xcb_atom_t WM_STATE;

                /**< @b _NET_WM_STATE : https://specifications.freedesktop.org/wm-spec/latest/ar01s05.html#idm46025198457920 */
                xcb_atom_t _NET_WM_STATE;
                xcb_atom_t _NET_WM_STATE_MODAL;
                xcb_atom_t _NET_WM_STATE_STICKY;
                xcb_atom_t _NET_WM_STATE_MAXIMIZED_VERT;
                xcb_atom_t _NET_WM_STATE_MAXIMIZED_HORZ;
                xcb_atom_t _NET_WM_STATE_SHADED;
                xcb_atom_t _NET_WM_STATE_SKIP_TASKBAR;
                xcb_atom_t _NET_WM_STATE_SKIP_PAGER;
                xcb_atom_t _NET_WM_STATE_HIDDEN;
                xcb_atom_t _NET_WM_STATE_FULLSCREEN;
                xcb_atom_t _NET_WM_STATE_ABOVE;
                xcb_atom_t _NET_WM_STATE_BELOW;
                xcb_atom_t _NET_WM_STATE_DEMANDS_ATTENTION;
                xcb_atom_t _NET_WM_STATE_FOCUSED;

                #define _NET_WM_STATE_REMOVE 0 /* remove/unset property */
                #define _NET_WM_STATE_ADD    1 /* add/set property */
                #define _NET_WM_STATE_TOGGLE 2 /* toggle property  */

                /**< @b _NET_WM_ALLOWED_ACTIONS : https://specifications.freedesktop.org/wm-spec/1.4/ar01s05.html */
                xcb_atom_t _NET_WM_ALLOWED_ACTIONS;
                xcb_atom_t _NET_WM_ACTION_MOVE;
                xcb_atom_t _NET_WM_ACTION_RESIZE;
                xcb_atom_t _NET_WM_ACTION_MINIMIZE;
                xcb_atom_t _NET_WM_ACTION_SHADE;
                xcb_atom_t _NET_WM_ACTION_STICK;
                xcb_atom_t _NET_WM_ACTION_MAXIMIZE_HORZ;
                xcb_atom_t _NET_WM_ACTION_MAXIMIZE_VERT;
                xcb_atom_t _NET_WM_ACTION_FULLSCREEN;
                xcb_atom_t _NET_WM_ACTION_CHANGE_DESKTOP;
                xcb_atom_t _NET_WM_ACTION_CLOSE;
                xcb_atom_t _NET_WM_ACTION_ABOVE;
                xcb_atom_t _NET_WM_ACTION_BELOW;

                /* to remove window borders */
                xcb_atom_t _MOTIF_WM_HINTS;

                /* atoms to manage window type 
                * https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html#idm45821695774192 */
                xcb_atom_t _NET_WM_WINDOW_TYPE;
                xcb_atom_t _NET_WM_WINDOW_TYPE_DESKTOP;
                xcb_atom_t _NET_WM_WINDOW_TYPE_DOCK;
                xcb_atom_t _NET_WM_WINDOW_TYPE_TOOLBAR;
                xcb_atom_t _NET_WM_WINDOW_TYPE_MENU;
                xcb_atom_t _NET_WM_WINDOW_TYPE_UTILITY;
                xcb_atom_t _NET_WM_WINDOW_TYPE_SPLASH;
                xcb_atom_t _NET_WM_WINDOW_TYPE_DIALOG;
                xcb_atom_t _NET_WM_WINDOW_TYPE_NORMAL;

                int32 Initialize();
                xcb_atom_t GetXcbAtom(const std::string& atomName);
            };
        #elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
            struct NKENTSEU_API WindowProtocolAtom{
                Atom deleteWindow;
                Atom fullScreen;
                Atom windowCreatedAtom;
                Atom wmMotifHints;
                Atom wmChangeState;
                Atom wmMinimize;
                Atom wmMaximizeHorz;
                Atom wmMaximizeVert;
            };
        #endif
    #endif

    class NKENTSEU_API PlatformState_ {
    public:
        static PlatformState_& Instance() {
            static PlatformState_ state;
            return state;
        }

        ARGV argv;

        #if defined(NKENTSEU_PLATFORM_WINDOWS)

        HINSTANCE   HInstance;
        HINSTANCE   HPrevInstance;
        LPSTR       LPCmdLine;
        int32       NCmdShow;

        int32 Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow);

        #elif defined(NKENTSEU_PLATFORM_ANDROID)

        android_app* App;

        int32 Init(android_app* app);

        #elif defined(NKENTSEU_PLATFORM_LINUX)
            #ifdef NKENTSEU_PLATFORM_LINUX_XCB
                ::Display* display = nullptr;

                int32 defaultScreen;

                int32 screenNumber = 0;
                xcb_connection_t* connection = nullptr;
                xcb_screen_t* screen = nullptr;
                XCBStateTools stateTools;
            #elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
                ::Display *display = nullptr;
                int32 screen;
                unsigned long rootWindow;
                ::Visual *visual;
                ::Colormap colormap;
                WindowProtocolAtom protocolAtom;
                void SetProtocol();
            #endif
                
            int32 Init(int argc, const char** argv);
            int32 Close();
        #else

        int32 Init(int argc, const char** argv);

        #endif

        PlatformState_();
    };

    #define PlatformState		PlatformState_::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_PLATFORM_STATE_H__