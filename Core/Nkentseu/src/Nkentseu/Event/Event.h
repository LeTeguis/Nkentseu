//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 5:34:24 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_H__
#define __NKENTSEU_EVENT_H__

#pragma once

// Include necessary headers
#include "System/System.h"
#include <System/Nature/Base.h>
#include "EventCategory.h"
#include "EventType.h"
#include "EventState.h"

#include <functional>

namespace nkentseu {

// ** Macros for Event Handling **

// ** EVENT_BIND_HANDLER(method_)**
// This macro creates an event handler function using `std::bind`. It binds the provided method_ to the current 
// Event object (this) and the first argument passed to the handler function (STDPH(1)).
#define EVENT_BIND_HANDLER(method_) std::bind(&method_, this, STDPH(1))

// ** EVENT_STATIC_TYPE(type)**
// This macro defines a static method named `GetStaticType` within an event class. 
// The method returns the static EventType corresponding to the provided event type (e.g., EventType::WindowCreated).
#define EVENT_STATIC_TYPE(type) static EventType::Code GetStaticType() { return EventType::type; }

// ** EVENT_TYPE_FLAGS(type)**
// This macro defines several methods within an event class:
//   - `GetStaticType`: Same as `EVENT_STATIC_TYPE`.
//   - `GetEventType`: Returns the event type using the `GetStaticType` method (virtual override).
//   - `GetEventTypeStr`: Returns a string representation of the event type using the preprocessor directive (#type). (virtual override)
//   - `GetName`: Same as `GetEventTypeStr`. (virtual override)
#define EVENT_TYPE_FLAGS(type) \
    static EventType::Code GetStaticType() { return EventType::type; } \
    virtual EventType::Code GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetEventTypeStr() const override { return #type; } \
    virtual const char* GetName() const override { return #type; } 

  // ** EVENT_CATEGORY_FLAGS(category)**
  // This macro defines a virtual method named `GetCategoryFlags` within an event class. 
  // The method returns the event category flags provided as an argument (category). (virtual override)
#define EVENT_CATEGORY_FLAGS(category) virtual int32 GetCategoryFlags() const override { return category; }


// ** Event Class **

    class NKENTSEU_API Event {
    public:
        // ** Virtual methods for event information retrieval **
        virtual EventType::Code GetEventType() const; // Returns the event type code. (virtual override)
        virtual const char* GetName() const;         // Returns the event name (same as event type string). (virtual override)
        virtual const char* GetEventTypeStr() const;  // Returns a string representation of the event type. (virtual override)
        virtual int32 GetCategoryFlags() const;       // Returns the event category flags. (virtual override)

        virtual std::string ToString() const;

        friend std::string ToString(const Event& v) {
            return v.ToString();
        }

        // Other virtual methods (not shown for brevity)
        virtual bool IsEqual(Event& event) const;

        // Helper method to check if event belongs to a specific category
        inline bool IsInCategory(EventCategory::Code category);

        // Flag indicating if the event has been handled
        bool handled = false;

        // Friend function for streaming Event objects to ostream (e.g., std::cout)
        friend std::ostream& operator<<(std::ostream& os, const Event& e) {
            return os << e.ToString();
        }

        // Template method for type checking events
        template<typename T>
        bool IsA() {
            return GetEventType() == T::GetStaticType();
        }

        // Template method for casting Event to specific event type
        template<typename T>
        T& GetProperties() {
            return static_cast<T&>(*this);
        }

        // Methods related to window association (not shown for brevity)
        uint64 GetWindow() const;

        Event(uint64 windowId);
        Event();
        Event(const Event& e);

    protected:
        uint64 m_WindowID = 0; // Window ID associated with the event (protected member)
    };

    using EventObserver = std::function<void(Event&)>;
} // namespace nkentseu

#endif  // __NKENTSEU_EVENT_H__
