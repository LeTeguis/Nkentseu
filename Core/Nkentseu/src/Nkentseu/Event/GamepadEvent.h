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
    struct NKENTSEU_API GamepadInfos {
        uintl32 joystickId;
        uintl32 vendorID;
        uintl32 productID;
        uintl32 versionNumber;
        uint16 usage;
        uint16 usagePage;
        std::string name;

        GamepadInfos(uintl32 vendorID, uintl32 productID, uintl32 versionNumber, uint16 usage, uint16 usagePage, const std::string& name);
        GamepadInfos();

        std::string ToString() const;
    };

    class NKENTSEU_API GamepadStatusEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GamepadStatus)
            EVENT_CATEGORY_FLAGS(EventCategory::Gamepad | EventCategory::Input)
    public:
        GamepadStatusEvent(uint64 win, StatusState::Code statusState, const GamepadInfos& GamepadInfos);
        GamepadStatusEvent(const GamepadStatusEvent& e);
        virtual std::string ToString() const override;

        StatusState::Code GetState() const;
        GamepadInfos GetInfos() const;
    protected:
        GamepadInfos m_GamepadInfos;
        StatusState::Code m_StatusState;
    };

    class NKENTSEU_API GamepadInputEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GamepadInput)
            EVENT_CATEGORY_FLAGS(EventCategory::Gamepad | EventCategory::Input)
    public:
        GamepadInputEvent(uint64 win, Gamepad::Button button, ButtonState::Code buttonState, const GamepadInfos& gamepadInfos);
        GamepadInputEvent(const GamepadInputEvent& e);
        virtual std::string ToString() const override;

        Gamepad::Button GetButton() const;
        ButtonState::Code GetState() const;
        GamepadInfos GetInfos() const;
    private:
        GamepadInfos m_GamepadInfos;
        ButtonState::Code m_ButtonState;
        Gamepad::Button m_Button;
    };

    class NKENTSEU_API GamepadAxisEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GamepadAxis)
            EVENT_CATEGORY_FLAGS(EventCategory::Gamepad | EventCategory::Input)
    public:
        GamepadAxisEvent(uint64 win, Gamepad::Axis axis, float32 value, const GamepadInfos& gamepadInfos);
        GamepadAxisEvent(const GamepadAxisEvent& e);
        virtual std::string ToString() const override;
        
        Gamepad::Axis GetAxis() const;
        float32 GetValue() const;
        GamepadInfos GetInfos() const;
    private:
        Gamepad::Axis m_Axis;
        float32 m_Value;
        GamepadInfos m_GamepadInfos;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_GAMEPADEVENT_H__