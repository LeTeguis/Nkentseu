//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
#include "NkentseuPch/ntspch.h"
#include "InternalContext.h"

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

namespace nkentseu {

    InternalContext::InternalContext(){
    }

    InternalContext::~InternalContext(){
    }

    bool nkentseu::InternalContext::Initialize(Window* window, const ContextProperties& contextProperties)
    {
        return false;
    }

}    // namespace nkentseu

#endif