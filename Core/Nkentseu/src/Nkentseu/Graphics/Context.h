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
#include "GraphicsProperties.h"

namespace nkentseu {
    class Window;

    class NKENTSEU_API Context
    {
        public:
            virtual bool Initialize() = 0;
            virtual bool Deinitialize() = 0;
            virtual bool IsInitialize() = 0;

            virtual bool EnableVSync() = 0;
            virtual bool DisableVSync() = 0;

            virtual bool Prepare() { return true; };
            virtual bool Present() { return true; };

            virtual const GraphicsInfos& GetGraphicsInfo() = 0;
            virtual const ContextProperties& GetProperties() = 0;

            virtual Memory::Shared<Window> GetWindow() = 0;

            static Memory::Shared<Context> Create(Memory::Shared<Window> window, const ContextProperties& contextProperties);
            static Memory::Shared<Context> CreateInitialized(Memory::Shared<Window> window, const ContextProperties& contextProperties);
    };
} // namespace nkentseu

#endif    // __NKENTSEU_CONTEXT_H__