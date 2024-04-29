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

#include "Nkentseu/Event/InputManager.h"
#include "WindowEventCode.h"

#include "WindowInternal.h"
#include "Nkentseu/Core/Window.h"
#include "Nkentseu/Platform/PlatformState.h"

#include "Nkentseu/Core/NkentseuLogger.h"

#include <xcb/xcb_keysyms.h>

namespace nkentseu {
    #define CONNEXION_IS_VALID(return_value)        xcb_connection_t* connection = PlatformState.connection; \
                                                    xcb_screen_t *screenHandle = PlatformState.screen; \
                                                    if (screenHandle == nullptr || connection == nullptr) {\
                                                        Log_nts.Error("Failed to get screen iterator and connexion");\
                                                        return return_value;\
                                                    }

    #define NATIVE_WINDOW_IS_VALID(window, return_value)    CONNEXION_IS_VALID(return_value)\
                                                            if (window->GetWindowDisplay() == nullptr || window->GetWindowDisplay()->windowHandle == (xcb_window_t)-1){\
                                                                return return_value;\
                                                            }

    static nts_size GetXCBAtomPropertyAtom(xcb_atom_t atom, xcb_window_t window, xcb_atom_t **values);

    WindowEventInternal& WindowEventInternal::GetInstance() {
        static WindowEventInternal eventManager;
        return eventManager;
    }

    WindowEventInternal::WindowEventInternal(){
    }

    WindowEventInternal::~WindowEventInternal(){
    }

