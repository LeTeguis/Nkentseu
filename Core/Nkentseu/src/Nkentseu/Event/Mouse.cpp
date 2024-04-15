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
            (code == LeftMask_ev || code == MiddleMask_ev || code == RightMask_ev || code == X1Mask_ev || code == X2Mask_ev);
    }

    // Implementation of the static method ToString
    std::string Mouse::ToString(Mouse::Button mb) {
        switch (mb) {
        case Unknown_ev: return "None";
        case Left_ev: return "Left";
        case Middle_ev: return "Middle";
        case Right_ev: return "Right";
        case X1_ev: return "X1";
        case X2_ev: return "X2";
        case Horizontal_ev: return "Horizontal";
        case Vertical_ev: return "Vertical";
        case HorizontalRel_ev: return "HorizontalRel";
        case VerticalRel_ev: return "VerticalRel";
        default_ev: return "Unknown";
        }
    }

    // Implementation of the static method FromString
    Mouse::Button Mouse::FromString(const std::string& mb) {
        if (mb == "None") return Unknown_ev;
        else if (mb == "Left") return Left_ev;
        else if (mb == "Middle") return Middle_ev;
        else if (mb == "Right") return Right_ev;
        else if (mb == "X1") return X1_ev;
        else if (mb == "X2") return X2_ev;
        else if (mb == "Horizontal") return Horizontal_ev;
        else if (mb == "Vertical") return Vertical_ev;
        else if (mb == "HorizontalRel") return HorizontalRel_ev;
        else if (mb == "VerticalRel") return VerticalRel_ev;
        else return Unknown_ev; // Or throw an exception for invalid string
    }

}    // namespace nkentseu