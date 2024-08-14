//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_EXPORTS_H__
#define __NKENTSEU_EXPORTS_H__

#pragma once


#include "Platform.h"

#if !defined(NKENTSEU_STATIC)

	#if defined(NKENTSEU_PLATFORM_WINDOWS)

		#define NKENTSEU_API_EXPORT __declspec(dllexport)
		#define NKENTSEU_API_IMPORT __declspec(dllimport)

		#ifdef _MSC_VER
			#pragma warning(disable : 4251)
		#endif

	#else

		#define NKENTSEU_API_EXPORT __attribute__((__visibility__("default")))
		#define NKENTSEU_API_IMPORT __attribute__((__visibility__("default")))

	#endif

#else

	#define NKENTSEU_API_EXPORT
	#define NKENTSEU_API_IMPORT

#endif

#if defined(NKENTSEU_EXPORTS)
	#define NKENTSEU_API NKENTSEU_API_EXPORT
#else
	#define NKENTSEU_API NKENTSEU_API_IMPORT
#endif

#endif    // __NKENTSEU_EXPORTS_H__