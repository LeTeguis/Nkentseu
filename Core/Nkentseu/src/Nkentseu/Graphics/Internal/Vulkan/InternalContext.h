//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INTERNA_LCONTEXT_H__
#define __NKENTSEU_INTERNA_LCONTEXT_H__

#pragma once


#ifdef NKENTSEU_GRAPHICS_API_VULKAN

#include "System/System.h"
#include "Nkentseu/Graphics/Context.h"

namespace nkentseu {
    class NKENTSEU_API InternalContext
    {
        public:
            InternalContext();
            ~InternalContext();

            bool Initialize(Window* window, const ContextProperties& contextProperties);
            bool Deinitialize();

            bool MakeCurrent();
            bool UnmakeCurrent();
            bool IsCurrent();

            Window* GetWindow();
            const ContextProperties& GetProperties();
        private:
    };
} // namespace nkentseu


#endif


#endif    // __NKENTSEU_INTERNA_LCONTEXT_H__