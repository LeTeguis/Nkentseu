//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EVENT_CATEGORY_H__
#define __NKENTSEU_EVENT_CATEGORY_H__

#pragma once

// Include necessary headers
#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {

    // Class representing different event categories in the system
    class NKENTSEU_API EventCategory {
    public:

        // Define a typedef for event category codes using uint32 for efficiency
        using Code = uint32_t;  // Use std::uint32_t for modern C++

        // Enum to represent various event categories using bit flags
        enum : Code {
            None_ev = 0,

            // Application category (for events related to the application itself)
            Application_ev = BIT(0),

            // Input category (for events related to user input)
            Input_ev = BIT(1),

            // Subcategories within Input (more specific categories can be added)
            Keyboard_ev = BIT(2),
            Mouse_ev = BIT(3),
            MouseButton_ev = BIT(4),  // Consider using a separate enum for mouse button events

            Window_ev = BIT(5),
            Graphics_ev = BIT(6),
            Touch_ev = BIT(7),
            Gamepad_ev = BIT(8),
            Custom_ev = BIT(9),
            Transfer_ev = BIT(10),
            GenericInput_ev = BIT(11),
            DragAndDrop_ev = BIT(12)
        };

        // Function to convert an event category code to a human-readable string
        static const std::string ToString(EventCategory::Code);

        // Function to convert a human-readable string to an event category code (if valid)
        static EventCategory::Code FromString(const std::string& type);
    };
} // namespace nkentseu

#endif // __NKENTSEU_EVENT_CATEGORY_H__
