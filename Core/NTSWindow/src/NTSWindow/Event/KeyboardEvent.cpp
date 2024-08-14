//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:33 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "KeyboardEvent.h"
#include <NTSLogger/Formatter.h>

namespace nkentseu {
    KeyboardEvent::KeyboardEvent(uint64 win, ButtonState::Code buttonState, Keyboard::Keycode keycode, Keyboard::Scancode scancode, const ModifierState& modifierState, uint64 keychar, uint32 repeatCount)
        : Event(win), m_Keycode(keycode), m_Scancode(scancode), m_ModifierState(modifierState), m_Keychar(keychar), m_ButtonState(buttonState), m_RepeatCount(repeatCount) {}

    KeyboardEvent::KeyboardEvent(const KeyboardEvent& e)
        : Event(e), m_Keycode(e.m_Keycode), m_Scancode(e.m_Scancode), m_ModifierState(e.m_ModifierState), m_Keychar(e.m_Keychar), m_ButtonState(e.m_ButtonState), m_RepeatCount(e.m_RepeatCount) {}

    std::string KeyboardEvent::ToString() const {
        return FORMATTER.Format("KeyboardEvent {Window ID : {0}, Button State : {1}, Keycode : {2}, Scancode : {3}, Modifier State : {4}, Keychar : {5}, Repeat Count : {6}}",
            m_WindowID, ButtonState::ToString(m_ButtonState), Keyboard::GetKeycode(m_Keycode), Keyboard::GetScancode(m_Scancode), m_ModifierState, m_Keychar, m_RepeatCount);
    }

    Keyboard::Keycode KeyboardEvent::GetKeycode() const {
        return m_Keycode;
    }

    Keyboard::Scancode KeyboardEvent::GetScancode() const {
        return m_Scancode;
    }

    ModifierState KeyboardEvent::GetModifierState() const {
        return m_ModifierState;
    }

    bool KeyboardEvent::IsKeycode(Keyboard::Keycode keycode) const {
        return m_Keycode == keycode;
    }

    bool KeyboardEvent::IsScancode(Keyboard::Scancode scancode) const {
        return m_Scancode == scancode;
    }

    bool KeyboardEvent::IsModifierState(const ModifierState& modifierState) const {
        return m_ModifierState == modifierState;
    }

    uint64 KeyboardEvent::GetKeychar() const {
        return m_Keychar;
    }

    ButtonState::Code KeyboardEvent::GetState() const {
        return m_ButtonState;
    }

    uint32 KeyboardEvent::GetRepeatCount() const{
        return m_RepeatCount;
    }

    //----------------------------------

    CharEnteredEvent::CharEnteredEvent(uint64 win, uint64 character, const std::string& text) : Event(win), m_Character(character), m_Text(text){}

    CharEnteredEvent::CharEnteredEvent(const CharEnteredEvent& e) : Event(e), m_Character(e.m_Character) {}

    std::string CharEnteredEvent::ToString() const {
        return FORMATTER.Format("CharEnteredEvent {Window ID : {0}, Unicode : {1}, Character : {2}}", m_WindowID, m_Character, m_Text);
    }

    uint64 CharEnteredEvent::GetCharacter() const {
        return m_Character;
    }

    std::string CharEnteredEvent::GetText() const
    {
        return m_Text;
    }

} // namespace nkentseu
