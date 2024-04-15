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
#include <Nkentseu/Event/ModifierState.h>

namespace nkentseu {

    // Class representing a keyboard key event, inherits from the Event class
    class NKENTSEU_API KeyEvent : public Event {
    public:
        // Get the virtual key code of the pressed/released key
        Keyboard::KeyCode GetKey() const; // Needs implementation

        // Get the hardware scancode of the pressed/released key
        Keyboard::ScanCode GetScan() const; // Needs implementation

        // Get the modifier state associated with the event
        ModifierState GetState() const; // Needs implementation

        // Check if a specific key was pressed/released
        bool IsKey(Keyboard::KeyCode k) const; // Needs implementation

        // Check if a specific scancode was pressed/released
        bool IsScan(Keyboard::ScanCode s) const; // Needs implementation

        // Check for a specific modifier state
        bool IsMState(ModifierState ms) const; // Needs implementation

        // Override the IsEqual method to compare event types and key code
        virtual bool IsEqual(Event& e) const override; // Needs implementation

        // Event category flags using macro
        EVENT_CATEGORY_FLAGS(EventCategory::Keyboard | EventCategory::Input)

    protected:
        // Protected constructor with window ID, key code, scan code, and modifier state
        KeyEvent(uint64 win, Keyboard::KeyCode key, Keyboard::ScanCode scan, const ModifierState& state); // Needs implementation

        Keyboard::KeyCode m_Key;  // The virtual key code of the pressed/released key
        Keyboard::ScanCode m_Scan;  // The hardware scancode of the pressed/released key
        ModifierState m_State;     // The modifier state associated with the event
    };

    // Class representing a key pressed event, inherits from the KeyEvent class
    class NKENTSEU_API KeyPressedEvent : public KeyEvent {
    public:
        // Constructors with window ID, key code, scan code, modifier state, and optional repeat count
        KeyPressedEvent(uint64 win, Keyboard::KeyCode key, Keyboard::ScanCode scan, const ModifierState& state, int32 repeatCount = 1); // Needs implementation

        // Check if the key is a repeated key press
        bool IsRepeat() const; // Needs implementation

        // Get the number of times the key has been pressed (repeat count)
        int32 GetRepeatCount() const; // Needs implementation

        // Override the ToString method to provide a string representation of the event
        virtual std::string ToString() const override; // Needs implementation

        // Event type flags using macro
        EVENT_TYPE_FLAGS(EventType::KeyPressed)

    private:
        int32 m_RepeatCount;  // The number of times the key has been pressed (repeat count)
    };

    // Class representing a key released event, inherits from the KeyEvent class
    class NKENTSEU_API KeyReleasedEvent : public KeyEvent {
    public:
        // Constructor with window ID, key code, scan code, and modifier state
        KeyReleasedEvent(uint64 win, Keyboard::KeyCode key, Keyboard::ScanCode scan, const ModifierState& state); // Needs implementation

        // Override the ToString method to provide a string representation of the event
        virtual std::string ToString() const override; // Needs implementation

        // Event type flags using macro
        EVENT_TYPE_FLAGS(EventType::KeyReleased)

    };

    // Class representing a character pressed event, inherits from the Event class
    class NKENTSEU_API CharPressedEvent : public Event {
    public:
        // Constructor with window ID and the pressed character
        CharPressedEvent(uint64 win, uint64 character); // Needs implementation

        // Get the pressed character
        uint64 GetChar() const; // Needs implementation

        // Override the ToString method to provide a string representation of the event
        virtual std::string ToString() const override; // Needs implementation

          // Event type and category flags using macros
        EVENT_TYPE_FLAGS(EventType::CharEntered)
            EVENT_CATEGORY_FLAGS(EventCategory::Keyboard | EventCategory::Input)

    private:
        uint64 m_Character;  // The pressed character
    };
} // namespace nkentseu

#endif  // __NKENTSEU_KEYBOARD_EVENT_H__

