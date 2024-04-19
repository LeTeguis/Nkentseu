//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Keyboard.h"

#include <cstring>

namespace nkentseu {

    const char* Keyboard::GetCodeName(Keyboard::Code code) {
        #define CODE_STR(code_id) if (code == code_id) return #code_id

        CODE_STR(Back);
        CODE_STR(Tab);
        CODE_STR(Clear);
        CODE_STR(Return);
        CODE_STR(Shift);
        CODE_STR(Ctrl);
        CODE_STR(Alt);
        CODE_STR(Pause);
        CODE_STR(CapsLock);
        CODE_STR(Kana);
        CODE_STR(Hangul);
        CODE_STR(One);
        CODE_STR(Junja);
        CODE_STR(Final);
        CODE_STR(Hanja);
        CODE_STR(Kanji);
        CODE_STR(Off);
        CODE_STR(Escape);
        CODE_STR(Convert);
        CODE_STR(NonConvert);
        CODE_STR(Accept);
        CODE_STR(ModeChange);
        CODE_STR(Space);
        CODE_STR(PageUp);
        CODE_STR(PageDown);
        CODE_STR(End);
        CODE_STR(Home);
        CODE_STR(Left);
        CODE_STR(Up);
        CODE_STR(Right);
        CODE_STR(Down);
        CODE_STR(Select);
        CODE_STR(Print);
        CODE_STR(Execute);
        CODE_STR(PrintScreen);
        CODE_STR(Insert);
        CODE_STR(Delete);
        CODE_STR(Help);

        CODE_STR(Keypad0);
        CODE_STR(Keypad1);
        CODE_STR(Keypad2);
        CODE_STR(Keypad3);
        CODE_STR(Keypad4);
        CODE_STR(Keypad5);
        CODE_STR(Keypad6);
        CODE_STR(Keypad7);
        CODE_STR(Keypad8);
        CODE_STR(Keypad9);

        CODE_STR(A);
        CODE_STR(B);
        CODE_STR(C);
        CODE_STR(D);
        CODE_STR(E);
        CODE_STR(F);
        CODE_STR(G);
        CODE_STR(H);
        CODE_STR(I);
        CODE_STR(J);
        CODE_STR(K);
        CODE_STR(L);
        CODE_STR(M);
        CODE_STR(N);
        CODE_STR(O);
        CODE_STR(P);
        CODE_STR(Q);
        CODE_STR(R);
        CODE_STR(S);
        CODE_STR(T);
        CODE_STR(U);
        CODE_STR(V);
        CODE_STR(W);
        CODE_STR(X);
        CODE_STR(Y);
        CODE_STR(Z);

        CODE_STR(LSuper);
        CODE_STR(RSuper);
        CODE_STR(Apps);
        CODE_STR(Sleep);

        CODE_STR(Numpad0);
        CODE_STR(Numpad1);
        CODE_STR(Numpad2);
        CODE_STR(Numpad3);
        CODE_STR(Numpad4);
        CODE_STR(Numpad5);
        CODE_STR(Numpad6);
        CODE_STR(Numpad7);
        CODE_STR(Numpad8);
        CODE_STR(Numpad9);

        CODE_STR(Multiply);
        CODE_STR(Add);
        CODE_STR(Separator);
        CODE_STR(Substract);
        CODE_STR(Decimal);
        CODE_STR(Divide);

        CODE_STR(F1);
        CODE_STR(F2);
        CODE_STR(F3);
        CODE_STR(F4);
        CODE_STR(F5);
        CODE_STR(F6);
        CODE_STR(F7);
        CODE_STR(F8);
        CODE_STR(F9);
        CODE_STR(F10);
        CODE_STR(F11);
        CODE_STR(F12);
        CODE_STR(F13);
        CODE_STR(F14);
        CODE_STR(F15);
        CODE_STR(F16);
        CODE_STR(F17);
        CODE_STR(F18);
        CODE_STR(F19);
        CODE_STR(F20);
        CODE_STR(F21);
        CODE_STR(F22);
        CODE_STR(F23);
        CODE_STR(F24);

        CODE_STR(NumLock);
        CODE_STR(ScrollLock);

        CODE_STR(LShift);
        CODE_STR(RShift);
        CODE_STR(LCtrl);
        CODE_STR(RCtrl);
        CODE_STR(LAlt);
        CODE_STR(RAlt);
        CODE_STR(BrowserBack);
        CODE_STR(BrowserForward);
        CODE_STR(BrowserRefresh);
        CODE_STR(BrowserStop);
        CODE_STR(BrowserSearch);
        CODE_STR(BrowserFavorites);
        CODE_STR(BrowserStart);
        CODE_STR(VolumeMute);
        CODE_STR(VolumeDown);
        CODE_STR(VolumeUp);
        CODE_STR(NextTrack);
        CODE_STR(PreviousTrack);
        CODE_STR(StopMedia);
        CODE_STR(PlayPauseMedia);
        CODE_STR(StartMail);
        CODE_STR(SelectMedia);
        CODE_STR(StartApp1);
        CODE_STR(StartApp2);

        CODE_STR(Semicolon);
        CODE_STR(Equal);
        CODE_STR(Comma);
        CODE_STR(Minus);
        CODE_STR(Period);
        CODE_STR(Slash);
        CODE_STR(Backtick);

        CODE_STR(OpeningBracket);
        CODE_STR(Backslash);
        CODE_STR(ClosingBracket);
        CODE_STR(Apostrophe);
        CODE_STR(Miscellaneous);

        CODE_STR(AngleBracket);

        CODE_STR(Process);
        CODE_STR(Unicode);

        CODE_STR(Attn);
        CODE_STR(CrSel);
        CODE_STR(ExSel);
        CODE_STR(EraseEOF);
        CODE_STR(Play);
        CODE_STR(Zoom);
        CODE_STR(NoName);
        CODE_STR(Pa1);
        CODE_STR(Clear);

        CODE_STR(Colon);
        CODE_STR(Quotation);
        CODE_STR(Plus);

        return "NotDefine";
    }

