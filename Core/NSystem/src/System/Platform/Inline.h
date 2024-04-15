//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INLINE_H__
#define __NKENTSEU_INLINE_H__

#pragma once

#if defined(__clang__) || defined(__gcc__) || defined(__GNUC__)

	#define NKENTSEU_INLINE			__attribute__((always_inline)) inline
	#define NKENTSEU_NO_INLINE		__attribute__((noinline))

#elif defined(_MSC_VER)

	#define NKENTSEU_INLINE			__forceinline
	#define NKENTSEU_NO_INLINE		__declspec(noinline)

#else

	#define NKENTSEU_INLINE			static inline
	#define NKENTSEU_NO_INLINE

#endif

#endif    // __NKENTSEU_INLINE_H__