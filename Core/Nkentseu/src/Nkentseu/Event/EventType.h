//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_TYPE_H__
#define __NKENTSEU_EVENT_TYPE_H__

#pragma once

// Include necessary headers
#include "System/System.h"
#include <System/Nature/Base.h>
#include <string>

namespace nkentseu {

    // Class representing different event types in the system
    class NKENTSEU_API EventType {
    public:

        // Define a typedef for event codes using uint32 for efficiency
        using Code = uint32;  // Use std::uint32_t for modern C++

        // Enum to represent various event codes
        enum : Code {
            None_ev = 0,

            // Window Events (for describing window-related events)
            WindowCreated_ev,
            WindowPainted_ev,
            WindowBackgroundErased_ev,
            WindowClosed_ev,
            WindowResized_ev,
            WindowFocused_ev,
            WindowLostFocus_ev,
            WindowDpiChanged_ev,
            WindowMoved_ev,

            // Application Events (for application-specific events)
            ApplicationTick_ev,
            ApplicationUpdate_ev,
            ApplicationRender_ev,

            // Keyboard Events (for keyboard input events)
            KeyPressed_ev,
            KeyReleased_ev,
            CharEntered_ev,

            // Mouse Events (for mouse input events)
            MouseButtonPressed_ev,
            MouseButtonReleased_ev,
            MouseButtonDoubleClicked_ev,
            MouseButtonRawInput_ev,
            MouseMoved_ev,
            MouseWheelScrolled_ev,
            MouseEnteredWindow_ev,
            MouseExitedWindow_ev,

            // Gamepad Events (for gamepad input events)
            GamepadConnected_ev,
            GamepadDisconnected_ev,
            GamepadButtonPressed_ev,
            GamepadButtonReleased_ev,
            GamepadAxisMoved_ev,
            GamepadStickMoved_ev,
            GamepadTriggerMoved_ev,

            // Touchpad Events (for touch input events)
            TouchReleased_ev,
            TouchPressed_ev,
            TouchMoved_ev,
            TouchCancelled_ev,
            TouchPinchGesture_ev,

            // Drag and Drop Events (for drag and drop interactions)
            FilesDropped_ev,
            DragStarted_ev,
            DropCompleted_ev,

            // Transfer Events (for data and file transfer events)
            DataTransfer_ev,
            FileTransfer_ev,

            // Generic Input Events (for handling various input devices)
            GenericInputConnected_ev,
            GenericInputDisconnected_ev,
            GenericInputPressed_ev,
            GenericInputReleased_ev,
            GenericInputAxisMoved_ev,
            GenericInputHatMoved_ev,

            // Custom Event (for handling application-specific events)
            CustomEvent_ev
        };

        // Function to convert an event code to a human-readable string
        static std::string ToString(EventType::Code);

        // Function to convert a human-readable string to an event code (if valid)
        static EventType::Code FromString(const std::string& type);
    };
} // namespace nkentseu

#endif // __NKENTSEU_EVENT_TYPE_H__
