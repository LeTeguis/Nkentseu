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
        MouseInputEvent(uint64 win, ButtonState::Code buttonState, ModifierState modifierState, Mouse::Button button, bool isDoubleClicked, const Vector2f& position, const Vector2f& globalPosition);
        MouseInputEvent(const MouseInputEvent& e);
        virtual std::string ToString() const override;

        Mouse::Button GetButton() const;
        ButtonState::Code GetState() const;
        ModifierState GetModifierState() const;
        bool IsDoubleCliked() const;
        Vector2f GetPosition() const;
        Vector2f GetGlobalPosition() const;
    private:
        Mouse::Button m_Button;
        ButtonState::Code m_ButtonState;
        ModifierState m_ModifierState;
        bool m_IsDoubleClicked;
        Vector2f m_Position;
        Vector2f m_GlobalPosition;
    };

    class NKENTSEU_API MouseMovedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseMoved)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseMovedEvent(uint64 win, const Vector2f& position, const Vector2f& globalPosition, const Vector2f& speed);
        MouseMovedEvent(const MouseMovedEvent& e);
        virtual std::string ToString() const override;
        
        AxisState::Code GetVerticalAxisState() const;
        AxisState::Code GetHorizontalAxisState() const;
        AxisDirection::Code GetAxisDirection() const;
        Vector2f GetPosition() const;
        Vector2f GetGlobalPosition() const;
        Vector2f GetSpeed() const;
    private:
        Vector2f m_Position;
        Vector2f m_Speed;
        Vector2f m_GlobalPosition;
    };

    class NKENTSEU_API MouseWheelEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseWheel)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseWheelEvent(uint64 win, Mouse::Wheel wheel, float32 delta, ModifierState modifierState, const Vector2i& position);
        MouseWheelEvent(const MouseWheelEvent& e);
        virtual std::string ToString() const override;

        ModifierState GetModifierState() const;
        Vector2i GetPosition() const;
        float32 GetDelta() const;
        Mouse::Wheel GetWheels() const;
    private:
        float32 m_Delta;
        Mouse::Wheel m_Wheel;
        ModifierState m_State;
        Vector2i m_Position;
        ModifierState m_ModifierState;
    };

    class NKENTSEU_API MouseWindowEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::MouseWindow)
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse | EventCategory::Input)
    public:
        MouseWindowEvent(uint64 win, RegionState::Code regionState, const Vector2i& position = {});
        MouseWindowEvent(const MouseWindowEvent& e);
        virtual std::string ToString() const override;

        Vector2i GetPosition() const;
        RegionState::Code GetRegionState() const;
    private:
        RegionState::Code m_State;
        Vector2i m_Position;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_MOUSE_EVENT_H__
