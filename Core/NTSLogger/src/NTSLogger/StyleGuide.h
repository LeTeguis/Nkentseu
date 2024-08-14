//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:44:11 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_STYLEGUIDE_H__
#define __NKENTSEU_STYLEGUIDE_H__

#pragma once

#include "NTSCore/System.h"

namespace nkentseu {
    /**
     * @brief Enumeration representing different text formatting styles.
     *
     * This enumeration defines various styles that can be applied to text formatting
     * within a logger. Each style specifies the positioning of numbers and delimiters
     * around the text.
     */
    ENUMERATION(StyleGuide, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_SET_CONTENT(Enum::Plain)
        ENUM_TO_STRING_SET_CONTENT(Enum::Brack)
        ENUM_TO_STRING_SET_CONTENT(Enum::NumBrack)
        ENUM_TO_STRING_END(Enum::FullGuide),
        ,
        /**
             * @brief Plain text style.
             *
             * No formatting is applied to the text.
             */
        Plain,
        /**
        * @brief Bracketed style.
        *
        * The text is enclosed within delimiters (e.g., square brackets, parentheses).
        * The specific delimiters might be configurable.
        */
        Brack,
        /**
        * @brief Numbered bracketed style.
        *
        * The text is preceded by a number followed by a delimiter (e.g., space, colon),
        * then enclosed within delimiters (e.g., square brackets, parentheses).
        * The specific delimiters might be configurable.
        */
        NumBrack,

        FullGuide
    );

} // namespace nkentseu

#endif    // __NKENTSEU_STYLEGUIDE_H__