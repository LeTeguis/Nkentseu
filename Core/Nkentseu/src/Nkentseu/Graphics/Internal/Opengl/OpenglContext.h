//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_CONTEXT_H__
#define __OPENGL_INTERNAL_CONTEXT_H__

#pragma once

#include "System/System.h"
#include "System/Definitions/Memory.h"
#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/GraphicsProperties.h"

namespace nkentseu {
    class NativeContext;
    class Window;

    class NKENTSEU_API OpenglContext : public Context
    {
        public:
            OpenglContext(Memory::Shared<Window> window, const ContextProperties& contextProperties);
            ~OpenglContext();

            bool Initialize() override;
            bool Deinitialize() override;
            bool IsInitialize() override;

            bool EnableVSync() override;
            bool DisableVSync() override;

            const GraphicsInfos& GetGraphicsInfo() override;
            const ContextProperties& GetProperties() override;
            Memory::Shared<Window> GetWindow() override;
            Memory::Shared<NativeContext> GetNative();

            bool MakeCurrent();
            bool UnmakeCurrent();
            bool IsCurrent();

            bool Swapchaine();
            bool IsValidContext();
        private:
            Memory::Shared<Window> m_Window = nullptr;
            Memory::Shared<NativeContext> m_NativeContext;

            ContextProperties m_Properties;

    };
} // namespace nkentseu

#endif    // __NKENTSEU_INTERNAL_CONTEXT_H__