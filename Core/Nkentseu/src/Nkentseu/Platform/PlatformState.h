//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 10:00:48 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_PLATFORM_STATE_H__
#define __NKENTSEU_PLATFORM_STATE_H__

#pragma once

#include "System/System.h"
#include <vector>
#include <System/Nature/Base.h>

#if defined(NKENTSEU_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

#if defined(NKENTSEU_PLATFORM_LINUX)
#include <xcb/xcb.h>
#endif

#if defined(NKENTSEU_PLATFORM_ANDROID)
#endif

#if defined(NKENTSEU_PLATFORM_IOS)
#endif

#if defined(NKENTSEU_PLATFORM_MACOS)
#endif

#if defined(NKENTSEU_PLATFORM_NDS)
#endif

namespace nkentseu {
    class NKENTSEU_API PlatformState_ {
    public:
        static PlatformState_& Instance() {
            static PlatformState_ state;
            return state;
        }

        ARGV argv;

        #if defined(NKENTSEU_PLATFORM_WINDOWS)

        HINSTANCE   HInstance;
        HINSTANCE   HPrevInstance;
        LPSTR       LPCmdLine;
        int32       NCmdShow;

        void Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow);

        #elif defined(NKENTSEU_PLATFORM_ANDROID)

        android_app* App;

        void Init(android_app* app);

        #elif defined(NKENTSEU_PLATFORM_LINUX)
        int screenNumber = 0;
        xcb_connection_t* connection = NULL;

        void Init(int argc, const char** argv);

        void Close();
        #else

        void Init(int argc, const char** argv);

        #endif

        PlatformState_();
    };

    #define PlatformState		PlatformState_::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_PLATFORM_STATE_H__