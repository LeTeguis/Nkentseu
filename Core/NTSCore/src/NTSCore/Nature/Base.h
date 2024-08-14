//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_BASE_H__
#define __NKENTSEU_BASE_H__

#pragma once

#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Platform/Inline.h"
#include "Enumeration.h"

namespace nkentseu{
    using ARGV = std::vector<std::string>;

    #define BIT(x) (1 << x)
    #define STDPH(v) std::placeholders::_##v

    #define STR_BOOL(b) ((b) ? "True" : "False")

    // Unsigned int types.
    typedef unsigned long               ulong;
    typedef unsigned long long          usize;
    typedef unsigned char               uint8;
    typedef unsigned short              uint16;
    typedef unsigned int                uint32;
    typedef unsigned long int           uintl32;
    typedef uintl32                     uisize;
    typedef unsigned long long int      uint64;

    // Signed int types.
    typedef signed char                 int8;
    typedef signed short int            int16;
    typedef signed int                  int32;
    typedef signed long long int        int64;
    typedef signed long long            usize_gpu;

    // Floating point types
    typedef float                       float32;
    typedef double                      float64;
    typedef long double                 float80;

    // Boolean types
    typedef int                         bool32;
    typedef unsigned char               Boolean;

    #ifndef True
        #define True                        ((Boolean)1)
    #endif

    #ifndef False
        #define False                       ((Boolean)0)
    #endif

    #ifndef Null
        #define Null                        ((void *)0)
    #endif

    #define INVALID_ID_UINT64           18446744073709551615UL
    #define INVALID_ID                  4294967295U
    #define INVALID_ID_UINT16           65535U
    #define INVALID_ID_UINT8            255U

    #define GIBIBYTES(amount)           ((amount) * 1024ULL * 1024ULL * 1024ULL)
    #define MEBIBYTES(amount)           ((amount) * 1024ULL * 1024ULL)
    #define KIBIBYTES(amount)           ((amount) * 1024ULL)

    #define GIGABYTES(amount)           ((amount) * 1000ULL * 1000ULL * 1000ULL)
    #define MEGABYTES(amount)           ((amount) * 1000ULL * 1000ULL)
    #define KILOBYTES(amount)           ((amount) * 1000ULL)

    #define KCLAMP(value, min, max)     ((value <= min) ? min : (value >= max) ? max : value)

    #define ARRAY_SIZE(arr)             sizeof (arr) / sizeof (arr[0])

    #ifndef SIZE_MAX
    #    define SIZE_MAX ((nts_size)1 << (sizeof (nts_size) * 8 - 1))
    #endif

    NKENTSEU_INLINE uint64 NKENTSEU_API GetAligned(uint64 operand, uint64 granularity) {
        return ((operand + (granularity - 1)) & ~(granularity - 1));
    }

    template <typename T>
    T AlignUp(T value, T alignment) {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    template <typename T>
    std::string ToString(const T& t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    std::string NKENTSEU_API ToString(bool value);
    std::string NKENTSEU_API UnicodeToUTF8(uint64 unicode_code);
    inline std::string NKENTSEU_API ToLower(std::string str);
} // namespace nkentseu


#endif  // __NKENTSEU_BASE_H__"