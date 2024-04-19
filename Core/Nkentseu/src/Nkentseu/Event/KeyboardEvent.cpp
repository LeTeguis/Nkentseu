//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:33 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "KeyboardEvent.h"

namespace nkentseu {

    // ---- KeyEvent ----

    // Constructor for KeyEvent, initializes with window ID, key code, scan code, and modifier state
    KeyEvent::KeyEvent(uint64 win, Keyboard::Keycode key, Keyboard::Scancode scan, const ModifierState& state)
        : Event(win), m_Key(key), m_Scan(scan), m_State(state) {}

    // Get the virtual key code of the pressed/released key
    Keyboard::Keycode KeyEvent::GetKey() const {
        return m_Key;
    }

    // Get the hardware scancode of the pressed/released key
    Keyboard::Scancode KeyEvent::GetScan() const {
        return m_Scan;
    }

    // Get the modifier state associated with the event
    ModifierState KeyEvent::GetState() const {
        return m_State;
    }

    // Check if a specific key was pressed/released
    bool KeyEvent::IsKey(Keyboard::Keycode k) const {
        return m_Key == k;
    }

    // Check if a specific scancode was pressed/released
    bool KeyEvent::IsScan(Keyboard::Scancode s) const {
        return m_Scan == s;
    }

    // Check for a specific modifier state
    bool KeyEvent::IsMState(ModifierState ms) const {
        return m_State == ms;
    }

    // Override the IsEqual method to compare event types and key code
    bool KeyEvent::IsEqual(Event& e) const {
        if (e.GetEventType() != GetEventType())
            return false;

        KeyEvent& other = dynamic_cast<KeyEvent&>(e);
        return m_Key == other.m_Key;
    }

    // ---- KeyPressedEvent ----

    // Constructor for KeyPressedEvent, inherits from KeyEvent and adds a repeat count
    KeyPressedEvent::KeyPressedEvent(uint64 win, Keyboard::Keycode key, Keyboard::Scancode scan, const ModifierState& state, int32 repeatCount)
        : KeyEvent(win, key, scan, state), m_RepeatCount(repeatCount) {}

    // Check if the key is a repeated key press
    bool KeyPressedEvent::IsRepeat() const {
        return m_RepeatCount > 1;
    }

    // Get the number of times the key has been pressed (repeat count)
    int32 KeyPressedEvent::GetRepeatCount() const {
        return m_RepeatCount;
    }

    // Override the ToString method to provide a string representation of the event
    std::string KeyPressedEvent::ToString() const {
        std::stringstream ss;
        ss << "KeyPressed: (kc = " << Keyboard::GetKeycode(m_Key) << ", sc = " << Keyboard::GetScancode(m_Scan) << ", state = " << m_State << "), " << m_RepeatCount;
        return ss.str();
    }

    // ---- KeyReleasedEvent ----

    // Constructor for KeyReleasedEvent, inherits from KeyEvent
    KeyReleasedEvent::KeyReleasedEvent(uint64 win, Keyboard::Keycode key, Keyboard::Scancode scan, const ModifierState& state)
        : KeyEvent(win, key, scan, state) {}

    // Override the ToString method to provide a string representation of the event
    std::string KeyReleasedEvent::ToString() const {
        std::stringstream ss;
        ss << "KeyReleased: (kc = " << Keyboard::GetKeycode(m_Key) << ", sc = " << Keyboard::GetScancode(m_Scan) << ", state = " << m_State << ")";
        return ss.str();
    }

    // ---- CharPressedEvent ----

    // Constructor for CharPressedEvent, initializes with window ID and the pressed character
    CharPressedEvent::CharPressedEvent(uint64 win, uint64 character) : Event(win), m_Character(character) {}

    // Get the pressed character
    uint64 CharPressedEvent::GetChar() const {
        return m_Character;
    }

    // Override the ToString method to provide a string representation of the event
    std::string CharPressedEvent::ToString() const {
        std::stringstream ss;
        ss << "CharPressed: " << (char)m_Character;
        return ss.str();
    }

} // namespace nkentseu
