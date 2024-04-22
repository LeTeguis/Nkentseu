//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 11:10:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "WindowEventInternal.h"

#ifdef NKENTSEU_PLATFORM_LINUX


#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_icccm.h>


#include "Nkentseu/Event/Event.h"
#include "Nkentseu/Event/WindowEvent.h"
#include "Nkentseu/Event/KeyboardEvent.h"
#include "Nkentseu/Event/MouseEvent.h"
#include "Nkentseu/Event/GenericInputEvent.h"

#include "Nkentseu/Event/InputManager.h"

#include "WindowInternal.h"
#include "Nkentseu/Core/Window.h"
#include "Nkentseu/Platform/PlatformState.h"

#include "Nkentseu/Core/NkentseuLogger.h"

namespace nkentseu {

    static xcb_generic_event_t* genericEvent;

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

    uint64 WindowEventInternal::PushEvent(xcb_generic_event_t* msg) {
        switch (XCB_EVENT_RESPONSE_TYPE(msg)) {
            //case XCB_CLIENT_MESSAGE: { return HandleWindowCloseEvent(msg); } break;
            case XCB_DESTROY_NOTIFY: {HandleWindowFocusedEvent(msg, 1);} break;
            case XCB_CREATE_NOTIFY: {HandleWindowFocusedEvent(msg, 1);} break;
            case XCB_VISIBILITY_NOTIFY: {HandleWindowFocusedEvent(msg, 1);} break;
            case XCB_FOCUS_IN: {HandleWindowFocusedEvent(msg, 1);} break;
            case XCB_FOCUS_OUT: {HandleWindowFocusedEvent(msg, 2);} break;
            case XCB_EXPOSE: {
                    Log_nts.Trace();
                    HandleWindowFocusedEvent(msg, 0);
                } 
                break;
            case XCB_BUTTON_PRESS: {Log_nts.Trace();HandleMouseButtonEvent(msg, true);} break;
            case XCB_BUTTON_RELEASE: {Log_nts.Trace();HandleMouseButtonEvent(msg, false);} break;
            case XCB_MOTION_NOTIFY: {Log_nts.Trace();HandleMouseMoveEvent(msg);} break;
            case XCB_ENTER_NOTIFY: {Log_nts.Trace();HandleMouseLeaveEvent(msg, false);} break;
            case XCB_LEAVE_NOTIFY: {Log_nts.Trace();HandleMouseLeaveEvent(msg, true);} break;
            case XCB_KEY_PRESS: {Log_nts.Trace();HandleKeyboardEvent(msg, true);} break;
            case XCB_KEY_RELEASE: {Log_nts.Trace();HandleKeyboardEvent(msg, false);} break;
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
    uint64 WindowEventInternal::HandleCreateEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }

    uint64 WindowEventInternal::HandleWindowCreateEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowPaintEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowBackgroundEraseEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }// xcb_generic_event_t* msg
    uint64 WindowEventInternal::HandleWindowCloseEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);

        Log_nts.Info("Prepare for Close");
        if (window != nullptr && event->data.data32[0] == window->GetWindowDisplay()->screenHandle->root) {

            Log_nts.Info("Close");
            return FinalizePushEvent(new WindowCloseEvent(window->ID()), 0);
        }
        Log_nts.Info("Not Close");
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowFocusedEvent(xcb_generic_event_t* msg, uint32 value) {
        if (value == 0){
            xcb_expose_event_t* event = (xcb_expose_event_t*)msg;
            WindowInternal* window = WindowInternal::GetCurrent(event->window);
            Log_nts.Trace();
            if (window != nullptr && event != nullptr){
                Log_nts.Trace();
                return FinalizePushEvent(new WindowFocusEvent((*window).ID(), true, Rectangle((*event).x, (*event).y, (*event).width, (*event).height)), 0);
            }
        } else if (value == 1 || value == 2){
            auto event = (value == 1) ? (xcb_focus_in_event_t*)msg : (xcb_focus_out_event_t*)msg;
            WindowInternal* window = WindowInternal::GetCurrent(event->event);
            Log_nts.Trace();
            if (window != nullptr && event != nullptr){
                Log_nts.Trace();
                return FinalizePushEvent(new WindowFocusEvent((*window).ID(), true, Rectangle()), 0);
            }
        }

        return 0;
    }
    uint64 WindowEventInternal::HandleWindowResizeEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowNCHITTESTEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowDpiEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowNCCALCSIZEEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowGETMINMAXINFOEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowMoveEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }

    uint64 WindowEventInternal::HandleMouseWheelEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseButtonEvent(xcb_generic_event_t* msg, bool isPressed) {
        auto event = (isPressed) ? (xcb_button_press_event_t*)msg : (xcb_button_release_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            uint32 mask = (*event).state;
            ModifierState modif(mask & 1, mask & 2, mask & 3, mask & 4);
            Vector2f position((*event).event_x, (*event).event_y);

            Mouse::Code button = Mouse::NotDefine;

            if ((*event).detail == 1) button = Mouse::Left;
            else if ((*event).detail == 2) button = Mouse::Middle;
            else if ((*event).detail == 3) button = Mouse::Right;
            else if ((*event).detail == 8) button = Mouse::X1;
            else if ((*event).detail == 9) button = Mouse::X2;

            // <xcb/xcb.h>
            if (button == Mouse::NotDefine && ((*event).detail >= 4 || (*event).detail <= 7)){
                button = ((*event).detail == 4 || (*event).detail == 5) ? Mouse::Vertical : Mouse::Horizontal;
                return FinalizePushEvent(new MouseWheelEvent((*window).ID(), button, 0.0f, position, modif), 0);
            }

            if (isPressed){
                return FinalizePushEvent(new MouseButtonPressedEvent((*window).ID(), modif, button, position), 0);
            } else {
                return FinalizePushEvent(new MouseButtonReleasedEvent((*window).ID(), modif, button, position), 0);
            }
        }

        return 0;
    }
    uint64 WindowEventInternal::HandleMouseButtonRawEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseMoveEvent(xcb_generic_event_t* msg) {
        xcb_motion_notify_event_t* event = (xcb_motion_notify_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            static bool isFirst = true;

            globalMousePosition = Vector2i((*event).event_x, (*event).event_y);
            Vector2i position(globalMousePosition);
            Vector2i move = globalMousePosition - previousMousePosition;
            previousMousePosition = globalMousePosition;

            mousePosition = position;

            if (isFirst){
                isFirst = false;
                return 0;
            }

            return FinalizePushEvent(new MouseMovedEvent((*window).ID(), position, move, globalMousePosition), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleMouseLeaveEvent(xcb_generic_event_t* msg, bool isLeave) {
        auto event = (isLeave) ? (xcb_leave_notify_event_t*)msg: (xcb_enter_notify_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            if (isLeave){
                (*(*window).GetWindowDisplay()).isMouseInside = false;
                return FinalizePushEvent(new MouseExitedEvent((*window).ID(), Vector2i((*event).event_x, (*event).event_y)), 0);
            }
                (*(*window).GetWindowDisplay()).isMouseInside = true;
            return FinalizePushEvent(new MouseEnteredEvent((*window).ID(), Vector2i((*event).event_x, (*event).event_y)), 0);
        }

        return 0;
    }

    uint64 WindowEventInternal::HandleKeyboardEvent(xcb_generic_event_t* msg, bool isPressed) {
        auto event = (isPressed) ? (xcb_key_press_event_t*)msg : (xcb_key_release_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            uint32 mask = (*event).state;
            ModifierState modif(mask & 1, mask & 2, mask & 3, mask & 4);

            Keyboard::Keycode keycode;
            Keyboard::Scancode scancode;

            if (isPressed){
                // xcb_keysym_t sym_shift = xcb_key_press_lookup_keysym(event);
                uint64 keychar;

                FinalizePushEvent(new KeyPressedEvent((*window).ID(), keycode, scancode, modif, 1), 0);
                return FinalizePushEvent(new CharPressedEvent((*window).ID(), keychar), 0);
            } else {
                return FinalizePushEvent(new KeyReleasedEvent((*window).ID(), keycode, scancode, modif), 0);
            }
        }

        return 0;
    }
    uint64 WindowEventInternal::HandleCharEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }

    uint64 WindowEventInternal::HandleInputRawEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleInputDeviceChangeEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleDropFilesEvent(xcb_generic_event_t* msg) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)msg;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }

    uint64 WindowEventInternal::RestricWindowSize(xcb_generic_event_t* msg) {
        return 0;
    }
}    // namespace nkentseu

#endif 