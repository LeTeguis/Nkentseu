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
#include <X11/extensions/Xrandr.h>

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
        float64 dpi = 0.0;

        uint64 PushEvent(XEvent xev);
        uint64 FinalizePushEvent(Event* event, uint64 info);

        uint64 HandleKeyboard(XKeyEvent event, bool isPressed);
        uint64 HandleMouseButton(XButtonEvent event, bool isPressed);
        uint64 HandleMotionNotify(XMotionEvent event);
        uint64 HandleEnterLeaveEvent(XCrossingEvent event, bool isEnter);
        uint64 HandleClientMessage(XClientMessageEvent event);
        uint64 HandleFocusEvent(XFocusChangeEvent event, bool isFocusIn);
        uint64 HandleExposeEvent(XExposeEvent event);
        uint64 HandleGraphicsExposeEvent(XGraphicsExposeEvent event);
        uint64 HandleConfigureEvent(XConfigureEvent event) ;
        uint64 HandleVisibilityEvent(XVisibilityEvent event);
        uint64 HandleResizeEvent(XResizeRequestEvent event);

        float64 CalculateDPI(int32 width_pixels, int32 height_pixels, float64 width_inches, float64 height_inches);
        float64 GetCurrentDpi(Display* display);

        void TriggerEvent(Event* event);
    };
    #define EventInternal      WindowEventInternal::GetInstance()
} // namespace nkentseu

#endif


#endif 