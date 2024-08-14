//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:07:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOWCURSOR_H__
#define __NKENTSEU_WINDOWCURSOR_H__

#pragma once

#include "NTSCore/System.h"
#include <NTSMaths/Vector/Vector2.h>

#ifdef NKENTSEU_PLATFORM_LINUX
#include <X11/Xlib.h>
#endif

namespace nkentseu {
    class NKENTSEU_API WindowCursor
    {
    public:
        using Code = uint32;

        enum : Code {
            NotAllowed,             //!< Action not allowed cursor
            Arrow,                  //!< Arrow cursor (default)
            ArrowWait,              //!< Busy arrow cursor
            Wait,                   //!< Busy cursor
            Text,                   //!< I-beam, cursor when hovering over a field allowing text entry
            Hand,                   //!< Pointing hand cursor
            SizeHorizontal,         //!< Horizontal double arrow cursor
            SizeVertical,           //!< Vertical double arrow cursor
            SizeTopLeftBottomRight, //!< Double arrow cursor going from top-left to bottom-right
            SizeBottomLeftTopRight, //!< Double arrow cursor going from bottom-left to top-right
            SizeLeft,               //!< Left arrow cursor on Linux, same as SizeHorizontal on other platforms
            SizeRight,              //!< Right arrow cursor on Linux, same as SizeHorizontal on other platforms
            SizeTop,                //!< Up arrow cursor on Linux, same as SizeVertical on other platforms
            SizeBottom,             //!< Down arrow cursor on Linux, same as SizeVertical on other platforms
            SizeTopLeft,            //!< Top-left arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
            SizeBottomRight,        //!< Bottom-right arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
            SizeBottomLeft,         //!< Bottom-left arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
            SizeTopRight,           //!< Top-right arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
            SizeAll,                //!< Combination of SizeHorizontal and SizeVertical
            Cross,                  //!< Crosshair cursor
            Help                    //!< Help cursor
        };

        WindowCursor();
        ~WindowCursor();
        WindowCursor(const WindowCursor&) = delete;
        WindowCursor& operator=(const WindowCursor&) = delete;
        bool LoadFromPixels(const uint8* pixels, maths::Vector2u size, maths::Vector2u hotspot);
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
        bool LoadFromPixelsInternal(const uint8* pixels, maths::Vector2u size, maths::Vector2u hotspot);
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