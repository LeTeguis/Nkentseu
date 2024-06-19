//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:03:21 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_WINDOW_EVENT_H__
#define __NKENTSEU_WINDOW_EVENT_H__

#pragma once

#include "System/System.h"
#include "Event.h"

#include <Nkentseu/Graphics/Color.h>
#include <Ntsm/Shapes/Rectangle.h>
#include <Nkentseu/Core/WindowProperties.h>

namespace nkentseu {
    class NKENTSEU_API WindowStatusEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowStatus)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowStatusEvent(uint64 win, WindowState::Code state, const WindowProperties& properties = {});
        WindowStatusEvent(const WindowStatusEvent& e);
        virtual std::string ToString() const override;

        WindowState::Code GetState() const;
        WindowProperties GetProperties() const;

    private:
        WindowState::Code m_State;
        WindowProperties m_Properties;
    };

    class NKENTSEU_API WindowRenderedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowRendered)
        EVENT_CATEGORY_FLAGS(EventCategory::Window) 
    public:
        WindowRenderedEvent(uint64 win, maths::Rectangle rectangle, Color color);
        WindowRenderedEvent(const WindowRenderedEvent& e);
        virtual std::string ToString() const override;  
        
        maths::Rectangle GetRenderRec() const;
        Color GetRenderColor() const;
    private:
        maths::Rectangle m_Rectangle;
        Color m_Color;
    };

    class NKENTSEU_API WindowBackgroundErasedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowBackgroundErased)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowBackgroundErasedEvent(uint64 win, maths::Rectangle rectangle, Color color);
        WindowBackgroundErasedEvent(const WindowBackgroundErasedEvent& e);
        virtual std::string ToString() const override;

        maths::Rectangle GetBackgroundRec() const;
        Color GetBackgroundColor() const;
    private:
        maths::Rectangle m_Rectangle;
        Color m_Color;
    };

    class NKENTSEU_API WindowResizedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowResized)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowResizedEvent(uint64 win, ResizeState::Code state, const maths::Vector2u& size = {});
        WindowResizedEvent(const WindowResizedEvent& e);
        virtual std::string ToString() const override;

        maths::Vector2u GetSize() const;
        ResizeState::Code GetRisizeState() const;
    private:
        maths::Vector2u m_Size;
        ResizeState::Code m_State;
    };

    class NKENTSEU_API WindowFocusedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowFocused)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowFocusedEvent(uint64 win, FocusState::Code state);
        WindowFocusedEvent(const WindowFocusedEvent& e);
        virtual std::string ToString() const override;

        FocusState::Code GetFocusState() const;
    private:
        FocusState::Code m_State;
    };

    class NKENTSEU_API WindowDpiChangedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowDpiChanged)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowDpiChangedEvent(uint64 win, float32 dpi);
        WindowDpiChangedEvent(const WindowDpiChangedEvent& e);
        virtual std::string ToString() const override;

        float32 GetDpi() const;
    private:
        float32 m_Dpi;
    };

    class NKENTSEU_API WindowMovedEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowMoved)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowMovedEvent(uint64 win, maths::Vector2i position, maths::Vector2i lastPosition);
        WindowMovedEvent(const WindowMovedEvent& e);
        virtual std::string ToString() const override;
        
        maths::Vector2i GetPosition() const;
        maths::Vector2i GetLastPosition() const;
    private:
        maths::Vector2i m_Position;
        maths::Vector2i m_LastPosition;
    };

    class NKENTSEU_API WindowVisibleEvent : public Event
    {
    public:
        EVENT_TYPE_FLAGS(EventType::WindowVisible)
        EVENT_CATEGORY_FLAGS(EventCategory::Window)
    public:
        WindowVisibleEvent(uint64 win, bool visible);
        WindowVisibleEvent(const WindowVisibleEvent& e);
        virtual std::string ToString() const override;
        
        bool IsVisible() const;
    private:
        bool m_Visible;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_EVENT_H__
