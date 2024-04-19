//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "EventType.h"

#include <unordered_map>

namespace nkentseu {

    // Map to store event codes and their corresponding strings
    static std::unordered_map<EventType::Code, std::string> staticEventTypeStrings = {
        {EventType::NotDefine, "NotDefine"},
        {EventType::WindowCreated, "WindowCreated"},
        {EventType::WindowPainted, "WindowPainted"},
        {EventType::WindowBackgroundErased, "WindowBackgroundErased"},
        {EventType::WindowClosed, "WindowClosed"},
        {EventType::WindowResized, "WindowResized"},
        {EventType::WindowFocused, "WindowFocused"},
        {EventType::WindowLostFocus, "WindowLostFocus"},
        {EventType::WindowDpiChanged, "WindowDpiChanged"},
        {EventType::WindowMoved, "WindowMoved"},
        {EventType::ApplicationTick, "ApplicationTick"},
        {EventType::ApplicationUpdate, "ApplicationUpdate"},
        {EventType::ApplicationRender, "ApplicationRender"},
        {EventType::KeyPressed, "KeyPressed"},
        {EventType::KeyReleased, "KeyReleased"},
        {EventType::CharEntered, "CharEntered"},
        {EventType::MouseButtonPressed, "MouseButtonPressed"},
        {EventType::MouseButtonReleased, "MouseButtonReleased"},
        {EventType::MouseButtonDoubleClicked, "MouseButtonDoubleClicked"},
        {EventType::MouseButtonRawInput, "MouseButtonRawInput"},
        {EventType::MouseMoved, "MouseMoved"},
        {EventType::MouseWheelScrolled, "MouseWheelScrolled"},
        {EventType::MouseEnteredWindow, "MouseEnteredWindow"},
        {EventType::MouseExitedWindow, "MouseExitedWindow"},
        {EventType::GamepadConnected, "GamepadConnected"},
        {EventType::GamepadDisconnected, "GamepadDisconnected"},
        {EventType::GamepadButtonPressed, "GamepadButtonPressed"},
        {EventType::GamepadButtonReleased, "GamepadButtonReleased"},
        {EventType::GamepadAxisMoved, "GamepadAxisMoved"},
        {EventType::GamepadStickMoved, "GamepadStickMoved"},
        {EventType::GamepadTriggerMoved, "GamepadTriggerMoved"},
        {EventType::TouchReleased, "TouchReleased"},
        {EventType::TouchPressed, "TouchPressed"},
        {EventType::TouchMoved, "TouchMoved"},
        {EventType::TouchCancelled, "TouchCancelled"},
        {EventType::TouchPinchGesture, "TouchPinchGesture"},
        {EventType::FilesDropped, "FilesDropped"},
        {EventType::DragStarted, "DragStarted"},
        {EventType::DropCompleted, "DropCompleted"},
        {EventType::DataTransfer, "DataTransfer"},
        {EventType::FileTransfer, "FileTransfer"},
        {EventType::GenericInputConnected, "GenericInputConnected"},
        {EventType::GenericInputDisconnected, "GenericInputDisconnected"},
        {EventType::GenericInputPressed, "GenericInputPressed"},
        {EventType::GenericInputReleased, "GenericInputReleased"},
        {EventType::GenericInputAxisMoved, "GenericInputAxisMoved"},
        {EventType::GenericInputHatMoved, "GenericInputHatMoved"},
        {EventType::CustomEvent, "CustomEvent"},
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