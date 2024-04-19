//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEUENT_TYPE_H__
#define __NKENTSEUENT_TYPE_H__

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
            NotDefine = 0,

            // Window Events (for describing window-related events)
            WindowCreated,
            WindowPainted,
            WindowBackgroundErased,
            WindowClosed,
            WindowResized,
            WindowFocused,
            WindowLostFocus,
            WindowDpiChanged,
            WindowMoved,

            // Application Events (for application-specific events)
            ApplicationTick,
            ApplicationUpdate,
            ApplicationRender,

            // Keyboard Events (for keyboard input events)
            KeyPressed,
            KeyReleased,
            CharEntered,

            // Mouse Events (for mouse input events)
            MouseButtonPressed,
            MouseButtonReleased,
            MouseButtonDoubleClicked,
            MouseButtonRawInput,
            MouseMoved,
            MouseWheelScrolled,
            MouseEnteredWindow,
            MouseExitedWindow,

            // Gamepad Events (for gamepad input events)
            GamepadConnected,
            GamepadDisconnected,
            GamepadButtonPressed,
            GamepadButtonReleased,
            GamepadAxisMoved,
            GamepadStickMoved,
            GamepadTriggerMoved,

            // Touchpad Events (for touch input events)
            TouchReleased,
            TouchPressed,
            TouchMoved,
            TouchCancelled,
            TouchPinchGesture,

            // Drag and Drop Events (for drag and drop interactions)
            FilesDropped,
            DragStarted,
            DropCompleted,

            // Transfer Events (for data and file transfer events)
            DataTransfer,
            FileTransfer,

            // Generic Input Events (for handling various input devices)
            GenericInputConnected,
            GenericInputDisconnected,
            GenericInputPressed,
            GenericInputReleased,
            GenericInputAxisMoved,
            GenericInputHatMoved,

            // Custom Event (for handling application-specific events)
            CustomEvent
        };

        // Function to convert an event code to a human-readable string
        static std::string ToString(EventType::Code);

        // Function to convert a human-readable string to an event code (if valid)
        static EventType::Code FromString(const std::string& type);
    };
} // namespace nkentseu

#endif // __NKENTSEUENT_TYPE_H__
