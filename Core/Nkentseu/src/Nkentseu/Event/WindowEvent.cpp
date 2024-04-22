//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:03:21 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEvent.h"

namespace nkentseu {
    
    WindowCreateEvent::WindowCreateEvent(uint64 win) : Event(win) {}
    WindowCreateEvent::WindowCreateEvent(const WindowCreateEvent& e) : Event(e.GetWindow()) {}

    bool WindowCreateEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType() && e.GetCategoryFlags() == GetCategoryFlags());
    }
    WindowPaintEvent::WindowPaintEvent(uint64 win, const Rectangle& rect, const Color& color) : Event(win), m_Rect(rect), m_Color(color) {}
    WindowPaintEvent::WindowPaintEvent(const WindowPaintEvent& e) : Event(e.GetWindow()), m_Rect(e.m_Rect), m_Color(e.m_Color) {}

    Rectangle WindowPaintEvent::GetRect() { return m_Rect; }
    Color WindowPaintEvent::GetColor() { return m_Color; }

    bool WindowPaintEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType() && e.GetCategoryFlags() == GetCategoryFlags());
    }

    WindowBackgroundEraseEvent::WindowBackgroundEraseEvent(uint64 win, const Rectangle& rect, const Color& color) : Event(win), m_Rect(rect), m_Color(color) {}
    WindowBackgroundEraseEvent::WindowBackgroundEraseEvent(const WindowBackgroundEraseEvent& e) : Event(e.GetWindow()), m_Rect(e.m_Rect), m_Color(e.m_Color) {}

    Rectangle WindowBackgroundEraseEvent::GetRect() { return m_Rect; }
    Color WindowBackgroundEraseEvent::GetColor() { return m_Color; }

    bool WindowBackgroundEraseEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType() && e.GetCategoryFlags() == GetCategoryFlags());
    }

    WindowCloseEvent::WindowCloseEvent(uint64 win) : Event(win) {}
    WindowCloseEvent::WindowCloseEvent(const WindowCloseEvent& e) : Event(e.GetWindow()) {}

    bool WindowCloseEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType() && e.GetCategoryFlags() == GetCategoryFlags());
    }

    WindowFocusEvent::WindowFocusEvent(uint64 win, bool focus, const Rectangle& exposure) : Event(win), m_Focus(focus), m_Exposure(exposure) {}
    WindowFocusEvent::WindowFocusEvent(const WindowFocusEvent& e) : Event(e.GetWindow()), m_Focus(e.m_Focus), m_Exposure(e.m_Exposure) {}

    bool WindowFocusEvent::HasFocus() { return m_Focus; }

    Rectangle WindowFocusEvent::GetExposureRegion(){
        return m_Exposure;
    }

    bool WindowFocusEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType());
    }

    std::string WindowFocusEvent::ToString() const {
        std::stringstream ss;
        ss << "WindowFocusEvent: " << ((m_Focus) ? "True" : "False");
        return ss.str();
    }

    WindowResizeEvent::WindowResizeEvent(uint64 win, const Vector2u& size, bool resizing) : Event(win), m_Size(size), m_Resizing(resizing) {}
    WindowResizeEvent::WindowResizeEvent(const WindowResizeEvent& e) : Event(e.GetWindow()), m_Resizing(e.m_Resizing), m_Size(e.m_Size) {}

    Vector2u WindowResizeEvent::GetSize() { return m_Size; }
    bool WindowResizeEvent::IsResizing() { return m_Resizing; }

    std::string WindowResizeEvent::ToString() const {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Size.width << "x" << m_Size.height << ((m_Resizing) ? "; True" : "; False");
        return ss.str();
    }

    bool WindowResizeEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType());
    }

    WindowDpiEvent::WindowDpiEvent(uint64 win, float32 dpi) : Event(win), m_Dpi(dpi) {}
    WindowDpiEvent::WindowDpiEvent(const WindowDpiEvent& e) : Event(e.GetWindow()), m_Dpi(e.m_Dpi) {}

    float32 WindowDpiEvent::GetDpi() { return m_Dpi; }

    bool WindowDpiEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType() && e.GetCategoryFlags() == GetCategoryFlags());
    }

    std::string WindowDpiEvent::ToString() const {
        std::stringstream ss;
        ss << "WindowDpiEvent: " << m_Dpi;
        return ss.str();
    }

    WindowMovedEvent::WindowMovedEvent(uint64 win, const Vector2i& position) : Event(win), m_Position(position) {}
    WindowMovedEvent::WindowMovedEvent(const WindowMovedEvent& e) : Event(e.GetWindow()), m_Position(e.m_Position) {}

    Vector2i WindowMovedEvent::GetPosition() { return m_Position; }

    std::string WindowMovedEvent::ToString() const {
        std::stringstream ss;
        ss << "WindowMovedEvent: " << m_Position.x << ", " << m_Position.y;
        return ss.str();
    }

    bool WindowMovedEvent::IsEqual(Event& e) const {
        return (e.GetEventType() == GetEventType());
    }
    
}   // namespace nkentseu