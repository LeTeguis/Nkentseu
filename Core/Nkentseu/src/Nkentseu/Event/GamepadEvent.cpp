//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 11:10:23 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "GamepadEvent.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    GamepadInfos::GamepadInfos(uintl32 vendorID, uintl32 productID, uintl32 versionNumber, uint16 usage, uint16 usagePage, const std::string& name)
        : joystickId(0), vendorID(vendorID), productID(productID), versionNumber(versionNumber), usage(usage), usagePage(usagePage), name(name) {}

    GamepadInfos::GamepadInfos()
        : joystickId(0), vendorID(0), productID(0), versionNumber(0), usage(0), usagePage(0), name("") {}

    std::string GamepadInfos::ToString() const {
        return FORMATTER.Format("GamepadInfos {Joystick ID : {0}, Vendor ID : {1}, Product ID : {2}, Version Number : {3}, Usage : {4}, Usage Page : {5}, Name : {6}}",
            joystickId, vendorID, productID, versionNumber, usage, usagePage, name);
    }

    //----------------
    GamepadStatusEvent::GamepadStatusEvent(uint64 win, StatusState::Code statusState, const GamepadInfos& gamepadInfos)
        : Event(win), m_StatusState(statusState), m_GamepadInfos(gamepadInfos) {}

    GamepadStatusEvent::GamepadStatusEvent(const GamepadStatusEvent& e)
        : Event(e), m_StatusState(e.m_StatusState), m_GamepadInfos(e.m_GamepadInfos) {}

    std::string GamepadStatusEvent::ToString() const {
        return FORMATTER.Format("GamepadStatusEvent {Window ID : {0}, Status State : {1}, Gamepad Infos : {2}}",
            m_WindowID, StatusState::ToString(m_StatusState), m_GamepadInfos.ToString());
    }

    StatusState::Code GamepadStatusEvent::GetState() const {
        return m_StatusState;
    }

    GamepadInfos GamepadStatusEvent::GetInfos() const {
        return m_GamepadInfos;
    }

    // GamepadInputEvent.cpp
    GamepadInputEvent::GamepadInputEvent(uint64 win, Gamepad::Button button, ButtonState::Code buttonState, const GamepadInfos& gamepadInfos)
        : Event(win), m_Button(button), m_ButtonState(buttonState), m_GamepadInfos(gamepadInfos) {}

    GamepadInputEvent::GamepadInputEvent(const GamepadInputEvent& e)
        : Event(e), m_Button(e.m_Button), m_ButtonState(e.m_ButtonState), m_GamepadInfos(e.m_GamepadInfos) {}

    std::string GamepadInputEvent::ToString() const {
        return FORMATTER.Format("GamepadInputEvent {Window ID : {0}, Button : {1}, Button State : {2}, Gamepad Infos : {3}}",
            m_WindowID, ((uint64)m_Button), ButtonState::ToString(m_ButtonState), m_GamepadInfos.ToString());
    }

    Gamepad::Button GamepadInputEvent::GetButton() const {
        return m_Button;
    }

    ButtonState::Code GamepadInputEvent::GetState() const {
        return m_ButtonState;
    }

    GamepadInfos GamepadInputEvent::GetInfos() const {
        return m_GamepadInfos;
    }

    // GamepadAxisEvent.cpp
    GamepadAxisEvent::GamepadAxisEvent(uint64 win, Gamepad::Axis axis, float32 value, const GamepadInfos& gamepadInfos)
        : Event(win), m_Axis(axis), m_Value(value), m_GamepadInfos(gamepadInfos) {}

    GamepadAxisEvent::GamepadAxisEvent(const GamepadAxisEvent& e)
        : Event(e), m_Axis(e.m_Axis), m_Value(e.m_Value), m_GamepadInfos(e.m_GamepadInfos) {}

    std::string GamepadAxisEvent::ToString() const {
        return FORMATTER.Format("GamepadAxisEvent {Window ID : {0}, Axis : {1}, Value : {2}, Gamepad Infos : {3}}",
            m_WindowID, (uint64)m_Axis, m_Value, m_GamepadInfos.ToString());
    }

    Gamepad::Axis GamepadAxisEvent::GetAxis() const {
        return m_Axis;
    }

    float32 GamepadAxisEvent::GetValue() const {
        return m_Value;
    }

    GamepadInfos GamepadAxisEvent::GetInfos() const {
        return m_GamepadInfos;
    }
}    // namespace nkentseu