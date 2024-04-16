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
        {EventType::None_ev, "None"},
        {EventType::WindowCreated_ev, "WindowCreated"},
        {EventType::WindowPainted_ev, "WindowPainted"},
        {EventType::WindowBackgroundErased_ev, "WindowBackgroundErased"},
        {EventType::WindowClosed_ev, "WindowClosed"},
        {EventType::WindowResized_ev, "WindowResized"},
        {EventType::WindowFocused_ev, "WindowFocused"},
        {EventType::WindowLostFocus_ev, "WindowLostFocus"},
        {EventType::WindowDpiChanged_ev, "WindowDpiChanged"},
        {EventType::WindowMoved_ev, "WindowMoved"},
        {EventType::ApplicationTick_ev, "ApplicationTick"},
        {EventType::ApplicationUpdate_ev, "ApplicationUpdate"},
        {EventType::ApplicationRender_ev, "ApplicationRender"},
        {EventType::KeyPressed_ev, "KeyPressed"},
        {EventType::KeyReleased_ev, "KeyReleased"},
        {EventType::CharEntered_ev, "CharEntered"},
        {EventType::MouseButtonPressed_ev, "MouseButtonPressed"},
        {EventType::MouseButtonReleased_ev, "MouseButtonReleased"},
        {EventType::MouseButtonDoubleClicked_ev, "MouseButtonDoubleClicked"},
        {EventType::MouseButtonRawInput_ev, "MouseButtonRawInput"},
        {EventType::MouseMoved_ev, "MouseMoved"},
        {EventType::MouseWheelScrolled_ev, "MouseWheelScrolled"},
        {EventType::MouseEnteredWindow_ev, "MouseEnteredWindow"},
        {EventType::MouseExitedWindow_ev, "MouseExitedWindow"},
        {EventType::GamepadConnected_ev, "GamepadConnected"},
        {EventType::GamepadDisconnected_ev, "GamepadDisconnected"},
        {EventType::GamepadButtonPressed_ev, "GamepadButtonPressed"},
        {EventType::GamepadButtonReleased_ev, "GamepadButtonReleased"},
        {EventType::GamepadAxisMoved_ev, "GamepadAxisMoved"},
        {EventType::GamepadStickMoved_ev, "GamepadStickMoved"},
        {EventType::GamepadTriggerMoved_ev, "GamepadTriggerMoved"},
        {EventType::TouchReleased_ev, "TouchReleased"},
        {EventType::TouchPressed_ev, "TouchPressed"},
        {EventType::TouchMoved_ev, "TouchMoved"},
        {EventType::TouchCancelled_ev, "TouchCancelled"},
        {EventType::TouchPinchGesture_ev, "TouchPinchGesture"},
        {EventType::FilesDropped_ev, "FilesDropped"},
        {EventType::DragStarted_ev, "DragStarted"},
        {EventType::DropCompleted_ev, "DropCompleted"},
        {EventType::DataTransfer_ev, "DataTransfer"},
        {EventType::FileTransfer_ev, "FileTransfer"},
        {EventType::GenericInputConnected_ev, "GenericInputConnected"},
        {EventType::GenericInputDisconnected_ev, "GenericInputDisconnected"},
        {EventType::GenericInputPressed_ev, "GenericInputPressed"},
        {EventType::GenericInputReleased_ev, "GenericInputReleased"},
        {EventType::GenericInputAxisMoved_ev, "GenericInputAxisMoved"},
        {EventType::GenericInputHatMoved_ev, "GenericInputHatMoved"},
        {EventType::CustomEvent_ev, "CustomEvent"},
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
        return EventType::None_ev;
    }

}    // namespace nkentseu