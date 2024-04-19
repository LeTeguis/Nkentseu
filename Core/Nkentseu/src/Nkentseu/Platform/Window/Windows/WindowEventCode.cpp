//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/17/2024 at 9:38:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventCode.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include <Windows.h>

namespace nkentseu {
    uint64 WindowEventCode::KeycodeToWinkey(Keyboard::Code code) {
        #define CODE_CONVERT(code_id, value) if (code == Keyboard::code_id) return value
        CODE_CONVERT(Back, 0x08);
        CODE_CONVERT(Tab, 0x09);
        CODE_CONVERT(Clear, 0x0C);
        CODE_CONVERT(Return, 0x0D);
        CODE_CONVERT(Shift, 0x10);
        CODE_CONVERT(Ctrl, 0x11);
        CODE_CONVERT(Alt, 0x12);
        CODE_CONVERT(Pause, 0x13);
        CODE_CONVERT(CapsLock, 0x14);
        CODE_CONVERT(Kana, 0x15);
        CODE_CONVERT(Hangul, 0x15);
        CODE_CONVERT(One, 0x16);
        CODE_CONVERT(Junja, 0x17);
        CODE_CONVERT(Final, 0x18);
        CODE_CONVERT(Hanja, 0x19);
        CODE_CONVERT(Kanji, 0x19);
        CODE_CONVERT(Off, 0x1A);
        CODE_CONVERT(Escape, 0x1B);
        CODE_CONVERT(Convert, 0x1C);
        CODE_CONVERT(NonConvert, 0x1D);
        CODE_CONVERT(Accept, 0x1E);
        CODE_CONVERT(ModeChange, 0x1F);
        CODE_CONVERT(Space, 0x20);
        CODE_CONVERT(PageUp, 0x21);
        CODE_CONVERT(PageDown, 0x22);
        CODE_CONVERT(End, 0x23);
        CODE_CONVERT(Home, 0x24);
        CODE_CONVERT(Left, 0x25);
        CODE_CONVERT(Up, 0x26);
        CODE_CONVERT(Right, 0x27);
        CODE_CONVERT(Down, 0x28);
        CODE_CONVERT(Select, 0x29);
        CODE_CONVERT(Print, 0x2A);
        CODE_CONVERT(Execute, 0x2B);
        CODE_CONVERT(PrintScreen, 0x2C);
        CODE_CONVERT(Insert, 0x2D);
        CODE_CONVERT(Delete, 0x2E);
        CODE_CONVERT(Help, 0x2F);

        CODE_CONVERT(Keypad0, 0x30);
        CODE_CONVERT(Keypad1, 0x31);
        CODE_CONVERT(Keypad2, 0x32);
        CODE_CONVERT(Keypad3, 0x33);
        CODE_CONVERT(Keypad4, 0x34);
        CODE_CONVERT(Keypad5, 0x35);
        CODE_CONVERT(Keypad6, 0x36);
        CODE_CONVERT(Keypad7, 0x37);
        CODE_CONVERT(Keypad8, 0x38);
        CODE_CONVERT(Keypad9, 0x39);

        CODE_CONVERT(A, 0x41);
        CODE_CONVERT(B, 0x42);
        CODE_CONVERT(C, 0x43);
        CODE_CONVERT(D, 0x44);
        CODE_CONVERT(E, 0x45);
        CODE_CONVERT(F, 0x46);
        CODE_CONVERT(G, 0x47);
        CODE_CONVERT(H, 0x48);
        CODE_CONVERT(I, 0x49);
        CODE_CONVERT(J, 0x4A);
        CODE_CONVERT(K, 0x4B);
        CODE_CONVERT(L, 0x4C);
        CODE_CONVERT(M, 0x4D);
        CODE_CONVERT(N, 0x4E);
        CODE_CONVERT(O, 0x4F);
        CODE_CONVERT(P, 0x50);
        CODE_CONVERT(Q, 0x51);
        CODE_CONVERT(R, 0x52);
        CODE_CONVERT(S, 0x53);
        CODE_CONVERT(T, 0x54);
        CODE_CONVERT(U, 0x55);
        CODE_CONVERT(V, 0x56);
        CODE_CONVERT(W, 0x57);
        CODE_CONVERT(X, 0x58);
        CODE_CONVERT(Y, 0x59);
        CODE_CONVERT(Z, 0x5A);

        CODE_CONVERT(LSuper, 0x5B);
        CODE_CONVERT(RSuper, 0x5C);
        CODE_CONVERT(Apps, 0x5D);
        CODE_CONVERT(Sleep, 0x5F);

        CODE_CONVERT(Numpad0, 0x60);
        CODE_CONVERT(Numpad1, 0x61);
        CODE_CONVERT(Numpad2, 0x62);
        CODE_CONVERT(Numpad3, 0x63);
        CODE_CONVERT(Numpad4, 0x64);
        CODE_CONVERT(Numpad5, 0x65);
        CODE_CONVERT(Numpad6, 0x66);
        CODE_CONVERT(Numpad7, 0x67);
        CODE_CONVERT(Numpad8, 0x68);
        CODE_CONVERT(Numpad9, 0x69);

        CODE_CONVERT(Multiply, 0x6A);
        CODE_CONVERT(Add, 0x6B);
        CODE_CONVERT(Separator, 0x6C);
        CODE_CONVERT(Substract, 0x6D);
        CODE_CONVERT(Decimal, 0x6E);
        CODE_CONVERT(Divide, 0x6F);

        CODE_CONVERT(F1, 0x70);
        CODE_CONVERT(F2, 0x71);
        CODE_CONVERT(F3, 0x72);
        CODE_CONVERT(F4, 0x73);
        CODE_CONVERT(F5, 0x74);
        CODE_CONVERT(F6, 0x75);
        CODE_CONVERT(F7, 0x76);
        CODE_CONVERT(F8, 0x77);
        CODE_CONVERT(F9, 0x78);
        CODE_CONVERT(F10, 0x79);
        CODE_CONVERT(F11, 0x7A);
        CODE_CONVERT(F12, 0x7B);
        CODE_CONVERT(F13, 0x7C);
        CODE_CONVERT(F14, 0x7D);
        CODE_CONVERT(F15, 0x7E);
        CODE_CONVERT(F16, 0x7F);
        CODE_CONVERT(F17, 0x80);
        CODE_CONVERT(F18, 0x81);
        CODE_CONVERT(F19, 0x82);
        CODE_CONVERT(F20, 0x83);
        CODE_CONVERT(F21, 0x84);
        CODE_CONVERT(F22, 0x85);
        CODE_CONVERT(F23, 0x86);
        CODE_CONVERT(F24, 0x87);

        CODE_CONVERT(NumLock, 0x90);
        CODE_CONVERT(ScrollLock, 0x91);

        CODE_CONVERT(LShift, 0xA0);
        CODE_CONVERT(RShift, 0xA1);
        CODE_CONVERT(LCtrl, 0xA2);
        CODE_CONVERT(RCtrl, 0xA3);
        CODE_CONVERT(LAlt, 0xA4);
        CODE_CONVERT(RAlt, 0xA5);
        CODE_CONVERT(BrowserBack, 0xA6);
        CODE_CONVERT(BrowserForward, 0xA7);
        CODE_CONVERT(BrowserRefresh, 0xA8);
        CODE_CONVERT(BrowserStop, 0xA9);
        CODE_CONVERT(BrowserSearch, 0xAA);
        CODE_CONVERT(BrowserFavorites, 0xAB);
        CODE_CONVERT(BrowserStart, 0xAC);
        CODE_CONVERT(VolumeMute, 0xAD);
        CODE_CONVERT(VolumeDown, 0xAE);
        CODE_CONVERT(VolumeUp, 0xAF);
        CODE_CONVERT(NextTrack, 0xB0);
        CODE_CONVERT(PreviousTrack, 0xB1);
        CODE_CONVERT(StopMedia, 0xB2);
        CODE_CONVERT(PlayPauseMedia, 0xB3);
        CODE_CONVERT(StartMail, 0xB4);
        CODE_CONVERT(SelectMedia, 0xB5);
        CODE_CONVERT(StartApp1, 0xB6);
        CODE_CONVERT(StartApp2, 0xB7);

        CODE_CONVERT(Semicolon, 0xBA);
        CODE_CONVERT(Equal, 0xBB);
        CODE_CONVERT(Comma, 0xBC);
        CODE_CONVERT(Minus, 0xBD);
        CODE_CONVERT(Period, 0xBE);
        CODE_CONVERT(Slash, 0xBF);
        CODE_CONVERT(Backtick, 0xC0);

        CODE_CONVERT(OpeningBracket, 0xDB);
        CODE_CONVERT(Backslash, 0xDC);
        CODE_CONVERT(ClosingBracket, 0xDD);
        CODE_CONVERT(Apostrophe, 0xDE);
        CODE_CONVERT(Miscellaneous, 0xDF);

        CODE_CONVERT(AngleBracket, 0xE2);

        CODE_CONVERT(Process, 0xE5);
        CODE_CONVERT(Unicode, 0xE7);

        CODE_CONVERT(Attn, 0xF6);
        CODE_CONVERT(CrSel, 0xF7);
        CODE_CONVERT(ExSel, 0xF8);
        CODE_CONVERT(EraseEOF, 0xF9);
        CODE_CONVERT(Play, 0xFA);
        CODE_CONVERT(Zoom, 0xFB);
        CODE_CONVERT(NoName, 0xFC);
        CODE_CONVERT(Pa1, 0xFD);
        CODE_CONVERT(Clear, 0xFE);

        CODE_CONVERT(Colon, 0xFE + 1);
        CODE_CONVERT(Quotation, 0xFE + 2);
        CODE_CONVERT(Plus, 0xFE + 3);

        return 0;
    }

