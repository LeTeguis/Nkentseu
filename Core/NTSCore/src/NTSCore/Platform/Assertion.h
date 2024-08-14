//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_SYSTEM_ASSERTION_H__
#define __NKENTSEU_SYSTEM_ASSERTION_H__

#pragma once


#include "Platform.h"


#ifdef NKENTSEU_DEBUG
	#include <cassert>
	#if defined(NKENTSEU_PLATFORM_WINDOWS)
		#define NKENTSEU_ASSERT_BREAK __debugbreak();
	#elif defined(NKENTSEU_PLATFORM_UNIX)
		#include <signal.h>
		#define NKENTSEU_ASSERT_BREAK raise(SIGTRAP);
	#elif defined(NKENTSEU_PLATFORM_APPLE)
		#define NKENTSEU_ASSERT_BREAK __builtin_debugtrap();
	#else
		#error "Platform doesn't support debugbreak yet!"
		#define NKENTSEU_ASSERT_BREAK
	#endif
#else
    #define NKENTSEU_ASSERT_BREAK
#endif

#endif    // __NKENTSEU_SYSTEM_ASSERTION_H__