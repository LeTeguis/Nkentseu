//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 11:10:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventInternal.h"

#ifdef NKENTSEU_PLATFORM_LINUX

#include "Nkentseu/Event/Event.h"
#include "Nkentseu/Event/WindowEvent.h"
#include "Nkentseu/Event/KeyboardEvent.h"
#include "Nkentseu/Event/MouseEvent.h"
#include "Nkentseu/Event/GenericInputEvent.h"

#include "WindowDisplay.h"
#include "Nkentseu/Core/Window.h"
#include "Nkentseu/Platform/PlatformState.h"

#include "Nkentseu/Core/NkentseuLogger.h"

namespace nkentseu {

    WindowEventInternal::WindowEventInternal(){
    }

    WindowEventInternal::~WindowEventInternal(){
    }

    void WindowEventInternal::Update() {
        do {
            genericEvent = xcb_poll_for_event(PlatformState_::Instance().connection);

            if (genericEvent != nullptr) {
                PushEvent(genericEvent);
                Log_nts.Debug();
            } 

        } while (genericEvent != nullptr);
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

    uint64 WindowEventInternal::PushEvent(xcb_generic_event_t* msg) {
        Log_nts.Info("------------");
        switch (XCB_EVENT_RESPONSE_TYPE(msg)) {
            case XCB_CLIENT_MESSAGE: { Log_nts.Info("if begin for Close"); return HandleWindowCloseEvent((xcb_client_message_event_t*)msg); } break;
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

    // Handle Event
    uint64 WindowEventInternal::HandleCreateEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }

    uint64 WindowEventInternal::HandleWindowCreateEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowPaintEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowBackgroundEraseEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowCloseEvent(xcb_client_message_event_t* msg) {
        WindowDisplay* window = WindowDisplay::GetCurrent(msg->window);
        Log_nts.Info("Prepare for Close");
        if (window != nullptr && msg->data.data32[0] == window->screenHandle->root) {
            Log_nts.Info("Close");
            return FinalizePushEvent(new WindowCloseEvent(window->windowSuper->ID()), 0);
        }
        Log_nts.Info("Not Close");
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowFocusedEvent(xcb_generic_event_t msg, WindowDisplay* window, bool focused) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowResizeEvent(xcb_generic_event_t msg, WindowDisplay* window, bool resizing) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowNCHITTESTEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowDpiEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowNCCALCSIZEEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowGETMINMAXINFOEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowMoveEvent(xcb_generic_event_t msg, WindowDisplay* window, uint8 t) {
        return 0;
    }

    uint64 WindowEventInternal::HandleMouseWheelEvent(xcb_generic_event_t msg, WindowDisplay* window, bool vertical) {
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseButtonEvent(xcb_generic_event_t msg, WindowDisplay* window, uint8 btn, bool pressed, bool dbclick) {
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseButtonRawEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseMoveEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseLeaveEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }

    uint64 WindowEventInternal::HandleKeyboardEvent(xcb_generic_event_t msg, WindowDisplay* window, bool keydown) {
        return 0;
    }
    uint64 WindowEventInternal::HandleCharEvent(xcb_generic_event_t msg, WindowDisplay* window, bool interpret) {
        return 0;
    }

    uint64 WindowEventInternal::HandleInputRawEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleInputDeviceChangeEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
    uint64 WindowEventInternal::HandleDropFilesEvent(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }

    uint64 WindowEventInternal::RestricWindowSize(xcb_generic_event_t msg, WindowDisplay* window) {
        return 0;
    }
}    // namespace nkentseu

#endif 