	Keyboard::Code WindowEventCode::WinkeyToKeycode(uint64 winkeycode)
	{
        #define CODE_CONVERT(code_id, value) if (winkeycode == value) return Keyboard::code_id
        
        CODE_CONVERT(Back, 0x08);
        CODE_CONVERT(Tab, 0x09);
        CODE_CONVERT(Clear, 0x0C);
        CODE_CONVERT(Return, 0x0D);
        CODE_CONVERT(Shift, 0x10);
        CODE_CONVERT(Ctrl, 0x11);
        CODE_CONVERT(Alt, 0x12);
        CODE_CONVERT(Pause, 0x13);
        CODE_CONVERT(CapsLock, 0x14);
        CODE_CONVERT(Kana, 0x15);
        CODE_CONVERT(Hangul, 0x15);
        CODE_CONVERT(One, 0x16);
        CODE_CONVERT(Junja, 0x17);
        CODE_CONVERT(Final, 0x18);
        CODE_CONVERT(Hanja, 0x19);
        CODE_CONVERT(Kanji, 0x19);
        CODE_CONVERT(Off, 0x1A);
        CODE_CONVERT(Escape, 0x1B);
        CODE_CONVERT(Convert, 0x1C);
        CODE_CONVERT(NonConvert, 0x1D);
        CODE_CONVERT(Accept, 0x1E);
        CODE_CONVERT(ModeChange, 0x1F);
        CODE_CONVERT(Space, 0x20);
        CODE_CONVERT(PageUp, 0x21);
        CODE_CONVERT(PageDown, 0x22);
        CODE_CONVERT(End, 0x23);
        CODE_CONVERT(Home, 0x24);
        CODE_CONVERT(Left, 0x25);
        CODE_CONVERT(Up, 0x26);
        CODE_CONVERT(Right, 0x27);
        CODE_CONVERT(Down, 0x28);
        CODE_CONVERT(Select, 0x29);
        CODE_CONVERT(Print, 0x2A);
        CODE_CONVERT(Execute, 0x2B);
        CODE_CONVERT(PrintScreen, 0x2C);
        CODE_CONVERT(Insert, 0x2D);
        CODE_CONVERT(Delete, 0x2E);
        CODE_CONVERT(Help, 0x2F);

        CODE_CONVERT(Keypad0, 0x30);
        CODE_CONVERT(Keypad1, 0x31);
        CODE_CONVERT(Keypad2, 0x32);
        CODE_CONVERT(Keypad3, 0x33);
        CODE_CONVERT(Keypad4, 0x34);
        CODE_CONVERT(Keypad5, 0x35);
        CODE_CONVERT(Keypad6, 0x36);
        CODE_CONVERT(Keypad7, 0x37);
        CODE_CONVERT(Keypad8, 0x38);
        CODE_CONVERT(Keypad9, 0x39);

        CODE_CONVERT(A, 0x41);
        CODE_CONVERT(B, 0x42);
        CODE_CONVERT(C, 0x43);
        CODE_CONVERT(D, 0x44);
        CODE_CONVERT(E, 0x45);
        CODE_CONVERT(F, 0x46);
        CODE_CONVERT(G, 0x47);
        CODE_CONVERT(H, 0x48);
        CODE_CONVERT(I, 0x49);
        CODE_CONVERT(J, 0x4A);
        CODE_CONVERT(K, 0x4B);
        CODE_CONVERT(L, 0x4C);
        CODE_CONVERT(M, 0x4D);
        CODE_CONVERT(N, 0x4E);
        CODE_CONVERT(O, 0x4F);
        CODE_CONVERT(P, 0x50);
        CODE_CONVERT(Q, 0x51);
        CODE_CONVERT(R, 0x52);
        CODE_CONVERT(S, 0x53);
        CODE_CONVERT(T, 0x54);
        CODE_CONVERT(U, 0x55);
        CODE_CONVERT(V, 0x56);
        CODE_CONVERT(W, 0x57);
        CODE_CONVERT(X, 0x58);
        CODE_CONVERT(Y, 0x59);
        CODE_CONVERT(Z, 0x5A);

        CODE_CONVERT(LSuper, 0x5B);
        CODE_CONVERT(RSuper, 0x5C);
        CODE_CONVERT(Apps, 0x5D);
        CODE_CONVERT(Sleep, 0x5F);

        CODE_CONVERT(Numpad0, 0x60);
        CODE_CONVERT(Numpad1, 0x61);
        CODE_CONVERT(Numpad2, 0x62);
        CODE_CONVERT(Numpad3, 0x63);
        CODE_CONVERT(Numpad4, 0x64);
        CODE_CONVERT(Numpad5, 0x65);
        CODE_CONVERT(Numpad6, 0x66);
        CODE_CONVERT(Numpad7, 0x67);
        CODE_CONVERT(Numpad8, 0x68);
        CODE_CONVERT(Numpad9, 0x69);

        CODE_CONVERT(Multiply, 0x6A);
        CODE_CONVERT(Add, 0x6B);
        CODE_CONVERT(Separator, 0x6C);
        CODE_CONVERT(Substract, 0x6D);
        CODE_CONVERT(Decimal, 0x6E);
        CODE_CONVERT(Divide, 0x6F);

        CODE_CONVERT(F1, 0x70);
        CODE_CONVERT(F2, 0x71);
        CODE_CONVERT(F3, 0x72);
        CODE_CONVERT(F4, 0x73);
        CODE_CONVERT(F5, 0x74);
        CODE_CONVERT(F6, 0x75);
        CODE_CONVERT(F7, 0x76);
        CODE_CONVERT(F8, 0x77);
        CODE_CONVERT(F9, 0x78);
        CODE_CONVERT(F10, 0x79);
        CODE_CONVERT(F11, 0x7A);
        CODE_CONVERT(F12, 0x7B);
        CODE_CONVERT(F13, 0x7C);
        CODE_CONVERT(F14, 0x7D);
        CODE_CONVERT(F15, 0x7E);
        CODE_CONVERT(F16, 0x7F);
        CODE_CONVERT(F17, 0x80);
        CODE_CONVERT(F18, 0x81);
        CODE_CONVERT(F19, 0x82);
        CODE_CONVERT(F20, 0x83);
        CODE_CONVERT(F21, 0x84);
        CODE_CONVERT(F22, 0x85);
        CODE_CONVERT(F23, 0x86);
        CODE_CONVERT(F24, 0x87);

        CODE_CONVERT(NumLock, 0x90);
        CODE_CONVERT(ScrollLock, 0x91);

        CODE_CONVERT(LShift, 0xA0);
        CODE_CONVERT(RShift, 0xA1);
        CODE_CONVERT(LCtrl, 0xA2);
        CODE_CONVERT(RCtrl, 0xA3);
        CODE_CONVERT(LAlt, 0xA4);
        CODE_CONVERT(RAlt, 0xA5);
        CODE_CONVERT(BrowserBack, 0xA6);
        CODE_CONVERT(BrowserForward, 0xA7);
        CODE_CONVERT(BrowserRefresh, 0xA8);
        CODE_CONVERT(BrowserStop, 0xA9);
        CODE_CONVERT(BrowserSearch, 0xAA);
        CODE_CONVERT(BrowserFavorites, 0xAB);
        CODE_CONVERT(BrowserStart, 0xAC);
        CODE_CONVERT(VolumeMute, 0xAD);
        CODE_CONVERT(VolumeDown, 0xAE);
        CODE_CONVERT(VolumeUp, 0xAF);
        CODE_CONVERT(NextTrack, 0xB0);
        CODE_CONVERT(PreviousTrack, 0xB1);
        CODE_CONVERT(StopMedia, 0xB2);
        CODE_CONVERT(PlayPauseMedia, 0xB3);
        CODE_CONVERT(StartMail, 0xB4);
        CODE_CONVERT(SelectMedia, 0xB5);
        CODE_CONVERT(StartApp1, 0xB6);
        CODE_CONVERT(StartApp2, 0xB7);

        CODE_CONVERT(Semicolon, 0xBA);
        CODE_CONVERT(Equal, 0xBB);
        CODE_CONVERT(Comma, 0xBC);
        CODE_CONVERT(Minus, 0xBD);
        CODE_CONVERT(Period, 0xBE);
        CODE_CONVERT(Slash, 0xBF);
        CODE_CONVERT(Backtick, 0xC0);

        CODE_CONVERT(OpeningBracket, 0xDB);
        CODE_CONVERT(Backslash, 0xDC);
        CODE_CONVERT(ClosingBracket, 0xDD);
        CODE_CONVERT(Apostrophe, 0xDE);
        CODE_CONVERT(Miscellaneous, 0xDF);

        CODE_CONVERT(AngleBracket, 0xE2);

        CODE_CONVERT(Process, 0xE5);
        CODE_CONVERT(Unicode, 0xE7);

        CODE_CONVERT(Attn, 0xF6);
        CODE_CONVERT(CrSel, 0xF7);
        CODE_CONVERT(ExSel, 0xF8);
        CODE_CONVERT(EraseEOF, 0xF9);
        CODE_CONVERT(Play, 0xFA);
        CODE_CONVERT(Zoom, 0xFB);
        CODE_CONVERT(NoName, 0xFC);
        CODE_CONVERT(Pa1, 0xFD);
        CODE_CONVERT(Clear, 0xFE);

        CODE_CONVERT(Colon, 0xFE + 1);
        CODE_CONVERT(Quotation, 0xFE + 2);
        CODE_CONVERT(Plus, 0xFE + 3);

        return Keyboard::NotDefine;
	}

