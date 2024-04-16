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
#include <Nkentseu/Event/ModifierState.h>
#include <Ntsm/Vector/Vector2.h>

namespace nkentseu {
    class NKENTSEU_API MouseWheelEvent : public Event {
    public:
        // Constructeurs et destructeur
        MouseWheelEvent(uint64 win, float32 delta, const ModifierState& state);
        MouseWheelEvent(const MouseWheelEvent& event);

        // Méthodes publiques
        float32 GetDelta() const;
        ModifierState GetState() const;
        virtual std::string ToString() const override;
        virtual bool IsEqual(Event& e) const override;

        // Flags d'événement
        EVENT_TYPE_FLAGS(EventType::MouseWheelScrolled_ev)
            EVENT_CATEGORY_FLAGS(EventCategory::Mouse_ev | EventCategory::Input_ev)
    private:
        float32 m_Delta;
        ModifierState m_State;
    };

    class NKENTSEU_API MouseButtonEvent : public Event {
    public:
        // Méthodes publiques
        inline Mouse::Button GetButton() const;
        inline Vector2i GetPosition() const;
        inline ModifierState GetState() const;
        bool IsButton(Mouse::Button b) const;
        bool IsMState(ModifierState ms) const;
        virtual bool IsEqual(Event& e) const override;
        EVENT_CATEGORY_FLAGS(EventCategory::Mouse_ev | EventCategory::Input_ev)
    protected:
        MouseButtonEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position = { 0.0f });
        MouseButtonEvent(const MouseButtonEvent& event);
        Mouse::Button m_Button;
        Vector2 m_Position;
        ModifierState m_State;
    };

    class NKENTSEU_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        // Constructeurs et destructeur
        MouseButtonPressedEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position);
        MouseButtonPressedEvent(uint64 win, const ModifierState& state, Mouse::Button button);
        MouseButtonPressedEvent(const MouseButtonPressedEvent& event);

        // Méthode publique
        virtual std::string ToString() const override;
        EVENT_TYPE_FLAGS(EventType::MouseButtonPressed_ev)
    };

    class NKENTSEU_API MouseButtonDBCLKEvent : public MouseButtonEvent {
    public:
        // Constructeurs et destructeur
        MouseButtonDBCLKEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position);
        MouseButtonDBCLKEvent(uint64 win, const ModifierState& state, Mouse::Button button);
        MouseButtonDBCLKEvent(const MouseButtonDBCLKEvent& event);

        // Méthode publique
        virtual std::string ToString() const override;
        EVENT_TYPE_FLAGS(EventType::MouseButtonPressed_ev)
    };

    class NKENTSEU_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        // Constructeurs et destructeur
        MouseButtonReleasedEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2& position);
        MouseButtonReleasedEvent(uint64 win, const ModifierState& state, Mouse::Button button);
        MouseButtonReleasedEvent(const MouseButtonReleasedEvent& event);

        // Méthode publique
        virtual std::string ToString() const override;
        EVENT_TYPE_FLAGS(EventType::MouseButtonReleased_ev)
    };

    class NKENTSEU_API MouseButtonRawEvent : public MouseButtonEvent {
    public:
        // Constructeurs et destructeur
        MouseButtonRawEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2i& delta, bool pressed, const Vector2& position);
        MouseButtonRawEvent(uint64 win, const ModifierState& state, Mouse::Button button, const Vector2i& delta, bool pressed);
        MouseButtonRawEvent(const MouseButtonRawEvent& event);

        // Méthodes publiques
        Vector2i GetDelta() const;
        bool IsPressed() const;
        virtual std::string ToString() const override;
        EVENT_TYPE_FLAGS(EventType::MouseButtonRawInput_ev)

    private:
        Vector2i m_Delta;
        bool m_Pressed;
    };

    class NKENTSEU_API MouseMovedEvent : public Event {
    public:
        // Constructeurs et destructeur
        MouseMovedEvent(uint64 win, const Vector2& position, const Vector2& move, const Vector2& positionGlobal);
        MouseMovedEvent(const MouseMovedEvent& event);

        // Méthodes publiques
        Vector2 GetPosition() const;
        Vector2 GetMove() const;
        Vector2 GetPositionGlobal() const;
        virtual std::string ToString() const override;
        virtual bool IsEqual(Event& e) const override;
        EVENT_TYPE_FLAGS(EventType::MouseMoved_ev)
            EVENT_CATEGORY_FLAGS(EventCategory::Mouse_ev | EventCategory::Input_ev)
    private:
        Vector2 m_Position;
        Vector2 m_Move;
        Vector2 m_PositionGlobal;
    };

    class NKENTSEU_API MouseEnteredEvent : public Event {
    public:
        // Constructeurs et destructeur
        MouseEnteredEvent(uint64 win);
        MouseEnteredEvent(const MouseEnteredEvent& event);

        // Méthode publique
        virtual bool IsEqual(Event& e) const override;
        EVENT_TYPE_FLAGS(EventType::MouseEnteredWindow_ev)
            EVENT_CATEGORY_FLAGS(EventCategory::Mouse_ev | EventCategory::Input_ev)
    };

    class NKENTSEU_API MouseExitedEvent : public Event {
    public:
        // Constructeurs et destructeur
        MouseExitedEvent(uint64 win);
        MouseExitedEvent(const MouseExitedEvent& event);

        // Méthode publique
        virtual bool IsEqual(Event& e) const override;
        EVENT_TYPE_FLAGS(EventType::MouseExitedWindow_ev)
            EVENT_CATEGORY_FLAGS(EventCategory::Mouse_ev | EventCategory::Input_ev)
    };
} // namespace nkentseu

#endif    // __NKENTSEU_MOUSE_EVENT_H__
