//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INTERNALCONTEXT_H__
#define __NKENTSEU_INTERNALCONTEXT_H__

#pragma once


#ifdef NKENTSEU_GRAPHICS_API_SOFTWARE

#include "System/System.h"
#include "Nkentseu/Graphics/Context.h"

namespace nkentseu {
    class NKENTSEU_API InternalContext
    {
        public:
            InternalContext();
            ~InternalContext();

            bool Initialize(Window* window, const ContextProperties& contextProperties);
        private:
    };
} // namespace nkentseu

#endif

#endif    // __NKENTSEU_INTERNALCONTEXT_H__