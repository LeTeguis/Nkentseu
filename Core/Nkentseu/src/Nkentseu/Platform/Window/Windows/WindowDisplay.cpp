//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:36 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowDisplay.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include "WindowEventInternal.h"
#include <Nkentseu/Core/WindowProperties.h>
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    bool WindowDisplay::Register(bool dbclk, const WindowProperties& windowProperties) {
        if (m_IsRegistered) return false;

        windowClassName = std::wstring(windowProperties.name.begin(), windowProperties.name.end());

        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW | (dbclk ? CS_DBLCLKS : 0) | ((windowProperties.hasShadow) ? CS_DROPSHADOW : 0);
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

    bool WindowDisplay::CreateHDC()
    {
        if (deviceContext == nullptr && windowHandle != nullptr) {
            deviceContext = GetDC(windowHandle);
            Log_nts.Debug();
            return true;
        }
        return false;
    }

    void WindowDisplay::Unregister()
    {
        if (deviceContext != nullptr) {
            ReleaseDC(windowHandle, deviceContext);
        }
    }

    const WCHAR* WindowDisplay::GetWindowClassName() {
        return windowClassName.c_str();
    }
}    // namespace nkentseu

#endif