//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "EventCategory.h"

namespace nkentseu {

    // Map to store event category codes and their corresponding strings
    static std::unordered_map<EventCategory::Code, std::string> eventCategoryStrings = {
      {EventCategory::None_ev, "None"},
      {EventCategory::Application_ev, "Application"},
      {EventCategory::Input_ev, "Input"},
      {EventCategory::Keyboard_ev, "Keyboard"},
      {EventCategory::Mouse_ev, "Mouse"},
      {EventCategory::MouseButton_ev, "MouseButton"},
      {EventCategory::Window_ev, "Window"},
      {EventCategory::Graphics_ev, "Graphics"},
      {EventCategory::Touch_ev, "Touch"},
      {EventCategory::Gamepad_ev, "Gamepad"},
      {EventCategory::Custom_ev, "Custom"},
      {EventCategory::Transfer_ev, "Transfer"},
      {EventCategory::GenericInput_ev, "GenericInput"},
      {EventCategory::DragAndDrop_ev, "DragDrop"},
    };

    // Function to convert an event category code to a human-readable string
    const std::string EventCategory::ToString(EventCategory::Code code) {
        // Use iterator to find the corresponding string
        auto it = eventCategoryStrings.find(code);
        if (it != eventCategoryStrings.end()) {
            return it->second;
        }
        else {
            // Return an empty string if code not found
            return "None";
        }
    }

    // Function to convert a human-readable string to an event category code (if valid)
    EventCategory::Code EventCategory::FromString(const std::string& type) {
        EventCategory::Code code = EventCategory::None_ev;
        // Loop through the map to find matching string, also consider bit flags
        for (const auto& pair : eventCategoryStrings) {
            if (pair.second == type) {
                code |= pair.first;
            }
        }
        return code;
    }


}    // namespace nkentseu