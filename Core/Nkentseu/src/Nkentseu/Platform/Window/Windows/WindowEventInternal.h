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
#include <Nkentseu/Platform/Window/Windows/WindowGenericInput.h>


namespace nkentseu {

    class NKENTSEU_API WindowEventInternal {
    public:
        friend class WindowInternal;

        static Memory::Shared<WindowEventInternal> GetInstance() {
            static Memory::Shared<WindowEventInternal> eventManager = Memory::Alloc<WindowEventInternal>();
            return eventManager;
        }

        // Méthodes

        WindowEventInternal();
        ~WindowEventInternal();

        void Update();

        Event* Front();

        void Pop();

        bool Empty();

        size_t Size();

        static LRESULT WindowProcStatic(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        LRESULT WindowProc(uint32 msg, WPARAM wparam, LPARAM lparam, class WindowInternal* nativeWindow);

        // Variables membres
        bool isInitialized;
        bool isQueueLocked;
        std::vector<Event*> eventQueue;
        std::vector<EventObserver> eventObservers;
        Vector2i mousePosition;
        Vector2i globalMousePosition;
        Vector2i previousMousePosition;

        Joysticks joysticks;


        LRESULT PushEvent(MSG msg, class WindowInternal* window);
        LRESULT FinalizePushEvent(Event* event, LRESULT info, MSG msg, class WindowInternal* native, const RECT& currentWindowRect = { -1, -1, -1, -1 });

        void TriggerEvent(Event* event);

        // Handle Event
        LRESULT HandleCreateEvent(MSG msg, class WindowInternal* window);

        LRESULT HandleWindowCreateEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowPaintEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowBackgroundEraseEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowCloseEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowFocusedEvent(MSG msg, class WindowInternal* window, bool focused);
        LRESULT HandleWindowResizeEvent(MSG msg, class WindowInternal* window, bool resizing);
        LRESULT HandleWindowNCHITTESTEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowDpiEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowNCCALCSIZEEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowGETMINMAXINFOEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleWindowMoveEvent(MSG msg, class WindowInternal* window, uint8 t);

        LRESULT HandleMouseWheelEvent(MSG msg, class WindowInternal* window, bool vertical);
        LRESULT HandleMouseButtonEvent(MSG msg, class WindowInternal* window, uint8 btn, bool pressed, bool dbclick);
        LRESULT HandleMouseButtonRawEvent(MSG msg, class WindowInternal* window, RAWINPUT* raw);
        LRESULT HandleMouseMoveEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleMouseLeaveEvent(MSG msg, class WindowInternal* window);

        LRESULT HandleKeyboardEvent(MSG msg, class WindowInternal* window, bool keydown);
        LRESULT HandleCharEvent(MSG msg, class WindowInternal* window, bool interpret);

        LRESULT HandleInputRawEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleInputDeviceChangeEvent(MSG msg, class WindowInternal* window);
        LRESULT HandleDropFilesEvent(MSG msg, class WindowInternal* window);

        LRESULT RestricWindowSize(MSG msg, class WindowInternal* window);

        void SetMouseTracking(bool track, class WindowInternal* window);
        void ParseRawInputData(MSG msg, class WindowInternal* window, PRAWINPUT pRawInput, UINT size);
        void UpdateRawInputData(MSG msg, class WindowInternal* window, RAWINPUT* input);
        void AnalyzeAxisRawInput(MSG msg, class WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value);
        void AnalyzeButtonRawInput(MSG msg, class WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed);

        // game pad type
        void SetPS3GamepadAxis(MSG msg, class WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value);
        void SetPS3GamepadButton(MSG msg, class WindowInternal* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed);
    };

    #define EventInternal      WindowEventInternal::GetInstance()
} // namespace nkentseu

#endif    // NKENTSEU_PLATFORM_WINDOWS


#endif    // __NKENTSEU_WINDOW_EVENT_INTERNAL_H__