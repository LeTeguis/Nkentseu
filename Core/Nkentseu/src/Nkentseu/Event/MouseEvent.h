//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:03:21 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MOUSE_EVENT_H__
#define __NKENTSEU_MOUSE_EVENT_H__

#pragma once

#include "System/System.h"
#include "Mouse.h"
#include "Event.h"
#include <Ntsm/Vector/Vector2.h>

namespace nkentseu {
    class NKENTSEU_API MouseInputEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseInput)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseInputEvent(uint64 win, ButtonState::Code buttonState, ModifierState modifierState, Mouse::Button button, bool isDoubleClicked, const maths::Vector2i& position, const maths::Vector2i& globalPosition);
        MouseInputEvent(const MouseInputEvent& e);
        virtual std::string ToString() const override;

        Mouse::Button GetButton() const;
        ButtonState::Code GetState() const;
        ModifierState GetModifierState() const;
        bool IsDoubleCliked() const;
        maths::Vector2i GetPosition() const;
        maths::Vector2i GetGlobalPosition() const;
    private:
        Mouse::Button m_Button;
        ButtonState::Code m_ButtonState;
        ModifierState m_ModifierState;
        bool m_IsDoubleClicked;
        maths::Vector2i m_Position;
        maths::Vector2i m_GlobalPosition;
    };

    class NKENTSEU_API MouseMovedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseMoved)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseMovedEvent(uint64 win, const maths::Vector2i& position, const maths::Vector2i& globalPosition, const maths::Vector2i& speed);
        MouseMovedEvent(const MouseMovedEvent& e);
        virtual std::string ToString() const override;
        
        AxisState::Code GetVerticalAxisState() const;
        AxisState::Code GetHorizontalAxisState() const;
        AxisDirection::Code GetAxisDirection() const;
        maths::Vector2i GetPosition() const;
        maths::Vector2i GetGlobalPosition() const;
        maths::Vector2i GetSpeed() const;
    private:
        maths::Vector2i m_Position;
        maths::Vector2i m_Speed;
        maths::Vector2i m_GlobalPosition;
    };

    class NKENTSEU_API MouseWheelEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseWheel)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseWheelEvent(uint64 win, Mouse::Wheel wheel, float32 delta, ModifierState modifierState, const maths::Vector2i& position);
        MouseWheelEvent(const MouseWheelEvent& e);
        virtual std::string ToString() const override;

        ModifierState GetModifierState() const;
        maths::Vector2i GetPosition() const;
        float32 GetDelta() const;
        Mouse::Wheel GetWheels() const;
    private:
        float32 m_Delta;
        Mouse::Wheel m_Wheel;
        ModifierState m_State;
        maths::Vector2i m_Position;
        ModifierState m_ModifierState;
    };

    class NKENTSEU_API MouseWindowEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseWindow)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseWindowEvent(uint64 win, RegionState::Code regionState, const maths::Vector2i& position = {});
        MouseWindowEvent(const MouseWindowEvent& e);
        virtual std::string ToString() const override;

        maths::Vector2i GetPosition() const;
        RegionState::Code GetRegionState() const;
    private:
        RegionState::Code m_State;
        maths::Vector2i m_Position;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_MOUSE_EVENT_H__
