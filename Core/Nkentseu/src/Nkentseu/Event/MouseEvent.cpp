//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:03:58 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "MouseEvent.h"

namespace nkentseu {

    // ---- MouseWheelEvent ----

    // Constructor with window ID, delta value, and modifier state
    MouseWheelEvent::MouseWheelEvent(uint64 win, Mouse::Button wheel, float32 delta, const Vector2i& position, const ModifierState& state) : Event(win), m_Delta(delta), m_State(state), m_Wheel(wheel), m_Position(position) {}

    // Copy constructor
    MouseWheelEvent::MouseWheelEvent(const MouseWheelEvent& event) : Event(event.GetWindow()), m_Delta(event.m_Delta), m_State(event.m_State) {}

    // Get the delta value of the wheel movement
    float32 MouseWheelEvent::GetDelta() const { return m_Delta; }

    Vector2i MouseWheelEvent::GetPosition() const
    {
        return m_Position;
    }

    Mouse::Button MouseWheelEvent::GetWheel() const
    {
        return m_Wheel;
    }

    // Get the modifier state associated with the event
    ModifierState MouseWheelEvent::GetState() const { return m_State; }

    // Override the ToString method to provide a string representation of the event
    std::string MouseWheelEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseWheel: " << m_Delta << "; " << m_State.ToString();
        return ss.str();
    }

    // Override the IsEqual method to compare event types
    bool MouseWheelEvent::IsEqual(Event& e) const {
        // This should be IsEqual, not isEqual
        return (e.GetEventType() == GetEventType());
    }

    // ---- MouseButtonEvent ----

    // methods to access member variables
    Mouse::Button MouseButtonEvent::GetButton() const { return m_Button; }
    Vector2i MouseButtonEvent::GetPosition() const { return m_Position; }
    ModifierState MouseButtonEvent::GetState() const { return m_State; }

    // Check if a specific button was pressed/released
    bool MouseButtonEvent::IsButton(Mouse::Button b) const {
        return m_Button == b;
    }

    // Check for a specific modifier state
    bool MouseButtonEvent::IsMState(ModifierState ms) const {
        return m_State == ms;
    }

    // Override the IsEqual method to compare event types and button
    bool MouseButtonEvent::IsEqual(Event& e) const {
        // This should be IsEqual, not IsEqual
        if (e.GetEventType() != GetEventType())
            return false;

        MouseButtonEvent& other = dynamic_cast<MouseButtonEvent&>(e);
        return m_Button == other.m_Button;
    }

    MouseButtonEvent::MouseButtonEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position) : Event(win), m_State(state), m_Button(button), m_Position(position)
    {
    }

    MouseButtonEvent::MouseButtonEvent(const MouseButtonEvent& event) : Event(event.GetWindow()), m_State(event.m_State), m_Button(event.m_Button), m_Position(event.m_Position)
    {
    }

    // ---- MouseButtonPressedEvent ----

    // Constructors with window ID, modifier state, mouse button, and optional position
    MouseButtonPressedEvent::MouseButtonPressedEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position)
        : MouseButtonEvent(win, state, button, position) {}

    MouseButtonPressedEvent::MouseButtonPressedEvent(uint64 win, const ModifierState& state, Mouse::Button button)
        : MouseButtonEvent(win, state, button) {}

    // Copy constructor
    MouseButtonPressedEvent::MouseButtonPressedEvent(const MouseButtonPressedEvent& event) : MouseButtonEvent(event.GetWindow(), event.m_State, event.m_Button, event.m_Position) {}

    // Override the ToString method to provide a string representation of the event
    std::string MouseButtonPressedEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseButtonPressed: " << Mouse::ToString(m_Button) << ", (" << m_Position.x << ", " << m_Position.y << ")";
        return ss.str();
    }

    // ---- MouseButtonDBCLKEvent ----

    // Constructors with window ID, modifier state, mouse button, and optional position
    MouseButtonDBCLKEvent::MouseButtonDBCLKEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position)
        : MouseButtonEvent(win, state, button, position) {}

    MouseButtonDBCLKEvent::MouseButtonDBCLKEvent(uint64 win, const ModifierState& state, Mouse::Button button)
        : MouseButtonEvent(win, state, button) {}

    // Copy constructor
    MouseButtonDBCLKEvent::MouseButtonDBCLKEvent(const MouseButtonDBCLKEvent& event) : MouseButtonEvent(event.GetWindow(), event.m_State, event.m_Button, event.m_Position) {}

    // Override the ToString method to provide a string representation of the event
    std::string MouseButtonDBCLKEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseButtonDBCLKEvent: " << Mouse::ToString(m_Button) << ", (" << m_Position.x << ", " << m_Position.y << ")";
        return ss.str();
    }

    // ---- MouseButtonReleasedEvent ----

    // Constructors with window ID, modifier state, mouse button, and optional position
    MouseButtonReleasedEvent::MouseButtonReleasedEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position)
        : MouseButtonEvent(win, state, button, position) {}

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(uint64 win, const ModifierState& state, Mouse::Button button)
        : MouseButtonEvent(win, state, button) {}

    // Copy constructor
    MouseButtonReleasedEvent::MouseButtonReleasedEvent(const MouseButtonReleasedEvent& event) : MouseButtonEvent(event.GetWindow(), event.m_State, event.m_Button, event.m_Position) {}

    // Override the ToString method to provide a string representation of the event
    std::string MouseButtonReleasedEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseButtonReleased: " << Mouse::ToString(m_Button) << ", (" << m_Position.x << ", " << m_Position.y << ")";
        return ss.str();
    }

    // ---- MouseButtonRawEvent ----

    // Constructors with window ID, modifier state, mouse button, delta, pressed state, and position
    MouseButtonRawEvent::MouseButtonRawEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2i& delta, bool pressed, const Vector2& position)
        : MouseButtonEvent(win, state, button, position), m_Delta(delta), m_Pressed(pressed) {}

    MouseButtonRawEvent::MouseButtonRawEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2i& delta, bool pressed)
        : MouseButtonEvent(win, state, button), m_Delta(delta), m_Pressed(pressed) {}

    // Copy constructor
    MouseButtonRawEvent::MouseButtonRawEvent(const MouseButtonRawEvent& event) : MouseButtonEvent(event.GetWindow(), event.m_State, event.m_Button, event.m_Position), m_Delta(event.m_Delta), m_Pressed(event.m_Pressed) {}

    // Get the delta movement of the mouse
    Vector2i MouseButtonRawEvent::GetDelta() const { return m_Delta; }

    // Check if the button was pressed or released
    bool MouseButtonRawEvent::IsPressed() const { return m_Pressed; }

    // Override the ToString method to provide a string representation of the event
    std::string MouseButtonRawEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseButtonRaw: " << Mouse::ToString(m_Button) << ", (" << m_Delta.x << ", " << m_Delta.y << ")" << ", (" << m_Position.x << ", " << m_Position.y << ")";
        return ss.str();
    }

    // ---- MouseMovedEvent ----

    // Constructor with window ID, position, movement, and global position
    MouseMovedEvent::MouseMovedEvent(uint64 win, const Vector2& position, const Vector2& move, const Vector2& positionGlobal)
        : Event(win), m_Position(position), m_Move(move), m_PositionGlobal(positionGlobal) {}

    // Copy constructor
    MouseMovedEvent::MouseMovedEvent(const MouseMovedEvent& event) : Event(event.GetWindow()), m_Position(event.m_Position), m_Move(event.m_Move), m_PositionGlobal(event.m_PositionGlobal) {}

    // Get the current position of the mouse cursor
    Vector2 MouseMovedEvent::GetPosition() const { return m_Position; }

    // Get the movement since the last event
    Vector2 MouseMovedEvent::GetMove() const { return m_Move; }

    // Get the global position of the mouse cursor
    Vector2 MouseMovedEvent::GetPositionGlobal() const { return m_PositionGlobal; }

    // Override the ToString method to provide a string representation of the event
    std::string MouseMovedEvent::ToString() const {
        std::stringstream ss;
        ss << "MouseMoved: (" << m_Position.x << ", " << m_Position.y << "); (" << m_Move.x << ", " << m_Move.y << ")";
        return ss.str();
    }

    // Override the IsEqual method to compare event types, position, and movement
    bool MouseMovedEvent::IsEqual(Event& e) const {
        if (e.GetEventType() != GetEventType())
            return false;

        MouseMovedEvent& other = dynamic_cast<MouseMovedEvent&>(e);
        return m_Position == other.m_Position && m_Move == other.m_Move;
    }

    // ---- MouseEnteredEvent ----

    // Constructor with window ID
    MouseEnteredEvent::MouseEnteredEvent(uint64 win, const Vector2& position) : Event(win), m_Position(position) {}

    // Copy constructor
    MouseEnteredEvent::MouseEnteredEvent(const MouseEnteredEvent& event) : Event(event.GetWindow()) {}

    Vector2 MouseEnteredEvent::GetPosition() const { return m_Position; }

    // Override the IsEqual method to compare event types
    bool MouseEnteredEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType());
    }

    // ---- MouseExitedEvent ----

    // Constructor with window ID
    MouseExitedEvent::MouseExitedEvent(uint64 win, const Vector2& position) : Event(win), m_Position(position) {}

    // Copy constructor
    MouseExitedEvent::MouseExitedEvent(const MouseExitedEvent& event) : Event(event.GetWindow()) {}

    Vector2 MouseExitedEvent::GetPosition() const { return m_Position; }

    // Override the IsEqual method to compare event types
    bool MouseExitedEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType());
    }
    
}    // namespace nkentseu