//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:02:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_FILTER_H__
#define __NKENTSEU_EVENT_FILTER_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector2.h>
#include "Event.h"

#include <System/Definitions/Memory.h>

namespace nkentseu {
    
    class NKENTSEU_API EventFilter {
    public:
        static EventFilter& Instance();

        void AddObserver(EventObserver observer);
        void RemoveObserver(const EventObserver& observer);

        void Pick();
        bool Pick(Event** event);
        //bool Pick(Event* event);
        Event* Picks();
        void TriggerEvent(Event* event);

        Vector2i GetMousePosition() const;
        Vector2i GetMousePositionGlobal() const;
        void SetMousePosition(int32 x, int32 y);
        void SetMousePosition(const Vector2i& pos);

    private:
        bool m_IsPick = false;
    };

    #define EventTrack       EventFilter::Instance()
    
} // namespace nkentseu

#endif    // __NKENTSEU_EVENT_FILTER_H__