//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:33 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_KEYBOARD_EVENT_H__
#define __NKENTSEU_KEYBOARD_EVENT_H__

#pragma once

// Include necessary header files
#include "System/System.h"
#include "Event.h"
#include "Keyboard.h"

namespace nkentseu {

    class NKENTSEU_API KeyboardEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::KeyboardInput)
        EVENT_CATEGORY_FLAGS(EventCategory::Keyboard | EventCategory::Input)
    public:
        KeyboardEvent(uint64 win, ButtonState::Code buttonState, Keyboard::Keycode keycode, Keyboard::Scancode scancode, const ModifierState& modifierState, uint64 keychar, uint32 repeatCount = 0);
        KeyboardEvent(const KeyboardEvent& e);
        virtual std::string ToString() const override;

        Keyboard::Keycode GetKeycode() const;
        Keyboard::Scancode GetScancode() const;
        ModifierState GetModifierState() const;
        bool IsKeycode(Keyboard::Keycode keycode) const;
        bool IsScancode(Keyboard::Scancode scancode) const;
        bool IsModifierState(const ModifierState& modiferState) const;
        uint64 GetKeychar() const;
        ButtonState::Code GetState() const;
        uint32 GetRepeatCount() const;
    protected:
        Keyboard::Keycode m_Keycode;
        Keyboard::Scancode m_Scancode;
        ModifierState m_ModifierState;
        uint64 m_Keychar;
        ButtonState::Code m_ButtonState;
        uint32 m_RepeatCount;
    };

    class NKENTSEU_API CharEnteredEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::CharEntered)
        EVENT_CATEGORY_FLAGS(EventCategory::Keyboard | EventCategory::Input)
    public:
        CharEnteredEvent(uint64 win, uint64 character);
        CharEnteredEvent(const CharEnteredEvent& e);
        virtual std::string ToString() const override;

        uint64 GetCharacter() const;
    private:
        uint64 m_Character;  // The pressed character
    };
} // namespace nkentseu

#endif  // __NKENTSEU_KEYBOARD_EVENT_H__

