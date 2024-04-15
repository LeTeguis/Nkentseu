//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:13:27 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_GENERICINPUTEVENT_H__
#define __NKENTSEU_GENERICINPUTEVENT_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include "Event.h"
#include <Logger/Formatter.h>
#include "GenericInput.h"

namespace nkentseu {
    enum class NKENTSEU_API InputDeviceType {
        Unknow,
        GameController
    };

    struct NKENTSEU_API GenericInputInfos {
        uintl32 vendorID;
        uintl32 productID;
        uintl32 versionNumber;
        uint16 usage;
        uint16 usagePage;
        std::string name;

        GenericInputInfos(uintl32 vendorID, uintl32 productID, uintl32 versionNumber, uint16 usage, uint16 usagePage, const std::string& name);

        std::string ToString() const;
    };


    class NKENTSEU_API GenericInputEvent : public Event {
    public:
        GenericInputEvent(uint64 win, const GenericInputInfos& genericInputInfos);

        GenericInputInfos GetGenericInputInfos() const;
        EVENT_CATEGORY_FLAGS(EventCategory::GenericInput | EventCategory::Input)
    protected:
        GenericInputInfos m_GenericInputInfos;
    };

    class NKENTSEU_API GenericInputConnectedEvent : public GenericInputEvent {
    public:
        GenericInputConnectedEvent(uint64 win, const GenericInputInfos& genericInputInfos);

        virtual std::string ToString() const override;

        EVENT_TYPE_FLAGS(EventType::GenericInputConnected)
    };

    class NKENTSEU_API GenericInputDisconnectedEvent : public GenericInputEvent {
    public:
        GenericInputDisconnectedEvent(uint64 win, const GenericInputInfos& genericInputInfos);

        virtual std::string ToString() const override;

        EVENT_TYPE_FLAGS(EventType::GenericInputDisconnected)
    };


    class NKENTSEU_API GenericInputButtonEvent : public GenericInputEvent {
    public:
        GenericInputButtonEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Button button);

        GenericInput::Button GetButton() const;
    protected:
        GenericInput::Button m_Button;
    };

    class NKENTSEU_API GenericInputButtonPressedEvent : public GenericInputButtonEvent {
    public:
        GenericInputButtonPressedEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Button button);
        virtual std::string ToString() const override;
        EVENT_TYPE_FLAGS(EventType::GenericInputPressed)
    };

    class NKENTSEU_API GenericInputButtonReleasedEvent : public GenericInputButtonEvent {
    public:
        GenericInputButtonReleasedEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Button button);
        virtual std::string ToString() const override;
        EVENT_TYPE_FLAGS(EventType::GenericInputReleased)
    };

    class NKENTSEU_API GenericInputAxisEvent : public GenericInputEvent {
    public:
        GenericInputAxisEvent(uint64 win, const GenericInputInfos& genericInputInfos, GenericInput::Axis axis, float32 value);

        GenericInput::Axis GetAxis() const;
        float32 GetValue() const;

        EVENT_TYPE_FLAGS(EventType::GenericInputAxisMoved)
            virtual std::string ToString() const override;
    protected:
        GenericInput::Axis m_Axis;
        float32 m_Value;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_GENERICINPUTEVENT_H__