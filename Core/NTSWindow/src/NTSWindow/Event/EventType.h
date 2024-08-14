//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEUENT_TYPE_H__
#define __NKENTSEUENT_TYPE_H__

#pragma once

// Include necessary headers
#include "NTSCore/System.h"
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
            WindowStatus,   // Created, Closed
            WindowRendered,
            WindowBackgroundErased,
            WindowResized,  // Expand, Reduce
            WindowFocused,    // Focused, LosFocus
            WindowDpiChanged,
            WindowMoved,
            WindowVisible,

            // Application Events (for application-specific events)
            ApplicationTick,
            ApplicationUpdate,
            ApplicationRender,

            // Keyboard Events (for keyboard input events) (Pressed, Released)
            KeyboardInput,
            CharEntered,

            // Mouse Events (for mouse input events) 
            MouseInput,    // Pressed, Released
            MouseMoved,     // Vertical, Horizontal, value
            MouseWheel,     // Vertical, Horizontal, value
            MouseWindow,    // Entered, Exited

            // Gamepad Events (for gamepad input events)
            GamepadStatus,   // Connected, Disconnected
            GamepadInput,
            GamepadAxis,
            GamepadStick,
            GamepadTrigger,

            // Touchpad Events (for touch input events)
            TouchButton,    // Pressed, Released
            TouchMoved,     // moved
            TouchCancelled,
            TouchPinchGesture,

            // Drag and Drop Events (for drag and drop interactions)
            FilesEntered,
            DraggedStatus,  // Dragged, Dropped, DragDropEnd

            // Transfer Events (for data and file transfer events)
            Transfer,   // Data, File

            // Generic Input Events (for handling various input devices)
            GenericStatus, // Connected, Disconnected
            GenericInput,  // Pressed, Released
            GenericAxis,
            GenericHat,

            MaxType
        };

        // Function to convert an event code to a human-readable string
        static std::string ToString(EventType::Code);

        // Function to convert a human-readable string to an event code (if valid)
        static EventType::Code FromString(const std::string& type);
    };
} // namespace nkentseu

#endif // __NKENTSEUENT_TYPE_H__
