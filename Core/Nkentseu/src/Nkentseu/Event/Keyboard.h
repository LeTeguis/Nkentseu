//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_KEYBOARD_H__
#define __NKENTSEU_KEYBOARD_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {
    class NKENTSEU_API Keyboard {
    public:

        #define MASK (1<<30)
        #define SCANCODE_TO_KEYCODE(X)  (X | MASK)

        using Keycode = uint64;
        using Scancode = uint64;
        using KeyMod = uint64;
        using Code = uint64;

        enum : Code {
            NotDefine = 0,

            Back, 
            Tab,

            Clear, 
            Return,

            Shift, 
            Ctrl, 
            Alt, 
            Pause, 
            CapsLock, 
            Kana, 
            Hangul, 
            One, 
            Junja, 
            
            Final, 
            Hanja,

            Kanji, 
            Off, 
            Escape, 
            Convert,
            NonConvert, 
            Accept, 
            ModeChange, 
            Space, 
            PageUp,
            PageDown,
            End,
            Home,
            Left,
            Up,
            Right,
            Down, 
            Select,
            Print, 
            Execute,
            PrintScreen, 
            Insert,
            Delete, 
            Help,

            Keypad0, 
            Keypad1, 
            Keypad2, 
            Keypad3, 
            Keypad4, 
            Keypad5, 
            Keypad6, 
            Keypad7, 
            Keypad8,
            Keypad9,

            A, 
            B, 
            C, 
            D, 
            E, 
            F, 
            G, 
            H, 
            I, 
            J, 
            K,
            L,
            M,
            N, 
            O,
            P,
            Q,
            R, 
            S, 
            T, 
            U, 
            V, 
            W, 
            X, 
            Y,
            Z,

            LSuper, 
            RSuper, 
            Apps,

            Sleep,

            Numpad0, 
            Numpad1, 
            Numpad2, 
            Numpad3, 
            Numpad4, 
            Numpad5, 
            Numpad6, 
            Numpad7, 
            Numpad8, 
            Numpad9,

            Multiply, 
            Add,
            Separator, 
            Substract, 
            Decimal, 
            Divide,

            F1, 
            F2, 
            F3, 
            F4, 
            F5, 
            F6, 
            F7, 
            F8, 
            F9, 
            F10,
            F11,
            F12,
            F13, 
            F14, 
            F15, 
            F16, 
            F17, 
            F18, 
            F19,
            F20, 
            F21, 
            F22, 
            F23, 
            F24,

            NumLock, 
            ScrollLock,

            LShift, 
            RShift, 
            LCtrl, 
            RCtrl, 
            LAlt, 
            RAlt, 
            BrowserBack, 
            BrowserForward, 
            BrowserRefresh,
            BrowserStop, 
            BrowserSearch, 
            BrowserFavorites, 
            BrowserStart, 
            VolumeMute, 
            VolumeDown, 
            VolumeUp,
            NextTrack, 
            PreviousTrack, 
            StopMedia, 
            PlayPauseMedia, 
            StartMail, 
            SelectMedia,
            StartApp1, 
            StartApp2,

            Semicolon, 
            Equal, 
            Comma, 
            Minus, 
            Period, 
            Slash, 
            Backtick,

            OpeningBracket, 
            Backslash, 
            ClosingBracket, 
            Apostrophe, 
            Miscellaneous,

            AngleBracket,

            Process,

            Unicode,

            Attn, 
            CrSel, 
            ExSel, 
            EraseEOF, 
            Play, 
            Zoom, 
            NoName, 
            Pa1, 
            OemClear,

            Colon, 
            Quotation, 
            Plus,

            Num,
 
            FirstCode = Back,
            LastCode = Plus,
        };

        static std::string GetKeycode(Keycode keycode);
        static Keycode GetKeycode(const std::string& str);

        static std::string GetScancode(Scancode scancode);
        static Scancode GetScancode(const std::string& str);

        static bool IsKeyCode(Keycode key);
        static bool IsScanCode(Scancode key);
        static bool IsKeyMode(KeyMod key);

    private:

        static Code GetCodeFromName(const char* code);
        static const char* GetCodeName(Code code);

        static std::string ToString(Code code);
        static Code FromString(const std::string& str);
    };
    
    #define KEYBOARD(code) Keyboard::code
} // namespace nkentseu

#endif    // __NKENTSEU_KEYBOARD_H__