//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Keyboard.h"

#include <cstring>

namespace nkentseu {

    const char* Keyboard::GetCodeName(Keyboard::Code code) {
        #define CODE_STR(code_id) if (code == Keyboard::code_id) return #code_id

        CODE_STR(Escape);
        CODE_STR(Tab);
        CODE_STR(Backtab);
        CODE_STR(Backspace);
        CODE_STR(Return);
        CODE_STR(Enter);
        CODE_STR(Insert);
        CODE_STR(Delete);
        CODE_STR(Pause);
        CODE_STR(Print);
        CODE_STR(SysReq);
        CODE_STR(Clear);
        CODE_STR(Home);
        CODE_STR(End);
        CODE_STR(Left);
        CODE_STR(Up);
        CODE_STR(Right);
        CODE_STR(Down);
        CODE_STR(PageUp);
        CODE_STR(PageDown);
        CODE_STR(ShiftLock);
        CODE_STR(Shift);
        CODE_STR(ShiftLeft);
        CODE_STR(ShiftRight);
        CODE_STR(Control);
        CODE_STR(ControlLeft);
        CODE_STR(ControlRight);
        CODE_STR(Meta);
        CODE_STR(MetaLeft);
        CODE_STR(MetaRight);
        CODE_STR(Alt);
        CODE_STR(AltLeft);
        CODE_STR(AltRight);
        CODE_STR(CapsLock);
        CODE_STR(NumLock);
        CODE_STR(ScrollLock);

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
        CODE_STR(F25); 
        CODE_STR(F26);
        CODE_STR(F27);
        CODE_STR(F28);
        CODE_STR(F29);
        CODE_STR(F30);
        CODE_STR(F31);
        CODE_STR(F32);
        CODE_STR(F33);
        CODE_STR(F34);
        CODE_STR(F35);

        CODE_STR(SuperLeft);
        CODE_STR(SuperRight);
        CODE_STR(Undo);
        CODE_STR(Redo);
        CODE_STR(Menu);
        CODE_STR(Find);
        CODE_STR(HyperLeft);
        CODE_STR(HyperRight);
        CODE_STR(Help);
        CODE_STR(DirectionLeft);
        CODE_STR(DirectionRight);
        CODE_STR(Space);
        CODE_STR(Any);
        CODE_STR(Exclam);
        CODE_STR(QuoteDbl);
        CODE_STR(NumberSign);
        CODE_STR(Dollar);
        CODE_STR(Percent);
        CODE_STR(Ampersand);
        CODE_STR(Apostrophe);
        CODE_STR(ParenLeft);
        CODE_STR(ParenRight);
        CODE_STR(Asterisk);
        CODE_STR(Plus);
        CODE_STR(Comma);
        CODE_STR(Minus);
        CODE_STR(Period);
        CODE_STR(Slash);

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

        CODE_STR(Colon);
        CODE_STR(Semicolon);
        CODE_STR(Less);
        CODE_STR(Equal);
        CODE_STR(Greater);
        CODE_STR(Question);
        CODE_STR(At);

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

        CODE_STR(Break);
        CODE_STR(BracketLeft);
        CODE_STR(Backslash);
        CODE_STR(BracketRight);
        CODE_STR(AsciiCircum);
        CODE_STR(Underscore);
        CODE_STR(QuoteLeft);
        CODE_STR(BraceLeft);
        CODE_STR(Bar);
        CODE_STR(BraceRight);
        CODE_STR(AsciiTilde);

        CODE_STR(Nobreakspace);
        CODE_STR(Exclamdown);
        CODE_STR(Cent);
        CODE_STR(Sterling);
        CODE_STR(Currency);
        CODE_STR(Yen);
        CODE_STR(Brokenbar);
        CODE_STR(Section);
        CODE_STR(Diaeresis);
        CODE_STR(Copyright);
        CODE_STR(Ordfeminine);
        CODE_STR(Guillemotleft);
        CODE_STR(Notsign);
        CODE_STR(Hyphen);
        CODE_STR(Registered);
        CODE_STR(Macron);
        CODE_STR(Degree);
        CODE_STR(Plusminus);
        CODE_STR(Twosuperior);
        CODE_STR(Threesuperior);
        CODE_STR(Acute);
        CODE_STR(Mu);
        CODE_STR(Paragraph);
        CODE_STR(Periodcentered);
        CODE_STR(Cedilla);
        CODE_STR(Onesuperior);
        CODE_STR(Masculine);
        CODE_STR(Guillemotright);
        CODE_STR(Onequarter);
        CODE_STR(Onehalf);
        CODE_STR(Threequarters);
        CODE_STR(Questiondown);
        CODE_STR(Agrave);
        CODE_STR(Aacute);
        CODE_STR(Acircumflex);
        CODE_STR(Atilde);
        CODE_STR(Adiaeresis);
        CODE_STR(Aring);
        CODE_STR(AE);
        CODE_STR(Ccedilla);
        CODE_STR(Egrave);
        CODE_STR(Eacute);
        CODE_STR(Ecircumflex);
        CODE_STR(Ediaeresis);
        CODE_STR(Igrave);
        CODE_STR(Iacute);
        CODE_STR(Icircumflex);
        CODE_STR(Idiaeresis);
        CODE_STR(ETH);
        CODE_STR(Eth);
        CODE_STR(Ntilde);
        CODE_STR(Ograve);
        CODE_STR(Oacute);
        CODE_STR(Ocircumflex);
        CODE_STR(Otilde);
        CODE_STR(Odiaeresis);
        CODE_STR(Multiply);
        CODE_STR(Ooblique);
        CODE_STR(Ugrave);
        CODE_STR(Uacute);
        CODE_STR(Ucircumflex);
        CODE_STR(Udiaeresis);
        CODE_STR(Yacute);
        CODE_STR(THORN);
        CODE_STR(Ssharp);
        CODE_STR(Division);
        CODE_STR(Ydiaeresis);

        CODE_STR(AltGr);
        CODE_STR(MultiKey);
        CODE_STR(Codeinput);
        CODE_STR(SingleCandidate);
        CODE_STR(MultipleCandidate);
        CODE_STR(PreviousCandidate);

        CODE_STR(ModeSwitch);
        CODE_STR(ScriptSwitch);

        CODE_STR(Kanji);
        CODE_STR(Muhenkan);
        CODE_STR(HenkanMode);
        CODE_STR(Henkan);
        CODE_STR(Romaji);
        CODE_STR(Hiragana);
        CODE_STR(Katakana);
        CODE_STR(HiraganaKatakana);
        CODE_STR(Zenkaku);
        CODE_STR(Hankaku);
        CODE_STR(ZenkakuHankaku);
        CODE_STR(Touroku);
        CODE_STR(Massyo);
        CODE_STR(KanaLock);
        CODE_STR(KanaShift);
        CODE_STR(EisuShift);
        CODE_STR(EisuToggle);
        CODE_STR(KanjiBangou);
        CODE_STR(ZenKoho);
        CODE_STR(MaeKoho);  

        CODE_STR(Hangul);
        CODE_STR(HangulStart);
        CODE_STR(HangulEnd); 
        CODE_STR(HangulHanja);
        CODE_STR(HangulJamo);
        CODE_STR(HangulRomaja);
        CODE_STR(HangulCodeinput);
        CODE_STR(HangulJeonja);
        CODE_STR(HangulBanja);
        CODE_STR(HangulPreHanja);
        CODE_STR(HangulPostHanja);
        CODE_STR(HangulSingleCandidate);
        CODE_STR(HangulMultipleCandidate);
        CODE_STR(HangulPreviousCandidate);
        CODE_STR(HangulSpecial);
        CODE_STR(Hangulswitch);

        CODE_STR(DeadGrave);
        CODE_STR(DeadAcute);
        CODE_STR(DeadCircumflex);
        CODE_STR(DeadTilde);
        CODE_STR(DeadMacron);
        CODE_STR(DeadBreve);
        CODE_STR(DeadAbovedot);
        CODE_STR(DeadDiaeresis);
        CODE_STR(DeadAbovering);
        CODE_STR(DeadDoubleacute);
        CODE_STR(DeadCaron);
        CODE_STR(DeadCedilla);
        CODE_STR(DeadOgonek);
        CODE_STR(DeadIota);
        CODE_STR(DeadVoicedSound);
        CODE_STR(DeadSemivoicedSound);
        CODE_STR(DeadBelowdot);
        CODE_STR(DeadHook);
        CODE_STR(DeadHorn);

        CODE_STR(Back);
        CODE_STR(Forward);
        CODE_STR(Stop);
        CODE_STR(Refresh);
        CODE_STR(VolumeDown);
        CODE_STR(VolumeMute);
        CODE_STR(VolumeUp);
        CODE_STR(BassBoost);
        CODE_STR(BassUp);
        CODE_STR(BassDown);
        CODE_STR(TrebleUp);
        CODE_STR(TrebleDown);
        CODE_STR(MediaPlay);
        CODE_STR(MediaStop);
        CODE_STR(MediaPrevious);
        CODE_STR(MediaNext);
        CODE_STR(MediaRecord);
        CODE_STR(MediaPause);
        CODE_STR(MediaTogglePlayPause);
        CODE_STR(HomePage);
        CODE_STR(Favorites);
        CODE_STR(Search);
        CODE_STR(Standby);
        CODE_STR(OpenUrl);

        CODE_STR(LaunchMail);
        CODE_STR(LaunchMedia);
        CODE_STR(Launch0);
        CODE_STR(Launch1);
        CODE_STR(Launch2);
        CODE_STR(Launch3);
        CODE_STR(Launch4);
        CODE_STR(Launch5);
        CODE_STR(Launch6);
        CODE_STR(Launch7);
        CODE_STR(Launch8);
        CODE_STR(Launch9);
        CODE_STR(LaunchA);
        CODE_STR(LaunchB);
        CODE_STR(LaunchC);
        CODE_STR(LaunchD);
        CODE_STR(LaunchE);
        CODE_STR(LaunchF);

        CODE_STR(MonBrightnessUp);
        CODE_STR(MonBrightnessDown);
        CODE_STR(KeyboardLightOnOff);
        CODE_STR(KeyboardBrightnessUp);
        CODE_STR(KeyboardBrightnessDown);
        CODE_STR(PowerOff);
        CODE_STR(WakeUp);
        CODE_STR(Eject);
        CODE_STR(ScreenSaver);
        CODE_STR(WWW);
        CODE_STR(Memo);
        CODE_STR(LightBulb);
        CODE_STR(Shop);
        CODE_STR(History);
        CODE_STR(AddFavorite);
        CODE_STR(HotLinks);
        CODE_STR(BrightnessAdjust);
        CODE_STR(Finance);
        CODE_STR(Community);
        CODE_STR(AudioRewind);
        CODE_STR(BackForward);
        CODE_STR(ApplicationLeft);
        CODE_STR(ApplicationRight);
        CODE_STR(Book);
        CODE_STR(CD);
        CODE_STR(Calculator);
        CODE_STR(ToDoList);
        CODE_STR(ClearGrab);
        CODE_STR(Close);
        CODE_STR(Copy);
        CODE_STR(Cut);
        CODE_STR(Display);
        CODE_STR(DOS);
        CODE_STR(Documents);
        CODE_STR(Excel);
        CODE_STR(Explorer);
        CODE_STR(Game);
        CODE_STR(Go);
        CODE_STR(iTouch);
        CODE_STR(LogOff);
        CODE_STR(Market);
        CODE_STR(Meeting);
        CODE_STR(MenuKB);
        CODE_STR(MenuPB);
        CODE_STR(MySites);
        CODE_STR(News);
        CODE_STR(OfficeHome);
        CODE_STR(Option);
        CODE_STR(Paste);
        CODE_STR(Phone);
        CODE_STR(Calendar);
        CODE_STR(Reply);
        CODE_STR(Reload);
        CODE_STR(RotateWindows);
        CODE_STR(RotationPB);
        CODE_STR(RotationKB);
        CODE_STR(Save);
        CODE_STR(Send);
        CODE_STR(Spell);
        CODE_STR(SplitScreen);
        CODE_STR(Support);
        CODE_STR(TaskPane);
        CODE_STR(Terminal);
        CODE_STR(Tools);
        CODE_STR(Travel);
        CODE_STR(Video);
        CODE_STR(Word);
        CODE_STR(Xfer);
        CODE_STR(ZoomIn);
        CODE_STR(ZoomOut);
        CODE_STR(Away);
        CODE_STR(Messenger);
        CODE_STR(WebCam);
        CODE_STR(MailForward);
        CODE_STR(Pictures);
        CODE_STR(Music);
        CODE_STR(Battery);
        CODE_STR(Bluetooth);
        CODE_STR(WLAN);
        CODE_STR(UWB);
        CODE_STR(AudioForward);
        CODE_STR(AudioRepeat);
        CODE_STR(AudioRandomPlay);
        CODE_STR(Subtitle);
        CODE_STR(AudioCycleTrack);
        CODE_STR(Time);
        CODE_STR(Hibernate);
        CODE_STR(View);
        CODE_STR(TopMenu);
        CODE_STR(PowerDown);
        CODE_STR(Suspend);
        CODE_STR(ContrastAdjust);

        CODE_STR(LaunchG);
        CODE_STR(LaunchH);

        CODE_STR(TouchpadToggle);
        CODE_STR(TouchpadOn);
        CODE_STR(TouchpadOff);

        CODE_STR(MicMute);

        CODE_STR(Red);
        CODE_STR(Green);
        CODE_STR(Yellow);
        CODE_STR(Blue);

        CODE_STR(ChannelUp);
        CODE_STR(ChannelDown);

        CODE_STR(MediaLast);

        CODE_STR(Select);
        CODE_STR(Yes);
        CODE_STR(No);

        CODE_STR(Cancel);
        CODE_STR(Printer);
        CODE_STR(Execute);
        CODE_STR(Sleep);
        CODE_STR(Play);
        CODE_STR(Zoom);
        CODE_STR(Jisho);
        CODE_STR(Oyayubi_Left);
        CODE_STR(Oyayubi_Right);

        CODE_STR(Context1);
        CODE_STR(Context2);
        CODE_STR(Context3);
        CODE_STR(Context4);
        CODE_STR(Call);
        CODE_STR(Hangup);
        CODE_STR(Flip);
        CODE_STR(ToggleCallHangup);
        CODE_STR(VoiceDial);
        CODE_STR(LastNumberRedial);

        CODE_STR(Convert);
        CODE_STR(NonConvert);
        CODE_STR(Accept);
        CODE_STR(ModeChange);
        CODE_STR(PrintScreen);
        CODE_STR(Apps);
        CODE_STR(Separator);
        CODE_STR(Attn);
        CODE_STR(CrSel);
        CODE_STR(ExSel);
        CODE_STR(EraseEOF);
        CODE_STR(NoName);
        CODE_STR(Pa1);
        CODE_STR(Quotation);
        CODE_STR(Backtick);

        CODE_STR(Camera);
        CODE_STR(CameraFocus);

        return "NotDefine";
    }

