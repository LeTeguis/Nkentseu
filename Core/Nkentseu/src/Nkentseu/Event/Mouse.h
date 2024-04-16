//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

// This header file defines the `Mouse` class within the `nkentseu` namespace.
// It is included by other parts of the code that need to interact with mouse input.
#ifndef __NKENTSEU_MOUSE_H__
#define __NKENTSEU_MOUSE_H__

#pragma once

// Include necessary header files
#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {

    // Class representing a mouse object
    class NKENTSEU_API Mouse {
    public:
        // Helper macro to create bitmasks for buttons
        // Example: BUTTON_MASK(Left) will create a bitmask with only the left button bit set (0b00000001)
#define BUTTON_MASK(X) ((1 << ((X)-1)))

// Define a type alias for `uint32` representing a mouse button
        using Button = int64;

        // Enum to define all supported mouse buttons
        enum : Button {
            Unknown_ev = 0,
            Left_ev = 1,
            Middle_ev = 2,
            Right_ev = 3,
            X1_ev = 4,
            X2_ev = 5,
            Horizontal_ev = 6,  // Scrolling horizontally
            Vertical_ev = 7,   // Scrolling vertically
            HorizontalRel_ev = 8,  // Relative horizontal movement
            VerticalRel_ev = 9,   // Relative vertical movement
            LeftMask_ev = BUTTON_MASK(Left_ev),  // Bitmask for left button
            MiddleMask_ev = BUTTON_MASK(Middle_ev),  // Bitmask for middle button
            RightMask_ev = BUTTON_MASK(Right_ev),  // Bitmask for right button
            X1Mask_ev = BUTTON_MASK(X1_ev),        // Bitmask for extra button 1
            X2Mask_ev = BUTTON_MASK(X2_ev),        // Bitmask for extra button 2

            Num_ev = 14,  // Total number of buttons defined
            FistButton_ev = Left_ev,  // The first defined button (Left)
            LastButton_ev = X2Mask_ev,  // The last defined button (Right extra button)
        };

        // Static function to check if a given code represents a valid mouse button
        static bool IsButton(Mouse::Button code);

        // Static function to convert a mouse button enum value to a string representation
        static std::string ToString(Mouse::Button mb);

        // Static function to convert a string representation to a mouse button enum value
        static Mouse::Button FromString(const std::string& mb);
    };

    #define MOUSE(button) Mouse::button##_ev
} // namespace nkentseu

#endif // __NKENTSEU_MOUSE_H__
