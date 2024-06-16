//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:04:02 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Window.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "Nkentseu/Platform/Window/Windows/Win32Window.h"
namespace nkentseu { using InternalWindow = Win32Window; }
#elif defined(NKENTSEU_PLATFORM_ANDROID)
#include "Nkentseu/Platform/Window/Android/WindowInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#include "Nkentseu/Platform/Window/Linux/XCB/WindowInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XLIB)
#include "Nkentseu/Platform/Window/Linux/XLIB/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_IOS)
#include "Nkentseu/Platform/Window/iOS/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_MACOS)
#include "Nkentseu/Platform/Window/Macos/WindowInternal.h"
#elif defined(NKENTSEU_PLATFORM_NDS)
#include "Nkentseu/Platform/Window/NDS/WindowInternal.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {

	Memory::Shared<Window> Window::Create(const WindowProperties& properties)
	{
		auto window_ptr = Memory::Alloc<InternalWindow>(properties);
		if (window_ptr != nullptr && window_ptr->IsCreate()) {
			return window_ptr;
		}
		Memory::Reset(window_ptr);
		return Memory::Shared<Window>();
	}
}    // namespace nkentseu