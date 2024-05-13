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

#ifdef  NKENTSEU_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined NKENTSEU_PLATFORM_LINUX

#else

#endif //  NKENTSEU_PLATFORM_WINDOWS

namespace nkentseu {
    class NativeContext;


    class NKENTSEU_API InternalContext
    {
        public:
            InternalContext();
            ~InternalContext();

            bool SetWindow(class Window* window);
            bool SetProperties(const ContextProperties& properties);

            bool Initialize();
            bool Initialize(Window* window, const ContextProperties& contextProperties);
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

            Memory::Shared<class NativeContext> GetNativeContext();

            Window* GetWindow();
            const ContextProperties& GetProperties();
        private:
            Window* m_Window = nullptr;
            Memory::Shared<class NativeContext> m_NativeContext;

    };
} // namespace nkentseu

#endif

#endif    // __NKENTSEU_INTERNAL_CONTEXT_H__