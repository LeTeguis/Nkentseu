//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INTERNAL_CONTEXT_H__
#define __NKENTSEU_INTERNAL_CONTEXT_H__

#pragma once

#ifdef NKENTSEU_GRAPHICS_API_OPENGL

#include "System/System.h"
#include "Nkentseu/Graphics/Context.h"

#include "Nkentseu/Core/Window.h"

namespace nkentseu {
    class NKENTSEU_API InternalContext
    {
        public:
            InternalContext();
            ~InternalContext();

            bool Initialize(Window* window, const ContextProperties& contextProperties);
        private:

#ifdef  NKENTSEU_PLATFORM_WINDOWS

#elif defined NKENTSEU_PLATFORM_LINUX

#else

#endif //  NKENTSEU_PLATFORM_WINDOWS

    };
} // namespace nkentseu

#endif

#endif    // __NKENTSEU_INTERNAL_CONTEXT_H__