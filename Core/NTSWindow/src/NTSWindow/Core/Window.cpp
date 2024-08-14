//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:04:02 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Window.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "NTSWindow/Platform/Window/Windows/Win32Window.h"
namespace nkentseu { using InternalWindow = Win32Window; }
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#include "NTSWindow/Platform/Window/Android/WindowInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#include "NTSWindow/Platform/Window/Linux/XCB/WindowInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#include "NTSWindow/Platform/Window/Linux/XLIB/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_IOS)
#include "NTSWindow/Platform/Window/iOS/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_MACOS)
#include "NTSWindow/Platform/Window/Macos/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_NDS)
#include "NTSWindow/Platform/Window/NDS/WindowInternal.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

#include <NTSWindow/Core/NkentseuLogger.h>

namespace nkentseu {

	Memory::Shared<Window> Window::Create(const WindowProperties& properties)
	{
		auto window_ptr = Memory::AllocateShared<InternalWindow>(properties);
		if (window_ptr != nullptr && window_ptr->IsCreate()) {
			return window_ptr;
		}
		Memory::Reset(window_ptr);
		return Memory::Shared<Window>();
	}
}    // namespace nkentseu