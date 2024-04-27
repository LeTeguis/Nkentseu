//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 4:58:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEUENT_CATEGORY_H__
#define __NKENTSEUENT_CATEGORY_H__

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
            NotDefine = 0,

            // Application category (for events related to the application itself)
            Application = BIT(1),

            // Input category (for events related to user input)
            Input = BIT(2),

            // Subcategories within Input (more specific categories can be added)
            Keyboard = BIT(3),
            Mouse = BIT(4),
            //MouseButton = BIT(4),  // Consider using a separate enum for mouse button events

            Window = BIT(5),
            Graphics = BIT(6),
            Touch = BIT(7),
            Gamepad = BIT(8),
            Custom = BIT(9),
            Transfer = BIT(10),
            GenericHid = BIT(11),
            DragAndDrop = BIT(12)
        };

        // Function to convert an event category code to a human-readable string
        static const std::string ToString(EventCategory::Code);

        // Function to convert a human-readable string to an event category code (if valid)
        static EventCategory::Code FromString(const std::string& type);
    };
} // namespace nkentseu

#endif // __NKENTSEUENT_CATEGORY_H__