    Keyboard::Code WindowEventCode::WinkeyToKeycodeSpecial(uint64 winkeycode, bool shifDown)
    {
        Keyboard::Code keycode = WindowEventCode::WinkeyToKeycode(winkeycode);

        if (keycode == Keyboard::LCtrl && GetKeyState(VK_RCONTROL)) {
            return Keyboard::RCtrl;
        }

        if (keycode == Keyboard::LShift && GetKeyState(VK_RSHIFT)) {
            return Keyboard::RShift;
        }

        if (keycode == Keyboard::LAlt && GetKeyState(VK_RMENU)) {
            return Keyboard::RAlt;
        }

        if (shifDown) {
            if (keycode == Keyboard::Semicolon) {
                return Keyboard::Colon;
            }

            if (keycode == Keyboard::Apostrophe) {
                return Keyboard::Quotation;
            }

            if (keycode == Keyboard::Equal) {
                return Keyboard::Plus;
            }
        }

        return keycode;
    }

    uint64 WindowEventCode::ScancodeToWinkey(Keyboard::Code code)
    {
        #define CODE_CONVERT(code_id, value) if (code == Keyboard::code_id##) return value;

        CODE_CONVERT(Off, 0x00);
        CODE_CONVERT(Escape, 0x01);

        CODE_CONVERT(Keypad1, 0x02);
        CODE_CONVERT(Keypad2, 0x03);
        CODE_CONVERT(Keypad3, 0x04);
        CODE_CONVERT(Keypad4, 0x05);
        CODE_CONVERT(Keypad5, 0x06);
        CODE_CONVERT(Keypad6, 0x07);
        CODE_CONVERT(Keypad7, 0x08);
        CODE_CONVERT(Keypad8, 0x09);
        CODE_CONVERT(Keypad9, 0x0A);
        CODE_CONVERT(Keypad0, 0x0B);

        CODE_CONVERT(Minus, 0x0C);
        CODE_CONVERT(Equal, 0x0D);
        CODE_CONVERT(Plus, 0x0D);
        CODE_CONVERT(Tab, 0x0F);

        CODE_CONVERT(Q, 0x10);
        CODE_CONVERT(W, 0x11);
        CODE_CONVERT(E, 0x12);
        CODE_CONVERT(R, 0x13);
        CODE_CONVERT(T, 0x14);
        CODE_CONVERT(Y, 0x15);
        CODE_CONVERT(U, 0x16);
        CODE_CONVERT(I, 0x17);
        CODE_CONVERT(O, 0x18);
        CODE_CONVERT(P, 0x19);

        CODE_CONVERT(OpeningBracket, 0x1A);
        CODE_CONVERT(ClosingBracket, 0x1B);
        CODE_CONVERT(Return, 0x1C);
        CODE_CONVERT(Accept, 0x1C);
        CODE_CONVERT(LCtrl, 0x1D);

        CODE_CONVERT(A, 0x1E);
        CODE_CONVERT(S, 0x1F);
        CODE_CONVERT(D, 0x20);
        CODE_CONVERT(F, 0x21);
        CODE_CONVERT(G, 0x22);
        CODE_CONVERT(H, 0x23);
        CODE_CONVERT(J, 0x24);
        CODE_CONVERT(K, 0x25);
        CODE_CONVERT(L, 0x26);

        CODE_CONVERT(Colon, 0x27);
        CODE_CONVERT(Semicolon, 0x27);
        CODE_CONVERT(Quotation, 0x28);
        CODE_CONVERT(Apostrophe, 0x28);
        CODE_CONVERT(Backtick, 0x29);
        CODE_CONVERT(Shift, 0x2A);
        CODE_CONVERT(LShift, 0x2A);
        CODE_CONVERT(Backslash, 0x2B);
        CODE_CONVERT(Execute, 0x2B);

        CODE_CONVERT(Z, 0x2C);
        CODE_CONVERT(X, 0x2D);
        CODE_CONVERT(C, 0x2E);
        CODE_CONVERT(V, 0x2F);
        CODE_CONVERT(B, 0x30);
        CODE_CONVERT(N, 0x31);
        CODE_CONVERT(M, 0x32);

        CODE_CONVERT(Comma, 0x33);
        CODE_CONVERT(Period, 0x34);
        CODE_CONVERT(Divide, 0x35);
        CODE_CONVERT(RShift, 0x36);
        CODE_CONVERT(Multiply, 0x37);
        CODE_CONVERT(Print, 0x37);
        CODE_CONVERT(LAlt, 0x38);
        CODE_CONVERT(LAlt, 0x38);
        CODE_CONVERT(Alt, 0x38);
        CODE_CONVERT(Space, 0x39);
        CODE_CONVERT(CapsLock, 0x3A);

        CODE_CONVERT(F1, 0x3B);
        CODE_CONVERT(F2, 0x3C);
        CODE_CONVERT(F3, 0x3D);
        CODE_CONVERT(F4, 0x3E);
        CODE_CONVERT(F5, 0x3F);
        CODE_CONVERT(F6, 0x40);
        CODE_CONVERT(F7, 0x41);
        CODE_CONVERT(F8, 0x42);
        CODE_CONVERT(F9, 0x43);
        CODE_CONVERT(F10, 0x44);

        CODE_CONVERT(NumLock, 0x45);
        CODE_CONVERT(Pause, 0x45);
        CODE_CONVERT(ScrollLock, 0x46);
        CODE_CONVERT(Home, 0x47);
        CODE_CONVERT(Up, 0x48);
        CODE_CONVERT(PageUp, 0x49);
        CODE_CONVERT(Left, 0x4B);
        CODE_CONVERT(Right, 0x4D);
        CODE_CONVERT(Substract, 0x4A);
        CODE_CONVERT(Add, 0x4E);
        CODE_CONVERT(End, 0x4F);
        CODE_CONVERT(Down, 0x50);
        CODE_CONVERT(PageDown, 0x51);
        CODE_CONVERT(Insert, 0x52);
        CODE_CONVERT(Delete, 0x53);
        CODE_CONVERT(Separator, 0x53);
        CODE_CONVERT(Decimal, 0x53);
        CODE_CONVERT(Miscellaneous, 0x56);
        CODE_CONVERT(LSuper, 0x5B);
        CODE_CONVERT(RSuper, 0x5C);
        CODE_CONVERT(Apps, 0x5D);

        CODE_CONVERT(F13, 0x64);
        CODE_CONVERT(F14, 0x65);
        CODE_CONVERT(F15, 0x66);
        CODE_CONVERT(F16, 0x67);
        CODE_CONVERT(F17, 0x68);
        CODE_CONVERT(F18, 0x69);

        CODE_CONVERT(BrowserSearch, 0x65);
        CODE_CONVERT(BrowserFavorites, 0x66);
        CODE_CONVERT(BrowserStart, 0x67);
        CODE_CONVERT(BrowserRefresh, 0x68);
        CODE_CONVERT(BrowserForward, 0x69);
        CODE_CONVERT(BrowserBack, 0x6A);
        CODE_CONVERT(BrowserStop, 0x6B);
        CODE_CONVERT(StartMail, 0x6C);
        CODE_CONVERT(SelectMedia, 0x6D);
        CODE_CONVERT(StartApp1, 0x6E);
        CODE_CONVERT(StartApp2, 0x6F);

        CODE_CONVERT(F19, 0x71);
        CODE_CONVERT(F20, 0x72);
        CODE_CONVERT(F21, 0x73);
        CODE_CONVERT(F22, 0x74);
        CODE_CONVERT(F23, 0x75);
        CODE_CONVERT(F24, 0x76);

        CODE_CONVERT(Select, 0x77);
        CODE_CONVERT(StopMedia, 0x78);
        CODE_CONVERT(PreviousTrack, 0x7B);
        CODE_CONVERT(PlayPauseMedia, 0x7C);
        CODE_CONVERT(NextTrack, 0x7E);
        CODE_CONVERT(VolumeMute, 0x7F);
        CODE_CONVERT(VolumeUp, 0x80);
        CODE_CONVERT(VolumeDown, 0x81);
        CODE_CONVERT(Unicode, 0xE4);
        CODE_CONVERT(Process, 0xE7);
        CODE_CONVERT(Attn, 0xF6);
        CODE_CONVERT(CrSel, 0xF7);
        CODE_CONVERT(ExSel, 0xF8);
        CODE_CONVERT(EraseEOF, 0xF9);
        CODE_CONVERT(Play, 0xFA);
        CODE_CONVERT(Zoom, 0xFB);
        CODE_CONVERT(NoName, 0xFC);
        CODE_CONVERT(Pa1, 0xFD);
        CODE_CONVERT(RAlt, 0xE038);

        return 0;
    }

