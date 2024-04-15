//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 5:34:24 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Event.h"

namespace nkentseu {

    EventType::Code Event::GetEventType() const {
        return EventType::None;
    }

    const char* Event::GetName() const {
        return "Event";
    }

    const char* Event::GetEventTypeStr() const {
        return "Event";
    }

    int32 Event::GetCategoryFlags() const {
        return EventCategory::None;
    }

    bool Event::IsEqual(Event& event) const {
        return (event.GetEventType() == GetEventType());
    };

    std::string Event::ToString() const { return GetName(); }

    bool Event::IsInCategory(EventCategory::Code category) {
        return (GetCategoryFlags() & category);
    }

    uint64 Event::GetWindow() const { return m_WindowID; }

    Event::Event(uint64 windowId) : m_WindowID(windowId) {}
    Event::Event() : m_WindowID(0) {}
    Event::Event(const Event& e) : m_WindowID(e.m_WindowID), m_Handled(e.m_Handled) {}
}    // namespace nkentseu