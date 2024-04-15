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
            prop.Size = Vector2u();
            prop.MaxSize = Vector2u();
            return prop;
        }

        WindowProperties();
        // Transform
        // 
        // Window Size
        Vector2u Size = Vector2u(640, 480);
        // Window minimum Size
        Vector2u MinSize = Vector2u(0, 0);
        // Window maximum Size
        Vector2u MaxSize = Vector2u(0xFFFF, 0xFFFF);

        // Window position based on Top Left
        Vector2i Position = Vector2i(0, 0);
        // Should this window be centered on creation?
        WindowPositionType PositionType = WindowPositionType::CenteredPosition;

        // Window Settings
        // 
        // Whether this window is resizable
        bool Resizable = true;
        // Whether this window is movable
        bool Movable = true;
        // Whether this window is closable
        bool Closable = true;
        // Whether this window is minimizable
        bool Minimizable = true;
        // whether this window is maximizable
        bool Maximizable = true;
        // Whether this window can be fullscreened
        bool CanFullscreen = true;
        // Vertical syncronization
        bool VSyn = true;

        bool AcceptFile = true;

        bool StayInTop = false;

        // Color and Style
        // 
        // Hexidecimal background color (0xffffffff = white)
        //Color BackgroundColor = 0xFFFFFFFF;
        Color BackgroundColor;
        // makes this window transparent
        bool Transparent = false;
        // does this window feature a frame?
        bool Frame = true;
        // if this window has a shadow
        bool HasShadow = false;

        // States
        // 
        // Is this window visible?
        bool Visible = true;
        // Is this window full screen?
        bool Fullscreen = false;
        // Is this window a modal?
        bool Modal = false;

        // App Data
        // 
        // Window Title
        std::string Title = std::string("Nkentseu");
        // Window name ID
        std::string Name = std::string("NkentseuApp");
        // Icon Path
        std::string IconPath = "";

        // Mouse Data
        bool DoubleClick = true;

        // context info
        // ContextInfo ContextInfo;

        // mouse 
        bool CursorVisible = true;

        // bpp
        uint32 BitsPerPixel{};
        static WindowProperties GetDesktopProperties();
        static const std::vector<WindowProperties>& GetFullscreenProperties();

        friend bool operator==(const WindowProperties& left, const WindowProperties& right) {
            return (left.Size == right.Size) && (left.BitsPerPixel == right.BitsPerPixel);
        }
        friend bool operator!=(const WindowProperties& left, const WindowProperties& right) {
            return !(left == right);
        }
        friend bool operator<(const WindowProperties& left, const WindowProperties& right) {
            if (left.BitsPerPixel == right.BitsPerPixel) {
                if (left.Size.x == right.Size.x) {
                    return left.Size.y < right.Size.y;
                }
                else {
                    return left.Size.x < right.Size.x;
                }
            }
            else {
                return left.BitsPerPixel < right.BitsPerPixel;
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