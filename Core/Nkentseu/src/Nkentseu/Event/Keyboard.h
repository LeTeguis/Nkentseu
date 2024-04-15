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

        using KeyCode = uint64;
        using ScanCode = uint64;
        using KeyMod = uint64;
        using Code = uint64;

        enum : Code {
            Unknown_ev = 0,

            Back_ev = 0x08, Tab_ev = 0x09,

            Clear_ev = 0x0C, Return_ev = 0x0D,

            Shift_ev = 0x10, Ctrl_ev, Alt_ev, Pause_ev, CapsLock_ev, Kana_ev = 0x15, Hangul_ev = 0x15, One_ev, Junja_ev, Final_ev, Hanja_ev = 0x19,
            Kanji_ev = 0x19, Off_ev, Escape_ev, Convert_ev, NonConvert_ev, Accept_ev, ModeChange_ev, Space_ev, PageUp_ev, PageDown_ev,
            End_ev, Home_ev, Left_ev, Up_ev, Right_ev, Down_ev, Select_ev, Print_ev, Execute_ev, PrintScreen_ev, Insert_ev,
            Delete_ev, Help_ev = 0x2F,

            KP_0_ev = 0x30, KP_1_ev, KP_2_ev, KP_3_ev, KP_4_ev, KP_5_ev, KP_6_ev, KP_7_ev, KP_8_ev, KP_9_ev = 0x39,

            A_ev = 0x41, B_ev, C_ev, D_ev, E_ev, F_ev, G_ev, H_ev, I_ev, J_ev, K_ev, L_ev, M_ev, N_ev, O_ev, P_ev, Q_ev, R_ev, S_ev, T_ev, U_ev, V_ev, W_ev, X_ev, Y_ev, Z_ev = 0x5A,

            LSuper_ev = 0x5B, RSuper_ev, Apps_ev = 0x5D,

            Sleep_ev = 0x5F,

            Num0_ev = 0x60, Num1_ev, Num2_ev, Num3_ev, Num4_ev, Num5_ev, Num6_ev, Num7_ev, Num8_ev, Num9_ev = 0x69,

            Multiply_ev = 0x6A, Add_ev, Separator_ev, Substract_ev, Decimal_ev, Divide_ev = 0x6F,

            F1_ev = 0x70, F2_ev, F3_ev, F4_ev, F5_ev, F6_ev, F7_ev, F8_ev, F9_ev, F10_ev, F11_ev, F12_ev, F13_ev, F14_ev, F15_ev, F16_ev, F17_ev, F18_ev, F19_ev,
            F20_ev, F21_ev, F22_ev, F23_ev, F24_ev = 0x87,

            NumLock_ev = 0x90, ScrollLock_ev = 0x91,

            LShift_ev = 0xA0, RShift_ev, LCtrl_ev, RCtrl_ev, LAlt_ev, RAlt_ev, BrowserBack_ev, BrowserForward_ev, BrowserRefresh_ev,
            BrowserStop_ev, BrowserSearch_ev, BrowserFavorites_ev, BrowserStart_ev, VolumeMute_ev, VolumeDown_ev, VolumeUp_ev,
            NextTrack_ev, PreviousTrack_ev, StopMedia_ev, PlayPauseMedia_ev, StartMail_ev, SelectMedia_ev,
            StartApp1_ev, StartApp2_ev = 0xB7,

            Semicolon_ev = 0xBA, Equal_ev, Comma_ev, Minus_ev, Period_ev, Slash_ev, Backtick_ev = 0xC0,

            OpeningBracket_ev = 0xDB, Backslash_ev, ClosingBracket_ev, Apostrophe_ev, Miscellaneous_ev = 0xDF,

            AngleBracket_ev = 0xE2,

            Process_ev = 0xE5,

            Unicode_ev = 0xE7,

            Attn_ev = 0xF6, CrSel_ev, ExSel_ev, EraseEOF_ev, Play_ev, Zoom_ev, NoName_ev, Pa1_ev, OemClear_ev = 0xFE,

            Colon_ev, Quotation_ev, Plus_ev,

            Num_ev = 172,
            FirstCode_ev = Back_ev,
            LastCode_ev = Quotation_ev
        };

        static std::string GetKeycode(KeyCode keycode);
        static KeyCode GetKeycode(const std::string& str);

        static std::string GetScancode(ScanCode scancode);
        static ScanCode GetScancode(const std::string& str);

        static bool IsKeyCode(KeyCode key);
        static bool IsScanCode(ScanCode key);
        static bool IsKeyMode(KeyMod key);

    private:

        static Code GetCodeFromName(const char* code);
        static const char* GetCodeName(Code code);

        static std::string ToString(Code code);
        static Code FromString(const std::string& str);
    };
    
    #define KEYBOARD(code) Keyboard::code##_ev
} // namespace nkentseu

#endif    // __NKENTSEU_KEYBOARD_H__