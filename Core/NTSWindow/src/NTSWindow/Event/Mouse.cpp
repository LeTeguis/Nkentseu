//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Mouse.h"

namespace nkentseu {

    /*/ Implementation of the static method IsButton
    bool Mouse::Buttons::IsButton(Mouse::Button code) {
        // Check if the code is within the valid button range (1-5)
        
    }

    // Implementation of the static method ToString
    std::string Mouse::Buttons::ToString(Mouse::Button mb) {
        
    }

    // Implementation of the static method FromString
    Mouse::Button Mouse::Buttons::FromString(const std::string& mb) {
        
    }

    // Implementation of the static method IsButton
    bool Mouse::Wheels::IsWheel(Mouse::Wheel code) {
        // Check if the code is within the valid button range (1-9)
        
    }

    // Implementation of the static method ToString
    std::string Mouse::Wheels::ToString(Mouse::Wheel mb) {
        
    }

    // Implementation of the static method FromString
    Mouse::Wheel Mouse::Wheels::FromString(const std::string& mb) {
         // Or throw an exception for invalid string
    }*/

    bool Mouse::Buttons::IsButton(Mouse::Button code)
    {
        return (code >= Mouse::Buttons::FirstButton && code <= Mouse::Buttons::X2) ||
            // Or it's a valid button mask
            (code == Buttons::LeftMask || code == Buttons::MiddleMask || code == Buttons::RightMask || code == Buttons::X1Mask || code == Buttons::X2Mask);
    }

    std::string Mouse::Buttons::ToString(Mouse::Button mb)
    {
        switch (mb) {
        case NotDefine: return "NotDefine";
        case Left: return "Left";
        case Middle: return "Middle";
        case Right: return "Right";
        case X1: return "X1";
        case X2: return "X2";
        }
        return "NotDefine";
    }

    Mouse::Button Mouse::Buttons::FromString(const std::string& mb)
    {
        if (mb == "NotDefine") return NotDefine;
        else if (mb == "Left") return Left;
        else if (mb == "Middle") return Middle;
        else if (mb == "Right") return Right;
        else if (mb == "X1") return X1;
        else if (mb == "X2") return X2;
        else return NotDefine; // Or throw an exception for invalid string
    }

    bool Mouse::Wheels::IsWheel(Mouse::Wheel code)
    {
        return (code >= Wheels::FirstWheel && code <= Wheels::LastWheel);
    }

    std::string Mouse::Wheels::ToString(Mouse::Wheel mb)
    {
        switch (mb) {
        case Horizontal: return "Horizontal";
        case Vertical: return "Vertical";
        case HorizontalRel: return "HorizontalRel";
        case VerticalRel: return "VerticalRel";
        }
        return "NotDefine";
    }

    Mouse::Wheel Mouse::Wheels::FromString(const std::string& mb)
    {
        if (mb == "NotDefine") return NotDefine;
        else if (mb == "Horizontal") return Horizontal;
        else if (mb == "Vertical") return Vertical;
        else if (mb == "HorizontalRel") return HorizontalRel;
        else if (mb == "VerticalRel") return VerticalRel;
        else return NotDefine;
    }

}    // namespace nkentseu