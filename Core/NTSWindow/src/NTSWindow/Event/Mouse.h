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
#include "NTSCore/System.h"

namespace nkentseu {

    // Class representing a mouse object
    class NKENTSEU_API Mouse {
    public:
        // Helper macro to create bitmasks for buttons
        // Example: BUTTON_MASK(Left) will create a bitmask with only the left button bit set (0b00000001)
        #define BUTTON_MASK(X) ((1 << ((X)-1)))

        // Define a type alias for `uint32` representing a mouse button
        using Code = uint64;
        using Button = Code;
        using Wheel = Code;

        struct Buttons{
            // Enum to define all supported mouse buttons
            enum : Code {
                NotDefine = 0,
                Left = 1,
                Middle = 2,
                Right = 3,
                X1 = 4,
                X2 = 5,

                LeftMask = BUTTON_MASK(Left),  // Bitmask for left button
                MiddleMask = BUTTON_MASK(Middle),  // Bitmask for middle button
                RightMask = BUTTON_MASK(Right),  // Bitmask for right button ><
                X1Mask = BUTTON_MASK(X1),        // Bitmask for extra button 1
                X2Mask = BUTTON_MASK(X2),        // Bitmask for extra button 2

                FirstButton = Left,  // The first defined button (Left)
                LastButton = X2Mask,  // The last defined button (Right extra button)
            };

            // Static function to check if a given code represents a valid mouse button
            static bool IsButton(Mouse::Button code);

            // Static function to convert a mouse button enum value to a string representation
            static std::string ToString(Mouse::Button mb);

            // Static function to convert a string representation to a mouse button enum value
            static Mouse::Button FromString(const std::string& mb);
        };

        struct Wheels{
            // Enum to define all supported mouse buttons
            enum : Code {
                NotDefine = 0,
                Horizontal,  // Scrolling horizontally
                Vertical,   // Scrolling vertically
                HorizontalRel,  // Relative horizontal movement
                VerticalRel,   // Relative vertical movement

                FirstWheel = Horizontal,  // The first defined button (Left)
                LastWheel = VerticalRel,  // The last defined button (Right extra button)
            };

            // Static function to check if a given code represents a valid mouse button
            static bool IsWheel(Mouse::Wheel code);

            // Static function to convert a mouse button enum value to a string representation
            static std::string ToString(Mouse::Wheel mb);

            // Static function to convert a string representation to a mouse button enum value
            static Mouse::Wheel FromString(const std::string& mb);
        };
    };

    #define MouseButtons Mouse::Buttons
    #define MouseWheels Mouse::Wheels
} // namespace nkentseu

#endif // __NKENTSEU_MOUSE_H__
