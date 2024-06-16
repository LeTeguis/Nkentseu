//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-12 at 05:32:07 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowData.h"
#include <Logger/Formatter.h>
#include "Windows/Win32Event.h"

namespace nkentseu {
    
#ifdef NKENTSEU_PLATFORM_WINDOWS

    bool WindowData::Register(bool dbclk, const WindowProperties& windowProperties)
    {
        if (isRegistered) return false;

        windowClassName = std::wstring(windowProperties.name.begin(), windowProperties.name.end());

        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW | (dbclk ? CS_DBLCLKS : 0) | ((windowProperties.hasShadow) ? CS_DROPSHADOW : 0);
        windowClass.lpfnWndProc = Win32Event::StaticProcedure;
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

        isRegistered = RegisterClassEx(&windowClass);

        return isRegistered;
    }

    const WCHAR* WindowData::GetWindowClassName()
    {
        return windowClassName.c_str();
    }

#elif defined(NKENTSEU_PLATFORM_ANDROID)
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#elif defined(NKENTSEU_PLATFORM_IOS)
#elif defined(NKENTSEU_PLATFORM_MACOS)
#elif defined(NKENTSEU_PLATFORM_NDS)
#endif

}  //  nkentseu