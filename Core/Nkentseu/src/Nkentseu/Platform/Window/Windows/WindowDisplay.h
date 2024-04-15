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
#include <Nkentseu/Core/WindowProperties.h>
#include <unordered_map>
#include "WindowBrush.h"

namespace nkentseu {
    class WindowInternal;
    class WindowEventInternal;

    class NKENTSEU_API WindowDisplay {
        friend class WindowEventInternal;
    public:
        HWND windowHandle = nullptr;
        HINSTANCE instanceHandle = nullptr;
        HDC deviceContext;
        HDC memoryDeviceContext;
        HBITMAP bitmapHandle;
        WindowProperties windowProperties;
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
        class Window* windowSuper;

        friend class WindowEventInternal;

        WindowDisplay() {}
    public:
        bool Register(bool dbclk);
        void StaticNative(class Window*);
        const WCHAR* GetWindowClassName();

    private:
        bool m_IsRegistered = false;

        static WindowDisplay* GetCurrent(HWND hwnd);
    };

    static thread_local WindowDisplay* currentWindowDisplay = nullptr;
    static thread_local std::unordered_map<HWND, WindowDisplay*> windowHandleMap = {};

    enum WindowStyle : DWORD {
        Windowed = WS_OVERLAPPEDWINDOW,
        AeroBorderless = WS_POPUP | WS_THICKFRAME,
        BasicBorderless = WS_CAPTION | WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
    };
} // namespace nkentseu

#endif    //    NKENTSEU_PLATFORM_WINDOWS

#endif    //    __NKENTSEU_WINDOW_DISPLAY_H__