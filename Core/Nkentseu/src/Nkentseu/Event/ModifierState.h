//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 5:52:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MODIFIER__STATE_H__
#define __NKENTSEU_MODIFIER__STATE_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {

    class ButtonState {
    public:
        // ** Type alias for button state **
        // This alias defines a `ButtonState` type as a `uint32`. It can be used to represent 
        // different states of buttons (Down, Up, None).
        using Code = uint32;

        // ** Enum for button states **
        // This enum defines three possible states for buttons: Down, Up, and None.
        enum : Code {
            Down,  // Button is currently pressed
            Up,    // Button is currently released
            NotDefine  // Button state is unknown or not applicable
        };
    };

    // ** Class representing modifier key state **

    class NKENTSEU_API ModifierState {
    public:

        // ** Member variables for individual modifier keys **
        // These boolean variables indicate the state of specific modifier keys:
        //  - Ctrl: Control key
        //  - Alt: Alt key
        //  - Shift: Shift key
        //  - Super: Meta buttons (e.g., Windows key, Mac's Command key)
        bool Ctrl;
        bool Alt;
        bool Shift;
        bool Super;

        // ** Friend function for streaming ModifierState objects to ostream **
        // This friend function allows you to directly output a `ModifierState` object to a stream 
        // like `std::cout`. The actual implementation of `ToString` is likely defined elsewhere.
        friend std::ostream& operator<<(std::ostream& os, const ModifierState& e) {
            return os << e.ToString();
        }

        // ** Method to convert ModifierState to a string **
        // This method likely converts the current modifier state into a human-readable string representation.
        // The implementation is likely defined elsewhere.
        std::string ToString() const;

        // ** Constructor to initialize ModifierState **
        // This constructor allows you to set the initial state of each modifier key during object creation.
        // You can provide default values (false) or specify individual states.
        ModifierState(bool ctrl = false, bool alt = false, bool shift = false, bool super = false);

        // ** Copy constructor for ModifierState **
        // This constructor creates a copy of another `ModifierState` object.
        ModifierState(const ModifierState& ms);

        // ** Overloaded comparison operators **
        // These operators allow you to compare two `ModifierState` objects for equality and inequality.
        bool operator==(const ModifierState& other) const;
        bool operator!=(const ModifierState& other) const;
    };
} // namespace nkentseu

#endif  // __NKENTSEU_MODIFIER__STATE_H__
