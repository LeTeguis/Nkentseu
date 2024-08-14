//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "EventType.h"

#include <unordered_map>

namespace nkentseu {

    // Map to store event codes and their corresponding strings
    static std::unordered_map<EventType::Code, std::string> staticEventTypeStrings = {
        {EventType::WindowStatus, "WindowStatus"},   // Created, Closed
        {EventType::WindowRendered, "WindowRendered"},
        {EventType::WindowBackgroundErased, "WindowBackgroundErased"},
        {EventType::WindowResized, "WindowResized"},  // Expand, Reduce
        {EventType::WindowFocused, "WindowFocused"},    // Focused, LosFocus
        {EventType::WindowDpiChanged, "WindowDpiChanged"},
        {EventType::WindowMoved, "WindowMoved"},

            // Application Events (for application-specific events)
        {EventType::ApplicationTick, "ApplicationTick"},
        {EventType::ApplicationUpdate, "ApplicationUpdate"},
        {EventType::ApplicationRender, "ApplicationRender"},

            // Keyboard Events (for keyboard input events) (Pressed, Released)
        {EventType::KeyboardInput, "KeyboardInput"},
        {EventType::CharEntered, "CharEntered"},

            // Mouse Events (for mouse input events) 
        {EventType::MouseInput, "MouseInput"},    // Pressed, Released
        {EventType::MouseMoved, "MouseMoved"},     // Vertical, Horizontal, value
        {EventType::MouseWheel, "MouseWheel"},     // Vertical, Horizontal, value
        {EventType::MouseWindow, "MouseWindow"},    // Entered, Exited

            // Gamepad Events (for gamepad input events)
        {EventType::GamepadStatus, "GamepadStatus" },   // Connected, Disconnected
        {EventType::GamepadInput, "GamepadInput" },
        {EventType::GamepadAxis, "GamepadAxis" },
        {EventType::GamepadStick, "GamepadStick" },
        {EventType::GamepadTrigger, "GamepadTrigger" },

            // Touchpad Events (for touch input events)
        {EventType::TouchButton, "TouchButton" },    // Pressed, Released
        {EventType::TouchMoved, "TouchMoved" },     // moved
        {EventType::TouchCancelled, "TouchCancelled" },
        {EventType::TouchPinchGesture, "TouchPinchGesture" },

            // Drag and Drop Events (for drag and drop interactions)
        {EventType::FilesEntered, "FilesEntered" },
        {EventType::DraggedStatus, "DraggedStatus" },  // Dragged, Dropped, DragDropEnd

            // Transfer Events (for data and file transfer events)
        {EventType::Transfer, "Transfer" },   // Data, File

            // Generic Input Events (for handling various input devices)
        {EventType::GenericStatus, "GenericStatus" }, // Connected, Disconnected
        {EventType::GenericInput, "GenericInput" },  // Pressed, Released
        {EventType::GenericAxis, "GenericAxis" },
        {EventType::GenericHat, "GenericHat" },

        {EventType::NotDefine, "NotDefine"},
    };

    // Function to convert an event code to a human-readable string
    std::string EventType::ToString(EventType::Code code) {
        // Use iterator to find the corresponding string
        auto it = staticEventTypeStrings.find(code);
        if (it != staticEventTypeStrings.end()) {
            return it->second;
        }
        else {
            // Return an empty string if code not found
            return "None";
        }
    }

    // Function to convert a human-readable string to an event code (if valid)
    EventType::Code EventType::FromString(const std::string& type) {
        // Loop through the map to find matching string
        for (const auto& pair : staticEventTypeStrings) {
            if (pair.second == type) {
                return pair.first;
            }
        }
        // Return None if string not found
        return EventType::NotDefine;
    }

}    // namespace nkentseu