    Keyboard::Code Keyboard::GetCodeFromName(const char* codeName) {
        #define CASE_CODE(code_id) if (strcmp(codeName, #code_id) == 0) return Keyboard::code_id

        CASE_CODE(Back);
        CASE_CODE(Tab);
        CASE_CODE(Clear);
        CASE_CODE(Return);
        CASE_CODE(Shift);
        CASE_CODE(Ctrl);
        CASE_CODE(Alt);
        CASE_CODE(Pause);
        CASE_CODE(CapsLock);
        CASE_CODE(Kana);
        CASE_CODE(Hangul);
        CASE_CODE(One);
        CASE_CODE(Junja);
        CASE_CODE(Final);
        CASE_CODE(Hanja);
        CASE_CODE(Kanji);
        CASE_CODE(Off);
        CASE_CODE(Escape);
        CASE_CODE(Convert);
        CASE_CODE(NonConvert);
        CASE_CODE(Accept);
        CASE_CODE(ModeChange);
        CASE_CODE(Space);
        CASE_CODE(PageUp);
        CASE_CODE(PageDown);
        CASE_CODE(End);
        CASE_CODE(Home);
        CASE_CODE(Left);
        CASE_CODE(Up);
        CASE_CODE(Right);
        CASE_CODE(Down);
        CASE_CODE(Select);
        CASE_CODE(Print);
        CASE_CODE(Execute);
        CASE_CODE(PrintScreen);
        CASE_CODE(Insert);
        CASE_CODE(Delete);
        CASE_CODE(Help);

        CASE_CODE(Keypad0);
        CASE_CODE(Keypad1);
        CASE_CODE(Keypad2);
        CASE_CODE(Keypad3);
        CASE_CODE(Keypad4);
        CASE_CODE(Keypad5);
        CASE_CODE(Keypad6);
        CASE_CODE(Keypad7);
        CASE_CODE(Keypad8);
        CASE_CODE(Keypad9);

        CASE_CODE(A);
        CASE_CODE(B);
        CASE_CODE(C);
        CASE_CODE(D);
        CASE_CODE(E);
        CASE_CODE(F);
        CASE_CODE(G);
        CASE_CODE(H);
        CASE_CODE(I);
        CASE_CODE(J);
        CASE_CODE(K);
        CASE_CODE(L);
        CASE_CODE(M);
        CASE_CODE(N);
        CASE_CODE(O);
        CASE_CODE(P);
        CASE_CODE(Q);
        CASE_CODE(R);
        CASE_CODE(S);
        CASE_CODE(T);
        CASE_CODE(U);
        CASE_CODE(V);
        CASE_CODE(W);
        CASE_CODE(X);
        CASE_CODE(Y);
        CASE_CODE(Z);

        CASE_CODE(LSuper);
        CASE_CODE(RSuper);
        CASE_CODE(Apps);
        CASE_CODE(Sleep);

        CASE_CODE(Numpad0);
        CASE_CODE(Numpad1);
        CASE_CODE(Numpad2);
        CASE_CODE(Numpad3);
        CASE_CODE(Numpad4);
        CASE_CODE(Numpad5);
        CASE_CODE(Numpad6);
        CASE_CODE(Numpad7);
        CASE_CODE(Numpad8);
        CASE_CODE(Numpad9);

        CASE_CODE(Multiply);
        CASE_CODE(Add);
        CASE_CODE(Separator);
        CASE_CODE(Substract);
        CASE_CODE(Decimal);
        CASE_CODE(Divide);

        CASE_CODE(F1);
        CASE_CODE(F2);
        CASE_CODE(F3);
        CASE_CODE(F4);
        CASE_CODE(F5);
        CASE_CODE(F6);
        CASE_CODE(F7);
        CASE_CODE(F8);
        CASE_CODE(F9);
        CASE_CODE(F10);
        CASE_CODE(F11);
        CASE_CODE(F12);
        CASE_CODE(F13);
        CASE_CODE(F14);
        CASE_CODE(F15);
        CASE_CODE(F16);
        CASE_CODE(F17);
        CASE_CODE(F18);
        CASE_CODE(F19);
        CASE_CODE(F20);
        CASE_CODE(F21);
        CASE_CODE(F22);
        CASE_CODE(F23);
        CASE_CODE(F24);

        CASE_CODE(NumLock);
        CASE_CODE(ScrollLock);

        CASE_CODE(LShift);
        CASE_CODE(RShift);
        CASE_CODE(LCtrl);
        CASE_CODE(RCtrl);
        CASE_CODE(LAlt);
        CASE_CODE(RAlt);
        CASE_CODE(BrowserBack);
        CASE_CODE(BrowserForward);
        CASE_CODE(BrowserRefresh);
        CASE_CODE(BrowserStop);
        CASE_CODE(BrowserSearch);
        CASE_CODE(BrowserFavorites);
        CASE_CODE(BrowserStart);
        CASE_CODE(VolumeMute);
        CASE_CODE(VolumeDown);
        CASE_CODE(VolumeUp);
        CASE_CODE(NextTrack);
        CASE_CODE(PreviousTrack);
        CASE_CODE(StopMedia);
        CASE_CODE(PlayPauseMedia);
        CASE_CODE(StartMail);
        CASE_CODE(SelectMedia);
        CASE_CODE(StartApp1);
        CASE_CODE(StartApp2);

        CASE_CODE(Semicolon);
        CASE_CODE(Equal);
        CASE_CODE(Comma);
        CASE_CODE(Minus);
        CASE_CODE(Period);
        CASE_CODE(Slash);
        CASE_CODE(Backtick);

        CASE_CODE(OpeningBracket);
        CASE_CODE(Backslash);
        CASE_CODE(ClosingBracket);
        CASE_CODE(Apostrophe);
        CASE_CODE(Miscellaneous);

        CASE_CODE(AngleBracket);

        CASE_CODE(Process);
        CASE_CODE(Unicode);

        CASE_CODE(Attn);
        CASE_CODE(CrSel);
        CASE_CODE(ExSel);
        CASE_CODE(EraseEOF);
        CASE_CODE(Play);
        CASE_CODE(Zoom);
        CASE_CODE(NoName);
        CASE_CODE(Pa1);
        CASE_CODE(Clear);

        CASE_CODE(Colon);
        CASE_CODE(Quotation);
        CASE_CODE(Plus);

        return Keyboard::NotDefine;
    }


