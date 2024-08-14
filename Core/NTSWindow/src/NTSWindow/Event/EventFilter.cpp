//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/13/2024 at 5:02:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "EventFilter.h"


#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "NTSWindow/Platform/Window/Windows/Win32Event.h"
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#include "Nkentseu/Platform/Window/Android/WindowEventInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#include "Nkentseu/Platform/Window/Linux/XCB/WindowEventInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#include "Nkentseu/Platform/Window/Linux/XLIB/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_IOS)
#include "Nkentseu/Platform/Window/iOS/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_MACOS)
#include "Nkentseu/Platform/Window/Macos/WindowEventInternal.h"
#elif defined(NKENTSEU_PLATFORM_NDS)
#include "Nkentseu/Platform/Window/NDS/WindowEventInternal.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

#include "NTSWindow/Core/NkentseuLogger.h"

namespace nkentseu {
    
    EventListener& EventListener::Instance()
    {
#ifdef NKENTSEU_PLATFORM_WINDOWS
        return Win32Event::UniqueInstance();
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#elif defined(NKENTSEU_PLATFORM_IOS)
#elif defined(NKENTSEU_PLATFORM_MACOS)
#elif defined(NKENTSEU_PLATFORM_NDS)
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif
    }

}    // namespace nkentseu