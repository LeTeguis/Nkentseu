//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:29 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowDisplay.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#ifdef NKENTSEU_PLATFORM_LINUX

#include "WindowEventInternal.h"

namespace nkentseu {

    WindowDisplay::WindowDisplay(){
    }

    WindowDisplay::~WindowDisplay(){
    }

    bool WindowDisplay::Register(bool dbclk) {
        if (m_IsRegistered) return false;

        windowHandle = XOpenDisplay(nullptr);
        m_IsRegistered = true;

        if (windowHandle == nullptr){
            Log_nts.Error("Failed to open X diplay {0}", XDisplayName(nullptr));
            m_IsRegistered = false;
        }
        windowRoot = DefaultRootWindow(windowHandle);
        screen = DefaultScreenOfDisplay(display);
        screenId = DefaultScreen(display);
        return m_IsRegistered;
    }

    bool WindowDisplay::UnRegister(bool dbclk) {
        if (!m_IsRegistered) return false;

        XCloseDisplay(windowHandle);
    }

    void WindowDisplay::StaticNative(class nkentseu::Window* window) {
        currentWindowDisplay = this;
        windowSuper = window;
    }

    
    WindowDisplay* WindowDisplay::GetCurrent(::Display display){
        WindowDisplay* _this;
        if (currentWindowDisplay != nullptr) {
            windowHandleMap.emplace(display, currentWindowDisplay);
            currentWindowDisplay->windowHandle = display;
            _this = currentWindowDisplay;
            currentWindowDisplay = nullptr;
        }
        else {
            auto existing = windowHandleMap.find(display);
            _this = existing->second;
        }
        return _this;
    }

}    // namespace nkentseu

#endif