    std::string Keyboard::ToString(Keyboard::Code CODE) {
        const char* codeName = GetCodeName(CODE);
        return codeName;
    }

    Keyboard::Code Keyboard::FromString(const std::string& str) {
        return GetCodeFromName(str.c_str());
    }

    std::string Keyboard::GetKeycode(Keyboard::Keycode keycode) {
        return GetCodeName(keycode);
    }

    Keyboard::Keycode Keyboard::GetKeycode(const std::string& str) {
        return FromString(str);
    }

    std::string Keyboard::GetScancode(Keyboard::Scancode keycode) {
        return GetCodeName(keycode);
    }

    Keyboard::Scancode Keyboard::GetScancode(const std::string& str) {
        return FromString(str);
    }

    bool Keyboard::IsKeyCode(Keyboard::Keycode key) {
        if (FirstCode <= (uint64)key && (uint64)key <= LastCode) return true;
        return false;
    }


    bool Keyboard::IsScanCode(Keyboard::Scancode key) {
        return IsKeyCode(key);
    }

    bool Keyboard::IsKeyMode(Keyboard::KeyMod key) {
        if (IsKeyCode(key) || IsScanCode(key)) {
            if (key == Keyboard::Ctrl || key == Keyboard::LCtrl || key == Keyboard::RCtrl ||
                key == Keyboard::Alt || key == Keyboard::LAlt || key == Keyboard::RAlt ||
                key == Keyboard::Shift || key == Keyboard::LShift || key == Keyboard::RShift ||
                key == Keyboard::LSuper || key == Keyboard::RSuper ||
                key == Keyboard::Num || key == Keyboard::CapsLock || key == Keyboard::ModeChange || key == Keyboard::ScrollLock) {
                return true;
            }
        }
        return false;
    }

}    // namespace nkentseu