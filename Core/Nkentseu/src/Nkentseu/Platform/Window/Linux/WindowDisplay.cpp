//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowDisplay.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#ifdef NKENTSEU_PLATFORM_LINUX

#include "WindowEventInternal.h"
#include "Nkentseu/Platform/PlatformState.h"

namespace nkentseu {

    WindowDisplay::WindowDisplay(){
    }

    WindowDisplay::~WindowDisplay(){
    }

    bool WindowDisplay::Register(bool dbclk) {
        if (m_IsRegistered) return false;
        
        m_IsRegistered = false;
        Log_nts.Debug();

        xcb_intern_atom_reply_t *intern_atom_reply;

        xcb_connection_t* connection = PlatformState_::Instance().connection;
        int screen_number = PlatformState_::Instance().screenNumber;
        Log_nts.Debug();

        InterneAtomReply(true, true, "WM_PROTOCOLS", NULL);
        Log_nts.Debug();
        if(m_IsRegistered) {
            Log_nts.Debug();
            InterneAtomReply(false, true, "WM_DELETE_WINDOW", NULL);
            Log_nts.Debug();
        }
        //Log_nts.Debug();

        screenHandle = xcb_aux_get_screen(connection, screen_number);
        //Log_nts.Debug();

        windowHandle = xcb_generate_id(connection);
        //Log_nts.Debug();

        return m_IsRegistered;
    }

    void WindowDisplay::InterneAtomReply(bool type, bool onlyIfExist, const std::string& name, xcb_generic_error_t** generic){
        Log_nts.Debug();
        internAtomCookie = xcb_intern_atom(PlatformState_::Instance().connection, onlyIfExist, name.size(), name.c_str());
        Log_nts.Debug();
        xcb_intern_atom_reply_t *intern_atom_reply = xcb_intern_atom_reply(PlatformState_::Instance().connection, internAtomCookie, generic);
        Log_nts.Debug();
        
        if (intern_atom_reply){
            if (type)
                windowManagerProtocolsProperty = intern_atom_reply->atom;
            else windowManagerWindowDeleteProtocol = intern_atom_reply->atom;
            Log_nts.Debug();
            m_IsRegistered = true;
        } else {
            Log_nts.Debug();
        }
        Log_nts.Debug();
        free(intern_atom_reply);
    }

    void WindowDisplay::StaticNative(class Window* window) {
        currentWindowDisplay = this;
        windowSuper = window;
    }

    
    WindowDisplay* WindowDisplay::GetCurrent(xcb_window_t windowHandle){
        WindowDisplay* _this;
        if (currentWindowDisplay != nullptr) {
            windowHandleMap.emplace(windowHandle, currentWindowDisplay);
            currentWindowDisplay->windowHandle = windowHandle;
            _this = currentWindowDisplay;
            currentWindowDisplay = nullptr;
        }
        else {
            auto existing = windowHandleMap.find(windowHandle);
            _this = existing->second;
        }
        return _this;
    }

}    // namespace nkentseu

#endif
