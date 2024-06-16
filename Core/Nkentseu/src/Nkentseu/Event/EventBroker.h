//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 4:34:34 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_BROKER_H__
#define __NKENTSEU_EVENT_BROKER_H__

#pragma once

#include "System/System.h"
#include "Event.h"

namespace nkentseu {
    template<typename T>
    using ClientEvent = std::function<bool(T&)>;

    #define REGISTER_CLIENT_EVENT(method_) std::bind(&method_, this, STDPH(1))

    // **EventBroker class:**
    // This class acts as a central hub for routing events to interested listeners.
    // It receives an Event object in its constructor and provides a method
    // to dispatch the event to appropriate listeners based on the event type.
    class NKENTSEU_API EventBroker {
    public:
        // **Constructor:**
        // Takes a reference to an Event object.
        //EventBroker(Event& event) : m_Event(event) {}
        EventBroker(Event& event) : m_Event(event) {}

        // **RouteEvent method:**
        // This method takes a template parameter `T` representing the specific event type
        // and a callback function that handles that event type. It checks if the received
        // Event object matches the type `T` and calls the provided callback function
        // with a reference to the event data if there's a match. The method returns true
        // if the event was successfully routed and handled, false otherwise.
        template<typename T>
        bool Route(ClientEvent<T> func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;  // Reference to the managed Event object.
    };
} // namespace nkentseu

#endif  // __NKENTSEU_EVENT_BROKER_H__
