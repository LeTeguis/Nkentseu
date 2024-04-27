//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:27 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GENERIC_INPUT_EVENT_H__
#define __NKENTSEU_GENERIC_INPUT_EVENT_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include "Event.h"
#include <Logger/Formatter.h>
#include "GenericInput.h"

namespace nkentseu {
    struct NKENTSEU_API GenericInputInfos {
        uintl32 joystickId;
        uintl32 vendorID;
        uintl32 productID;
        uintl32 versionNumber;
        uint16 usage;
        uint16 usagePage;
        std::string name;

        GenericInputInfos(uintl32 vendorID, uintl32 productID, uintl32 versionNumber, uint16 usage, uint16 usagePage, const std::string& name);
        GenericInputInfos();

        std::string ToString() const;
    };

    class NKENTSEU_API GenericStatusEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GenericStatus)
        EVENT_CATEGORY_FLAGS(EventCategory::GenericHid | EventCategory::Input)
    public:
        GenericStatusEvent(uint64 win, StatusState::Code statusState, const GenericInputInfos& genericInputInfos);
        GenericStatusEvent(const GenericStatusEvent& e);
        virtual std::string ToString() const override;

        StatusState::Code GetState() const;
        GenericInputInfos GetInfo() const;
    protected:
        GenericInputInfos m_GenericInputInfos;
        StatusState::Code m_StatusState;
    };

    class NKENTSEU_API GenericInputEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GenericInput)
            EVENT_CATEGORY_FLAGS(EventCategory::GenericHid | EventCategory::Input)
    public:
        GenericInputEvent(uint64 win, GenericInput::Button button, ButtonState::Code buttonState, const GenericInputInfos& genericInputInfos);
        GenericInputEvent(const GenericInputEvent& e);
        virtual std::string ToString() const override;

        ButtonState::Code GetState() const;
        GenericInput::Button GetButton() const;
        GenericInputInfos GetInfo() const;
        private:
        GenericInputInfos m_GenericInputInfos;
        ButtonState::Code m_ButtonState;
        GenericInput::Button m_Button;
    };

    class NKENTSEU_API GenericAxisEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GenericAxis)
            EVENT_CATEGORY_FLAGS(EventCategory::GenericHid | EventCategory::Input)
    public:
        GenericAxisEvent(uint64 win, GenericInput::Axis axis, float32 value, const GenericInputInfos& genericInputInfos);
        GenericAxisEvent(const GenericAxisEvent& e);
        virtual std::string ToString() const override;

        GenericInput::Axis GetAxis() const;
        float32 GetValue() const;
        GenericInputInfos GetInfo() const;
        private:
        GenericInput::Axis m_Axis;
        float32 m_Value;
        GenericInputInfos m_GenericInputInfos;
    };

    class NKENTSEU_API GenericHatEvent : public Event {
    public:
        EVENT_TYPE_FLAGS(EventType::GenericHat)
            EVENT_CATEGORY_FLAGS(EventCategory::GenericHid | EventCategory::Input)
    public:
        GenericHatEvent(uint64 win, GenericInput::Hat hat, float32 value, const GenericInputInfos& genericInputInfos);
        GenericHatEvent(const GenericHatEvent& e);
        virtual std::string ToString() const override;

        GenericInput::Hat GetHat() const;
        float32 GetValue() const;
        GenericInputInfos GetInfo() const;
    protected:
        GenericInput::Hat m_Hate;
        float32 m_Value;
        GenericInputInfos m_GenericInputInfos;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_GENERIC_INPUT_EVENT_H__