    Keyboard::Code WindowEventCode::WinkeyToScancode(uint64 winkeycode)
    {
        #define CODE_CONVERT(code_id, value) if (winkeycode == value) return Keyboard::code_id

        CODE_CONVERT(Off, 0x00);
        CODE_CONVERT(Escape, 0x01);

        CODE_CONVERT(Keypad1, 0x02);
        CODE_CONVERT(Keypad2, 0x03);
        CODE_CONVERT(Keypad3, 0x04);
        CODE_CONVERT(Keypad4, 0x05);
        CODE_CONVERT(Keypad5, 0x06);
        CODE_CONVERT(Keypad6, 0x07);
        CODE_CONVERT(Keypad7, 0x08);
        CODE_CONVERT(Keypad8, 0x09);
        CODE_CONVERT(Keypad9, 0x0A);
        CODE_CONVERT(Keypad0, 0x0B);

        CODE_CONVERT(Minus, 0x0C);
        CODE_CONVERT(Equal, 0x0D);
        CODE_CONVERT(Plus, 0x0D);
        CODE_CONVERT(Tab, 0x0F);

        CODE_CONVERT(Q, 0x10);
        CODE_CONVERT(W, 0x11);
        CODE_CONVERT(E, 0x12);
        CODE_CONVERT(R, 0x13);
        CODE_CONVERT(T, 0x14);
        CODE_CONVERT(Y, 0x15);
        CODE_CONVERT(U, 0x16);
        CODE_CONVERT(I, 0x17);
        CODE_CONVERT(O, 0x18);
        CODE_CONVERT(P, 0x19);

        CODE_CONVERT(OpeningBracket, 0x1A);
        CODE_CONVERT(ClosingBracket, 0x1B);
        CODE_CONVERT(Return, 0x1C);
        CODE_CONVERT(Accept, 0x1C);
        CODE_CONVERT(LCtrl, 0x1D);

        CODE_CONVERT(A, 0x1E);
        CODE_CONVERT(S, 0x1F);
        CODE_CONVERT(D, 0x20);
        CODE_CONVERT(F, 0x21);
        CODE_CONVERT(G, 0x22);
        CODE_CONVERT(H, 0x23);
        CODE_CONVERT(J, 0x24);
        CODE_CONVERT(K, 0x25);
        CODE_CONVERT(L, 0x26);

        CODE_CONVERT(Colon, 0x27);
        CODE_CONVERT(Semicolon, 0x27);
        CODE_CONVERT(Quotation, 0x28);
        CODE_CONVERT(Apostrophe, 0x28);
        CODE_CONVERT(Backtick, 0x29);
        CODE_CONVERT(Shift, 0x2A);
        CODE_CONVERT(LShift, 0x2A);
        CODE_CONVERT(Backslash, 0x2B);
        CODE_CONVERT(Execute, 0x2B);

        CODE_CONVERT(Z, 0x2C);
        CODE_CONVERT(X, 0x2D);
        CODE_CONVERT(C, 0x2E);
        CODE_CONVERT(V, 0x2F);
        CODE_CONVERT(B, 0x30);
        CODE_CONVERT(N, 0x31);
        CODE_CONVERT(M, 0x32);

        CODE_CONVERT(Comma, 0x33);
        CODE_CONVERT(Period, 0x34);
        CODE_CONVERT(Divide, 0x35);
        CODE_CONVERT(RShift, 0x36);
        CODE_CONVERT(Multiply, 0x37);
        CODE_CONVERT(Print, 0x37);
        CODE_CONVERT(LAlt, 0x38);
        CODE_CONVERT(LAlt, 0x38);
        CODE_CONVERT(Alt, 0x38);
        CODE_CONVERT(Space, 0x39);
        CODE_CONVERT(CapsLock, 0x3A);

        CODE_CONVERT(F1, 0x3B);
        CODE_CONVERT(F2, 0x3C);
        CODE_CONVERT(F3, 0x3D);
        CODE_CONVERT(F4, 0x3E);
        CODE_CONVERT(F5, 0x3F);
        CODE_CONVERT(F6, 0x40);
        CODE_CONVERT(F7, 0x41);
        CODE_CONVERT(F8, 0x42);
        CODE_CONVERT(F9, 0x43);
        CODE_CONVERT(F10, 0x44);

        CODE_CONVERT(NumLock, 0x45);
        CODE_CONVERT(Pause, 0x45);
        CODE_CONVERT(ScrollLock, 0x46);
        CODE_CONVERT(Home, 0x47);
        CODE_CONVERT(Up, 0x48);
        CODE_CONVERT(PageUp, 0x49);
        CODE_CONVERT(Left, 0x4B);
        CODE_CONVERT(Right, 0x4D);
        CODE_CONVERT(Substract, 0x4A);
        CODE_CONVERT(Add, 0x4E);
        CODE_CONVERT(End, 0x4F);
        CODE_CONVERT(Down, 0x50);
        CODE_CONVERT(PageDown, 0x51);
        CODE_CONVERT(Insert, 0x52);
        CODE_CONVERT(Delete, 0x53);
        CODE_CONVERT(Separator, 0x53);
        CODE_CONVERT(Decimal, 0x53);
        CODE_CONVERT(Miscellaneous, 0x56);
        CODE_CONVERT(LSuper, 0x5B);
        CODE_CONVERT(RSuper, 0x5C);
        CODE_CONVERT(Apps, 0x5D);

        CODE_CONVERT(F13, 0x64);
        CODE_CONVERT(F14, 0x65);
        CODE_CONVERT(F15, 0x66);
        CODE_CONVERT(F16, 0x67);
        CODE_CONVERT(F17, 0x68);
        CODE_CONVERT(F18, 0x69);

        CODE_CONVERT(BrowserSearch, 0x65);
        CODE_CONVERT(BrowserFavorites, 0x66);
        CODE_CONVERT(BrowserStart, 0x67);
        CODE_CONVERT(BrowserRefresh, 0x68);
        CODE_CONVERT(BrowserForward, 0x69);
        CODE_CONVERT(BrowserBack, 0x6A);
        CODE_CONVERT(BrowserStop, 0x6B);
        CODE_CONVERT(StartMail, 0x6C);
        CODE_CONVERT(SelectMedia, 0x6D);
        CODE_CONVERT(StartApp1, 0x6E);
        CODE_CONVERT(StartApp2, 0x6F);

        CODE_CONVERT(F19, 0x71);
        CODE_CONVERT(F20, 0x72);
        CODE_CONVERT(F21, 0x73);
        CODE_CONVERT(F22, 0x74);
        CODE_CONVERT(F23, 0x75);
        CODE_CONVERT(F24, 0x76);

        CODE_CONVERT(Select, 0x77);
        CODE_CONVERT(StopMedia, 0x78);
        CODE_CONVERT(PreviousTrack, 0x7B);
        CODE_CONVERT(PlayPauseMedia, 0x7C);
        CODE_CONVERT(NextTrack, 0x7E);
        CODE_CONVERT(VolumeMute, 0x7F);
        CODE_CONVERT(VolumeUp, 0x80);
        CODE_CONVERT(VolumeDown, 0x81);
        CODE_CONVERT(Unicode, 0xE4);
        CODE_CONVERT(Process, 0xE7);
        CODE_CONVERT(Attn, 0xF6);
        CODE_CONVERT(CrSel, 0xF7);
        CODE_CONVERT(ExSel, 0xF8);
        CODE_CONVERT(EraseEOF, 0xF9);
        CODE_CONVERT(Play, 0xFA);
        CODE_CONVERT(Zoom, 0xFB);
        CODE_CONVERT(NoName, 0xFC);
        CODE_CONVERT(Pa1, 0xFD);
        CODE_CONVERT(RAlt, 0xE038);

        return Keyboard::NotDefine;
    }

