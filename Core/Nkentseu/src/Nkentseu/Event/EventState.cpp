//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/25/2024 at 8:50:07 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "EventState.h"

#include <Logger/Formatter.h>

namespace nkentseu {
    ModifierState::ModifierState() : control(false), alt(false), shift(false), super(false) {}

    ModifierState::ModifierState(bool control, bool alt, bool shift, bool super)
        : control(control), alt(alt), shift(shift), super(super) {}

	std::string ModifierState::ToString() const {
        return FORMATTER.Format("ModifierState {control : {0}, alt : {1}, shift : {2}, meta : {3}}", STR_BOOL(control), STR_BOOL(alt), STR_BOOL(shift), STR_BOOL(super));
    }

}    // namespace nkentseu