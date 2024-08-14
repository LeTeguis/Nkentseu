//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 10:30:13 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_APPEVENT_H__
#define __NKENTSEU_APPEVENT_H__

#pragma once

#include "NTSCore/System.h"
#include "Event.h"

namespace nkentseu {
    class NKENTSEU_API AppTickEvent : public Event
    {
    public:
        AppTickEvent(uint64 win) : Event(win) {}

        virtual bool IsEqual(Event& e) const override {
            return (e.GetEventType() == GetEventType());
        }

        EVENT_TYPE_FLAGS(EventType::ApplicationTick)
            EVENT_CATEGORY_FLAGS(EventCategory::Application)
    };

    class NKENTSEU_API AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent(uint64 win) : Event(win) {}

        virtual bool IsEqual(Event& e) const override {
            return (e.GetEventType() == GetEventType());
        }

        EVENT_TYPE_FLAGS(EventType::ApplicationUpdate)
            EVENT_CATEGORY_FLAGS(EventCategory::Application)
    };

    class NKENTSEU_API AppRenderEvent : public Event
    {
    public:
        AppRenderEvent(uint64 win) : Event(win) {}

        virtual bool IsEqual(Event& e) const override {
            return (e.GetEventType() == GetEventType());
        }

        EVENT_TYPE_FLAGS(EventType::ApplicationRender)
        EVENT_CATEGORY_FLAGS(EventCategory::Application)
    };
} // namespace nkentseu

#endif    // __NKENTSEU_APPEVENT_H__