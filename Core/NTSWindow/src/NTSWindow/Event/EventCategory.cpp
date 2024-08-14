//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "EventCategory.h"

namespace nkentseu {

    // Map to store event category codes and their corresponding strings
    static std::unordered_map<EventCategory::Code, std::string> eventCategoryStrings = {
      {EventCategory::NotDefine, "NotDefine"},
      {EventCategory::Application, "Application"},
      {EventCategory::Input, "Input"},
      {EventCategory::Keyboard, "Keyboard"},
      {EventCategory::Mouse, "Mouse"},
      {EventCategory::Window, "Window"},
      {EventCategory::Graphics, "Graphics"},
      {EventCategory::Touch, "Touch"},
      {EventCategory::Gamepad, "Gamepad"},
      {EventCategory::Custom, "Custom"},
      {EventCategory::Transfer, "Transfer"},
      {EventCategory::GenericHid, "GenericInput"},
      {EventCategory::DragAndDrop, "DragDrop"},
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
            return "NotDefine";
        }
    }

    // Function to convert a human-readable string to an event category code (if valid)
    EventCategory::Code EventCategory::FromString(const std::string& type) {
        EventCategory::Code code = EventCategory::NotDefine;
        // Loop through the map to find matching string, also consider bit flags
        for (const auto& pair : eventCategoryStrings) {
            if (pair.second == type) {
                code |= pair.first;
            }
        }
        return code;
    }


}    // namespace nkentseu