//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:56:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_SEVERITY_H__
#define __NKENTSEU_SEVERITY_H__

#pragma once

// Include guards to prevent multiple inclusions
#include "NTSCore/System.h"

#include <string>

namespace nkentseu {

    /**
     * @brief An enumeration class representing logging severity levels.
     *
     * This class defines a set of constants representing different severity levels
     * for logging messages. It also provides static methods to convert between
     * severity level types (uint8) and their string representations.
     */
    ENUMERATION(Severity, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_SET_CONTENT2(Info)
        ENUM_TO_STRING_SET_CONTENT2(Trace)
        ENUM_TO_STRING_SET_CONTENT2(Debug)
        ENUM_TO_STRING_SET_CONTENT2(Warning)
        ENUM_TO_STRING_SET_CONTENT2(Error)
        ENUM_TO_STRING_SET_CONTENT2(Critical)
        ENUM_TO_STRING_SET_CONTENT2(Fatal)
        ENUM_TO_STRING_SET_CONTENT2(Assert)
        ENUM_TO_STRING_END(Enum::Default),
        static Severity FromString(const std::string& type);
        ,
        Info = 1,
        Trace,
        Debug,
        Warning,
        Error,
        Critical,
        Fatal,
        Assert,
        Default = Trace,
        );
} // namespace nkentseu

#endif // __NKENTSEU_SEVERITY_H__