    Keyboard::Code Keyboard::GetCodeFromName(const char* codeName) {
        #define STR_CODE(code_id) if (strcmp(codeName, #code_id) == 0) return Keyboard::code_id

        STR_CODE(Escape);
        STR_CODE(Tab);
        STR_CODE(Backtab);
        STR_CODE(Backspace);
        STR_CODE(Return);
        STR_CODE(Enter);
        STR_CODE(Insert);
        STR_CODE(Delete);
        STR_CODE(Pause);
        STR_CODE(Print);
        STR_CODE(SysReq);
        STR_CODE(Clear);
        STR_CODE(Home);
        STR_CODE(End);
        STR_CODE(Left);
        STR_CODE(Up);
        STR_CODE(Right);
        STR_CODE(Down);
        STR_CODE(PageUp);
        STR_CODE(PageDown);
        STR_CODE(ShiftLock);
        STR_CODE(Shift);
        STR_CODE(ShiftLeft);
        STR_CODE(ShiftRight);
        STR_CODE(Control);
        STR_CODE(ControlLeft);
        STR_CODE(ControlRight);
        STR_CODE(Meta);
        STR_CODE(MetaLeft);
        STR_CODE(MetaRight);
        STR_CODE(Alt);
        STR_CODE(AltLeft);
        STR_CODE(AltRight);
        STR_CODE(CapsLock);
        STR_CODE(NumLock);
        STR_CODE(ScrollLock);

        STR_CODE(F1);
        STR_CODE(F2);
        STR_CODE(F3);
        STR_CODE(F4);
        STR_CODE(F5);
        STR_CODE(F6);
        STR_CODE(F7);
        STR_CODE(F8);
        STR_CODE(F9);
        STR_CODE(F10);
        STR_CODE(F11);
        STR_CODE(F12);
        STR_CODE(F13);
        STR_CODE(F14);
        STR_CODE(F15);
        STR_CODE(F16);
        STR_CODE(F17);
        STR_CODE(F18);
        STR_CODE(F19);
        STR_CODE(F20);
        STR_CODE(F21);
        STR_CODE(F22);
        STR_CODE(F23);
        STR_CODE(F24);
        STR_CODE(F25);
        STR_CODE(F26);
        STR_CODE(F27);
        STR_CODE(F28);
        STR_CODE(F29);
        STR_CODE(F30);
        STR_CODE(F31);
        STR_CODE(F32);
        STR_CODE(F33);
        STR_CODE(F34);
        STR_CODE(F35);

        STR_CODE(SuperLeft);
        STR_CODE(SuperRight);
        STR_CODE(Undo);
        STR_CODE(Redo);
        STR_CODE(Menu);
        STR_CODE(Find);
        STR_CODE(HyperLeft);
        STR_CODE(HyperRight);
        STR_CODE(Help);
        STR_CODE(DirectionLeft);
        STR_CODE(DirectionRight);
        STR_CODE(Space);
        STR_CODE(Any);
        STR_CODE(Exclam);
        STR_CODE(QuoteDbl);
        STR_CODE(NumberSign);
        STR_CODE(Dollar);
        STR_CODE(Percent);
        STR_CODE(Ampersand);
        STR_CODE(Apostrophe);
        STR_CODE(ParenLeft);
        STR_CODE(ParenRight);
        STR_CODE(Asterisk);
        STR_CODE(Plus);
        STR_CODE(Comma);
        STR_CODE(Minus);
        STR_CODE(Period);
        STR_CODE(Slash);

        STR_CODE(Numpad0);
        STR_CODE(Numpad1);
        STR_CODE(Numpad2);
        STR_CODE(Numpad3);
        STR_CODE(Numpad4);
        STR_CODE(Numpad5);
        STR_CODE(Numpad6);
        STR_CODE(Numpad7);
        STR_CODE(Numpad8);
        STR_CODE(Numpad9);

        STR_CODE(Keypad0);
        STR_CODE(Keypad1);
        STR_CODE(Keypad2);
        STR_CODE(Keypad3);
        STR_CODE(Keypad4);
        STR_CODE(Keypad5);
        STR_CODE(Keypad6);
        STR_CODE(Keypad7);
        STR_CODE(Keypad8);
        STR_CODE(Keypad9);

        STR_CODE(Colon);
        STR_CODE(Semicolon);
        STR_CODE(Less);
        STR_CODE(Equal);
        STR_CODE(Greater);
        STR_CODE(Question);
        STR_CODE(At);

        STR_CODE(A);
        STR_CODE(B);
        STR_CODE(C);
        STR_CODE(D);
        STR_CODE(E);
        STR_CODE(F);
        STR_CODE(G);
        STR_CODE(H);
        STR_CODE(I);
        STR_CODE(J);
        STR_CODE(K);
        STR_CODE(L);
        STR_CODE(M);
        STR_CODE(N);
        STR_CODE(O);
        STR_CODE(P);
        STR_CODE(Q);
        STR_CODE(R);
        STR_CODE(S);
        STR_CODE(T);
        STR_CODE(U);
        STR_CODE(V);
        STR_CODE(W);
        STR_CODE(X);
        STR_CODE(Y);
        STR_CODE(Z);

        STR_CODE(Break);
        STR_CODE(BracketLeft);
        STR_CODE(Backslash);
        STR_CODE(BracketRight);
        STR_CODE(AsciiCircum);
        STR_CODE(Underscore);
        STR_CODE(QuoteLeft);
        STR_CODE(BraceLeft);
        STR_CODE(Bar);
        STR_CODE(BraceRight);
        STR_CODE(AsciiTilde);

        STR_CODE(Nobreakspace);
        STR_CODE(Exclamdown);
        STR_CODE(Cent);
        STR_CODE(Sterling);
        STR_CODE(Currency);
        STR_CODE(Yen);
        STR_CODE(Brokenbar);
        STR_CODE(Section);
        STR_CODE(Diaeresis);
        STR_CODE(Copyright);
        STR_CODE(Ordfeminine);
        STR_CODE(Guillemotleft);
        STR_CODE(Notsign);
        STR_CODE(Hyphen);
        STR_CODE(Registered);
        STR_CODE(Macron);
        STR_CODE(Degree);
        STR_CODE(Plusminus);
        STR_CODE(Twosuperior);
        STR_CODE(Threesuperior);
        STR_CODE(Acute);
        STR_CODE(Mu);
        STR_CODE(Paragraph);
        STR_CODE(Periodcentered);
        STR_CODE(Cedilla);
        STR_CODE(Onesuperior);
        STR_CODE(Masculine);
        STR_CODE(Guillemotright);
        STR_CODE(Onequarter);
        STR_CODE(Onehalf);
        STR_CODE(Threequarters);
        STR_CODE(Questiondown);
        STR_CODE(Agrave);
        STR_CODE(Aacute);
        STR_CODE(Acircumflex);
        STR_CODE(Atilde);
        STR_CODE(Adiaeresis);
        STR_CODE(Aring);
        STR_CODE(AE);
        STR_CODE(Ccedilla);
        STR_CODE(Egrave);
        STR_CODE(Eacute);
        STR_CODE(Ecircumflex);
        STR_CODE(Ediaeresis);
        STR_CODE(Igrave);
        STR_CODE(Iacute);
        STR_CODE(Icircumflex);
        STR_CODE(Idiaeresis);
        STR_CODE(ETH);
        STR_CODE(Eth);
        STR_CODE(Ntilde);
        STR_CODE(Ograve);
        STR_CODE(Oacute);
        STR_CODE(Ocircumflex);
        STR_CODE(Otilde);
        STR_CODE(Odiaeresis);
        STR_CODE(Multiply);
        STR_CODE(Ooblique);
        STR_CODE(Ugrave);
        STR_CODE(Uacute);
        STR_CODE(Ucircumflex);
        STR_CODE(Udiaeresis);
        STR_CODE(Yacute);
        STR_CODE(THORN);
        STR_CODE(Ssharp);
        STR_CODE(Division);
        STR_CODE(Ydiaeresis);

        STR_CODE(AltGr);
        STR_CODE(MultiKey);
        STR_CODE(Codeinput);
        STR_CODE(SingleCandidate);
        STR_CODE(MultipleCandidate);
        STR_CODE(PreviousCandidate);

        STR_CODE(ModeSwitch);
        STR_CODE(ScriptSwitch);

        STR_CODE(Kanji);
        STR_CODE(Muhenkan);
        STR_CODE(HenkanMode);
        STR_CODE(Henkan);
        STR_CODE(Romaji);
        STR_CODE(Hiragana);
        STR_CODE(Katakana);
        STR_CODE(HiraganaKatakana);
        STR_CODE(Zenkaku);
        STR_CODE(Hankaku);
        STR_CODE(ZenkakuHankaku);
        STR_CODE(Touroku);
        STR_CODE(Massyo);
        STR_CODE(KanaLock);
        STR_CODE(KanaShift);
        STR_CODE(EisuShift);
        STR_CODE(EisuToggle);
        STR_CODE(KanjiBangou);
        STR_CODE(ZenKoho);
        STR_CODE(MaeKoho);

        STR_CODE(Hangul);
        STR_CODE(HangulStart);
        STR_CODE(HangulEnd);
        STR_CODE(HangulHanja);
        STR_CODE(HangulJamo);
        STR_CODE(HangulRomaja);
        STR_CODE(HangulCodeinput);
        STR_CODE(HangulJeonja);
        STR_CODE(HangulBanja);
        STR_CODE(HangulPreHanja);
        STR_CODE(HangulPostHanja);
        STR_CODE(HangulSingleCandidate);
        STR_CODE(HangulMultipleCandidate);
        STR_CODE(HangulPreviousCandidate);
        STR_CODE(HangulSpecial);
        STR_CODE(Hangulswitch);

        STR_CODE(DeadGrave);
        STR_CODE(DeadAcute);
        STR_CODE(DeadCircumflex);
        STR_CODE(DeadTilde);
        STR_CODE(DeadMacron);
        STR_CODE(DeadBreve);
        STR_CODE(DeadAbovedot);
        STR_CODE(DeadDiaeresis);
        STR_CODE(DeadAbovering);
        STR_CODE(DeadDoubleacute);
        STR_CODE(DeadCaron);
        STR_CODE(DeadCedilla);
        STR_CODE(DeadOgonek);
        STR_CODE(DeadIota);
        STR_CODE(DeadVoicedSound);
        STR_CODE(DeadSemivoicedSound);
        STR_CODE(DeadBelowdot);
        STR_CODE(DeadHook);
        STR_CODE(DeadHorn);

        STR_CODE(Back);
        STR_CODE(Forward);
        STR_CODE(Stop);
        STR_CODE(Refresh);
        STR_CODE(VolumeDown);
        STR_CODE(VolumeMute);
        STR_CODE(VolumeUp);
        STR_CODE(BassBoost);
        STR_CODE(BassUp);
        STR_CODE(BassDown);
        STR_CODE(TrebleUp);
        STR_CODE(TrebleDown);
        STR_CODE(MediaPlay);
        STR_CODE(MediaStop);
        STR_CODE(MediaPrevious);
        STR_CODE(MediaNext);
        STR_CODE(MediaRecord);
        STR_CODE(MediaPause);
        STR_CODE(MediaTogglePlayPause);
        STR_CODE(HomePage);
        STR_CODE(Favorites);
        STR_CODE(Search);
        STR_CODE(Standby);
        STR_CODE(OpenUrl);
        STR_CODE(LaunchMail);
        STR_CODE(LaunchMedia);
        STR_CODE(Launch0);
        STR_CODE(Launch1);
        STR_CODE(Launch2);
        STR_CODE(Launch3);
        STR_CODE(Launch4);
        STR_CODE(Launch5);
        STR_CODE(Launch6);
        STR_CODE(Launch7);
        STR_CODE(Launch8);
        STR_CODE(Launch9);
        STR_CODE(LaunchA);
        STR_CODE(LaunchB);
        STR_CODE(LaunchC);
        STR_CODE(LaunchD);
        STR_CODE(LaunchE);
        STR_CODE(LaunchF);
        STR_CODE(MonBrightnessUp);
        STR_CODE(MonBrightnessDown);
        STR_CODE(KeyboardLightOnOff);
        STR_CODE(KeyboardBrightnessUp);
        STR_CODE(KeyboardBrightnessDown);
        STR_CODE(PowerOff);
        STR_CODE(WakeUp);
        STR_CODE(Eject);
        STR_CODE(ScreenSaver);
        STR_CODE(WWW);
        STR_CODE(Memo);
        STR_CODE(LightBulb);
        STR_CODE(Shop);
        STR_CODE(History);
        STR_CODE(AddFavorite);
        STR_CODE(HotLinks);
        STR_CODE(BrightnessAdjust);
        STR_CODE(Finance);
        STR_CODE(Community);
        STR_CODE(AudioRewind);
        STR_CODE(BackForward);
        STR_CODE(ApplicationLeft);
        STR_CODE(ApplicationRight);
        STR_CODE(Book);
        STR_CODE(CD);
        STR_CODE(Calculator);
        STR_CODE(ToDoList);
        STR_CODE(ClearGrab);
        STR_CODE(Close);
        STR_CODE(Copy);
        STR_CODE(Cut);
        STR_CODE(Display);
        STR_CODE(DOS);
        STR_CODE(Documents);
        STR_CODE(Excel);
        STR_CODE(Explorer);
        STR_CODE(Game);
        STR_CODE(Go);
        STR_CODE(iTouch);
        STR_CODE(LogOff);
        STR_CODE(Market);
        STR_CODE(Meeting);
        STR_CODE(MenuKB);
        STR_CODE(MenuPB);
        STR_CODE(MySites);
        STR_CODE(News);
        STR_CODE(OfficeHome);
        STR_CODE(Option);
        STR_CODE(Paste);
        STR_CODE(Phone);
        STR_CODE(Calendar);
        STR_CODE(Reply);
        STR_CODE(Reload);
        STR_CODE(RotateWindows);
        STR_CODE(RotationPB);
        STR_CODE(RotationKB);
        STR_CODE(Save);
        STR_CODE(Send);
        STR_CODE(Spell);
        STR_CODE(SplitScreen);
        STR_CODE(Support);
        STR_CODE(TaskPane);
        STR_CODE(Terminal);
        STR_CODE(Tools);
        STR_CODE(Travel);
        STR_CODE(Video);
        STR_CODE(Word);
        STR_CODE(Xfer);
        STR_CODE(ZoomIn);
        STR_CODE(ZoomOut);
        STR_CODE(Away);
        STR_CODE(Messenger);
        STR_CODE(WebCam);
        STR_CODE(MailForward);
        STR_CODE(Pictures);
        STR_CODE(Music);
        STR_CODE(Battery);
        STR_CODE(Bluetooth);
        STR_CODE(WLAN);
        STR_CODE(UWB);
        STR_CODE(AudioForward);
        STR_CODE(AudioRepeat);
        STR_CODE(AudioRandomPlay);
        STR_CODE(Subtitle);
        STR_CODE(AudioCycleTrack);
        STR_CODE(Time);
        STR_CODE(Hibernate);
        STR_CODE(View);
        STR_CODE(TopMenu);
        STR_CODE(PowerDown);
        STR_CODE(Suspend);
        STR_CODE(ContrastAdjust);

        STR_CODE(LaunchG);
        STR_CODE(LaunchH);

        STR_CODE(TouchpadToggle);
        STR_CODE(TouchpadOn);
        STR_CODE(TouchpadOff);

        STR_CODE(MicMute);

        STR_CODE(Red);
        STR_CODE(Green);
        STR_CODE(Yellow);
        STR_CODE(Blue);

        STR_CODE(ChannelUp);
        STR_CODE(ChannelDown);

        STR_CODE(MediaLast);

        STR_CODE(Select);
        STR_CODE(Yes);
        STR_CODE(No);

        STR_CODE(Cancel);
        STR_CODE(Printer);
        STR_CODE(Execute);
        STR_CODE(Sleep);
        STR_CODE(Play);
        STR_CODE(Zoom);
        STR_CODE(Jisho);
        STR_CODE(Oyayubi_Left);
        STR_CODE(Oyayubi_Right);

        STR_CODE(Context1);
        STR_CODE(Context2);
        STR_CODE(Context3);
        STR_CODE(Context4);
        STR_CODE(Call);
        STR_CODE(Hangup);
        STR_CODE(Flip);
        STR_CODE(ToggleCallHangup);
        STR_CODE(VoiceDial);
        STR_CODE(LastNumberRedial);

        STR_CODE(Convert);
        STR_CODE(NonConvert);
        STR_CODE(Accept);
        STR_CODE(ModeChange);
        STR_CODE(PrintScreen);
        STR_CODE(Apps);
        STR_CODE(Separator);
        STR_CODE(Attn);
        STR_CODE(CrSel);
        STR_CODE(ExSel);
        STR_CODE(EraseEOF);
        STR_CODE(NoName);
        STR_CODE(Pa1);
        STR_CODE(Quotation);
        STR_CODE(Backtick);

        STR_CODE(Camera);
        STR_CODE(CameraFocus);

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
            if (key == Keyboard::Control || key == Keyboard::ControlLeft || key == Keyboard::ControlRight ||
                key == Keyboard::Alt || key == Keyboard::AltLeft || key == Keyboard::AltRight ||
                key == Keyboard::Shift || key == Keyboard::ShiftLeft || key == Keyboard::ShiftRight ||
                key == Keyboard::MetaLeft || key == Keyboard::MetaRight ||
                key == Keyboard::NumLock || key == Keyboard::CapsLock || key == Keyboard::ModeSwitch || key == Keyboard::ScrollLock) {
                return true;
            }
        }
        return false;
    }

}    // namespace nkentseu