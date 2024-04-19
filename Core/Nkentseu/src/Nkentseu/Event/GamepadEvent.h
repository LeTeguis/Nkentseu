//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 11:10:23 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GAMEPADEVENT_H__
#define __NKENTSEU_GAMEPADEVENT_H__

#pragma once

#include "System/System.h"
#include "Event.h"
#include "Gamepad.h"

#include <string>

namespace nkentseu {
    class NKENTSEU_API GamepadEvent : public Event {
    public:
        GamepadEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type) : Event(win), m_GamepadID(GamepadID), m_Type(type) {}

        std::string GetID() const { return m_GamepadID; }
        Gamepad::Type GetType() const { return m_Type; }
        EVENT_CATEGORY_FLAGS(EventCategory::Gamepad | EventCategory::Input)
    protected:
        std::string m_GamepadID;
        Gamepad::Type m_Type;
    };

    class NKENTSEU_API GamepadConnectedEvent : public GamepadEvent {
    public:
        GamepadConnectedEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type) : GamepadEvent(win, GamepadID, type) {}

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "GamepadConnected: [" << Gamepad::TypeNames(m_Type) << "] -> " << m_GamepadID;
            return ss.str();
        }

        EVENT_TYPE_FLAGS(EventType::GamepadConnected)
    };

    class NKENTSEU_API GamepadDisconnectedEvent : public GamepadEvent {
    public:
        GamepadDisconnectedEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type) : GamepadEvent(win, GamepadID, type) {}

        virtual std::string ToString() const override {
            std::stringstream ss;
            ss << "GamepadDisconnected: [" << Gamepad::TypeNames(m_Type) << "] -> " << m_GamepadID;
            return ss.str();
        }

        EVENT_TYPE_FLAGS(EventType::GamepadDisconnected)
    };

    class NKENTSEU_API GamepadButtonEvent : public GamepadEvent {
    public:
        GamepadButtonEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type, Gamepad::Button button) : GamepadEvent(win, GamepadID, type), m_Button(button) {}

        Gamepad::Button GetButton() const { return m_Button; }
    protected:
        Gamepad::Button m_Button;
    };

    class NKENTSEU_API GamepadButtonPressedEvent : public GamepadButtonEvent {
    public:
        GamepadButtonPressedEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type, Gamepad::Button button) : GamepadButtonEvent(win, GamepadID, type, button) {}
        virtual std::string ToString() const override {
            std::string type = Gamepad::TypeNames(GetType());
            std::string namecode = Gamepad::GetName(GetButton(), GetType(), Gamepad::Nature::Button);
            std::stringstream ss;
            ss << "GamepadButtonPressed [" << type << "]: (" << GetButton() << ", " << namecode << ", " << GetID() << ")";
            return ss.str();
        }
        EVENT_TYPE_FLAGS(EventType::GamepadButtonPressed)
    };

    class NKENTSEU_API GamepadButtonReleasedEvent : public GamepadButtonEvent {
    public:
        GamepadButtonReleasedEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type, Gamepad::Button button) : GamepadButtonEvent(win, GamepadID, type, button) {}
        virtual std::string ToString() const override {
            std::string type = Gamepad::TypeNames(GetType());
            std::string namecode = Gamepad::GetName(GetButton(), GetType(), Gamepad::Nature::Button);
            std::stringstream ss;
            ss << "GamepadButtonReleased [" << type << "]: (" << GetButton() << ", " << namecode << ", " << GetID() << ")";
            return ss.str();
        }
        EVENT_TYPE_FLAGS(EventType::GamepadButtonReleased)
    };

    class NKENTSEU_API GamepadAxisEvent : public GamepadEvent {
    public:
        GamepadAxisEvent(uint64 win, const std::string& GamepadID, Gamepad::Type type, Gamepad::Axis axis, float32 value) : GamepadEvent(win, GamepadID, type), m_Axis(axis), m_Value(value) {}

        Gamepad::Axis GetAxis() const { return m_Axis; }
        float32 GetValue() const { return m_Value; }
        EVENT_TYPE_FLAGS(EventType::GamepadAxisMoved)
            virtual std::string ToString() const override {
            std::string type = Gamepad::TypeNames(GetType());
            std::string namecode = Gamepad::GetName(GetAxis(), GetType(), Gamepad::Nature::Axis);
            std::stringstream ss;
            ss << "GamepadAxis [" << type << "]: (" << GetAxis() << ", " << namecode << ", " << GetID() << ")";
            return ss.str();
        }
    protected:
        Gamepad::Axis m_Axis;
        float32 m_Value;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_GAMEPADEVENT_H__