    void WindowEventInternal::Update() {
        CONNEXION_IS_VALID();
        xcb_flush(connection);

        while (genericEvent = xcb_poll_for_event(connection)){
            PushEvent(genericEvent);
            free(genericEvent);
            genericEvent = nullptr;
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

    uint64 WindowEventInternal::PushEvent(xcb_generic_event_t* xcbEvent) {
        CONNEXION_IS_VALID(0);
        if (xcbEvent == nullptr || xcbEvent->response_type == 0) {
            Log_nts.Warning();
            return 0;
        }
        uint8 eventCode = xcbEvent->response_type & 0x7f;
        Log_nts.Debug("Event code = [{0}]", (uint32)xcbEvent->response_type);
        
        switch (eventCode) {
            case XCB_MAP_NOTIFY:{
        Log_nts.Debug();
                xcb_map_notify_event_t *event = (xcb_map_notify_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->window);
                Log_nts.Debug();

                if (window != nullptr && event != nullptr){
                    Log_nts.Debug();
                    return FinalizePushEvent(new WindowVisibleEvent(window->ID(), true), 0);
                }
                break;
            }
            case XCB_UNMAP_NOTIFY:{
        Log_nts.Debug();
                xcb_unmap_notify_event_t *event = (xcb_unmap_notify_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->window);

                if (window != nullptr && event != nullptr){
                    Log_nts.Debug();
                    return FinalizePushEvent(new WindowVisibleEvent(window->ID(), false), 0);
                }
                break;
            }
            case XCB_FOCUS_IN : {
        Log_nts.Debug();
                xcb_focus_in_event_t *event = (xcb_focus_in_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->event);

                if (window != nullptr && event != nullptr){
                    return FinalizePushEvent(new WindowFocusedEvent(window->ID(), FocusState::Focused), 0);
                }
                break;
            }
            case XCB_FOCUS_OUT : {
        Log_nts.Debug();
                xcb_focus_out_event_t *event = (xcb_focus_out_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->event);

                if (window != nullptr && event != nullptr){
                    return FinalizePushEvent(new WindowFocusedEvent(window->ID(), FocusState::Unfocused), 0);
                }
                break;
            }
            case XCB_CONFIGURE_NOTIFY : {
        Log_nts.Debug();
                xcb_configure_notify_event_t *event = (xcb_configure_notify_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->event);

                if (window != nullptr && event != nullptr){
                    if (Vector2u(event->width, event->height) != window->GetSize()) {
                        bool resize = false;
                        Vector2u new_size(event->width, event->height);
                        if (event->width < window->m_Properties.minSize.width || event->width > window->m_Properties.maxSize.width) {
                            new_size.width = window->GetSize().width;
                            resize = true;
                        }
                        if (event->height < window->m_Properties.minSize.height || event->height > window->m_Properties.maxSize.height) {
                             new_size.height = window->GetSize().height;
                            resize = true;
                        }

                        if (resize){
                            window->SetSize(new_size);
                            Log_nts.Debug("sizing");
                        }
                        // window resize
                        float32 area = window->GetSize().width * window->GetSize().height;
		                Rectangle win;
                        win.corner = window->GetProperties().position;
                        win.size = new_size;

                        float32 newarea = win.size.width * win.size.height;
                        ResizeState::Code state = ResizeState::NotChange;

                        if (newarea > area) {
                            state = ResizeState::Expanded;
                        }
                        else if (newarea < area) {
                            state = ResizeState::Reduced;
                        }
                        window->m_Properties.size = win.size;
                        return FinalizePushEvent(new WindowResizedEvent(window->ID(), state, win), 0);
                    } 
                    
                    if ((int32)event->x != window->GetPosition().x || (int32)event->y != window->GetPosition().y) {
                        // window moved
                        Vector2f lasposition = window->GetPosition();
		                window->m_Properties.position = Vector2i((int32)event->x, (int32)event->y);
                        return FinalizePushEvent(new WindowMovedEvent(window->ID(), window->m_Properties.position, lasposition), 0);
                    }
                    /* if (event->border_width != window->GetBorder()) { // not defined
                        // window border 
                    } 
                    if (event->above_sibling != XCB_WINDOW_NONE) { // not defined
                        // window Stack change
                    }*/
                }
                break;
            }
            case XCB_EXPOSE : {
        Log_nts.Debug();
                xcb_expose_event_t *event = (xcb_expose_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->window);

                if (window != nullptr && event != nullptr){
                    Rectangle rectangle(event->x, event->y, event->height, event->width);
                    return FinalizePushEvent(new WindowRenderedEvent(window->ID(), rectangle, window->m_Properties.backgroundColor), 0);
                }
                break;
            }
            case XCB_RESIZE_REQUEST : {
        Log_nts.Debug();
                xcb_resize_request_event_t *event = (xcb_resize_request_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->window);

                if (window != nullptr && event != nullptr){
                    bool resize = false;
                    Vector2u new_size(event->width, event->height);
                    if (event->width < window->m_Properties.minSize.width || event->width > window->m_Properties.maxSize.width) {
                        new_size.width = window->GetSize().width;
                        resize = true;
                    }
                    if (event->height < window->m_Properties.minSize.height || event->height > window->m_Properties.maxSize.height) {
                            new_size.height = window->GetSize().height;
                        resize = true;
                    }

                    if (resize){
                        window->SetSize(new_size);
                        Log_nts.Debug("sizing");
                    }

                    float32 area = window->GetSize().width * window->GetSize().height;
                    Rectangle win;
                    win.corner = window->GetProperties().position;
                    win.size = new_size;

                    float32 newarea = win.size.width * win.size.height;
                    ResizeState::Code state = ResizeState::NotChange;

                    if (newarea > area) {
                        state = ResizeState::Expanded;
                    }
                    else if (newarea < area) {
                        state = ResizeState::Reduced;
                    }
                    window->m_Properties.size = win.size;
                    Log_nts.Debug("resize 02 ---------------------------------------------------------------------");

                    return FinalizePushEvent(new WindowResizedEvent(window->ID(), state, win), 0);
                }
                break;
            }
            case XCB_ENTER_NOTIFY : {
        Log_nts.Debug();
                xcb_enter_notify_event_t *event = (xcb_enter_notify_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->event);

                if (window != nullptr && event != nullptr){
                    return FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Entered, Vector2i(event->event_x, event->event_y)), 0);
                }
                break;
            }
            case XCB_LEAVE_NOTIFY : {
        Log_nts.Debug();
                xcb_leave_notify_event_t *event = (xcb_leave_notify_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->event);

                if (window != nullptr && event != nullptr){
                    return FinalizePushEvent(new MouseWindowEvent(window->ID(), RegionState::Exited, Vector2i(event->event_x, event->event_y)), 0);
                }
                break;
            }
            case XCB_CLIENT_MESSAGE : {
        Log_nts.Debug();
                xcb_client_message_event_t *event = (xcb_client_message_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->window);

                if (window != nullptr && event != nullptr){
                    if (event->type == PlatformState.stateTools.WM_PROTOCOLS && event->format == 32) {
                        xcb_atom_t protocol = event->data.data32[0];
                        if (protocol == PlatformState.stateTools.WM_DELETE_WINDOW) {
                            return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Closed, window->GetProperties()), 0);
                        }
                    }
                }
                break;
            }
            case XCB_PROPERTY_NOTIFY : {
        Log_nts.Debug();
                xcb_property_notify_event_t *event = (xcb_property_notify_event_t *)xcbEvent;
                WindowInternal* window = WindowInternal::GetCurrent(event->window);

                if (window != nullptr && event != nullptr && event->atom == PlatformState.stateTools._NET_WM_STATE){
                    xcb_atom_t *values = nullptr;
                    nts_size   value_count = GetXCBAtomPropertyAtom (
                                                                            PlatformState.stateTools._NET_WM_STATE,
                                                                            window->GetWindowDisplay()->windowHandle,
                                                                            &values
                                                                        );

                    NtsWindowState new_state = window->GetWindowDisplay()->state;
                    int32          add       = event->state;
                    for (nts_size s = 0; s < value_count; s++) {
                        if (values[s] == PlatformState.stateTools._NET_WM_STATE_MODAL) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_MODAL :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_MODAL;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_STICKY) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_STICKY :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_STICKY;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_MAXIMIZED_VERT) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_VERT :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_VERT;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_MAXIMIZED_HORZ) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_HORZ :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_MAXIMIZED_HORZ;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_SHADED) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_SHADED :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_SHADED;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_SKIP_TASKBAR) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_SKIP_TASKBAR :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_SKIP_TASKBAR;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_SKIP_PAGER) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_SKIP_PAGER :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_SKIP_PAGER;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_HIDDEN) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_HIDDEN :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_HIDDEN;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_FULLSCREEN) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_FULLSCREEN :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_FULLSCREEN;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_ABOVE) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_ABOVE :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_ABOVE;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_BELOW) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_BELOW :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_BELOW;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_DEMANDS_ATTENTION) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_DEMANDS_ATTENTION :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_DEMANDS_ATTENTION;
                        } else if (values[s] == PlatformState.stateTools._NET_WM_STATE_FOCUSED) {
                            new_state = add ? new_state | NKENTSEU_WINDOW_STATE_MASK_FOCUSED :
                                            new_state & ~NKENTSEU_WINDOW_STATE_MASK_FOCUSED;
                        }
                    }

                    // event not defined
                }
                break;
            }
            case XCB_BUTTON_PRESS :
            case XCB_BUTTON_RELEASE : {
        Log_nts.Debug();
                HandleMouseButtonEvent(xcbEvent, eventCode == XCB_BUTTON_PRESS);
                break;
            }
            case XCB_MOTION_NOTIFY : {
        Log_nts.Debug();
                HandleMouseMoveEvent(xcbEvent);
                break;
            }
            case XCB_KEY_PRESS : 
            case XCB_KEY_RELEASE : {
        Log_nts.Debug();
                HandleKeyboardEvent(xcbEvent, eventCode == XCB_KEY_PRESS);
                break;
            }
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
    uint64 WindowEventInternal::HandleCreateEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }

    uint64 WindowEventInternal::HandleWindowCreateEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowPaintEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowBackgroundEraseEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    
    // xcb_generic_event_t* xcbEvent
    uint64 WindowEventInternal::HandleWindowCloseEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }

    uint64 WindowEventInternal::HandleWindowFocusedEvent(xcb_generic_event_t* xcbEvent, uint32 value) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowResizeEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowNCHITTESTEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowDpiEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowNCCALCSIZEEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowGETMINMAXINFOEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleWindowMoveEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }

    uint64 WindowEventInternal::HandleMouseWheelEvent(xcb_generic_event_t* xcbEvent) {
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseButtonEvent(xcb_generic_event_t* xcbEvent, bool isPressed) {
        auto event = isPressed ? (xcb_button_press_event_t *)xcbEvent : (xcb_button_release_event_t *)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            uint32 state = event->state;
            ModifierState modif;

            modif.control      = (state & XCB_MOD_MASK_CONTROL);
            modif.shift     = (state & XCB_MOD_MASK_SHIFT);
            //modif.caps_lock = (state & XCB_MOD_MASK_LOCK);
            //modif.num_lock  = (state & XCB_MOD_MASK_2);
            modif.alt       = (state & XCB_MOD_MASK_1);
            //modif.caps_lock = (state & XCB_MOD_MASK_LOCK);
            modif.super      = (state & XCB_MOD_MASK_4);

            Vector2f position(event->event_x, event->event_y);

            Mouse::Button button = Mouse::Buttons::NotDefine;

            if (event->detail == 1) button = Mouse::Buttons::Left;
            else if (event->detail == 2) button = Mouse::Buttons::Middle;
            else if (event->detail == 3) button = Mouse::Buttons::Right;
            else if (event->detail == 8) button = Mouse::Buttons::X1;
            else if (event->detail == 9) button = Mouse::Buttons::X2;

            if (event->detail >= 4 || event->detail <= 7){
                Mouse::Wheel wheel = (event->detail == 4 || event->detail == 5) ? Mouse::Wheels::Vertical : Mouse::Wheels::Horizontal;
                FinalizePushEvent(new MouseWheelEvent(window->ID(), wheel, 0.0f, modif, Vector2i(position.x, position.y)), 0);
            }

            ButtonState::Code bState = isPressed ? ButtonState::Pressed : ButtonState::Released;

            if (button != Mouse::Buttons::NotDefine){
                return FinalizePushEvent(new MouseInputEvent(window->ID(), bState, modif, button, false, position, globalMousePosition), 0);
            }
        }
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseButtonRawEvent(xcb_generic_event_t* xcbEvent) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleMouseMoveEvent(xcb_generic_event_t* xcbEvent) {
        xcb_motion_notify_event_t *event = (xcb_motion_notify_event_t *)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            static bool isFirst = true;

            globalMousePosition = Vector2i(event->root_x, event->root_y);
            Vector2i position(event->event_x, event->event_y);
            Vector2i move = globalMousePosition - previousMousePosition;
            previousMousePosition = globalMousePosition;

            mousePosition = position;

            if (isFirst){
                isFirst = false;
                return 0;
            }

            return FinalizePushEvent(new MouseMovedEvent((*window).ID(), position, globalMousePosition, move), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleMouseLeaveEvent(xcb_generic_event_t* xcbEvent, bool isLeave) {
        auto event = (isLeave) ? (xcb_leave_notify_event_t*)xcbEvent: (xcb_enter_notify_event_t*)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            RegionState::Code rs = RegionState::Entered;
            if (isLeave){
                (*(*window).GetWindowDisplay()).isMouseInside = false;
                rs = RegionState::Exited;
            } else {
                (*(*window).GetWindowDisplay()).isMouseInside = true;
            }
		    return FinalizePushEvent(new MouseWindowEvent(window->ID(), rs, Vector2i(event->event_x, event->event_y)), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleKeyboardEvent(xcb_generic_event_t* xcbEvent, bool isPressed) {
        auto event = isPressed ? (xcb_key_press_event_t *)xcbEvent : (xcb_key_release_event_t *)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->event);

        if (window != nullptr && event != nullptr){
            uint32 state = event->state;
            ModifierState modif;

            modif.control      = (state & XCB_MOD_MASK_CONTROL);
            modif.shift     = (state & XCB_MOD_MASK_SHIFT);
            //modif.caps_lock = (state & XCB_MOD_MASK_LOCK);
            //modif.num_lock  = (state & XCB_MOD_MASK_2);
            modif.alt       = (state & XCB_MOD_MASK_1);
            //modif.caps_lock = (state & XCB_MOD_MASK_LOCK);
            modif.super      = (state & XCB_MOD_MASK_4);

            static xcb_key_symbols_t *SYM = 0;

            if(SYM==0){ 
                SYM = xcb_key_symbols_alloc(PlatformState.connection); 
            }

            xcb_keysym_t symbol = xcb_key_symbols_get_keysym(SYM, event->detail, 0);
            
            Keyboard::Keycode keycode = WindowEventCode::WinkeyToKeycode(symbol);
            Keyboard::Scancode scancode = WindowEventCode::WinkeyToKeycode(event->detail);

            ButtonState::Code bState = ButtonState::Released;
            uint64 keychar = 0;

            if (isPressed){
                bState = ButtonState::Pressed;

                // Récupérer le caractère associé au keycode
                xcb_keysym_t upper;
                xcb_keysym_t lower;
                xcb_keycode_t* keycodes = xcb_key_symbols_get_keycode(SYM, event->detail);

                if (keycodes != nullptr) {
                    upper = xcb_key_symbols_get_keysym(SYM, event->detail, 0);
                    lower = xcb_key_symbols_get_keysym(SYM, event->detail, 1);
                    free(keycodes);
                } else {
                    upper = lower = XCB_NO_SYMBOL;
                }

                // Utiliser la touche majuscule ou minuscule selon le cas
                xcb_keysym_t selected = (modif.shift) ? upper : lower;
                keychar = selected;
            }
            
            return FinalizePushEvent(new KeyboardEvent(window->ID(), bState, keycode, scancode, modif, keychar), 0);
        }
        return 0;
    }
    uint64 WindowEventInternal::HandleCharEvent(xcb_generic_event_t* xcbEvent) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }

    uint64 WindowEventInternal::HandleInputRawEvent(xcb_generic_event_t* xcbEvent) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleInputDeviceChangeEvent(xcb_generic_event_t* xcbEvent) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }
    uint64 WindowEventInternal::HandleDropFilesEvent(xcb_generic_event_t* xcbEvent) {
        xcb_client_message_event_t* event = (xcb_client_message_event_t*)xcbEvent;
        WindowInternal* window = WindowInternal::GetCurrent(event->window);
        return 0;
    }

    uint64 WindowEventInternal::RestricWindowSize(xcb_generic_event_t* xcbEvent) {
        return 0;
    }

    static nts_size GetXCBAtomPropertyAtom (xcb_atom_t atom, xcb_window_t window, xcb_atom_t **values) {
        xcb_get_property_cookie_t cookie = xcb_get_property (
            PlatformState.connection, // xcb_connection_t *c,
            False,               // uint8_t           _delete,
            window,              // xcb_window_t      window,
            atom,                // xcb_atom_t        property,
            XCB_ATOM_ATOM,       // xcb_atom_t        type,
            0,                   // uint32_t          long_offset,
            UINT32_MAX           // uint32_t          long_length
        );
        xcb_get_property_reply_t *reply = xcb_get_property_reply (PlatformState.connection, cookie, nullptr);
        *values                         = (xcb_atom_t *)xcb_get_property_value (reply);
        nts_size length                     = xcb_get_property_value_length (reply);
        free(reply);

        return length;
    }
}    // namespace nkentseu

#endif 