//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/17/2024 at 9:38:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventCode.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include <Windows.h>

namespace nkentseu {
    #define CODE_CONVERT_KC_WK(keycode, vkey)   if (keycode_to_vkey) {\
                                                    if (code == keycode) return vkey;\
                                                } else {\
                                                    if (vkey == code) return keycode;\
                                                }

    uint64 WindowEventCode::ConvertKeycodeWinkey(uint64 code, bool keycode_to_vkey) {
        CODE_CONVERT_KC_WK(Keyboard::Escape, VK_ESCAPE);
        CODE_CONVERT_KC_WK(Keyboard::Tab, VK_TAB);
        CODE_CONVERT_KC_WK(Keyboard::Backspace, VK_BACK);
        // Enter key can be mapped to either VK_RETURN or VK_ENTER depending on the shift key state
        // This implementation omits handling the shift key and simply maps Enter to VK_RETURN
        CODE_CONVERT_KC_WK(Keyboard::Return, VK_RETURN);
        CODE_CONVERT_KC_WK(Keyboard::Insert, VK_INSERT);
        CODE_CONVERT_KC_WK(Keyboard::Delete, VK_DELETE);
        CODE_CONVERT_KC_WK(Keyboard::Pause, VK_PAUSE);
        CODE_CONVERT_KC_WK(Keyboard::Print, VK_PRINT);
        CODE_CONVERT_KC_WK(Keyboard::Clear, VK_CLEAR);
        // SysReq, Clear are not directly mapped to virtual key codes
        // ... (add mappings for other unsupported keys if needed)
        CODE_CONVERT_KC_WK(Keyboard::Home, VK_HOME);
        CODE_CONVERT_KC_WK(Keyboard::End, VK_END);
        CODE_CONVERT_KC_WK(Keyboard::Left, VK_LEFT);
        CODE_CONVERT_KC_WK(Keyboard::Up, VK_UP);
        CODE_CONVERT_KC_WK(Keyboard::Right, VK_RIGHT);
        CODE_CONVERT_KC_WK(Keyboard::Down, VK_DOWN);
        CODE_CONVERT_KC_WK(Keyboard::PageUp, VK_PRIOR);
        CODE_CONVERT_KC_WK(Keyboard::PageDown, VK_NEXT);
        CODE_CONVERT_KC_WK(Keyboard::Shift, VK_SHIFT);
        CODE_CONVERT_KC_WK(Keyboard::Control, VK_CONTROL);
        CODE_CONVERT_KC_WK(Keyboard::Alt, VK_MENU);
        CODE_CONVERT_KC_WK(Keyboard::CapsLock, VK_CAPITAL);
        CODE_CONVERT_KC_WK(Keyboard::NumLock, VK_NUMLOCK);
        CODE_CONVERT_KC_WK(Keyboard::ScrollLock, VK_SCROLL);

        // Left/Right versions of modifiers can be mapped similarly
        CODE_CONVERT_KC_WK(Keyboard::ShiftLeft, VK_LSHIFT);
        CODE_CONVERT_KC_WK(Keyboard::ControlLeft, VK_LCONTROL);
        CODE_CONVERT_KC_WK(Keyboard::AltLeft, VK_LMENU);

        CODE_CONVERT_KC_WK(Keyboard::F1, VK_F1);
        CODE_CONVERT_KC_WK(Keyboard::F2, VK_F2);
        CODE_CONVERT_KC_WK(Keyboard::F3, VK_F3);
        CODE_CONVERT_KC_WK(Keyboard::F4, VK_F4);
        CODE_CONVERT_KC_WK(Keyboard::F5, VK_F5);
        CODE_CONVERT_KC_WK(Keyboard::F6, VK_F6);
        CODE_CONVERT_KC_WK(Keyboard::F7, VK_F7);
        CODE_CONVERT_KC_WK(Keyboard::F8, VK_F8);
        CODE_CONVERT_KC_WK(Keyboard::F9, VK_F9);
        CODE_CONVERT_KC_WK(Keyboard::F10, VK_F10);
        CODE_CONVERT_KC_WK(Keyboard::F11, VK_F11);
        CODE_CONVERT_KC_WK(Keyboard::F12, VK_F12);
        CODE_CONVERT_KC_WK(Keyboard::F13, VK_F13);
        CODE_CONVERT_KC_WK(Keyboard::F14, VK_F14);
        CODE_CONVERT_KC_WK(Keyboard::F15, VK_F15);
        CODE_CONVERT_KC_WK(Keyboard::F16, VK_F16);
        CODE_CONVERT_KC_WK(Keyboard::F17, VK_F17);
        CODE_CONVERT_KC_WK(Keyboard::F18, VK_F18);
        CODE_CONVERT_KC_WK(Keyboard::F19, VK_F19);
        CODE_CONVERT_KC_WK(Keyboard::F20, VK_F20);
        CODE_CONVERT_KC_WK(Keyboard::F21, VK_F21);
        CODE_CONVERT_KC_WK(Keyboard::F22, VK_F22);
        CODE_CONVERT_KC_WK(Keyboard::F23, VK_F23);
        CODE_CONVERT_KC_WK(Keyboard::F24, VK_F24);

        // Meta key (Windows key) is not directly mapped in this implementation
        CODE_CONVERT_KC_WK(Keyboard::MetaLeft, VK_LWIN);  // Replace with appropriate virtual key code for Meta
        CODE_CONVERT_KC_WK(Keyboard::MetaRight, VK_RWIN);
        //CODE_CONVERT_KC_WK(Keyboard::Undo, VK_UNDO);
        //CODE_CONVERT_KC_WK(Keyboard::Redo, VK_REDO);
        CODE_CONVERT_KC_WK(Keyboard::Menu, VK_MENU);
        //CODE_CONVERT_KC_WK(Keyboard::Find, VK_FIND);
        //CODE_CONVERT_KC_WK(Keyboard::HyperLeft, VK_LMETA);
        //CODE_CONVERT_KC_WK(Keyboard::HyperRight, VK_RMETA);
        CODE_CONVERT_KC_WK(Keyboard::Help, VK_HELP);
        CODE_CONVERT_KC_WK(Keyboard::DirectionLeft, VK_LEFT);
        CODE_CONVERT_KC_WK(Keyboard::DirectionRight, VK_RIGHT);
        CODE_CONVERT_KC_WK(Keyboard::Space, VK_SPACE);
        CODE_CONVERT_KC_WK(Keyboard::Any, VK_SPACE);
        CODE_CONVERT_KC_WK(Keyboard::Exclam, VK_OEM_1);
        CODE_CONVERT_KC_WK(Keyboard::QuoteDbl, VK_OEM_7);
        CODE_CONVERT_KC_WK(Keyboard::NumberSign, VK_OEM_3);
        CODE_CONVERT_KC_WK(Keyboard::Dollar, 0x24);
        CODE_CONVERT_KC_WK(Keyboard::Percent, VK_OEM_5);
        CODE_CONVERT_KC_WK(Keyboard::Ampersand, VK_OEM_7);
        CODE_CONVERT_KC_WK(Keyboard::Apostrophe, VK_OEM_7);
        CODE_CONVERT_KC_WK(Keyboard::ParenLeft, VK_OEM_4);
        CODE_CONVERT_KC_WK(Keyboard::ParenRight, VK_OEM_6);
        CODE_CONVERT_KC_WK(Keyboard::Asterisk, VK_MULTIPLY);
        CODE_CONVERT_KC_WK(Keyboard::Plus, VK_OEM_PLUS);
        CODE_CONVERT_KC_WK(Keyboard::Comma, VK_OEM_COMMA);
        CODE_CONVERT_KC_WK(Keyboard::Minus, VK_OEM_MINUS);
        CODE_CONVERT_KC_WK(Keyboard::Period, VK_OEM_PERIOD);
        CODE_CONVERT_KC_WK(Keyboard::Slash, VK_OEM_2);

        CODE_CONVERT_KC_WK(Keyboard::Numpad0, VK_NUMPAD0);
        CODE_CONVERT_KC_WK(Keyboard::Numpad1, VK_NUMPAD1);
        CODE_CONVERT_KC_WK(Keyboard::Numpad2, VK_NUMPAD2);
        CODE_CONVERT_KC_WK(Keyboard::Numpad3, VK_NUMPAD3);
        CODE_CONVERT_KC_WK(Keyboard::Numpad4, VK_NUMPAD4);
        CODE_CONVERT_KC_WK(Keyboard::Numpad5, VK_NUMPAD5);
        CODE_CONVERT_KC_WK(Keyboard::Numpad6, VK_NUMPAD6);
        CODE_CONVERT_KC_WK(Keyboard::Numpad7, VK_NUMPAD7);
        CODE_CONVERT_KC_WK(Keyboard::Numpad8, VK_NUMPAD8);
        CODE_CONVERT_KC_WK(Keyboard::Numpad9, VK_NUMPAD9);

        CODE_CONVERT_KC_WK(Keyboard::Keypad0, 0x30);
        CODE_CONVERT_KC_WK(Keyboard::Keypad1, 0x31);
        CODE_CONVERT_KC_WK(Keyboard::Keypad2, 0x32);
        CODE_CONVERT_KC_WK(Keyboard::Keypad3, 0x33);
        CODE_CONVERT_KC_WK(Keyboard::Keypad4, 0x34);
        CODE_CONVERT_KC_WK(Keyboard::Keypad5, 0x35);
        CODE_CONVERT_KC_WK(Keyboard::Keypad6, 0x36);
        CODE_CONVERT_KC_WK(Keyboard::Keypad7, 0x37);
        CODE_CONVERT_KC_WK(Keyboard::Keypad8, 0x38);
        CODE_CONVERT_KC_WK(Keyboard::Keypad9, 0x39);

        CODE_CONVERT_KC_WK(Keyboard::Colon, VK_OEM_1);
        CODE_CONVERT_KC_WK(Keyboard::Semicolon, VK_OEM_1);
        CODE_CONVERT_KC_WK(Keyboard::Less, VK_OEM_COMMA);
        CODE_CONVERT_KC_WK(Keyboard::Equal, VK_OEM_PLUS);
        CODE_CONVERT_KC_WK(Keyboard::Greater, VK_OEM_PERIOD);
        CODE_CONVERT_KC_WK(Keyboard::Question, VK_OEM_2);
        CODE_CONVERT_KC_WK(Keyboard::At, VK_OEM_3);

        CODE_CONVERT_KC_WK(Keyboard::A, 0x41);
        CODE_CONVERT_KC_WK(Keyboard::B, 0x42);
        CODE_CONVERT_KC_WK(Keyboard::C, 0x43);
        CODE_CONVERT_KC_WK(Keyboard::D, 0x44);
        CODE_CONVERT_KC_WK(Keyboard::E, 0x45);
        CODE_CONVERT_KC_WK(Keyboard::F, 0x46);
        CODE_CONVERT_KC_WK(Keyboard::G, 0x47);
        CODE_CONVERT_KC_WK(Keyboard::H, 0x48);
        CODE_CONVERT_KC_WK(Keyboard::I, 0x49);
        CODE_CONVERT_KC_WK(Keyboard::J, 0x4A);
        CODE_CONVERT_KC_WK(Keyboard::K, 0x4B);
        CODE_CONVERT_KC_WK(Keyboard::L, 0x4C);
        CODE_CONVERT_KC_WK(Keyboard::M, 0x4D);
        CODE_CONVERT_KC_WK(Keyboard::N, 0x4E);
        CODE_CONVERT_KC_WK(Keyboard::O, 0x4F);
        CODE_CONVERT_KC_WK(Keyboard::P, 0x50);
        CODE_CONVERT_KC_WK(Keyboard::Q, 0x51);
        CODE_CONVERT_KC_WK(Keyboard::R, 0x52);
        CODE_CONVERT_KC_WK(Keyboard::S, 0x53);
        CODE_CONVERT_KC_WK(Keyboard::T, 0x54);
        CODE_CONVERT_KC_WK(Keyboard::U, 0x55);
        CODE_CONVERT_KC_WK(Keyboard::V, 0x56);
        CODE_CONVERT_KC_WK(Keyboard::W, 0x57);
        CODE_CONVERT_KC_WK(Keyboard::X, 0x58);
        CODE_CONVERT_KC_WK(Keyboard::Y, 0x59);
        CODE_CONVERT_KC_WK(Keyboard::Z, 0x5A);

        CODE_CONVERT_KC_WK(Keyboard::Break, VK_CANCEL);
        CODE_CONVERT_KC_WK(Keyboard::BracketLeft, VK_OEM_4);
        CODE_CONVERT_KC_WK(Keyboard::Backslash, VK_OEM_5);
        CODE_CONVERT_KC_WK(Keyboard::BracketRight, VK_OEM_6);
        CODE_CONVERT_KC_WK(Keyboard::AsciiCircum, VK_OEM_7);
        CODE_CONVERT_KC_WK(Keyboard::Underscore, VK_OEM_MINUS);
        CODE_CONVERT_KC_WK(Keyboard::QuoteLeft, VK_OEM_3);
        CODE_CONVERT_KC_WK(Keyboard::BraceLeft, VK_OEM_4);
        CODE_CONVERT_KC_WK(Keyboard::Bar, VK_OEM_5);
        CODE_CONVERT_KC_WK(Keyboard::BraceRight, VK_OEM_6);
        CODE_CONVERT_KC_WK(Keyboard::AsciiTilde, VK_OEM_3);

        CODE_CONVERT_KC_WK(Keyboard::Nobreakspace, 0x0A0);
        CODE_CONVERT_KC_WK(Keyboard::Exclamdown, 0x00A1);
        CODE_CONVERT_KC_WK(Keyboard::Cent, 0x00A2);
        CODE_CONVERT_KC_WK(Keyboard::Sterling, 0x00A3);
        CODE_CONVERT_KC_WK(Keyboard::Currency, 0x00A4);
        CODE_CONVERT_KC_WK(Keyboard::Yen, 0x00A5);
        CODE_CONVERT_KC_WK(Keyboard::Brokenbar, 0x00A6);
        CODE_CONVERT_KC_WK(Keyboard::Section, 0x00A7);
        CODE_CONVERT_KC_WK(Keyboard::Diaeresis, 0x00A8);
        CODE_CONVERT_KC_WK(Keyboard::Copyright, 0x00A9);
        CODE_CONVERT_KC_WK(Keyboard::Ordfeminine, 0x00AA);
        CODE_CONVERT_KC_WK(Keyboard::Guillemotleft, 0x00AB);
        CODE_CONVERT_KC_WK(Keyboard::Notsign, 0x00AC);
        CODE_CONVERT_KC_WK(Keyboard::Hyphen, 0x00AD);
        CODE_CONVERT_KC_WK(Keyboard::Registered, 0x00AE);
        CODE_CONVERT_KC_WK(Keyboard::Macron, 0x00AF);
        CODE_CONVERT_KC_WK(Keyboard::Degree, 0x00B0);
        CODE_CONVERT_KC_WK(Keyboard::Plusminus, 0x00B1);
        CODE_CONVERT_KC_WK(Keyboard::Twosuperior, 0x00B2);
        CODE_CONVERT_KC_WK(Keyboard::Threesuperior, 0x00B3);
        CODE_CONVERT_KC_WK(Keyboard::Acute, 0x00B4);
        CODE_CONVERT_KC_WK(Keyboard::Mu, 0x00B5);
        CODE_CONVERT_KC_WK(Keyboard::Paragraph, 0x00B6);
        CODE_CONVERT_KC_WK(Keyboard::Periodcentered, 0x00B7);
        CODE_CONVERT_KC_WK(Keyboard::Cedilla, 0x00B8);
        CODE_CONVERT_KC_WK(Keyboard::Onesuperior, 0x00B9);
        CODE_CONVERT_KC_WK(Keyboard::Masculine, 0x00BA);
        CODE_CONVERT_KC_WK(Keyboard::Guillemotright, 0x00BB);
        CODE_CONVERT_KC_WK(Keyboard::Onequarter, 0x00BC);
        CODE_CONVERT_KC_WK(Keyboard::Onehalf, 0x00BD);
        CODE_CONVERT_KC_WK(Keyboard::Threequarters, 0x00BE);
        CODE_CONVERT_KC_WK(Keyboard::Questiondown, 0x00BF);
        CODE_CONVERT_KC_WK(Keyboard::Agrave, 0x00C0);
        CODE_CONVERT_KC_WK(Keyboard::Aacute, 0x00C1);
        CODE_CONVERT_KC_WK(Keyboard::Acircumflex, 0x00C2);
        CODE_CONVERT_KC_WK(Keyboard::Atilde, 0x00C3);
        CODE_CONVERT_KC_WK(Keyboard::Adiaeresis, 0x00C4);
        CODE_CONVERT_KC_WK(Keyboard::Aring, 0x00C5);
        CODE_CONVERT_KC_WK(Keyboard::AE, 0x00C6);
        CODE_CONVERT_KC_WK(Keyboard::Ccedilla, 0x00C7);
        CODE_CONVERT_KC_WK(Keyboard::Egrave, 0x00C8);
        CODE_CONVERT_KC_WK(Keyboard::Eacute, 0x00C9);
        CODE_CONVERT_KC_WK(Keyboard::Ecircumflex, 0x00CA);
        CODE_CONVERT_KC_WK(Keyboard::Ediaeresis, 0x00CB);
        CODE_CONVERT_KC_WK(Keyboard::Igrave, 0x00CC);
        CODE_CONVERT_KC_WK(Keyboard::Iacute, 0x00CD);
        CODE_CONVERT_KC_WK(Keyboard::Icircumflex, 0x00CE);
        CODE_CONVERT_KC_WK(Keyboard::Idiaeresis, 0x00CF);
        CODE_CONVERT_KC_WK(Keyboard::ETH, 0x00D0);
        CODE_CONVERT_KC_WK(Keyboard::Eth, 0x00D0);
        CODE_CONVERT_KC_WK(Keyboard::Ntilde, 0x00D1);
        CODE_CONVERT_KC_WK(Keyboard::Ograve, 0x00D2);
        CODE_CONVERT_KC_WK(Keyboard::Oacute, 0x00D3);
        CODE_CONVERT_KC_WK(Keyboard::Ocircumflex, 0x00D4);
        CODE_CONVERT_KC_WK(Keyboard::Otilde, 0x00D5);
        CODE_CONVERT_KC_WK(Keyboard::Odiaeresis, 0x00D6);
        CODE_CONVERT_KC_WK(Keyboard::Multiply, 0x00D7);
        CODE_CONVERT_KC_WK(Keyboard::Ooblique, 0x00D8);
        CODE_CONVERT_KC_WK(Keyboard::Ugrave, 0x00D9);
        CODE_CONVERT_KC_WK(Keyboard::Uacute, 0x00DA);
        CODE_CONVERT_KC_WK(Keyboard::Ucircumflex, 0x00DB);
        CODE_CONVERT_KC_WK(Keyboard::Udiaeresis, 0x00DC);
        CODE_CONVERT_KC_WK(Keyboard::Yacute, 0x00DD);
        CODE_CONVERT_KC_WK(Keyboard::THORN, 0x00DE);
        CODE_CONVERT_KC_WK(Keyboard::Ssharp, 0x00DF);
        CODE_CONVERT_KC_WK(Keyboard::Division, 0x00F7);
        CODE_CONVERT_KC_WK(Keyboard::Ydiaeresis, 0x00FF);

        CODE_CONVERT_KC_WK(Keyboard::AltGr, VK_RMENU);
        CODE_CONVERT_KC_WK(Keyboard::MultiKey, VK_MULTIPLY);
        CODE_CONVERT_KC_WK(Keyboard::Codeinput, VK_PROCESSKEY);
        CODE_CONVERT_KC_WK(Keyboard::SingleCandidate, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MultipleCandidate, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::PreviousCandidate, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::ModeSwitch, VK_MODECHANGE);
        CODE_CONVERT_KC_WK(Keyboard::ScriptSwitch, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Kanji, VK_KANJI);
        CODE_CONVERT_KC_WK(Keyboard::Muhenkan, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HenkanMode, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Henkan, VK_CONVERT);
        CODE_CONVERT_KC_WK(Keyboard::Romaji, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Hiragana, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Katakana, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HiraganaKatakana, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Zenkaku, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Hankaku, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ZenkakuHankaku, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Touroku, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Massyo, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::KanaLock, VK_KANA);
        CODE_CONVERT_KC_WK(Keyboard::KanaShift, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::EisuShift, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::EisuToggle, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::KanjiBangou, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ZenKoho, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MaeKoho, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Hangul, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulStart, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulEnd, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulHanja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulJamo, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulRomaja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulCodeinput, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulJeonja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulBanja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulPreHanja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulPostHanja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulSingleCandidate, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulMultipleCandidate, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulPreviousCandidate, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HangulSpecial, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Hangulswitch, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::DeadGrave, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadAcute, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadCircumflex, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadTilde, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadMacron, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadBreve, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadAbovedot, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadDiaeresis, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadAbovering, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadDoubleacute, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadCaron, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadCedilla, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadOgonek, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadIota, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadVoicedSound, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadSemivoicedSound, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadBelowdot, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadHook, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DeadHorn, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Back, VK_BROWSER_BACK);
        CODE_CONVERT_KC_WK(Keyboard::Forward, VK_BROWSER_FORWARD);
        CODE_CONVERT_KC_WK(Keyboard::Stop, VK_BROWSER_STOP);
        CODE_CONVERT_KC_WK(Keyboard::Refresh, VK_BROWSER_REFRESH);
        CODE_CONVERT_KC_WK(Keyboard::VolumeDown, VK_VOLUME_DOWN);
        CODE_CONVERT_KC_WK(Keyboard::VolumeMute, VK_VOLUME_MUTE);
        CODE_CONVERT_KC_WK(Keyboard::VolumeUp, VK_VOLUME_UP);
        CODE_CONVERT_KC_WK(Keyboard::BassBoost, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::BassUp, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::BassDown, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::TrebleUp, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::TrebleDown, VK_NONAME);
        //CODE_CONVERT_KC_WK(Keyboard::MediaPlay, VK_MEDIA_PLAY);
        CODE_CONVERT_KC_WK(Keyboard::MediaStop, VK_MEDIA_STOP);
        CODE_CONVERT_KC_WK(Keyboard::MediaPrevious, VK_MEDIA_PREV_TRACK);
        CODE_CONVERT_KC_WK(Keyboard::MediaNext, VK_MEDIA_NEXT_TRACK);
        CODE_CONVERT_KC_WK(Keyboard::MediaRecord, VK_NONAME);
        //CODE_CONVERT_KC_WK(Keyboard::MediaPause, VK_MEDIA_PAUSE);
        CODE_CONVERT_KC_WK(Keyboard::MediaTogglePlayPause, VK_MEDIA_PLAY_PAUSE);
        CODE_CONVERT_KC_WK(Keyboard::HomePage, VK_BROWSER_HOME);
        CODE_CONVERT_KC_WK(Keyboard::Favorites, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Search, VK_BROWSER_SEARCH);
        CODE_CONVERT_KC_WK(Keyboard::Standby, VK_SLEEP);
        CODE_CONVERT_KC_WK(Keyboard::OpenUrl, VK_BROWSER_FAVORITES);

        CODE_CONVERT_KC_WK(Keyboard::LaunchMail, VK_LAUNCH_MAIL);
        CODE_CONVERT_KC_WK(Keyboard::LaunchMedia, VK_LAUNCH_MEDIA_SELECT);
        CODE_CONVERT_KC_WK(Keyboard::Launch0, VK_LAUNCH_APP1);
        CODE_CONVERT_KC_WK(Keyboard::Launch1, VK_LAUNCH_APP2);
        CODE_CONVERT_KC_WK(Keyboard::Launch2, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch3, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch4, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch5, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch6, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch7, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch8, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Launch9, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchA, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchB, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchC, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchD, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchE, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchF, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::MonBrightnessUp, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MonBrightnessDown, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::KeyboardLightOnOff, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::KeyboardBrightnessUp, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::KeyboardBrightnessDown, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::PowerOff, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::WakeUp, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Eject, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ScreenSaver, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::WWW, VK_BROWSER_HOME);
        CODE_CONVERT_KC_WK(Keyboard::Memo, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LightBulb, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Shop, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::History, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::AddFavorite, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::HotLinks, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::BrightnessAdjust, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Finance, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Community, VK_NONAME);
        // CODE_CONVERT_KC_WK(Keyboard::AudioRewind, VK_MEDIA_REWIND);
        CODE_CONVERT_KC_WK(Keyboard::BackForward, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ApplicationLeft, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ApplicationRight, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Book, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::CD, VK_NONAME);
        // CODE_CONVERT_KC_WK(Keyboard::Calculator, VK_CALCULATOR);
        CODE_CONVERT_KC_WK(Keyboard::ToDoList, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ClearGrab, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Close, VK_NONAME);
        // CODE_CONVERT_KC_WK(Keyboard::Copy, VK_COPY);
        // CODE_CONVERT_KC_WK(Keyboard::Cut, VK_CUT);
        CODE_CONVERT_KC_WK(Keyboard::Display, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::DOS, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Documents, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Excel, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Explorer, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Game, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Go, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::iTouch, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LogOff, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Market, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Meeting, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MenuKB, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MenuPB, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MySites, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::News, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::OfficeHome, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Option, VK_NONAME);
        // CODE_CONVERT_KC_WK(Keyboard::Paste, VK_PASTE);
        CODE_CONVERT_KC_WK(Keyboard::Phone, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Calendar, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Reply, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Reload, VK_BROWSER_REFRESH);
        CODE_CONVERT_KC_WK(Keyboard::RotateWindows, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::RotationPB, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::RotationKB, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Save, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Send, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Spell, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::SplitScreen, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Support, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::TaskPane, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Terminal, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Tools, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Travel, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Video, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Word, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Xfer, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ZoomIn, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ZoomOut, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Away, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Messenger, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::WebCam, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::MailForward, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Pictures, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Music, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Battery, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Bluetooth, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::WLAN, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::UWB, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::AudioForward, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::AudioRepeat, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::AudioRandomPlay, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Subtitle, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::AudioCycleTrack, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Time, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Hibernate, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::View, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::TopMenu, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::PowerDown, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Suspend, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ContrastAdjust, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::LaunchG, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LaunchH, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::TouchpadToggle, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::TouchpadOn, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::TouchpadOff, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::MicMute, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Red, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Green, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Yellow, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Blue, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::ChannelUp, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ChannelDown, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::MediaLast, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Select, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Yes, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::No, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Cancel, VK_CANCEL);
        CODE_CONVERT_KC_WK(Keyboard::Printer, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Execute, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Sleep, VK_SLEEP);
        // CODE_CONVERT_KC_WK(Keyboard::Play, VK_MEDIA_PLAY);
        CODE_CONVERT_KC_WK(Keyboard::Zoom, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Jisho, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Oyayubi_Left, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Oyayubi_Right, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Context1, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Context2, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Context3, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Context4, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Call, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Hangup, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Flip, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::ToggleCallHangup, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::VoiceDial, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::LastNumberRedial, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Kana, VK_KANA);
        CODE_CONVERT_KC_WK(Keyboard::One, 0x16);
        CODE_CONVERT_KC_WK(Keyboard::Junja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Final, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Hanja, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::Off, VK_NONAME);

        CODE_CONVERT_KC_WK(Keyboard::Convert, 0x1C);
        CODE_CONVERT_KC_WK(Keyboard::NonConvert, 0x1D);
        CODE_CONVERT_KC_WK(Keyboard::Accept, 0x1E);
        CODE_CONVERT_KC_WK(Keyboard::ModeChange, 0x1F);
        CODE_CONVERT_KC_WK(Keyboard::PrintScreen, 0x2C);
        CODE_CONVERT_KC_WK(Keyboard::Apps, 0x5D);
        CODE_CONVERT_KC_WK(Keyboard::Separator, 0x6C);

        CODE_CONVERT_KC_WK(Keyboard::Attn, 0xF6);
        CODE_CONVERT_KC_WK(Keyboard::CrSel, 0xF7);
        CODE_CONVERT_KC_WK(Keyboard::ExSel, 0xF8);
        CODE_CONVERT_KC_WK(Keyboard::EraseEOF, 0xF9);
        CODE_CONVERT_KC_WK(Keyboard::Play, 0xFA);
        CODE_CONVERT_KC_WK(Keyboard::Zoom, 0xFB);
        CODE_CONVERT_KC_WK(Keyboard::NoName, 0xFC);
        CODE_CONVERT_KC_WK(Keyboard::Pa1, 0xFD);
        CODE_CONVERT_KC_WK(Keyboard::Clear, 0xFE);

        CODE_CONVERT_KC_WK(Keyboard::Camera, VK_NONAME);
        CODE_CONVERT_KC_WK(Keyboard::CameraFocus, VK_NONAME);

        return 0;
    }

    Keyboard::Code WindowEventCode::WinkeyToKeycodeSpecial(uint64 winkeycode, bool shifDown)
    {
        Keyboard::Code keycode = WindowEventCode::ConvertKeycodeWinkey(winkeycode);

        if (keycode == Keyboard::ControlLeft && GetKeyState(VK_RCONTROL)) {
            return Keyboard::ControlRight;
        }

        if (keycode == Keyboard::ShiftLeft && GetKeyState(VK_RSHIFT)) {
            return Keyboard::ShiftRight;
        }

        if (keycode == Keyboard::AltLeft && GetKeyState(VK_RMENU)) {
            return Keyboard::AltRight;
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
    #define CODE_CONVERT_SC_WK(scancode, vkey)   if (scancode_to_vkey) {\
                                                        if (code == scancode) return vkey;\
                                                    } else {\
                                                        if (vkey == code) return scancode;\
                                                    }

    uint64 WindowEventCode::ConvertScancodeWinkey(Keyboard::Code code, bool scancode_to_vkey)
    {
        CODE_CONVERT_SC_WK(Keyboard::Off, 0x00);
        CODE_CONVERT_SC_WK(Keyboard::Escape, 0x01);

        CODE_CONVERT_SC_WK(Keyboard::Keypad1, 0x02);
        CODE_CONVERT_SC_WK(Keyboard::Keypad2, 0x03);
        CODE_CONVERT_SC_WK(Keyboard::Keypad3, 0x04);
        CODE_CONVERT_SC_WK(Keyboard::Keypad4, 0x05);
        CODE_CONVERT_SC_WK(Keyboard::Keypad5, 0x06);
        CODE_CONVERT_SC_WK(Keyboard::Keypad6, 0x07);
        CODE_CONVERT_SC_WK(Keyboard::Keypad7, 0x08);
        CODE_CONVERT_SC_WK(Keyboard::Keypad8, 0x09);
        CODE_CONVERT_SC_WK(Keyboard::Keypad9, 0x0A);
        CODE_CONVERT_SC_WK(Keyboard::Keypad0, 0x0B);

        CODE_CONVERT_SC_WK(Keyboard::Minus, 0x0C);
        CODE_CONVERT_SC_WK(Keyboard::Equal, 0x0D);
        CODE_CONVERT_SC_WK(Keyboard::Plus, 0x0D);
        CODE_CONVERT_SC_WK(Keyboard::Tab, 0x0F);

        CODE_CONVERT_SC_WK(Keyboard::Q, 0x10);
        CODE_CONVERT_SC_WK(Keyboard::W, 0x11);
        CODE_CONVERT_SC_WK(Keyboard::E, 0x12);
        CODE_CONVERT_SC_WK(Keyboard::R, 0x13);
        CODE_CONVERT_SC_WK(Keyboard::T, 0x14);
        CODE_CONVERT_SC_WK(Keyboard::Y, 0x15);
        CODE_CONVERT_SC_WK(Keyboard::U, 0x16);
        CODE_CONVERT_SC_WK(Keyboard::I, 0x17);
        CODE_CONVERT_SC_WK(Keyboard::O, 0x18);
        CODE_CONVERT_SC_WK(Keyboard::P, 0x19);

        CODE_CONVERT_SC_WK(Keyboard::ParenLeft, 0x1A);
        CODE_CONVERT_SC_WK(Keyboard::ParenRight, 0x1B);
        CODE_CONVERT_SC_WK(Keyboard::Return, 0x1C);
        CODE_CONVERT_SC_WK(Keyboard::Accept, 0x1C);
        CODE_CONVERT_SC_WK(Keyboard::ControlLeft, 0x1D);

        CODE_CONVERT_SC_WK(Keyboard::A, 0x1E);
        CODE_CONVERT_SC_WK(Keyboard::S, 0x1F);
        CODE_CONVERT_SC_WK(Keyboard::D, 0x20);
        CODE_CONVERT_SC_WK(Keyboard::F, 0x21);
        CODE_CONVERT_SC_WK(Keyboard::G, 0x22);
        CODE_CONVERT_SC_WK(Keyboard::H, 0x23);
        CODE_CONVERT_SC_WK(Keyboard::J, 0x24);
        CODE_CONVERT_SC_WK(Keyboard::K, 0x25);
        CODE_CONVERT_SC_WK(Keyboard::L, 0x26);

        CODE_CONVERT_SC_WK(Keyboard::Colon, 0x27);
        CODE_CONVERT_SC_WK(Keyboard::Semicolon, 0x27);
        CODE_CONVERT_SC_WK(Keyboard::Quotation, 0x28);
        CODE_CONVERT_SC_WK(Keyboard::Apostrophe, 0x28);
        CODE_CONVERT_SC_WK(Keyboard::Backtick, 0x29);
        CODE_CONVERT_SC_WK(Keyboard::Shift, 0x2A);
        CODE_CONVERT_SC_WK(Keyboard::ShiftLeft, 0x2A);
        CODE_CONVERT_SC_WK(Keyboard::Backslash, 0x2B);
        CODE_CONVERT_SC_WK(Keyboard::Execute, 0x2B);

        CODE_CONVERT_SC_WK(Keyboard::Z, 0x2C);
        CODE_CONVERT_SC_WK(Keyboard::X, 0x2D);
        CODE_CONVERT_SC_WK(Keyboard::C, 0x2E);
        CODE_CONVERT_SC_WK(Keyboard::V, 0x2F);
        CODE_CONVERT_SC_WK(Keyboard::B, 0x30);
        CODE_CONVERT_SC_WK(Keyboard::N, 0x31);
        CODE_CONVERT_SC_WK(Keyboard::M, 0x32);

        CODE_CONVERT_SC_WK(Keyboard::Comma, 0x33);
        CODE_CONVERT_SC_WK(Keyboard::Period, 0x34);
        CODE_CONVERT_SC_WK(Keyboard::Divide, 0x35);
        CODE_CONVERT_SC_WK(Keyboard::ShiftRight, 0x36);
        CODE_CONVERT_SC_WK(Keyboard::Multiply, 0x37);
        CODE_CONVERT_SC_WK(Keyboard::Print, 0x37);
        CODE_CONVERT_SC_WK(Keyboard::AltLeft, 0x38);
        CODE_CONVERT_SC_WK(Keyboard::Alt, 0x38);
        CODE_CONVERT_SC_WK(Keyboard::Space, 0x39);
        CODE_CONVERT_SC_WK(Keyboard::CapsLock, 0x3A);

        CODE_CONVERT_SC_WK(Keyboard::F1, 0x3B);
        CODE_CONVERT_SC_WK(Keyboard::F2, 0x3C);
        CODE_CONVERT_SC_WK(Keyboard::F3, 0x3D);
        CODE_CONVERT_SC_WK(Keyboard::F4, 0x3E);
        CODE_CONVERT_SC_WK(Keyboard::F5, 0x3F);
        CODE_CONVERT_SC_WK(Keyboard::F6, 0x40);
        CODE_CONVERT_SC_WK(Keyboard::F7, 0x41);
        CODE_CONVERT_SC_WK(Keyboard::F8, 0x42);
        CODE_CONVERT_SC_WK(Keyboard::F9, 0x43);
        CODE_CONVERT_SC_WK(Keyboard::F10, 0x44);

        CODE_CONVERT_SC_WK(Keyboard::NumLock, 0x45);
        CODE_CONVERT_SC_WK(Keyboard::Pause, 0x45);
        CODE_CONVERT_SC_WK(Keyboard::ScrollLock, 0x46);
        CODE_CONVERT_SC_WK(Keyboard::Home, 0x47);
        CODE_CONVERT_SC_WK(Keyboard::Up, 0x48);
        CODE_CONVERT_SC_WK(Keyboard::PageUp, 0x49);
        CODE_CONVERT_SC_WK(Keyboard::Left, 0x4B);
        CODE_CONVERT_SC_WK(Keyboard::Right, 0x4D);
        CODE_CONVERT_SC_WK(Keyboard::Minus, 0x4A);
        CODE_CONVERT_SC_WK(Keyboard::Plus, 0x4E);
        CODE_CONVERT_SC_WK(Keyboard::End, 0x4F);
        CODE_CONVERT_SC_WK(Keyboard::Down, 0x50);
        CODE_CONVERT_SC_WK(Keyboard::PageDown, 0x51);
        CODE_CONVERT_SC_WK(Keyboard::Insert, 0x52);
        CODE_CONVERT_SC_WK(Keyboard::Delete, 0x53);
        CODE_CONVERT_SC_WK(Keyboard::Separator, 0x53);
        CODE_CONVERT_SC_WK(Keyboard::Decimal, 0x53);
        //CODE_CONVERT_SC_WK(Keyboard::Miscellaneous, 0x56);
        CODE_CONVERT_SC_WK(Keyboard::SuperLeft, 0x5B);
        CODE_CONVERT_SC_WK(Keyboard::SuperRight, 0x5C);
        CODE_CONVERT_SC_WK(Keyboard::Apps, 0x5D);

        CODE_CONVERT_SC_WK(Keyboard::F13, 0x64);
        CODE_CONVERT_SC_WK(Keyboard::F14, 0x65);
        CODE_CONVERT_SC_WK(Keyboard::F15, 0x66);
        CODE_CONVERT_SC_WK(Keyboard::F16, 0x67);
        CODE_CONVERT_SC_WK(Keyboard::F17, 0x68);
        CODE_CONVERT_SC_WK(Keyboard::F18, 0x69);

        CODE_CONVERT_SC_WK(Keyboard::Search, 0x65);
        CODE_CONVERT_SC_WK(Keyboard::Favorites, 0x66);
        //CODE_CONVERT_SC_WK(Keyboard::Start, 0x67);
        CODE_CONVERT_SC_WK(Keyboard::Refresh, 0x68);
        CODE_CONVERT_SC_WK(Keyboard::Forward, 0x69);
        CODE_CONVERT_SC_WK(Keyboard::Back, 0x6A);
        CODE_CONVERT_SC_WK(Keyboard::Stop, 0x6B);
        //CODE_CONVERT_SC_WK(Keyboard::Mail, 0x6C);
        //CODE_CONVERT_SC_WK(Keyboard::Media, 0x6D);
        //CODE_CONVERT_SC_WK(Keyboard::App1, 0x6E);
        //CODE_CONVERT_SC_WK(Keyboard::App2, 0x6F);

        CODE_CONVERT_SC_WK(Keyboard::F19, 0x71);
        CODE_CONVERT_SC_WK(Keyboard::F20, 0x72);
        CODE_CONVERT_SC_WK(Keyboard::F21, 0x73);
        CODE_CONVERT_SC_WK(Keyboard::F22, 0x74);
        CODE_CONVERT_SC_WK(Keyboard::F23, 0x75);
        CODE_CONVERT_SC_WK(Keyboard::F24, 0x76);

        CODE_CONVERT_SC_WK(Keyboard::Select, 0x77);
        //CODE_CONVERT_SC_WK(Keyboard::StopMedia, 0x78);
        //CODE_CONVERT_SC_WK(Keyboard::PreviousTrack, 0x7B);
        //CODE_CONVERT_SC_WK(Keyboard::PlayPauseMedia, 0x7C);
        //CODE_CONVERT_SC_WK(Keyboard::NextTrack, 0x7E);
        CODE_CONVERT_SC_WK(Keyboard::VolumeMute, 0x7F);
        CODE_CONVERT_SC_WK(Keyboard::VolumeUp, 0x80);
        CODE_CONVERT_SC_WK(Keyboard::VolumeDown, 0x81);
        //CODE_CONVERT_SC_WK(Keyboard::Unicode, 0xE4);
        //CODE_CONVERT_SC_WK(Keyboard::Process, 0xE7);
        CODE_CONVERT_SC_WK(Keyboard::Attn, 0xF6);
        CODE_CONVERT_SC_WK(Keyboard::CrSel, 0xF7);
        CODE_CONVERT_SC_WK(Keyboard::ExSel, 0xF8);
        CODE_CONVERT_SC_WK(Keyboard::EraseEOF, 0xF9);
        CODE_CONVERT_SC_WK(Keyboard::Play, 0xFA);
        CODE_CONVERT_SC_WK(Keyboard::Zoom, 0xFB);
        CODE_CONVERT_SC_WK(Keyboard::NoName, 0xFC);
        CODE_CONVERT_SC_WK(Keyboard::Pa1, 0xFD);
        CODE_CONVERT_SC_WK(Keyboard::AltRight, 0xE038);

        // Misc Keys
        /*CODE_CONVERT_SC_WK(Keyboard::Escape, 0x01);
        CODE_CONVERT_SC_WK(Keyboard::Tab, 0x09);
        CODE_CONVERT_SC_WK(Keyboard::Backtab, 0x0F);
        CODE_CONVERT_SC_WK(Keyboard::Backspace, 0x08);
        CODE_CONVERT_SC_WK(Keyboard::Return, 0x0D);  // Carriage return
        CODE_CONVERT_SC_WK(Keyboard::Enter, 0x0D);  // Typically same as Return
        CODE_CONVERT_SC_WK(Keyboard::Insert, 0x2D);
        CODE_CONVERT_SC_WK(Keyboard::Delete, 0x2E);
        CODE_CONVERT_SC_WK(Keyboard::Pause, 0x13);  // Break key
        CODE_CONVERT_SC_WK(Keyboard::Print, 0x2A);
        CODE_CONVERT_SC_WK(Keyboard::SysReq, 0x85);
        CODE_CONVERT_SC_WK(Keyboard::Clear, 0x0C);

        // Cursor Movement
        CODE_CONVERT_SC_WK(Keyboard::Home, 0x47);
        CODE_CONVERT_SC_WK(Keyboard::End, 0x4F);
        CODE_CONVERT_SC_WK(Keyboard::Left, 0x4B);
        CODE_CONVERT_SC_WK(Keyboard::Up, 0x48);
        CODE_CONVERT_SC_WK(Keyboard::Right, 0x4D);
        CODE_CONVERT_SC_WK(Keyboard::Down, 0x50);
        CODE_CONVERT_SC_WK(Keyboard::PageUp, 0x49);
        CODE_CONVERT_SC_WK(Keyboard::PageDown, 0x51);

        // Modifiers
        CODE_CONVERT_SC_WK(Keyboard::ShiftLock, 0x14);  // Caps Lock
        CODE_CONVERT_SC_WK(Keyboard::Shift, 0x16);      // Left Shift
        CODE_CONVERT_SC_WK(Keyboard::ShiftLeft, 0x16);
        CODE_CONVERT_SC_WK(Keyboard::ShiftRight, 0xA6);   // Right Shift
        CODE_CONVERT_SC_WK(Keyboard::Control, 0x11);      // Left Control
        CODE_CONVERT_SC_WK(Keyboard::ControlLeft, 0x11);
        CODE_CONVERT_SC_WK(Keyboard::ControlRight, 0xA5); // Right Control
        CODE_CONVERT_SC_WK(Keyboard::Meta, 0xE5);         // Windows key
        CODE_CONVERT_SC_WK(Keyboard::MetaLeft, 0xE5);     // Left Windows key
        CODE_CONVERT_SC_WK(Keyboard::MetaRight, 0xE6);    // Right Windows key
        CODE_CONVERT_SC_WK(Keyboard::Alt, 0x1E);          // Left Alt
        CODE_CONVERT_SC_WK(Keyboard::AltLeft, 0x1E);
        CODE_CONVERT_SC_WK(Keyboard::AltRight, 0xA4);      // Right Alt

        // Function Keys
        CODE_CONVERT_SC_WK(Keyboard::F1, 0x59);
        CODE_CONVERT_SC_WK(Keyboard::F2, 0x5A);
        CODE_CONVERT_SC_WK(Keyboard::F3, 0x5B);
        CODE_CONVERT_SC_WK(Keyboard::F4, 0x5C);
        CODE_CONVERT_SC_WK(Keyboard::F5, 0x5D);
        CODE_CONVERT_SC_WK(Keyboard::F6, 0x5E);
        CODE_CONVERT_SC_WK(Keyboard::F7, 0x5F);
        CODE_CONVERT_SC_WK(Keyboard::F8, 0x60);
        CODE_CONVERT_SC_WK(Keyboard::F9, 0x61);
        CODE_CONVERT_SC_WK(Keyboard::F10, 0x62);
        CODE_CONVERT_SC_WK(Keyboard::F11, 0x63);
        CODE_CONVERT_SC_WK(Keyboard::F12, 0x64);
        CODE_CONVERT_SC_WK(Keyboard::F13, 0x65);
        CODE_CONVERT_SC_WK(Keyboard::F14, 0x66);
        CODE_CONVERT_SC_WK(Keyboard::F15, 0x67);
        CODE_CONVERT_SC_WK(Keyboard::F16, 0x68);

        // F17 to F48 (assuming they follow the same pattern)
        for (int i = 0; i <= 35; ++i) {
            CODE_CONVERT_SC_WK(Keyboard::F17 + i, 0xE7 + i - 16);
        }

        // Special Keys
        CODE_CONVERT_SC_WK(Keyboard::SuperLeft, 0xE5);  // Left Windows key
        CODE_CONVERT_SC_WK(Keyboard::SuperRight, 0xE6); // Right Windows key
        // Undo, Redo, Menu, Find - May vary depending on the application

        // Assuming no defined scancode for HyperLeft and HyperRight
        // CODE_CONVERT_SC_WK(Keyboard::HyperLeft, 0x??);
        // CODE_CONVERT_SC_WK(Keyboard::HyperRight, 0x??);

        CODE_CONVERT_SC_WK(Keyboard::Help, 0xF1);
        CODE_CONVERT_SC_WK(Keyboard::DirectionLeft, 0xCB);  // Left arrow
        CODE_CONVERT_SC_WK(Keyboard::DirectionRight, 0xCD); // Right arrow
        CODE_CONVERT_SC_WK(Keyboard::Space, 0x20);

        // Alphanumeric Keys and Symbol Keys
        CODE_CONVERT_SC_WK(Keyboard::Exclam, 0x31);
        CODE_CONVERT_SC_WK(Keyboard::QuoteDbl, 0x34);
        CODE_CONVERT_SC_WK(Keyboard::NumberSign, 0x33);
        CODE_CONVERT_SC_WK(Keyboard::Dollar, 0x04);
        CODE_CONVERT_SC_WK(Keyboard::Percent, 0x25);
        CODE_CONVERT_SC_WK(Keyboard::Ampersand, 0x16);
        CODE_CONVERT_SC_WK(Keyboard::Apostrophe, 0x27);
        CODE_CONVERT_SC_WK(Keyboard::ParenLeft, 0x28);
        CODE_CONVERT_SC_WK(Keyboard::ParenRight, 0x29);
        CODE_CONVERT_SC_WK(Keyboard::Asterisk, 0x2A);
        CODE_CONVERT_SC_WK(Keyboard::Plus, 0xBB);
        CODE_CONVERT_SC_WK(Keyboard::Comma, 0x3C);
        CODE_CONVERT_SC_WK(Keyboard::Minus, 0x2D);
        CODE_CONVERT_SC_WK(Keyboard::Period, 0x30);
        CODE_CONVERT_SC_WK(Keyboard::Slash, 0x2F);

        // Numpad and Keypad (assuming they have the same scancodes)
        CODE_CONVERT_SC_WK(Keyboard::Numpad0, 0x60);
        CODE_CONVERT_SC_WK(Keyboard::Numpad1, 0x61);
        CODE_CONVERT_SC_WK(Keyboard::Numpad2, 0x62);
        CODE_CONVERT_SC_WK(Keyboard::Numpad3, 0x63);
        CODE_CONVERT_SC_WK(Keyboard::Numpad4, 0x64);
        CODE_CONVERT_SC_WK(Keyboard::Numpad5, 0x65);
        CODE_CONVERT_SC_WK(Keyboard::Numpad6, 0x66);
        CODE_CONVERT_SC_WK(Keyboard::Numpad7, 0x67);
        CODE_CONVERT_SC_WK(Keyboard::Numpad8, 0x68);
        CODE_CONVERT_SC_WK(Keyboard::Numpad9, 0x69);

        // Remaining symbol keys
        CODE_CONVERT_SC_WK(Keyboard::Colon, 0x3A);
        CODE_CONVERT_SC_WK(Keyboard::Semicolon, 0x3B);
        CODE_CONVERT_SC_WK(Keyboard::Less, 0x3C);
        CODE_CONVERT_SC_WK(Keyboard::Equal, 0x3D);
        CODE_CONVERT_SC_WK(Keyboard::Greater, 0x3E);
        CODE_CONVERT_SC_WK(Keyboard::Question, 0x3F);
        CODE_CONVERT_SC_WK(Keyboard::At, 0x40);

        CODE_CONVERT_SC_WK(Keyboard::A, 0x41);
        CODE_CONVERT_SC_WK(Keyboard::B, 0x42);
        CODE_CONVERT_SC_WK(Keyboard::C, 0x43);
        CODE_CONVERT_SC_WK(Keyboard::D, 0x44);
        CODE_CONVERT_SC_WK(Keyboard::E, 0x45);
        CODE_CONVERT_SC_WK(Keyboard::F, 0x46);
        CODE_CONVERT_SC_WK(Keyboard::G, 0x47);
        CODE_CONVERT_SC_WK(Keyboard::H, 0x48);
        CODE_CONVERT_SC_WK(Keyboard::I, 0x49);
        CODE_CONVERT_SC_WK(Keyboard::J, 0x4A);
        CODE_CONVERT_SC_WK(Keyboard::K, 0x4B);
        CODE_CONVERT_SC_WK(Keyboard::L, 0x4C);
        CODE_CONVERT_SC_WK(Keyboard::M, 0x4D);
        CODE_CONVERT_SC_WK(Keyboard::N, 0x4E);
        CODE_CONVERT_SC_WK(Keyboard::O, 0x4F);
        CODE_CONVERT_SC_WK(Keyboard::P, 0x50);
        CODE_CONVERT_SC_WK(Keyboard::Q, 0x51);
        CODE_CONVERT_SC_WK(Keyboard::R, 0x52);
        CODE_CONVERT_SC_WK(Keyboard::S, 0x53);
        CODE_CONVERT_SC_WK(Keyboard::T, 0x54);
        CODE_CONVERT_SC_WK(Keyboard::U, 0x55);
        CODE_CONVERT_SC_WK(Keyboard::V, 0x56);
        CODE_CONVERT_SC_WK(Keyboard::W, 0x57);
        CODE_CONVERT_SC_WK(Keyboard::X, 0x58);
        CODE_CONVERT_SC_WK(Keyboard::Y, 0x59);
        CODE_CONVERT_SC_WK(Keyboard::Z, 0x5A);

        CODE_CONVERT_SC_WK(Keyboard::BracketLeft, 0x5B);
        CODE_CONVERT_SC_WK(Keyboard::Backslash, 0x5C);
        CODE_CONVERT_SC_WK(Keyboard::BracketRight, 0x5D);
        CODE_CONVERT_SC_WK(Keyboard::AsciiCircum, 0x5E);
        CODE_CONVERT_SC_WK(Keyboard::Underscore, 0x5F);*/

        return 0;
    }

    Keyboard::Code WindowEventCode::WinkeyToScancodeSpecial(uint64 winkeycode, bool shifDown)
    {
        Keyboard::Code scancode = WindowEventCode::ConvertScancodeWinkey(winkeycode);

        if (scancode == Keyboard::ControlLeft && GetKeyState(VK_RCONTROL)) {
            return Keyboard::ControlRight;
        }

        if (scancode == Keyboard::ShiftLeft && GetKeyState(VK_RSHIFT)) {
            return Keyboard::ShiftRight;
        }

        if (scancode == Keyboard::AltLeft && GetKeyState(VK_RMENU)) {
            return Keyboard::AltRight;
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