    Keyboard::Code WindowEventCode::WinkeyToScancodeSpecial(uint64 winkeycode, bool shifDown)
    {
        Keyboard::Code scancode = WindowEventCode::WinkeyToScancode(winkeycode);

        if (scancode == Keyboard::LCtrl && GetKeyState(VK_RCONTROL)) {
            return Keyboard::RCtrl;
        }

        if (scancode == Keyboard::LShift && GetKeyState(VK_RSHIFT)) {
            return Keyboard::RShift;
        }

        if (scancode == Keyboard::LAlt && GetKeyState(VK_RMENU)) {
            return Keyboard::RAlt;
        }

        if (shifDown) {
            if (scancode == Keyboard::Semicolon) {
                return Keyboard::Colon;
            }

            if (scancode == Keyboard::Apostrophe) {
                return Keyboard::Quotation;
            }

            if (scancode == Keyboard::Equal) {
                return Keyboard::Plus;
            }
        }

        return scancode;
    }

    ModifierState WindowEventCode::ModifierStateToWinkey()
    {
        bool ctrl = GetKeyState(VK_CONTROL) & 0x8000;
        bool alt = GetKeyState(VK_MENU) & 0x8000;
        bool shift = (GetKeyState(VK_SHIFT) & 0x8000) | (GetKeyState(VK_CAPITAL) & 0x0001);
        bool super = false;
        return ModifierState(ctrl, alt, shift, super);
    }
}    // namespace nkentseu

#endif