//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:03:21 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "WindowEvent.h"
#include <NTSLogger/Formatter.h>

namespace nkentseu {
    using namespace maths;

    WindowStatusEvent::WindowStatusEvent(uint64 win, WindowState::Code state, const WindowProperties& properties) : Event(win), m_State(state), m_Properties(properties) {}

    WindowStatusEvent::WindowStatusEvent(const WindowStatusEvent& e) : Event(e), m_State(e.m_State), m_Properties(e.m_Properties) {}

    std::string WindowStatusEvent::ToString() const {
        return FORMATTER.Format("WindowStatusEvent {Window ID : {0}, State : {1}}", m_WindowID, WindowState::ToString(m_State));
    }

    WindowState::Code WindowStatusEvent::GetState() const{
        return m_State;
    }

    WindowProperties WindowStatusEvent::GetProperties() const{
        return m_Properties;
    }

    WindowRenderedEvent::WindowRenderedEvent(uint64 win, Rectangle rectangle, Color color)
        : Event(win), m_Rectangle(rectangle), m_Color(color) {}

    WindowRenderedEvent::WindowRenderedEvent(const WindowRenderedEvent& e) : Event(e), m_Rectangle(e.m_Rectangle), m_Color(e.m_Color) {}

    std::string WindowRenderedEvent::ToString() const {
        return FORMATTER.Format("WindowRenderedEvent {Window ID : {0}, Rectangle : {1}, Color: {2}}", m_WindowID, m_Rectangle, m_Color);
    }

    Rectangle WindowRenderedEvent::GetRenderRec() const{
        return m_Rectangle;
    }

    Color WindowRenderedEvent::GetRenderColor() const{
        return m_Color;
    }

    WindowBackgroundErasedEvent::WindowBackgroundErasedEvent(uint64 win, Rectangle rectangle, Color color)
        : Event(win), m_Rectangle(rectangle), m_Color(color) {}

    WindowBackgroundErasedEvent::WindowBackgroundErasedEvent(const WindowBackgroundErasedEvent& e)
        : Event(e), m_Rectangle(e.m_Rectangle), m_Color(e.m_Color) {}

    std::string WindowBackgroundErasedEvent::ToString() const {
        return FORMATTER.Format("WindowBackgroundErasedEvent {Window ID : {0}, Rectangle : {1}, Color: {2}}", m_WindowID, m_Rectangle, m_Color);
    }

    Rectangle WindowBackgroundErasedEvent::GetBackgroundRec() const{
        return m_Rectangle;
    }

    Color WindowBackgroundErasedEvent::GetBackgroundColor() const{
        return m_Color;
    }

    WindowResizedEvent::WindowResizedEvent(uint64 win, ResizeState::Code state, const Vector2u& size)
        : Event(win), m_Size(size), m_State(state) {}

    WindowResizedEvent::WindowResizedEvent(const WindowResizedEvent& e) : Event(e), m_Size(e.m_Size), m_State(e.m_State) {}

    std::string WindowResizedEvent::ToString() const {
        return FORMATTER.Format("WindowResizedEvent {Window ID : {0}, Resize State : {2}, size: {1}}", m_WindowID, m_Size, ResizeState::ToString(m_State));
    }

    Vector2u WindowResizedEvent::GetSize() const{
        return m_Size;
    }

    ResizeState::Code WindowResizedEvent::GetRisizeState() const{
        return m_State;
    }

    WindowFocusedEvent::WindowFocusedEvent(uint64 win, FocusState::Code state) : Event(win), m_State(state) {}

    WindowFocusedEvent::WindowFocusedEvent(const WindowFocusedEvent& e) : Event(e), m_State(e.m_State) {}
    
    std::string WindowFocusedEvent::ToString() const {
        return FORMATTER.Format("WindowFocusedEvent {Window ID : {0}, FocusState : {1}}", m_WindowID, FocusState::ToString(m_State));
    }

    FocusState::Code WindowFocusedEvent::GetFocusState() const{
        return m_State;
    }

    WindowDpiChangedEvent::WindowDpiChangedEvent(uint64 win, float32 dpi) : Event(win), m_Dpi(dpi) {}

    WindowDpiChangedEvent::WindowDpiChangedEvent(const WindowDpiChangedEvent& e) : Event(e), m_Dpi(e.m_Dpi) {}

    std::string WindowDpiChangedEvent::ToString() const {
        return FORMATTER.Format("WindowDpiChangedEvent {Window ID : {0}, DPI : {1}}", m_WindowID, m_Dpi);
    }

    float32 WindowDpiChangedEvent::GetDpi() const{
        return m_Dpi;
    }

    WindowMovedEvent::WindowMovedEvent(uint64 win, Vector2i position, Vector2i lastPosition) : Event(win), m_Position(position), m_LastPosition(lastPosition) {}

    WindowMovedEvent::WindowMovedEvent(const WindowMovedEvent& e)
        : Event(e), m_Position(e.m_Position), m_LastPosition(e.m_LastPosition) {}

    std::string WindowMovedEvent::ToString() const{
        return FORMATTER.Format("WindowMovedEvent {Window ID : {0}, Position : {1}, Last Position : {2}}", m_WindowID, m_Position, m_LastPosition);
    }

    Vector2i WindowMovedEvent::GetPosition() const{
        return m_Position;
    }

    Vector2i WindowMovedEvent::GetLastPosition() const{
        return m_LastPosition;
    }

    WindowVisibleEvent::WindowVisibleEvent(uint64 win, bool visible) : Event(win), m_Visible(visible) {
    }

    WindowVisibleEvent::WindowVisibleEvent(const WindowVisibleEvent& e) : Event(e), m_Visible(e.m_Visible){
    }

    std::string WindowVisibleEvent::ToString() const{
        return FORMATTER.Format("WindowVisibleEvent {Window ID : {0}, Is Visible : {1}}", m_WindowID, STR_BOOL(m_Visible));
    }
    
    bool WindowVisibleEvent::IsVisible() const { return m_Visible; }
}   // namespace nkentseu