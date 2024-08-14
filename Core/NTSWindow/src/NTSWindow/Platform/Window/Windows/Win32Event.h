//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-12 at 05:42:10 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __WIN32_EVENT_H__
#define __WIN32_EVENT_H__

#pragma once

#include <NTSCore/System.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS

#include "NTSWindow/Event/EventFilter.h"
#include "NTSWindow/Event/Event.h"
#include "NTSWindow/Event/GenericInput.h"

#include <NTSMaths/Vector/Vector2.h>

#include <Windows.h>
#include <queue>
#include <vector>

namespace nkentseu {
    class Win32Window;
    
    class NKENTSEU_API Win32Event : public EventListener {
        public:
            static Win32Event& UniqueInstance();

            void AddObserver(EventObserver observer) override;

            void RemoveObserver(const EventObserver& observer) override;

            std::vector<Event*> ProcessQueue() override;

            void Update();
            void Pop();
            bool Empty();
            usize Size();

            static LRESULT StaticProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
            LRESULT Procedure(uint32 msg, WPARAM wparam, LPARAM lparam, Win32Window* window);

            LRESULT PushEvent(MSG msg, Win32Window* window);
            LRESULT FinalizePushEvent(Event* event, LRESULT info, MSG msg, Win32Window* native, const RECT& currentWindowRect = { -1, -1, -1, -1 });

            // handle input raw
            LRESULT HandleInputRawEvent(MSG msg, Win32Window* window);
            void UpdateRawInputData(MSG msg, Win32Window* window, RAWINPUT* input);
            void AnalyzeAxisRawInput(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value);
            void AnalyzeButtonRawInput(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed);
            void SetPS3GamepadAxis(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Axis axis, float32 transmit_value);
            void SetPS3GamepadButton(MSG msg, Win32Window* window, RID_DEVICE_INFO_HID devicecInfoHid, const std::string& name, GenericInput::Button button, bool isPressed);

            LRESULT HandleInputDeviceChangeEvent(MSG msg, Win32Window* window);

            LRESULT HandleCreateEvent(MSG msg, Win32Window* window);

            LRESULT HandleWindowCreateEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowPaintEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowCloseEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowFocusedEvent(MSG msg, Win32Window* window, bool focused);
            LRESULT HandleWindowResizeEvent(MSG msg, Win32Window* window, bool resizing);
            LRESULT HandleWindowMoveEvent(MSG msg, Win32Window* window, uint8 t);
            LRESULT HandleWindowNCHITTESTEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowDpiEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowNCCALCSIZEEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowGETMINMAXINFOEvent(MSG msg, Win32Window* window);
            LRESULT HandleWindowBackgroundEraseEvent(MSG msg, Win32Window* window);

            LRESULT HandleMouseWheelEvent(MSG msg, Win32Window* window, bool vertical);
            LRESULT HandleMouseButtonEvent(MSG msg, Win32Window* window, uint8 btn, bool pressed, bool dbclick);
            LRESULT HandleMouseButtonRawEvent(MSG msg, Win32Window* window, RAWINPUT* raw);
            LRESULT HandleMouseMoveEvent(MSG msg, Win32Window* window);
            LRESULT HandleMouseLeaveEvent(MSG msg, Win32Window* window);

            LRESULT HandleKeyboardEvent(MSG msg, Win32Window* window, bool keydown);
            LRESULT HandleCharEvent(MSG msg, Win32Window* window, bool interpret);

            LRESULT HandleDropFilesEvent(MSG msg, Win32Window* window);

            LRESULT RestricWindowSize(MSG msg, Win32Window* window);

            void SetMouseTracking(bool track, Win32Window* window);
        public:
            std::vector<Event*> eventQueue;
            std::vector<EventObserver> eventObservers;
            bool isPick = false;
            bool isQueueLocked = false;
            maths::Vector2i mousePosition;
            maths::Vector2i globalMousePosition;
            maths::Vector2i previousMousePosition;
    };

}  //  nkentseu

#endif

#endif  // __WIN32_EVENT_H__!