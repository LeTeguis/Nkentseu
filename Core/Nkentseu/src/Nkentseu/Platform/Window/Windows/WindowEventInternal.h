//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 4:51:55 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#pragma once
#include "System/System.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS


#ifndef __NKENTSEU_WINDOW_EVENT_INTERNAL_H__
#define __NKENTSEU_WINDOW_EVENT_INTERNAL_H__

#define QWORD uint64_t

#include <Windows.h>
#include <windowsx.h>

#include <queue>
#include <vector>

#include <stdio.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>

#include <Nkentseu/Event/Event.h>
#include <Nkentseu/Event/GenericInput.h>
#include <Ntsm/Vector/Vector2.h>
#include <Nkentseu/Event/EventObservable.h>


namespace nkentseu {
    class NKENTSEU_API WindowEventInternal {
    public:
        friend class WindowDisplay;
        friend class WindowInternal;

        static std::shared_ptr<WindowEventInternal> GetInstance() {
            static std::shared_ptr<WindowEventInternal> eventManager = std::make_shared<WindowEventInternal>();
            return eventManager;
        }

        // Méthodes

        WindowEventInternal();

        void Update();

        Event* Front();

        void Pop();

        bool Empty();

        size_t Size();

        static LRESULT WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT WindowProc(uint32 msg, WPARAM wparam, LPARAM lparam, class WindowDisplay* nativeWindow);

        // Variables membres
        bool isInitialized;
        bool isQueueLocked;
        std::vector<Event*> eventQueue;
        std::vector<EventObserver> eventObservers;
        Vector2i mousePosition;
        Vector2i globalMousePosition;
        Vector2i previousMousePosition;
        //PtrType(JoystickInternal) joystickInfo;


        LRESULT PushEvent(MSG msg, class WindowDisplay* window);
        LRESULT FinalizePushEvent(Event* event, LRESULT info, MSG msg, class WindowDisplay* native, const RECT& currentWindowRect = { -1, -1, -1, -1 });

        // Handle Event
        LRESULT HandleCreateEvent(MSG msg, class WindowDisplay* window);

        LRESULT HandleWindowCreateEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowPaintEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowBackgroundEraseEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowCloseEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowFocusedEvent(MSG msg, class WindowDisplay* window, bool focused);
        LRESULT HandleWindowResizeEvent(MSG msg, class WindowDisplay* window, bool resizing);
        LRESULT HandleWindowNCHITTESTEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowDpiEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowNCCALCSIZEEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowGETMINMAXINFOEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleWindowMoveEvent(MSG msg, class WindowDisplay* window, uint8 t);

        LRESULT HandleMouseWheelEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleMouseButtonEvent(MSG msg, class WindowDisplay* window, uint8 btn, bool pressed, bool dbclick);
        LRESULT HandleMouseButtonRawEvent(MSG msg, class WindowDisplay* window, RAWINPUT* raw);
        LRESULT HandleMouseMoveEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleMouseLeaveEvent(MSG msg, class WindowDisplay* window);

        LRESULT HandleKeyboardEvent(MSG msg, class WindowDisplay* window, bool keydown);
        LRESULT HandleCharEvent(MSG msg, class WindowDisplay* window, bool interpret);

        LRESULT HandleInputRawEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleInputDeviceChangeEvent(MSG msg, class WindowDisplay* window);
        LRESULT HandleDropFilesEvent(MSG msg, class WindowDisplay* window);


        void SetMouseTracking(bool track, class WindowDisplay* window);
        void ParseRawInputData(MSG msg, class WindowDisplay* window, PRAWINPUT pRawInput, UINT size);
        void UpdateRawInputData(MSG msg, class WindowDisplay* window, RAWINPUT* input);
        void AnalyzeAxisRawInput(MSG msg, class WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value);
        void AnalyzeButtonRawInput(MSG msg, class WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed);

        // game pad type
        void SetPS3GamepadAxis(MSG msg, class WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value);
        void SetPS3GamepadButton(MSG msg, class WindowDisplay* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed);
    };
} // namespace nkentseu

#define EventInternal      WindowEventInternal::GetInstance()

#endif    // NKENTSEU_PLATFORM_WINDOWS


#endif    // __NKENTSEU_WINDOW_EVENT_INTERNAL_H__