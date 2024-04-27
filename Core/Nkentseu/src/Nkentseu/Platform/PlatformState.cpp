//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 10:00:48 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "PlatformState.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#ifdef NKENTSEU_PLATFORM_LINUX
/* atom names */
#define WM_PROTOCOLS_ATOM_NAME     "WM_PROTOCOLS"
#define WM_DELETE_WINDOW_ATOM_NAME "WM_DELETE_WINDOW"
#define WM_STATE_ATOM_NAME         "WM_STATE"

#define _NET_WM_STATE_ATOM_NAME                   "_NET_WM_STATE"
#define _NET_WM_STATE_MODAL_ATOM_NAME             "_NET_WM_STATE_MODAL"
#define _NET_WM_STATE_STICKY_ATOM_NAME            "_NET_WM_STATE_STICKY"
#define _NET_WM_STATE_MAXIMIZED_VERT_ATOM_NAME    "_NET_WM_STATE_MAXIMIZED_VERT"
#define _NET_WM_STATE_MAXIMIZED_HORZ_ATOM_NAME    "_NET_WM_STATE_MAXIMIZED_HORZ"
#define _NET_WM_STATE_SHADED_ATOM_NAME            "_NET_WM_STATE_SHADED"
#define _NET_WM_STATE_SKIP_TASKBAR_ATOM_NAME      "_NET_WM_STATE_SKIP_TASKBAR"
#define _NET_WM_STATE_SKIP_PAGER_ATOM_NAME        "_NET_WM_STATE_SKIP_PAGER"
#define _NET_WM_STATE_HIDDEN_ATOM_NAME            "_NET_WM_STATE_HIDDEN"
#define _NET_WM_STATE_FULLSCREEN_ATOM_NAME        "_NET_WM_STATE_FULLSCREEN"
#define _NET_WM_STATE_ABOVE_ATOM_NAME             "_NET_WM_STATE_ABOVE"
#define _NET_WM_STATE_BELOW_ATOM_NAME             "_NET_WM_STATE_BELOW"
#define _NET_WM_STATE_DEMANDS_ATTENTION_ATOM_NAME "_NET_WM_STATE_DEMANDS_ATTENTION"
#define _NET_WM_STATE_FOCUSED_ATOM_NAME           "_NET_WM_STATE_FOCUSED"

#define _NET_WM_ALLOWED_ACTIONS_ATOM_NAME       "_NET_WM_ALLOWED_ACTIONS"
#define _NET_WM_ACTION_MOVE_ATOM_NAME           "_NET_WM_ACTION_MOVE"
#define _NET_WM_ACTION_RESIZE_ATOM_NAME         "_NET_WM_ACTION_RESIZE"
#define _NET_WM_ACTION_MINIMIZE_ATOM_NAME       "_NET_WM_ACTION_MINIMIZE"
#define _NET_WM_ACTION_SHADE_ATOM_NAME          "_NET_WM_ACTION_SHADE"
#define _NET_WM_ACTION_STICK_ATOM_NAME          "_NET_WM_ACTION_STICK"
#define _NET_WM_ACTION_MAXIMIZE_HORZ_ATOM_NAME  "_NET_WM_ACTION_MAXIMIZE_HORZ"
#define _NET_WM_ACTION_MAXIMIZE_VERT_ATOM_NAME  "_NET_WM_ACTION_MAXIMIZE_VERT"
#define _NET_WM_ACTION_FULLSCREEN_ATOM_NAME     "_NET_WM_ACTION_FULLSCREEN"
#define _NET_WM_ACTION_CHANGE_DESKTOP_ATOM_NAME "_NET_WM_ACTION_CHANGE_DESKTOP"
#define _NET_WM_ACTION_CLOSE_ATOM_NAME          "_NET_WM_ACTION_CLOSE"
#define _NET_WM_ACTION_ABOVE_ATOM_NAME          "_NET_WM_ACTION_ABOVE"
#define _NET_WM_ACTION_BELOW_ATOM_NAME          "_NET_WM_ACTION_BELOW"

