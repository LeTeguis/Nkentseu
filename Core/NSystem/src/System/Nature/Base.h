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
#include "System/System.h"

namespace nkentseu{
    using ARGV = std::vector<std::string>;

    #define BIT(x) (1 << x)
    #define STDPH(v) std::placeholders::_##v

    #define STR_BOOL(b) ((b) ? "True" : "False")

    // Unsigned int types.
    typedef unsigned char               uint8;
    typedef unsigned short              uint16;
    typedef unsigned int                uint32;
    typedef unsigned long int           uintl32;
    typedef uintl32                     nts_size;
    typedef unsigned long long int      uint64;

    // Signed int types.
    typedef signed char                 int8;
    typedef signed short int            int16;
    typedef signed int                  int32;
    typedef signed long long int        int64;

    // Floating point types
    typedef float                       float32;
    typedef double                      float64;
    typedef long double                 float80;

    // Boolean types
    typedef int                         bool32;
    typedef unsigned char               Bool;

    #define True                        ((Bool)1)
    #define False                       ((Bool)0)
    #define Null                        ((void *)0)

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

    NKENTSEU_INLINE std::ostream& operator<<(std::ostream& os, bool value) {
        return os << ((value) ? "True" : "False");
    }

    template <typename T>
    std::string ToString(const T& t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    std::string ToString(bool value);
} // namespace nkentseu


#endif  // __NKENTSEU_BASE_H__"