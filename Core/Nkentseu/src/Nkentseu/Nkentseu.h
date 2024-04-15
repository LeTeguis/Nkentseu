//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024/04/05.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef NKENTSEU_NKENTSEU_H_
#define NKENTSEU_NKENTSEU_H_

#pragma once

#include "System/System.h"
#include "System/Definitions/Memory.h"
#include "Core/NkentseuLogger.h"

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include "Platform/Main/WindowsMain.h"
#elif defined(NKENTSEU_PLATFORM_ANDROID)
//#include "Platform/Main/AndroidMain.h"
#elif defined(NKENTSEU_PLATFORM_LINUX)
#include "Platform/Main/LinuxMain.h"
#elif defined(NKENTSEU_PLATFORM_IOS)
//#include "Platform/Main/IOSMain.h"
#elif defined(NKENTSEU_PLATFORM_MACOS)
//#include "Platform/Main/MacosMain.h"
#elif defined(NKENTSEU_PLATFORM_NDS)
//#include "Platform/Main/NDSMain.h"
#else
#error "Plaform ("  + STR_PLATFORM + ") cannot supported"
#endif

#include "Core/Window.h"


#endif  // NKENTSEU_NKENTSEU_H_