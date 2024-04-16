//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:51:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowProperties.h"

namespace nkentseu {
    WindowProperties::WindowProperties() : backgroundColor(Color::DarkGray)
    {
    }

#ifdef NKENTSEU_PLATFORM_WINDOWS
    #include <Windows.h>

    WindowProperties WindowProperties::GetDesktopProperties() {
        DEVMODE win32Mode;
        win32Mode.dmSize = sizeof(win32Mode);
        win32Mode.dmDriverExtra = 0;
        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &win32Mode);

        WindowProperties wprop;
        wprop.size = Vector2u(win32Mode.dmPelsWidth, win32Mode.dmPelsHeight);
        wprop.bitsPerPixel = win32Mode.dmBitsPerPel;

        return wprop;
    }

    const std::vector<WindowProperties>& WindowProperties::GetFullscreenProperties() {
        static const auto modes = [] {
            std::vector<WindowProperties> modes;

            DEVMODE win32Mode;
            win32Mode.dmSize = sizeof(win32Mode);
            win32Mode.dmDriverExtra = 0;
            for (int32 count = 0; EnumDisplaySettings(nullptr, static_cast<DWORD>(count), &win32Mode); ++count) {
                WindowProperties mode;
                mode.size = Vector2u(win32Mode.dmPelsWidth, win32Mode.dmPelsHeight);
                mode.bitsPerPixel = win32Mode.dmBitsPerPel;

                if (std::find(modes.begin(), modes.end(), mode) == modes.end()) {
                    modes.push_back(mode);
                }
            }
            std::sort(modes.begin(), modes.end(), std::greater<>());
            return modes;
        }();

        return modes;
    }
#endif
}    // namespace nkentseu