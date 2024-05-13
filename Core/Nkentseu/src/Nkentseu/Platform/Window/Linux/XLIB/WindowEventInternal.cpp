//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 11:10:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventInternal.h"

#if defined(NKENTSEU_PLATFORM_LINUX_XLIB)

#include "Nkentseu/Event/Event.h"
#include "Nkentseu/Event/WindowEvent.h"
#include "Nkentseu/Event/KeyboardEvent.h"
#include "Nkentseu/Event/MouseEvent.h"
#include "Nkentseu/Event/GenericInputEvent.h"

#include "Nkentseu/Event/InputManager.h"
#include "WindowEventCode.h"

#include "WindowInternal.h"
#include "Nkentseu/Core/Window.h"
#include "Nkentseu/Platform/PlatformState.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#include <xcb/xcb_keysyms.h>
#include "WindowEventInternal.h"

namespace nkentseu {

    WindowEventInternal& WindowEventInternal::GetInstance() {
        static WindowEventInternal eventManager;
        return eventManager;
    }

    WindowEventInternal::WindowEventInternal(){
    }

    WindowEventInternal::~WindowEventInternal(){
    }

    void WindowEventInternal::Update() {
        while (XPending(PlatformState.display)) {
            XEvent xevent;
            XNextEvent(PlatformState.display, &xevent);
            PushEvent(xevent);
        }
        Input.private__update__axis();
    }

    Event* WindowEventInternal::Front() {
        return eventQueue.front();
    }

    void WindowEventInternal::Pop() {
        if (eventQueue.empty()) return;

        Event* event = eventQueue.front();
        eventQueue.erase(eventQueue.begin());

        delete event;
    }

    bool WindowEventInternal::Empty() {
        return eventQueue.empty();
    }

    size_t WindowEventInternal::Size() {
        return eventQueue.size();
    }

    uint64 WindowEventInternal::PushEvent(XEvent event) {
        switch(event.type) {
            case ClientMessage:
                if(event.xclient.data.l[0] == PlatformState.protocolAtom.deleteWindow) {
                    WindowInternal* window = WindowInternal::GetCurrent(event.xclient.window);

                    if (window != nullptr){
                        return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Closed, window->GetProperties()), 0);
                    }
                    return 0;
                }
                break;
        }
        return 0;
    }

    uint64 WindowEventInternal::FinalizePushEvent(Event* event, uint64 info) {
        if (event->GetEventType() != EventType::NotDefine) {
            if (!isQueueLocked) {
                eventQueue.push_back(event);
            }

            for (auto& eventCallback : eventObservers) {
                if (eventCallback != nullptr) {
                    eventCallback(*event);
                }
            }
        }
        return info;
    }

    void WindowEventInternal::TriggerEvent(Event* event) {
        return;
    }

    Event& WindowEventInternal::FrontReference()
    {
        return *eventQueue.front();
    }
}    // namespace nkentseu

#endif 