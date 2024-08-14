//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024/04/05.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Platform.h"

namespace nkentseu
{
    Platform::Type Platform::GetPlatform() {
        #if defined(NKENTSEU_PLATFORM_WIN64)
            return Win64;
        #elif defined(NKENTSEU_PLATFORM_WIN32)
            return Win32;
        #elif defined(NKENTSEU_PLATFORM_ANDROID)
            return Android;
        #elif defined(NKENTSEU_PLATFORM_IOS)
            /*if constexpr (sizeof(void*) == 8) {
                return Mac;
            }*/
            return iOS;
        #elif defined(NKENTSEU_PLATFORM_MACOS)
            return Mac;
        #elif defined(NKENTSEU_PLATFORM_NDS)
            return Nds;
        #elif defined(NKENTSEU_PLATFORM_LINUX)
            return Linux;
        #elif defined(NKENTSEU_PLATFORM_FREEBSD)
            return FreeBsd;
        #elif defined(NKENTSEU_PLATFORM_OPENBSD)
            return OpenBsd;
        #elif defined(NKENTSEU_PLATFORM_NETBSD)
            return NetBsd;
            /*if constexpr (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) {
                char* name = nullptr;
                uname(&name);
                if (std::string(name).find("Linux") != std::string::npos) {
                    return Linux;
                }
            }*/
        #endif
        return Unknow;
    }

    std::string Platform::GetPlatformName(Platform::Type platform) {
        return STR_PLATFORM;
    }
} // namespace nkentseu
