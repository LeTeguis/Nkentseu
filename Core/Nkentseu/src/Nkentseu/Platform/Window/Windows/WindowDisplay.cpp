//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:36 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowDisplay.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include "WindowEventInternal.h"

namespace nkentseu {
    WindowDisplay* WindowDisplay::GetCurrent(HWND hwnd) {
        WindowDisplay* _this;
        if (currentWindowDisplay != nullptr) {
            windowHandleMap.emplace(hwnd, currentWindowDisplay);
            currentWindowDisplay->windowHandle = hwnd;
            _this = currentWindowDisplay;
            currentWindowDisplay = nullptr;
        }
        else {
            auto existing = windowHandleMap.find(hwnd);
            _this = existing->second;
        }
        return _this;
    }

    bool WindowDisplay::Register(bool dbclk) {
        if (m_IsRegistered) return false;

        windowClassName = std::wstring(windowProperties.Name.begin(), windowProperties.Name.end());

        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW | (dbclk ? CS_DBLCLKS : 0) | ((windowProperties.HasShadow) ? CS_DROPSHADOW : 0);
        windowClass.lpfnWndProc = WindowEventInternal::WindowProcStatic;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = WS_EX_NOPARENTNOTIFY;
        windowClass.hInstance = instanceHandle;
        //windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        windowClass.hIcon = nullptr;
        //WndClass__.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.hCursor = lastCursor;
        windowClass.hbrBackground = backgroundColor.GetBrushHandle();
        windowClass.lpszMenuName = nullptr;
        windowClass.lpszClassName = GetWindowClassName();
        windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

        m_IsRegistered = RegisterClassEx(&windowClass);

        return m_IsRegistered;
    }

    void WindowDisplay::StaticNative(Window* win) {
        currentWindowDisplay = this;
        windowSuper = win;
    }

    const WCHAR* WindowDisplay::GetWindowClassName() {
        return windowClassName.c_str();
    }
}    // namespace nkentseu

#endif