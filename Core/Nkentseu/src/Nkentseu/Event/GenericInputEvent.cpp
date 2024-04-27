//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:27 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "GenericInputEvent.h"

namespace nkentseu {
    GenericInputInfos::GenericInputInfos(uintl32 vendorID, uintl32 productID, uintl32 versionNumber, uint16 usage, uint16 usagePage, const std::string& name)
        : joystickId(0), vendorID(vendorID), productID(productID), versionNumber(versionNumber), usage(usage), usagePage(usagePage), name(name) {}

    GenericInputInfos::GenericInputInfos()
        : joystickId(0), vendorID(0), productID(0), versionNumber(0), usage(0), usagePage(0), name("") {}

    std::string GenericInputInfos::ToString() const {
        return FORMATTER.Format("GenericInputInfos {Joystick ID : {0}, Vendor ID : {1}, Product ID : {2}, Version Number : {3}, Usage : {4}, Usage Page : {5}, Name : {6}}",
            joystickId, vendorID, productID, versionNumber, usage, usagePage, name);
    }

    //-------------------------
    GenericInputEvent::GenericInputEvent(uint64 win, GenericInput::Button button, ButtonState::Code buttonState, const GenericInputInfos& genericInputInfos)
        : Event(win), m_Button(button), m_ButtonState(buttonState), m_GenericInputInfos(genericInputInfos) {}

    GenericInputEvent::GenericInputEvent(const GenericInputEvent& e)
        : Event(e), m_Button(e.m_Button), m_ButtonState(e.m_ButtonState), m_GenericInputInfos(e.m_GenericInputInfos) {}

    std::string GenericInputEvent::ToString() const {
        return FORMATTER.Format("GenericInputEvent {Window ID : {0}, Button : {1}, Button State : {2}, Infos : {3}}",
            m_WindowID, m_Button, ButtonState::ToString(m_ButtonState), m_GenericInputInfos.ToString());
    }

    ButtonState::Code GenericInputEvent::GetState() const {
        return m_ButtonState;
    }

    GenericInput::Button GenericInputEvent::GetButton() const {
        return m_Button;
    }

    GenericInputInfos GenericInputEvent::GetInfo() const {
        return m_GenericInputInfos;
    }

    GenericStatusEvent::GenericStatusEvent(uint64 win, StatusState::Code statusState, const GenericInputInfos& genericInputInfos)
        : Event(win), m_StatusState(statusState), m_GenericInputInfos(genericInputInfos) {}

    GenericStatusEvent::GenericStatusEvent(const GenericStatusEvent& e)
        : Event(e), m_StatusState(e.m_StatusState), m_GenericInputInfos(e.m_GenericInputInfos) {}

    std::string GenericStatusEvent::ToString() const {
        return FORMATTER.Format("GenericStatusEvent {Window ID : {0}, Status State : {1}, Infos : {2}}",
            m_WindowID, StatusState::ToString(m_StatusState), m_GenericInputInfos.ToString());
    }

    StatusState::Code GenericStatusEvent::GetState() const {
        return m_StatusState;
    }

    GenericInputInfos GenericStatusEvent::GetInfo() const {
        return m_GenericInputInfos;
    }

    GenericAxisEvent::GenericAxisEvent(uint64 win, GenericInput::Axis axis, float32 value, const GenericInputInfos& genericInputInfos)
        : Event(win), m_Axis(axis), m_Value(value), m_GenericInputInfos(genericInputInfos) {}

    GenericAxisEvent::GenericAxisEvent(const GenericAxisEvent& e)
        : Event(e), m_Axis(e.m_Axis), m_Value(e.m_Value), m_GenericInputInfos(e.m_GenericInputInfos) {}

    std::string GenericAxisEvent::ToString() const {
        return FORMATTER.Format("GenericAxisEvent {Window ID : {0}, Axis : {1}, Value : {2}, Infos : {3}}",
            m_WindowID, m_Axis, m_Value, m_GenericInputInfos.ToString());
    }

    GenericInput::Axis GenericAxisEvent::GetAxis() const {
        return m_Axis;
    }

    float32 GenericAxisEvent::GetValue() const {
        return m_Value;
    }

    GenericInputInfos GenericAxisEvent::GetInfo() const {
        return m_GenericInputInfos;
    }

    GenericHatEvent::GenericHatEvent(uint64 win, GenericInput::Hat hat, float32 value, const GenericInputInfos& genericInputInfos)
        : Event(win), m_Hate(hat), m_Value(value), m_GenericInputInfos(genericInputInfos) {}

    GenericHatEvent::GenericHatEvent(const GenericHatEvent& e)
        : Event(e), m_Hate(e.m_Hate), m_Value(e.m_Value), m_GenericInputInfos(e.m_GenericInputInfos) {}

    std::string GenericHatEvent::ToString() const {
        return FORMATTER.Format("GenericHatEvent {Window ID : {0}, Hat : {1}, Value : {2}, Infos : {3}}",
            m_WindowID, m_Hate, m_Value, m_GenericInputInfos.ToString());
    }

    GenericInput::Hat GenericHatEvent::GetHat() const {
        return m_Hate;
    }

    float32 GenericHatEvent::GetValue() const {
        return m_Value;
    }

    GenericInputInfos GenericHatEvent::GetInfo() const {
        return m_GenericInputInfos;
    }
}    // namespace nkentseu