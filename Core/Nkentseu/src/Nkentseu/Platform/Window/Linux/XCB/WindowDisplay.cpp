//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowDisplay.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XCB)

#include "WindowEventInternal.h"
#include "Nkentseu/Platform/PlatformState.h"

/* xcb/x11 includes */
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>

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
        m_IsRegistered = true;

        xcb_connection_t* connection = PlatformState.connection;
        xcb_screen_t *screenHandle = PlatformState.screen;

        if (screenHandle == nullptr || connection == nullptr) {
            Log_nts.Error("Failed to get screen iterator and connexion");
            return false;
        }
        windowHandle = (xcb_window_t)-1;
        windowHandle = xcb_generate_id(connection);

        if (windowHandle == (xcb_window_t)-1) {
            Log_nts.Error("Failed to generate window ID");
            return 1;
        }

        return m_IsRegistered;
    }

    void WindowDisplay::SetState(NtsWindowState state){
        if (!m_IsRegistered) return;
        xcb_connection_t* connection = PlatformState.connection;
        xcb_screen_t *screenHandle = PlatformState.screen;

        if (screenHandle == nullptr || connection == nullptr) {
            Log_nts.Error("Failed to get screen iterator and connexion");
            return;
        }
        if (windowHandle == (xcb_window_t)-1) {
            Log_nts.Error("Failed to generate window ID");
            return;
        }

        XCBStateTools& stateTools = PlatformState.stateTools;

        /* create pairing of atom with it's mask */
        struct {
            xcb_atom_t        atom;
            NtsWindowStateMask mask;
        } atoms[] = {
            {            stateTools._NET_WM_STATE_MODAL,             NKENTSEU_WINDOW_STATE_MASK_MODAL},
            {           stateTools._NET_WM_STATE_STICKY,            NKENTSEU_WINDOW_STATE_MASK_STICKY},
            {   stateTools._NET_WM_STATE_MAXIMIZED_VERT,    NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_VERT},
            {   stateTools._NET_WM_STATE_MAXIMIZED_HORZ,    NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_HORZ},
            {           stateTools._NET_WM_STATE_SHADED,            NKENTSEU_WINDOW_STATE_MASK_SHADED},
            {     stateTools._NET_WM_STATE_SKIP_TASKBAR,      NKENTSEU_WINDOW_STATE_MASK_SKIP_TASKBAR},
            {       stateTools._NET_WM_STATE_SKIP_PAGER,        NKENTSEU_WINDOW_STATE_MASK_SKIP_PAGER},
            {           stateTools._NET_WM_STATE_HIDDEN,            NKENTSEU_WINDOW_STATE_MASK_HIDDEN},
            {       stateTools._NET_WM_STATE_FULLSCREEN,        NKENTSEU_WINDOW_STATE_MASK_FULLSCREEN},
            {            stateTools._NET_WM_STATE_ABOVE,             NKENTSEU_WINDOW_STATE_MASK_ABOVE},
            {            stateTools._NET_WM_STATE_BELOW,             NKENTSEU_WINDOW_STATE_MASK_BELOW},
            {stateTools._NET_WM_STATE_DEMANDS_ATTENTION, NKENTSEU_WINDOW_STATE_MASK_DEMANDS_ATTENTION},
            {          stateTools._NET_WM_STATE_FOCUSED,           NKENTSEU_WINDOW_STATE_MASK_FOCUSED},
        };

        /* reset _NET_WM_STATE atom array */
        xcb_change_property (
            stateTools.connection,    /* conn*/
            XCB_PROP_MODE_REPLACE,  /* mode */
            windowHandle,    /* window */
            stateTools._NET_WM_STATE, /* property */
            XCB_ATOM_ATOM,          /* type */
            32,                     /* format */
            0,                      /* length */
            Null                    /* data */
        );

        for (nts_size i = 0; i < ARRAY_SIZE(atoms); i++) {
            /* append to state if mask is set */
            if (state & atoms[i].mask) {
                xcb_change_property (
                    stateTools.connection,    /* conn*/
                    XCB_PROP_MODE_APPEND,   /* mode */
                    windowHandle,    /* window */
                    stateTools._NET_WM_STATE, /* property */
                    XCB_ATOM_ATOM,          /* type */
                    32,                     /* format */
                    1,                      /* length */
                    &atoms[i].atom          /* data */
                );
            }

            /* fill data array */
            xcb_client_message_data_t data;
            data.data32[0] = (state & atoms[i].mask) ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
            data.data32[1] = atoms[i].atom;
            data.data32[2] = XCB_ATOM_NONE; /* source indicator */

            /* prepare event and send event */
            xcb_client_message_event_t payload;
            {
                payload.response_type = XCB_CLIENT_MESSAGE;
                payload.type          = stateTools._NET_WM_STATE;
                payload.format        = 32;
                payload.window        = windowHandle;
                payload.data          = data;
            }

            xcb_send_event (
                stateTools.connection,
                False,               /* whether to propagate the event or not */
                windowHandle, /* destination window */
                XCB_EVENT_MASK_STRUCTURE_NOTIFY,
                (char*)&payload
            );
        }

        xcb_flush (stateTools.connection);
        this->state = state;
    }

    void WindowDisplay::SetPermission(NtsWindowActionPermissions permissions){
        if (!m_IsRegistered) return;
        xcb_connection_t* connection = PlatformState.connection;
        xcb_screen_t *screenHandle = PlatformState.screen;

        if (screenHandle == nullptr || connection == nullptr) {
            Log_nts.Error("Failed to get screen iterator and connexion");
            return;
        }
        if (windowHandle == (xcb_window_t)-1) {
            Log_nts.Error("Failed to generate window ID");
            return;
        }

        XCBStateTools& stateTools = PlatformState.stateTools;

        struct {
            xcb_atom_t                atom;
            NtsWindowActionPermissions mask;
        } atoms[] = {
            {          stateTools._NET_WM_ACTION_MOVE,           NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MOVE},
            {        stateTools._NET_WM_ACTION_RESIZE,         NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_RESIZE},
            {      stateTools._NET_WM_ACTION_MINIMIZE,       NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MINIMIZE},
            {         stateTools._NET_WM_ACTION_SHADE,          NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_SHADE},
            {         stateTools._NET_WM_ACTION_STICK,          NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_STICK},
            { stateTools._NET_WM_ACTION_MAXIMIZE_HORZ,  NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE_HORZ},
            { stateTools._NET_WM_ACTION_MAXIMIZE_VERT,  NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_MAXIMIZE_VERT},
            {    stateTools._NET_WM_ACTION_FULLSCREEN,     NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_FULLSCREEN},
            {stateTools._NET_WM_ACTION_CHANGE_DESKTOP, NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_CHANGE_DESKTOP},
            {         stateTools._NET_WM_ACTION_CLOSE,          NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_CLOSE},
            {         stateTools._NET_WM_ACTION_ABOVE,          NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_ABOVE},
            {         stateTools._NET_WM_ACTION_BELOW,          NKENTSEU_WINDOW_ACTION_PERMISSION_MASK_BELOW},
        };

        /* reset _NET_WM_ALLOWED_ACTIONS atom array */
        xcb_change_property (
            stateTools.connection,              /* conn*/
            XCB_PROP_MODE_REPLACE,            /* mode */
            windowHandle,              /* window */
            stateTools._NET_WM_ALLOWED_ACTIONS, /* property */
            XCB_ATOM_ATOM,                    /* type */
            32,                               /* format */
            0,                                /* length */
            Null                              /* data */
        );

        /* append permissions to permissions array */
        for (nts_size i = 0; i < ARRAY_SIZE (atoms); i++) {
            if (permissions & atoms[i].mask) {
                xcb_change_property (
                    stateTools.connection,              /* conn*/
                    XCB_PROP_MODE_APPEND,             /* mode */
                    windowHandle,              /* window */
                    stateTools._NET_WM_ALLOWED_ACTIONS, /* property */
                    XCB_ATOM_ATOM,                    /* type */
                    32,                               /* format */
                    1,                                /* length */
                    &atoms[i].atom                    /* data */
                );
            }
        }

        xcb_flush (stateTools.connection);

        this->permissions = permissions;
    }
}    // namespace nkentseu

#endif