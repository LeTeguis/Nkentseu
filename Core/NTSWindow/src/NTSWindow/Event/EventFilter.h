//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:02:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_FILTER_H__
#define __NKENTSEU_EVENT_FILTER_H__

#pragma once

#include "NTSCore/System.h"
#include <NTSMaths/Vector/Vector2.h>
#include "Event.h"

namespace nkentseu {

    class NKENTSEU_API EventListener {
    public:
        static EventListener& Instance();

        virtual void AddObserver(EventObserver observer) = 0;

        virtual void RemoveObserver(const EventObserver& observer) = 0;

        virtual std::vector<Event*> ProcessQueue() = 0;
    };

    #define EventTraker           EventListener::Instance()
    
} // namespace nkentseu

#endif    // __NKENTSEU_EVENT_FILTER_H__