//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:07:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOWCURSOR_H__
#define __NKENTSEU_WINDOWCURSOR_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector2.h>

#ifdef NKENTSEU_PLATFORM_LINUX
#include <X11/Xlib.h>
#endif

namespace nkentseu {
    class NKENTSEU_API WindowCursor
    {
    public:
        using Code = uint32;

        enum : Code {
            None_ev,
            Arrow_ev,                  //!< Arrow cursor (default)
            ArrowWait_ev,              //!< Busy arrow cursor
            Wait_ev,                   //!< Busy cursor
            Text_ev,                   //!< I-beam, cursor when hovering over a field allowing text entry
            Hand_ev,                   //!< Pointing hand cursor
            SizeHorizontal_ev,         //!< Horizontal double arrow cursor
            SizeVertical_ev,           //!< Vertical double arrow cursor
            SizeTopLeftBottomRight_ev, //!< Double arrow cursor going from top-left to bottom-right
            SizeBottomLeftTopRight_ev, //!< Double arrow cursor going from bottom-left to top-right
            SizeLeft_ev,               //!< Left arrow cursor on Linux, same as SizeHorizontal on other platforms
            SizeRight_ev,              //!< Right arrow cursor on Linux, same as SizeHorizontal on other platforms
            SizeTop_ev,                //!< Up arrow cursor on Linux, same as SizeVertical on other platforms
            SizeBottom_ev,             //!< Down arrow cursor on Linux, same as SizeVertical on other platforms
            SizeTopLeft_ev,            //!< Top-left arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
            SizeBottomRight_ev,        //!< Bottom-right arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
            SizeBottomLeft_ev,         //!< Bottom-left arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
            SizeTopRight_ev,           //!< Top-right arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
            SizeAll_ev,                //!< Combination of SizeHorizontal and SizeVertical
            Cross_ev,                  //!< Crosshair cursor
            Help_ev,                   //!< Help cursor
            NotAllowed_ev              //!< Action not allowed cursor
        };

        WindowCursor();
        ~WindowCursor();
        WindowCursor(const WindowCursor&) = delete;
        WindowCursor& operator=(const WindowCursor&) = delete;
        bool LoadFromPixels(const uint8* pixels, Vector2u size, Vector2u hotspot);
        bool LoadFromSystem(WindowCursor::Code type);

#ifdef NKENTSEU_PLATFORM_WINDOWS
        void* GetCursor() const{
            return m_Cursor;
        }
#endif

    private:
        void Release();
        bool  m_SystemCursor{};
        // WindowCursor::Type m_CursorType = WindowCursor::Type::NoCursor;
        bool LoadFromPixelsInternal(const uint8* pixels, Vector2u size, Vector2u hotspot);
        bool LoadFromSystemInternal(WindowCursor::Code type);

#ifdef NKENTSEU_PLATFORM_WINDOWS
        void* m_Cursor{};
#elif defined(NKENTSEU_PLATFORM_LINUX)
        bool LoadFromPixelsARGB(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot);
        bool LoadFromPixelsMonochrome(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot);
        bool IsColorCursorSupported();
        ::Display* m_Display;
        ::Cursor m_Cursor{ None };
#endif
    };

} // namespace nkentseu

#endif    // __NKENTSEU_WINDOWCURSOR_H__