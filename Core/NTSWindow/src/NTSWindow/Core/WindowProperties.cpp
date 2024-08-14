//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:51:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "WindowProperties.h"
#include "NTSWindow/Platform/PlatformState.h"

#include <NTSMaths/Color.h>

namespace nkentseu {
    using namespace maths;

    WindowProperties::WindowProperties() : backgroundColor(Color::DarkGray())
    {
    }

    const WindowProperties& WindowProperties::GetInValid() {
        static WindowProperties prop;
        prop.size = Vector2u();
        prop.maxSize = Vector2u();
        return prop;
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
#elif defined(NKENTSEU_PLATFORM_LINUX)
    #if defined(NKENTSEU_PLATFORM_LINUX_XCB)
    WindowProperties WindowProperties::GetDesktopProperties() {
        WindowProperties wprop;
        int screen_number = PlatformState.screenNumber;
        xcb_screen_iterator_t iter;
        iter = xcb_setup_roots_iterator(xcb_get_setup(PlatformState.connection));

        for (; iter.rem; --screen_number, xcb_screen_next(&iter)){
            xcb_screen_t *screen = (xcb_screen_t*)iter.data;
            if (screen != nullptr && screen_number == 0){
                WindowProperties prop;

                prop.size.x = (*screen).width_in_pixels;
                prop.size.y = (*screen).height_in_pixels;
                prop.bitsPerPixel = (*screen).root_depth;
                prop.blackPixel = (*screen).black_pixel;
                prop.whitePixel = (*screen).white_pixel;

                return prop;
            }
        }

        return wprop;
    }

    const std::vector<WindowProperties>& WindowProperties::GetFullscreenProperties() {
        static const auto modes = [] {
            std::vector<WindowProperties> modes;
            int screen_number = PlatformState.screenNumber;
            xcb_screen_iterator_t iter;

            iter = xcb_setup_roots_iterator(xcb_get_setup(PlatformState.connection));

            for (; iter.rem; --screen_number, xcb_screen_next(&iter)){
                xcb_screen_t *screen = (xcb_screen_t*)iter.data;
                if (screen != nullptr){
                    WindowProperties prop;

                    prop.size.x = (*screen).width_in_pixels;
                    prop.size.y = (*screen).height_in_pixels;
                    prop.bitsPerPixel = (*screen).root_depth;
                    prop.blackPixel = (*screen).black_pixel;
                    prop.whitePixel = (*screen).white_pixel;

                    modes.push_back(prop);
                }
            }

            return modes;
        }();

        return modes;
    }
    #elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
    WindowProperties WindowProperties::GetDesktopProperties() {
        WindowProperties wprop;
        return wprop;
    }

    const std::vector<WindowProperties>& WindowProperties::GetFullscreenProperties() {
        static const auto modes = [] {
            std::vector<WindowProperties> modes;
            return modes;
        }();

        return modes;
    }
    #endif
#endif
}    // namespace nkentseu