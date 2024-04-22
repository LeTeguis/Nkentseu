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
        m_IsRegistered = true;

        xcb_connection_t* connection = PlatformState_::Instance().connection;
        screenHandle = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

        if (screenHandle == NULL) {
            Log_nts.Error("Failed to get screen iterator");
            return false;
        }
        
        windowHandle = xcb_generate_id(connection);

        if (windowHandle == 0) {
            Log_nts.Error("Failed to generate window ID");
            return 1;
        }

        return m_IsRegistered;
    }
}    // namespace nkentseu

#endif