#define _MOTIF_WM_HINTS_ATOM_NAME               "_MOTIF_WM_HINTS"

#define _NET_WM_WINDOW_TYPE_ATOM_NAME         "_NET_WM_WINDOW_TYPE"
#define _NET_WM_WINDOW_TYPE_DESKTOP_ATOM_NAME "_NET_WM_WINDOW_TYPE_DESKTOP"
#define _NET_WM_WINDOW_TYPE_DOCK_ATOM_NAME    "_NET_WM_WINDOW_TYPE_DOCK"
#define _NET_WM_WINDOW_TYPE_TOOLBAR_ATOM_NAME "_NET_WM_WINDOW_TYPE_TOOLBAR"
#define _NET_WM_WINDOW_TYPE_MENU_ATOM_NAME    "_NET_WM_WINDOW_TYPE_MENU"
#define _NET_WM_WINDOW_TYPE_UTILITY_ATOM_NAME "_NET_WM_WINDOW_TYPE_UTILITY"
#define _NET_WM_WINDOW_TYPE_SPLASH_ATOM_NAME  "_NET_WM_WINDOW_TYPE_SPLASH"
#define _NET_WM_WINDOW_TYPE_DIALOG_ATOM_NAME  "_NET_WM_WINDOW_TYPE_DIALOG"
#define _NET_WM_WINDOW_TYPE_NORMAL_ATOM_NAME  "_NET_WM_WINDOW_TYPE_NORMAL"

#include <string.h>
#endif

namespace nkentseu {

#if defined(NKENTSEU_PLATFORM_WINDOWS)
    int32 PlatformState_::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow) {
        //Input;

        MSG msg;    // message
        int argc = 0;

        msg.wParam = 0;
        LPWSTR* commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argc);

        if (NULL == commandLineArgs) {
            argc = 0;
        }

        if (argc > 0) {
            for (int iii = 0; iii < argc; iii++) {
                size_t wideCharLen = wcslen(commandLineArgs[iii]);
                size_t numConverted = 0;

                char* argv = (char*)malloc(sizeof(char) * (wideCharLen + 1));
                if (argv != NULL) {
                    wcstombs_s(&numConverted, argv, wideCharLen + 1, commandLineArgs[iii], wideCharLen + 1);
                    this->argv.push_back(argv);
                    free(argv);
                }
            }
        }

        HInstance = (hInstance);
        HPrevInstance = (hPrevInstance);
        LPCmdLine = (lpCmdLine);
        NCmdShow = (nCmdShow);
        return 0;
    }

#elif defined(NKENTSEU_PLATFORM_LINUX)
    int32 PlatformState_::Init(int argc, const char** argv) {
        if (argc == 0 || argv == 0) {
            return 0;
        }
        else {
            int32 index = 0;
            for (index = 0; index < argc; index++) {
                this->argv.push_back(argv[index]);
            }
        }
        if (connection != nullptr) return 0;

        connection = xcb_connect(nullptr, &screenNumber);

        if (xcb_connection_has_error(connection) > 0)
        {
            Log_nts.Error("Connection au serveur echouer");
            return 1;
        }

        /* Get the screen whose number is screenNumber */

        const xcb_setup_t* setup = xcb_get_setup(connection);
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

        // we want the screen at index screenNumber of the iterator
        for (int i = 0; i < screenNumber; ++i)
        {
            xcb_screen_next(&iter);
        }

        screen = iter.data;

        int32 result = stateTools.Initialize();

        return result;
    }

    int32 PlatformState_::Close() {
        if (connection != nullptr){
            xcb_disconnect(connection);
            connection = nullptr;
            Log_nts.Trace("Connexion terminer avec le serveur X");
        }
        return 0;
    }

