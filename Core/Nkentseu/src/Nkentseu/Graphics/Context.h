//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:48:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_CONTEXT_H__
#define __NKENTSEU_CONTEXT_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <System/Definitions/Memory.h>

namespace nkentseu {
    class InternalContext;
    class Window;

    struct InternalContextInfo {
        Memory::Shared<InternalContext> internalInfo = null_pointer;
        bool isLoad;
    };

    struct NKENTSEU_API ContextProperties {

    };

    class NKENTSEU_API Context
    {
        public:
            Context();
            Context(class Window* window, const ContextProperties& contextProperties);
            ~Context();

            bool Initialize(class Window* window, const ContextProperties& contextProperties);

            virtual InternalContextInfo* GetInternal();
        private:
            Memory::Shared<InternalContextInfo> m_InternalContextInfo = null_pointer;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_CONTEXT_H__