//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 11:10:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once

#include "System/System.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XCB)


#ifndef __NKENTSEU_WINDOW_EVENT_INTERNAL_H__
#define __NKENTSEU_WINDOW_EVENT_INTERNAL_H__

#pragma once

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_icccm.h>

#include <System/Definitions/Memory.h>

#include "Nkentseu/Event/Event.h"
#include <Ntsm/Vector/Vector2.h>

namespace nkentseu {

    class XCBGenericEvent;

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

        xcb_generic_event_t* genericEvent;

        uint64 PushEvent(xcb_generic_event_t* xcbEvent);
        uint64 FinalizePushEvent(Event* event, uint64 info);

        void TriggerEvent(Event* event);

        // Handle Event 
        uint64 HandleCreateEvent(xcb_generic_event_t* xcbEvent);

        uint64 HandleWindowCreateEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowPaintEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowBackgroundEraseEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowCloseEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowFocusedEvent(xcb_generic_event_t* xcbEvent, uint32 value);
        uint64 HandleWindowResizeEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowNCHITTESTEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowDpiEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowNCCALCSIZEEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowGETMINMAXINFOEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleWindowMoveEvent(xcb_generic_event_t* xcbEvent);

        uint64 HandleMouseWheelEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleMouseButtonEvent(xcb_generic_event_t* xcbEvent, bool isPressed);
        uint64 HandleMouseButtonRawEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleMouseMoveEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleMouseLeaveEvent(xcb_generic_event_t* xcbEvent, bool isLeave);

        uint64 HandleKeyboardEvent(xcb_generic_event_t* xcbEvent, bool isPressed);
        uint64 HandleCharEvent(xcb_generic_event_t* xcbEvent);

        uint64 HandleInputRawEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleInputDeviceChangeEvent(xcb_generic_event_t* xcbEvent);
        uint64 HandleDropFilesEvent(xcb_generic_event_t* xcbEvent);

        uint64 RestricWindowSize(xcb_generic_event_t* xcbEvent);
    };
    #define EventInternal      WindowEventInternal::GetInstance()
} // namespace nkentseu

#endif


#endif 