#elif defined(NKENTSEU_PLATFORM_ANDROID)

    int32 PlatformState_::Init(android_app* app) {
        //Input;
        App = app;
        return 0;
    }

#else

    int32 PlatformState_::Init(int argc, const char** argv) {
        //Input;

        if (argc == 0 || argv == 0) {
            return 0;
        }
        else {
            int32 index = 0;
            for (index = 0; index < argc; index++) {
                this->argv.push_back(argv[index]);
            }
        }
        return 0;
    }

#endif

    PlatformState_::PlatformState_() {}

#ifdef NKENTSEU_PLATFORM_LINUX

    int32 XCBStateTools::Initialize(){
        if (PlatformState.connection == nullptr){
            return 1;
        }

        /* get xcb setup to help us get screen iterator */
        const xcb_setup_t *setup = xcb_get_setup (PlatformState.connection);

        if (setup == nullptr){
            return 2;
        }

        /* get screen iterator to help us get screen */
        xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator (setup);

        /* update xw_state */
        this->screen_iterator = screen_iter;
        this->connection      = PlatformState.connection;

        /* get protocol atom */
        this->WM_PROTOCOLS     = GetXcbAtom(WM_PROTOCOLS_ATOM_NAME);
        this->WM_DELETE_WINDOW = GetXcbAtom(WM_DELETE_WINDOW_ATOM_NAME);

        /* get WM_STATE */
        this->WM_STATE = GetXcbAtom(WM_STATE_ATOM_NAME);

        /* get _NET_WM_STATE and it's possible values */
        this->_NET_WM_STATE        = GetXcbAtom(_NET_WM_STATE_ATOM_NAME);
        this->_NET_WM_STATE_MODAL  = GetXcbAtom(_NET_WM_STATE_MODAL_ATOM_NAME);
        this->_NET_WM_STATE_HIDDEN = GetXcbAtom(_NET_WM_STATE_HIDDEN_ATOM_NAME);
        this->_NET_WM_STATE_MAXIMIZED_VERT =
            GetXcbAtom(_NET_WM_STATE_MAXIMIZED_VERT_ATOM_NAME);
        this->_NET_WM_STATE_MAXIMIZED_HORZ =
            GetXcbAtom(_NET_WM_STATE_MAXIMIZED_HORZ_ATOM_NAME);
        this->_NET_WM_STATE_SHADED       = GetXcbAtom(_NET_WM_STATE_SHADED_ATOM_NAME);
        this->_NET_WM_STATE_SKIP_TASKBAR = GetXcbAtom(_NET_WM_STATE_SKIP_TASKBAR_ATOM_NAME);
        this->_NET_WM_STATE_SKIP_PAGER   = GetXcbAtom(_NET_WM_STATE_SKIP_PAGER_ATOM_NAME);
        this->_NET_WM_STATE_FULLSCREEN   = GetXcbAtom(_NET_WM_STATE_FULLSCREEN_ATOM_NAME);
        this->_NET_WM_STATE_ABOVE        = GetXcbAtom(_NET_WM_STATE_ABOVE_ATOM_NAME);
        this->_NET_WM_STATE_BELOW        = GetXcbAtom(_NET_WM_STATE_BELOW_ATOM_NAME);
        this->_NET_WM_STATE_DEMANDS_ATTENTION =
            GetXcbAtom(_NET_WM_STATE_DEMANDS_ATTENTION_ATOM_NAME);
        this->_NET_WM_STATE_FOCUSED = GetXcbAtom(_NET_WM_STATE_FOCUSED_ATOM_NAME);

        /* get allowed action atoms */
        this->_NET_WM_ALLOWED_ACTIONS = GetXcbAtom(_NET_WM_ALLOWED_ACTIONS_ATOM_NAME);
        this->_NET_WM_ACTION_MOVE     = GetXcbAtom(_NET_WM_ACTION_MOVE_ATOM_NAME);
        this->_NET_WM_ACTION_RESIZE   = GetXcbAtom(_NET_WM_ACTION_RESIZE_ATOM_NAME);
        this->_NET_WM_ACTION_MINIMIZE = GetXcbAtom(_NET_WM_ACTION_MINIMIZE_ATOM_NAME);
        this->_NET_WM_ACTION_SHADE    = GetXcbAtom(_NET_WM_ACTION_SHADE_ATOM_NAME);
        this->_NET_WM_ACTION_STICK    = GetXcbAtom(_NET_WM_ACTION_STICK_ATOM_NAME);
        this->_NET_WM_ACTION_MAXIMIZE_HORZ =
            GetXcbAtom(_NET_WM_ACTION_MAXIMIZE_HORZ_ATOM_NAME);
        this->_NET_WM_ACTION_MAXIMIZE_VERT =
            GetXcbAtom(_NET_WM_ACTION_MAXIMIZE_VERT_ATOM_NAME);
        this->_NET_WM_ACTION_FULLSCREEN = GetXcbAtom(_NET_WM_ACTION_FULLSCREEN_ATOM_NAME);
        this->_NET_WM_ACTION_CHANGE_DESKTOP =
            GetXcbAtom(_NET_WM_ACTION_CHANGE_DESKTOP_ATOM_NAME);
        this->_NET_WM_ACTION_CLOSE = GetXcbAtom(_NET_WM_ACTION_CLOSE_ATOM_NAME);
        this->_NET_WM_ACTION_ABOVE = GetXcbAtom(_NET_WM_ACTION_ABOVE_ATOM_NAME);
        this->_NET_WM_ACTION_BELOW = GetXcbAtom(_NET_WM_ACTION_BELOW_ATOM_NAME);

        this->_MOTIF_WM_HINTS = GetXcbAtom(_MOTIF_WM_HINTS_ATOM_NAME);

        /* get atoms for setting windows state */
        this->_NET_WM_WINDOW_TYPE         = GetXcbAtom(_NET_WM_WINDOW_TYPE_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_DESKTOP = GetXcbAtom(_NET_WM_WINDOW_TYPE_DESKTOP_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_DOCK    = GetXcbAtom(_NET_WM_WINDOW_TYPE_DOCK_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_TOOLBAR = GetXcbAtom(_NET_WM_WINDOW_TYPE_TOOLBAR_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_MENU    = GetXcbAtom(_NET_WM_WINDOW_TYPE_MENU_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_UTILITY = GetXcbAtom(_NET_WM_WINDOW_TYPE_UTILITY_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_SPLASH  = GetXcbAtom(_NET_WM_WINDOW_TYPE_SPLASH_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_DIALOG  = GetXcbAtom(_NET_WM_WINDOW_TYPE_DIALOG_ATOM_NAME);
        this->_NET_WM_WINDOW_TYPE_NORMAL  = GetXcbAtom(_NET_WM_WINDOW_TYPE_NORMAL_ATOM_NAME);

        return 0;
    }

    xcb_atom_t XCBStateTools::GetXcbAtom (const std::string& atom_name) {
        /* make a request to create a new atom with given name */
        xcb_intern_atom_cookie_t cookie =
            xcb_intern_atom_unchecked (connection, False, atom_name.size(), atom_name.c_str());

        /* wait for reply */
        xcb_intern_atom_reply_t *reply = nullptr;
        reply                          = xcb_intern_atom_reply (connection, cookie, nullptr);

        if (reply == nullptr){
            Log_nts.Error("Atom reply failed (got Null)");
            free(reply);
            return XCB_ATOM_NONE;
        }

        /* set the atom and free reply */
        xcb_atom_t atom = reply->atom;
        if (atom == XCB_ATOM_NONE){
            Log_nts.Error("Retrieved atom is XCB_ATOM_NONE");
        }

        /* free reply before exit */
        free(reply);
        return atom;
    }
#endif

}    // namespace nkentseu