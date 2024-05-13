//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 11:10:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "System/System.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XLIB)


#ifndef __NKENTSEU_WINDOW_EVENT_INTERNAL_H__
#define __NKENTSEU_WINDOW_EVENT_INTERNAL_H__

#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <System/Definitions/Memory.h>

#include "Nkentseu/Event/Event.h"
#include <Ntsm/Vector/Vector2.h>

namespace nkentseu {

    class NKENTSEU_API WindowEventInternal {
    public:
        // friend class WindowInternal;

        static WindowEventInternal& GetInstance();

        // Méthodes

        WindowEventInternal();
        ~WindowEventInternal();

        void Update();

        Event* Front();
        Event& FrontReference();

        void Pop();

        bool Empty();

        size_t Size();

        // Variables membres
        bool isInitialized;
        bool isQueueLocked;
        std::vector<Event*> eventQueue;
        std::vector<EventObserver> eventObservers;
        Vector2i mousePosition;
        Vector2i globalMousePosition;
        Vector2i previousMousePosition;

        uint64 PushEvent(XEvent xev);
        uint64 FinalizePushEvent(Event* event, uint64 info);

        void TriggerEvent(Event* event);
    };
    #define EventInternal      WindowEventInternal::GetInstance()
} // namespace nkentseu

#endif


#endif 