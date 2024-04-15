//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:44:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_STYLEGUIDE_H__
#define __NKENTSEU_STYLEGUIDE_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"

namespace nkentseu {
    /**
     * @brief Enumeration representing different text formatting styles.
     *
     * This enumeration defines various styles that can be applied to text formatting
     * within a logger. Each style specifies the positioning of numbers and delimiters
     * around the text.
     */
    class NKENTSEU_API StyleGuide {
    public:
        using Code = uint32;
        enum : Code {
            /**
             * @brief Plain text style.
             *
             * No formatting is applied to the text.
             */
            Plain_ev,
            /**
            * @brief Bracketed style.
            *
            * The text is enclosed within delimiters (e.g., square brackets, parentheses).
            * The specific delimiters might be configurable.
            */
            Brack_ev,
            /**
            * @brief Numbered bracketed style.
            *
            * The text is preceded by a number followed by a delimiter (e.g., space, colon),
            * then enclosed within delimiters (e.g., square brackets, parentheses).
            * The specific delimiters might be configurable.
            */
            NumBrack_ev,

            FullGuide_ev
        };
    };
} // namespace nkentseu

#endif    // __NKENTSEU_STYLEGUIDE_H__