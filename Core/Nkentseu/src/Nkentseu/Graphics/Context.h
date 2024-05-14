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
    class InternalContext;
    class Window;

    struct InternalContextInfo {
        Memory::Shared<InternalContext> internalInfo = null_pointer;
        bool isLoad = false;
    };

    class NKENTSEU_API Context
    {
        public:
            Context();
            Context(class Window* window, const ContextProperties& contextProperties);
            ~Context();

            bool SetWindow(class Window* window);
            bool SetProperties(const ContextProperties& properties);

            bool Initialize();
            bool Initialize(class Window* window, const ContextProperties& contextProperties);
            bool Deinitialize();
            bool IsInitialize();

            bool MakeCurrent();
            bool UnmakeCurrent();
            bool IsCurrent();

            bool EnableVSync();
            bool DisableVSync();

            bool Present();
            bool Swapchaine();

            const GraphicsInfos& GetGraphicsInfo();

            class Window* GetWindow();

            virtual InternalContextInfo* GetInternal();
            const ContextProperties& GetProperties();
        private:
            Memory::Shared<InternalContextInfo> m_InternalContextInfo = null_pointer;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_CONTEXT_H__