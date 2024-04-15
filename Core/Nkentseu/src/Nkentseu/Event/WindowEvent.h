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

namespace nkentseu {
    class NKENTSEU_API WindowCreateEvent : public Event
    {
    public:
        WindowCreateEvent(uint64 win);
        WindowCreateEvent(const WindowCreateEvent& e);
        virtual bool IsEqual(Event& e) const override;

        EVENT_TYPE_FLAGS(EventType::WindowCreated)
            EVENT_CATEGORY_FLAGS(EventCategory::Window)
    };

    class NKENTSEU_API WindowPaintEvent : public Event
    {
    public:
        WindowPaintEvent(uint64 win, const Rectangle& rect, const Color& color);
        WindowPaintEvent(const WindowPaintEvent& e);

        Rectangle GetRect();
        Color GetColor();

        virtual bool IsEqual(Event& e) const override;

        EVENT_TYPE_FLAGS(EventType::WindowPainted)
            EVENT_CATEGORY_FLAGS(EventCategory::Window | EventCategory::Graphics)
    private:
        Rectangle m_Rect;
        Color m_Color;
    };

    class NKENTSEU_API WindowBackgroundEraseEvent : public Event
    {
    public:
        WindowBackgroundEraseEvent(uint64 win, const Rectangle& rect, const Color& color);
        WindowBackgroundEraseEvent(const WindowBackgroundEraseEvent& e);

        Rectangle GetRect();
        Color GetColor();

        virtual bool IsEqual(Event& e) const override;

        EVENT_TYPE_FLAGS(EventType::WindowBackgroundErased)
            EVENT_CATEGORY_FLAGS(EventCategory::Window | EventCategory::Graphics)
    private:
        Rectangle m_Rect;
        Color m_Color;
    };

    class NKENTSEU_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent(uint64 win);
        WindowCloseEvent(const WindowCloseEvent& e);

        virtual bool IsEqual(Event& e) const override;

        EVENT_TYPE_FLAGS(EventType::WindowClosed)
            EVENT_CATEGORY_FLAGS(EventCategory::Window)
    };

    class NKENTSEU_API WindowFocusEvent : public Event {
    public:
        WindowFocusEvent(uint64 win, bool focus);
        WindowFocusEvent(const WindowFocusEvent& e);

        bool HasFocus();

        virtual bool IsEqual(Event& e) const override;

        virtual std::string ToString() const override;

        EVENT_TYPE_FLAGS(EventType::WindowFocused)
            EVENT_CATEGORY_FLAGS(EventCategory::Window)
    private:
        bool m_Focus;
    };

    class NKENTSEU_API WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint64 win, const Vector2u& size, bool resizing);
        WindowResizeEvent(const WindowResizeEvent& e);

        Vector2u GetSize();
        bool IsResizing();

        virtual std::string ToString() const override;

        virtual bool IsEqual(Event& e) const override;

        EVENT_TYPE_FLAGS(EventType::WindowResized)
            EVENT_CATEGORY_FLAGS(EventCategory::Window | EventCategory::Graphics)
    private:
        Vector2u m_Size;
        bool m_Resizing;
    };

    class NKENTSEU_API WindowDpiEvent : public Event {
    public:
        WindowDpiEvent(uint64 win, float32 dpi);
        WindowDpiEvent(const WindowDpiEvent& e);

        float32 GetDpi();

        virtual bool IsEqual(Event& e) const override;

        virtual std::string ToString() const override;

        EVENT_TYPE_FLAGS(EventType::WindowDpiChanged)
            EVENT_CATEGORY_FLAGS(EventCategory::Window)
    private:
        float32 m_Dpi;
    };

    class NKENTSEU_API WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent(uint64 win, const Vector2i& position);
        WindowMovedEvent(const WindowMovedEvent& e);

        Vector2i GetPosition();

        virtual std::string ToString() const override;

        virtual bool IsEqual(Event& e) const override;

        EVENT_TYPE_FLAGS(EventType::WindowMoved)
            EVENT_CATEGORY_FLAGS(EventCategory::Window)
    private:
        Vector2i m_Position;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_WINDOW_EVENT_H__
