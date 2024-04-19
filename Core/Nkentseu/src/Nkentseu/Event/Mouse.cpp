//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:04:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Mouse.h"

namespace nkentseu {

    // Implementation of the static method IsButton
    bool Mouse::IsButton(Mouse::Button code) {
        // Check if the code is within the valid button range (1-9)
        return (code >= 1 && code <= 9) ||
            // Or it's a valid button mask
            (code == LeftMask || code == MiddleMask || code == RightMask || code == X1Mask || code == X2Mask);
    }

    // Implementation of the static method ToString
    std::string Mouse::ToString(Mouse::Button mb) {
        switch (mb) {
        case NotDefine: return "NotDefine";
        case Left: return "Left";
        case Middle: return "Middle";
        case Right: return "Right";
        case X1: return "X1";
        case X2: return "X2";
        case Horizontal: return "Horizontal";
        case Vertical: return "Vertical";
        case HorizontalRel: return "HorizontalRel";
        case VerticalRel: return "VerticalRel"; 
        }
        return "NotDefine";
    }

    // Implementation of the static method FromString
    Mouse::Button Mouse::FromString(const std::string& mb) {
        if (mb == "NotDefine") return NotDefine;
        else if (mb == "Left") return Left;
        else if (mb == "Middle") return Middle;
        else if (mb == "Right") return Right;
        else if (mb == "X1") return X1;
        else if (mb == "X2") return X2;
        else if (mb == "Horizontal") return Horizontal;
        else if (mb == "Vertical") return Vertical;
        else if (mb == "HorizontalRel") return HorizontalRel;
        else if (mb == "VerticalRel") return VerticalRel;
        else return NotDefine; // Or throw an exception for invalid string
    }

}    // namespace nkentseu