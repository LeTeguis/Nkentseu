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
    static KeySym GetKeysym(uint64 keycode);
    static uint64 TextToUint64(const std::string& text);

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
        if (dpi == 0){
            dpi = GetCurrentDpi(PlatformState.display);
        } else {
            float64 current_dpi = GetCurrentDpi(PlatformState.display);

            if (current_dpi != dpi){
                dpi = current_dpi;
                FinalizePushEvent(new WindowDpiChangedEvent(0, dpi), 0);
            }
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
            case KeyPress:
            case KeyRelease:
                return HandleKeyboard(event.xkey, event.type == KeyPress);
                break;
            case ButtonPress:
            case ButtonRelease:
                return HandleMouseButton(event.xbutton, event.type == ButtonPress);
                break;
            case MotionNotify:
                return HandleMotionNotify(event.xmotion);
                break;
            case EnterNotify:
            case LeaveNotify:
                return HandleEnterLeaveEvent(event.xcrossing, event.type == EnterNotify);
                break;
            case FocusIn:
            case FocusOut:
                return HandleFocusEvent(event.xfocus, event.type == FocusIn);
                break;
            case KeymapNotify:
                // Log_nts.Debug("KeymapNotify event received");
                break;
            case Expose:
                return HandleExposeEvent(event.xexpose);
                break;
            case GraphicsExpose:
                return HandleGraphicsExposeEvent(event.xgraphicsexpose);
                break;
            case NoExpose:
                //Log_nts.Debug("NoExpose event received");
                break;
            case VisibilityNotify:
                return HandleVisibilityEvent(event.xvisibility);
                break;
            case CreateNotify:
                //Log_nts.Debug("CreateNotify event received");
                break;
            case DestroyNotify:
                //Log_nts.Debug("DestroyNotify event received");
                break;
            case UnmapNotify:
                //Log_nts.Debug("UnmapNotify event received");
                break;
            case MapNotify:
                //Log_nts.Debug("MapNotify event received");
                break;
            case MapRequest:
                //Log_nts.Debug("MapRequest event received");
                break;
            case ReparentNotify:
                //Log_nts.Debug("ReparentNotify event received");
                break;
            case ConfigureNotify:
                HandleConfigureEvent(event.xconfigure);
                break;
            case ConfigureRequest:
                //Log_nts.Debug("ConfigureRequest event received");
                break;
            case GravityNotify:
                // Log_nts.Debug("GravityNotify event received");
                break;
            case ResizeRequest:
                return HandleResizeEvent(event.xresizerequest);
                break;
            case CirculateNotify:
                //Log_nts.Debug("CirculateNotify event received");
                break;
            case CirculateRequest:
                //Log_nts.Debug("CirculateRequest event received");
                break;
            case PropertyNotify:
                //Log_nts.Debug("PropertyNotify event received");
                break;
            case SelectionClear:
                //Log_nts.Debug("SelectionClear event received");
                break;
            case SelectionRequest:
                //Log_nts.Debug("SelectionRequest event received");
                break;
            case SelectionNotify:
                //Log_nts.Debug("SelectionNotify event received");
                break;
            case ColormapNotify:
                //Log_nts.Debug("ColormapNotify event received");
                break;
            case ClientMessage:
                return HandleClientMessage(event.xclient);
                break;
            case MappingNotify:
                //Log_nts.Debug("MappingNotify event received");
                break;
            case GenericEvent:
                //Log_nts.Debug("GenericEvent event received");
                break;
            default:
                //Log_nts.Debug("Unknown event received");
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
    
    uint64 WindowEventInternal::HandleResizeEvent(XResizeRequestEvent event){
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr){
            Vector2u size = window->m_Properties.size;
            
            float32 prevArea = size.width * size.height;
            float32 currentArea = event.width * event.height;
            ResizeState::Code state = ResizeState::NotChange;

            if (prevArea > currentArea){
                state = ResizeState::Reduced;
            } else if (prevArea < currentArea){
                state = ResizeState::Expanded;
            }

            window->m_Properties.size = Vector2u(event.width, event.height);
            FinalizePushEvent(new WindowResizedEvent(window->ID(), state, window->m_Properties.size), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleKeyboard(XKeyEvent event, bool isPressed){
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr){
            uint64 xkeycode = event.keycode;
            //KeySym xscancode = GetKeysym(xkeycode);
            KeySym xscancode;
            uint64 xstate = event.state;
            char text[255] = "";
            XLookupString(&event, text, 255, &xscancode, 0);
            KeySym keysym = XLookupKeysym(&event, 0);

            ButtonState::Code keystate = isPressed ? ButtonState::Pressed : ButtonState::Released;
            Keyboard::Keycode keycode = WindowEventCode::WinkeyToKeycode(xscancode);
            Keyboard::Scancode scancode = WindowEventCode::WinkeyToKeycode(keysym);
            ModifierState modifierState(xstate & ControlMask, xstate & Mod1Mask, xstate & ShiftMask, false);
            uint64 keychar = TextToUint64(std::string(text));

            FinalizePushEvent(new KeyboardEvent(window->ID(), keystate, keycode, scancode, modifierState, keychar), 0);
            return FinalizePushEvent(new CharEnteredEvent(window->ID(), keychar, std::string(text)), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleMouseButton(XButtonEvent event, bool isPressed){
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr){
            mousePosition = Vector2i(event.x, event.y);
            globalMousePosition = Vector2i(event.x_root, event.y_root);

            uint64 xstate = event.state;
            ButtonState::Code state = isPressed ? ButtonState::Pressed : ButtonState::Released;
            ModifierState modifierState(xstate & ControlMask, xstate & Mod1Mask, xstate & ShiftMask, false);

            Mouse::Button button = Mouse::Buttons::NotDefine;

            if (event.button == 1) button = Mouse::Buttons::Left;
            else if (event.button == 2) button = Mouse::Buttons::Middle;
            else if (event.button == 3) button = Mouse::Buttons::Right;
            else if (event.button == 8) button = Mouse::Buttons::X1;
            else if (event.button == 9) button = Mouse::Buttons::X2;

            if (event.button == 4 || event.button == 5 || event.button == 6 || event.button == 7){
                Mouse::Wheel wheel = Mouse::Wheels::NotDefine;

                if (event.button == 4 || event.button == 5) wheel = Mouse::Wheels::Vertical;
                else wheel = Mouse::Wheels::Horizontal;

                float32 delta = 0.0f;

                // wheel
                if (event.button == 4 || event.button == 7){
                    // wheel up
                    delta = 1.0f;
                } else if (event.button == 5 || event.button == 6){
                    // wheel down
                    delta = -1.0f;
                }

                return FinalizePushEvent(new MouseWheelEvent(window->ID(), wheel, delta, modifierState, mousePosition), 0);
            }

            if (button != Mouse::Buttons::NotDefine){
                return FinalizePushEvent(new MouseInputEvent(window->ID(), state, modifierState, button, false, mousePosition, globalMousePosition), 0);
            }
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleMotionNotify(XMotionEvent event) {
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr) {
            static bool isFirst = true;

            globalMousePosition = Vector2i(event.x_root, event.y_root);
            Vector2i position(event.x, event.y);
            Vector2i speed = globalMousePosition - previousMousePosition;
            previousMousePosition = globalMousePosition;

            mousePosition = position;

            if (isFirst){
                isFirst = false;
                return 0;
            }

            return FinalizePushEvent(new MouseMovedEvent(window->ID(), position, globalMousePosition, speed), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleEnterLeaveEvent(XCrossingEvent event, bool isEnter) {
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr) {
            Vector2i position(event.x, event.y);
            mousePosition = position;

            RegionState::Code state = RegionState::NotDefine;
            if (isEnter) {
                state = RegionState::Entered;
            } else {
                state = RegionState::Exited;
            }
            return FinalizePushEvent(new MouseWindowEvent(window->ID(), state, position), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleFocusEvent(XFocusChangeEvent event, bool isFocusIn) {
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr) {
            return FinalizePushEvent(new WindowFocusedEvent(window->ID(), ((isFocusIn) ? FocusState::Focused : FocusState::Unfocused)), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleClientMessage(XClientMessageEvent event){
        WindowInternal* window = WindowInternal::GetCurrent(event.window);
        if (window != nullptr){
            if(event.data.l[0] == PlatformState.protocolAtom.deleteWindow) {
                return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Closed, window->GetProperties()), 0);
            } else if (event.message_type == PlatformState.protocolAtom.windowCreatedAtom) {
                return FinalizePushEvent(new WindowStatusEvent(window->ID(), WindowState::Created, window->GetProperties()), 0);
            }
        }
        return 0;
    } // 

    uint64 WindowEventInternal::HandleExposeEvent(XExposeEvent event) {
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr) {
            return FinalizePushEvent(new WindowBackgroundErasedEvent(window->ID(), Rectangle(event.x, event.y, event.width, event.height), window->GetBackgroundColor()), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleGraphicsExposeEvent(XGraphicsExposeEvent event) {
        WindowInternal* window = WindowInternal::GetCurrent(event.drawable);

        if (window != nullptr) {
            return FinalizePushEvent(new WindowRenderedEvent(window->ID(), Rectangle(event.x, event.y, event.width, event.height), window->GetBackgroundColor()), 0);
        }
        return 0;
    }

    uint64 WindowEventInternal::HandleConfigureEvent(XConfigureEvent event) {
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr) {
            Vector2i position = window->m_Properties.position;
            Vector2u size = window->m_Properties.size;

            if (position != Vector2i(event.x, event.y)){
                // Window moved
                window->m_Properties.position = Vector2i(event.x, event.y);
                FinalizePushEvent(new WindowMovedEvent(window->ID(), window->m_Properties.position, position), 0);
            }

            if (size != Vector2u(event.width, event.height)){
                // Window resize
                float32 prevArea = size.width * size.height;
                float32 currentArea = event.width * event.height;
                ResizeState::Code state = ResizeState::NotChange;

                if (prevArea > currentArea){
                    state = ResizeState::Reduced;
                } else if (prevArea < currentArea){
                    state = ResizeState::Expanded;
                }
                window->m_Properties.size = Vector2u(event.width, event.height);
                FinalizePushEvent(new WindowResizedEvent(window->ID(), state, window->m_Properties.size), 0);
            }
        }
        return 0;
    }
    
    uint64 WindowEventInternal::HandleVisibilityEvent(XVisibilityEvent event){
        WindowInternal* window = WindowInternal::GetCurrent(event.window);

        if (window != nullptr) {
            bool state = event.state == VisibilityUnobscured || event.state == VisibilityPartiallyObscured;
            return FinalizePushEvent(new WindowVisibleEvent(window->ID(), state), 0);
        }
        return 0;       
    }

    float64 WindowEventInternal::CalculateDPI(int32 width_pixels, int32 height_pixels, float64 width_inches, float64 height_inches) {
        float64 horizontal_dpi = width_pixels / width_inches;
        float64 vertical_dpi = height_pixels / height_inches;
        // Vous pouvez également calculer une valeur moyenne des DPI si nécessaire
        return (horizontal_dpi + vertical_dpi) / 2.0;
    }

    float64 WindowEventInternal::GetCurrentDpi(Display* display) {
        float64 dpi_detect = 0.0f;
        XRRScreenResources* res = XRRGetScreenResourcesCurrent(PlatformState.display, PlatformState.rootWindow);
        if (res != nullptr) {
            for (int32 i = 0; i < res->noutput; ++i) {
                XRROutputInfo* output_info = XRRGetOutputInfo(PlatformState.display, res, res->outputs[i]);
                if (output_info != nullptr && output_info->connection == RR_Connected) {
                    float64 width_mm = output_info->mm_width;
                    float64 height_mm = output_info->mm_height;

                    XRRCrtcInfo* crtc_info = XRRGetCrtcInfo(PlatformState.display, res, output_info->crtc);
                    if (crtc_info != nullptr) {
                        int32 width_pixels = crtc_info->width;
                        int32 height_pixels = crtc_info->height;

                        dpi_detect = CalculateDPI(width_pixels, height_pixels, width_mm, height_mm);

                        XRRFreeCrtcInfo(crtc_info);
                    }
                    XRRFreeOutputInfo(output_info);
                }
            }
            XRRFreeScreenResources(res);
        }
        return dpi_detect;
    }

    KeySym GetKeysym(uint64 keycode){
        KeySym *possible_keysyms;
        int32 keysyms_per_keycode;

        possible_keysyms = XGetKeyboardMapping(PlatformState.display, keycode, 1, &keysyms_per_keycode);

        if (!keysyms_per_keycode){
            return NoSymbol;
        }

        KeySym result = possible_keysyms[0];
        XFree(possible_keysyms);
        return result;
    }

    uint64 TextToUint64(const std::string& text) {
        // Convertir le texte en UTF-32
        std::wstring utf32_text = std::wstring(text.begin(), text.end());

        // Initialiser la valeur uint64
        uint64 uint64_value = 0;

        // Itérer sur les caractères UTF-32
        for (wchar_t c : utf32_text) {
            // Obtenir le point de code Unicode
            uint32 code_point = c;

            // Décaler la valeur uint64 de 32 bits et ajouter le point de code
            uint64_value = (uint64_value << 32) | code_point;
        }

        return uint64_value;
    }
}    // namespace nkentseu

#endif 