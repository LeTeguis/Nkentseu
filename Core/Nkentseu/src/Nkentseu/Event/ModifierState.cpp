//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 5:52:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "ModifierState.h"

namespace nkentseu {

    std::string ModifierState::ToString() const {
        std::stringstream ss;
        ss << "ctrl(" << STR_BOOL(Ctrl) << "), " << " alt(" << STR_BOOL(Alt) << "), " << " shift(" << STR_BOOL(Shift) << "), " << " super(" << STR_BOOL(Super) << ")";
        return ss.str();
    }

    ModifierState::ModifierState(bool ctrl, bool alt, bool shift, bool super) : Ctrl(ctrl), Alt(alt), Shift(shift), Super(super) {}

    ModifierState::ModifierState(const ModifierState& ms) : Ctrl(ms.Ctrl), Alt(ms.Alt), Shift(ms.Shift), Super(ms.Super) {}

    bool ModifierState::operator==(const ModifierState& other) const {
        return Ctrl == other.Ctrl && Alt == other.Alt && Shift == other.Shift && Super == other.Super;
    }

    bool ModifierState::operator!=(const ModifierState& other) const {
        return !(*this == other);
    }

}    // namespace nkentseu