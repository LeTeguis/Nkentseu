//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 3:12:36 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//


#pragma once

#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#ifndef __NKENTSEU_WINDOW_DISPLAY_H__
#define __NKENTSEU_WINDOW_DISPLAY_H__

#include <Windows.h>
#include <ShObjIdl.h>
#include <unordered_map>
#include "WindowBrush.h"

namespace nkentseu {
    //class WindowInternal;
    class WindowEventInternal;

    struct NKENTSEU_API WindowDisplay {
        friend class WindowEventInternal;
        HWND windowHandle = nullptr;
        HINSTANCE instanceHandle = nullptr;
        HDC deviceContext;
        HDC memoryDeviceContext;
        HBITMAP bitmapHandle;
        WNDCLASSEX windowClass = { 0 };
        RECT windowRectangle = { 0 };
        DEVMODE screenSettings = { 0 };
        DWORD extendedStyle = 0x0;
        DWORD windowStyle = 0x0;
        ITaskbarList3* taskbarList;
        WindowBrush backgroundColor;
        std::wstring windowClassName;
        bool isCursorVisible;
        bool isCursorGrabbed;
        HCURSOR lastCursor{ LoadCursor(nullptr, IDC_ARROW) };
        bool isMouseInside = true;
        HICON icon;

        friend class WindowEventInternal;

        WindowDisplay() {}

        bool Register(bool dbclk, const class WindowProperties& windowProperties);
        const WCHAR* GetWindowClassName();

    private:
        bool m_IsRegistered = false;
    };

} // namespace nkentseu

#endif    //    NKENTSEU_PLATFORM_WINDOWS

#endif    //    __NKENTSEU_WINDOW_DISPLAY_H__