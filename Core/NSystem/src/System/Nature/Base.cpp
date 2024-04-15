//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "Base.h"

namespace nkentseu {
    std::string ToString(bool value) {
        std::stringstream ss;
        ss << ((value) ? "True" : "False");
        return ss.str();
    }
}