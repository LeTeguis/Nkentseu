//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:51:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOW_PROPERTIES_H__
#define __NKENTSEU_WINDOW_PROPERTIES_H__

#pragma once

#include "System/System.h"
#include <vector>
#include <System/Nature/Base.h>
#include <Ntsm/Vector/Vector2.h>
#include <Nkentseu/Graphics/Color.h>

namespace nkentseu {
    enum class WindowPositionType { CenteredPosition, RandomPosition, AbsolutePosition };

    class NKENTSEU_API WindowProperties
    {
    public:
        static const WindowProperties& GetInValid() {
            static WindowProperties prop;
            prop.size = Vector2u();
            prop.maxSize = Vector2u();
            return prop;
        }

        WindowProperties();
        // Transform
        // 
        // Window Size
        Vector2u size = Vector2u(640, 480);
        // Window minimum Size
        Vector2u minSize = Vector2u(0, 0);
        // Window maximum Size
        Vector2u maxSize = Vector2u(0xFFFF, 0xFFFF);

        // Window position based on Top Left
        Vector2i position = Vector2i(0, 0);
        // Should this window be centered on creation?
        WindowPositionType positionType = WindowPositionType::CenteredPosition;

        // Window Settings
        // 
        // Whether this window is resizable
        bool resizable = true;
        // Whether this window is movable
        bool movable = true;
        // Whether this window is closable
        bool closable = true;
        // Whether this window is minimizable
        bool minimizable = true;
        // whether this window is maximizable
        bool maximizable = true;
        // Whether this window can be fullscreened
        bool canFullscreen = true;
        // Vertical syncronization
        bool vSyn = true;

        bool acceptFile = true;

        bool stayInTop = false;

        // Color and Style
        // 
        // Hexidecimal background color (0xffffffff = white)
        //Color BackgroundColor = 0xFFFFFFFF;
        Color backgroundColor;
        // makes this window transparent
        bool transparent = false;
        // does this window feature a frame?
        bool frame = true;
        // if this window has a shadow
        bool hasShadow = false;

        // States
        // 
        // Is this window visible?
        bool visible = true;
        // Is this window full screen?
        bool fullscreen = false;
        // Is this window a modal?
        bool modal = false;

        // App Data
        // 
        // Window Title
        std::string title = std::string("Nkentseu");
        // Window name ID
        std::string name = std::string("NkentseuLib");
        // Icon Path
        std::string iconPath = "";

        // Mouse Data
        bool doubleClick = true;

        // context info
        // ContextInfo ContextInfo;

        // mouse 
        bool cursorVisible = true;

        // bpp
        uint32 bitsPerPixel{};
        static WindowProperties GetDesktopProperties();
        static const std::vector<WindowProperties>& GetFullscreenProperties();

        friend bool operator==(const WindowProperties& left, const WindowProperties& right) {
            return (left.size == right.size) && (left.bitsPerPixel == right.bitsPerPixel);
        }
        friend bool operator!=(const WindowProperties& left, const WindowProperties& right) {
            return !(left == right);
        }
        friend bool operator<(const WindowProperties& left, const WindowProperties& right) {
            if (left.bitsPerPixel == right.bitsPerPixel) {
                if (left.size.x == right.size.x) {
                    return left.size.y < right.size.y;
                }
                else {
                    return left.size.x < right.size.x;
                }
            }
            else {
                return left.bitsPerPixel < right.bitsPerPixel;
            }
        }
        friend bool operator>(const WindowProperties& left, const WindowProperties& right) {
            return right < left;
        }
        friend bool operator<=(const WindowProperties& left, const WindowProperties& right) {
            return !(right < left);
        }
        friend bool operator>=(const WindowProperties& left, const WindowProperties& right) {
            return !(left < right);
        }
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_PROPERTIES_H__