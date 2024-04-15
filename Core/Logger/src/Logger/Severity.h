//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:56:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_SEVERITY_H__
#define __NKENTSEU_SEVERITY_H__

#pragma once

// Include guards to prevent multiple inclusions
#ifndef __NKENTSEU_SYSTEM_H__
#include "System/System.h"
#endif

#ifndef __NKENTSEU_SYSTEM_NATURE_BASE_H__
#include "System/Nature/Base.h"
#endif

#include <string>

namespace nkentseu {

    /**
     * @brief An enumeration class representing logging severity levels.
     *
     * This class defines a set of constants representing different severity levels
     * for logging messages. It also provides static methods to convert between
     * severity level types (uint8) and their string representations.
     */
    class NKENTSEU_API Severity {
    public:
        /**
         * @brief Underlying type for severity level.
         *
         * Uses `uint8` to represent the severity level as a small, unsigned 8-bit integer.
         */
        using Code = uint8;

        /**
         * @brief Enumeration of severity levels.
         *
         * Defines different severity levels for logging messages:
         * - None: No severity level
         * - Info: Informational message
         * - Trace: Trace message for debugging
         * - Debug: Debug message
         * - Warning: Warning message
         * - Error: Error message
         * - Critical: Critical error message
         * - Fatal: Fatal error message
         * - Assert: Assertion failure message
         */
        enum : Code {
            None_ev = 0,
            Info_ev,
            Trace_ev,
            Debug_ev,
            Warning_ev,
            Error_ev,
            Critical_ev,
            Fatal_ev,
            Assert_ev
        };


        /**
         * @brief Converts a severity level type to its string representation.
         *
         * This static method takes a `Severity::Type` value and returns the corresponding
         * string representation.
         *
         * @param type The severity level type.
         * @return The string representation of the severity level.
         */
        static const std::string ToString(Severity::Code);

        /**
         * @brief Converts a string representation to a severity level type.
         *
         * This static method takes a string representing a severity level and returns
         * the corresponding `Severity::Type` value.
         *
         * @param type The string representation of the severity level.
         * @return The severity level type.
         */
        static Severity::Code FromString(const std::string& type);
    };
} // namespace nkentseu

#endif // __NKENTSEU_SEVERITY_H__
