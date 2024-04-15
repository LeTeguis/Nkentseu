//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:27 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "GenericInputEvent.h"

namespace nkentseu {

    GenericInputInfos::GenericInputInfos(uintl32 vendorID, uintl32 productID, uintl32 versionNumber, uint16 usage, uint16 usagePage, const std::string& name) : vendorID(vendorID), productID(productID), versionNumber(versionNumber), usage(usage), usagePage(usagePage), name(name) {

    }

    std::string GenericInputInfos::ToString() const {
        std::stringstream ss;
        ss << "HID - [VID_" << vendorID << ", PID_" << productID << ", VN_" << versionNumber << ", US_" << usage << ", USP_" << usagePage << ", Code[" << name << "]]";
        return ss.str();
    }


    GenericInputEvent::GenericInputEvent(uint64 win, const GenericInputInfos& genericInputInfos) : Event(win), m_GenericInputInfos(genericInputInfos) {}

    GenericInputInfos GenericInputEvent::GetGenericInputInfos() const { return m_GenericInputInfos; }

    GenericInputConnectedEvent::GenericInputConnectedEvent(uint64 win, const GenericInputInfos& genericInputInfos) : GenericInputEvent(win, genericInputInfos) {}

    std::string GenericInputConnectedEvent::ToString() const {
        std::stringstream ss;
        ss << "GenericInputConnectedEvent: " << m_GenericInputInfos.ToString();
        return ss.str();
    }


    GenericInputDisconnectedEvent::GenericInputDisconnectedEvent(uint64 win, const GenericInputInfos& genericInputInfos) : GenericInputEvent(win, genericInputInfos) {}

    std::string GenericInputDisconnectedEvent::ToString() const {
        std::stringstream ss;
        ss << "GenericInputDisconnectedEvent: " << m_GenericInputInfos.ToString();
        return ss.str();
    }

    GenericInputButtonEvent::GenericInputButtonEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Button button) : GenericInputEvent(win, genericInputInfos), m_Button(button) {}

    GenericInput::Button GenericInputButtonEvent::GetButton() const { return m_Button; }

    GenericInputButtonPressedEvent::GenericInputButtonPressedEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Button button) : GenericInputButtonEvent(win, genericInputInfos, button) {}
    std::string GenericInputButtonPressedEvent::ToString() const {
        return FORMATTER.Format("GenericInputButtonPressed: {0}, {1}, {2}", GetButton(), GenericInput::GetButton(GetButton()), m_GenericInputInfos.ToString());
    }

    GenericInputButtonReleasedEvent::GenericInputButtonReleasedEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Button button) : GenericInputButtonEvent(win, genericInputInfos, button) {}
    std::string GenericInputButtonReleasedEvent::ToString() const {
        return FORMATTER.Format("GenericInputButtonReleased: {0}, {1}, {2}", GetButton(), GenericInput::GetButton(GetButton()), m_GenericInputInfos.ToString());
    }

    GenericInputAxisEvent::GenericInputAxisEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Axis axis, float32 value) : GenericInputEvent(win, genericInputInfos), m_Axis(axis), m_Value(value) {}

    GenericInput::Axis GenericInputAxisEvent::GetAxis() const { return m_Axis; }
    float32 GenericInputAxisEvent::GetValue() const { return m_Value; }

    std::string GenericInputAxisEvent::ToString() const {
        return FORMATTER.Format("GenericInputAxis: {0}, {1}, {2}", m_Value, GenericInput::GetAxis(GetAxis()), m_GenericInputInfos.ToString());
    }

}    // namespace nkentseu