//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_PLATFORM_H__
#define __NKENTSEU_PLATFORM_H__

#pragma once

#define NKENTSEU_VERSION_MAJOR      1
#define NKENTSEU_VERSION_MINOR      0

#if defined(_WIN64)
	#define NKENTSEU_PLATFORM_WINDOWS
	#define NKENTSEU_PLATFORM_WIN64
	#define STR_PLATFORM "Windows 64 bits"
#elif defined(_WIN32)
	#define NKENTSEU_PLATFORM_WINDOWS
	#define NKENTSEU_PLATFORM_WIN32
	#define STR_PLATFORM "Windows 32 bits"
#elif defined(__unix__)
	#define NKENTSEU_PLATFORM_UNIX
	#if defined(__ANDROID__)
		#define NKENTSEU_PLATFORM_ANDROID
		#define STR_PLATFORM "Android"
	#elif defined(__linux__)
		#define NKENTSEU_PLATFORM_LINUX
		#define STR_PLATFORM "Linux"
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
		#define NKENTSEU_PLATFORM_FREEBSD
		#define STR_PLATFORM "FreeBsd"
	#elif defined(__OpenBSD__)
		#define NKENTSEU_PLATFORM_OPENBSD
		#define STR_PLATFORM "OpenBsd"
	#elif defined(__NetBSD__)
		#define NKENTSEU_PLATFORM_NETBSD
		#define STR_PLATFORM "NetBsd"
	#else
		#error "This UNIX operating system is not supported by NKENTSEU"
		#define NKENTSEU_PLATFORM_NO_DEFINE
		#define STR_PLATFORM "Unknow"
		#undef NKENTSEU_PLATFORM_UNIX
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	#include "TargetConditionals.h"
	#define NKENTSEU_PLATFORM_APPLE

	#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
		#define NKENTSEU_PLATFORM_IOS
		#define STR_PLATFORM "iOS"
	#elif defined(TARGET_OS_MAC)
		#define NKENTSEU_PLATFORM_MACOS
		#define STR_PLATFORM "MacOS"
	#else
		#error "This Apple operating system is not supported by NKENTSEU"
		#define NKENTSEU_PLATFORM_NO_DEFINE
		#define STR_PLATFORM "Unknow"
		#undef NKENTSEU_PLATFORM_APPLE
	#endif
#elif defined(NDS) || defined(__DS__)
	#define NKENTSEU_PLATFORM_NDS
	#define STR_PLATFORM "Nintendo DS"
#else
	#error "This operating system is not supported by NKENTSEU"
	#define NKENTSEU_PLATFORM_NO_DEFINE
	#define STR_PLATFORM "Unknow"
#endif

#include <string>

namespace nkentseu {
	class Platform {
		public:
		using Type = unsigned char;

		enum : Type {
			Unknow,
			Win32,
			Win64,
			Android,
			iOS,
			Mac,
			Nds,
			Linux,
			FreeBsd,
			NetBsd,
			OpenBsd
		};

		static Type GetPlatform() ;

		static std::string GetPlatformName(Type platform);
	};
}

#endif    // __NKENTSEU_PLATFORM_H__