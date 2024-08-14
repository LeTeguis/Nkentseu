//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-12 at 05:32:07 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __WINDOW_DATA_H__
#define __WINDOW_DATA_H__

#pragma once

#include <NTSCore/System.h>
#include "NTSWindow/Core/WindowProperties.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include <Windows.h>
#include <ShObjIdl.h>
#include <unordered_map>
#include "Windows/WindowBrush.h"

using WINDOW_HANDLE = HWND;
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#elif defined(NKENTSEU_PLATFORM_IOS)
#elif defined(NKENTSEU_PLATFORM_MACOS)
#elif defined(NKENTSEU_PLATFORM_NDS)
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

namespace nkentseu {
    
    struct NKENTSEU_API WindowData {
        WINDOW_HANDLE windowHandle = nullptr;
        maths::Vector2f extend;
        float32 border;
        float32 frame;

#ifdef NKENTSEU_PLATFORM_WINDOWS
        HINSTANCE instanceHandle = nullptr;
        HBITMAP bitmapHandle;
        WNDCLASSEX windowClass = { 0 };
        RECT windowRectangle = { 0 };
        DEVMODE screenSettings = { 0 };
        DWORD extendedStyle = 0x0;
        DWORD windowStyle = 0x0;
        ITaskbarList3* taskbarList;
        WindowBrush backgroundColor;
        std::wstring windowClassName;
        bool isCursorVisible = true;
        bool isCursorGrabbed;
        HCURSOR lastCursor{ LoadCursor(nullptr, IDC_ARROW) };
        bool isMouseInside = true;
        HICON icon;
        bool isRegistered = false;

        bool Register(bool dbclk, const class WindowProperties& windowProperties);
        const WCHAR* GetWindowClassName();
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#elif defined(NKENTSEU_PLATFORM_IOS)
#elif defined(NKENTSEU_PLATFORM_MACOS)
#elif defined(NKENTSEU_PLATFORM_NDS)
#endif
    };

}  //  nkentseu

#endif  // __WINDOW_DATA_H__!