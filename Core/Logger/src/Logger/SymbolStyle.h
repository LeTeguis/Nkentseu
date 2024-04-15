//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:44:00 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_SYMBOLSTYLE_H__
#define __NKENTSEU_SYMBOLSTYLE_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"

namespace nkentseu {
    /**
     * @brief Enumeration representing different special symbols used for text formatting.
     *
     * This enumeration defines various special symbols that can be used for text formatting purposes.
     * Each value represents a specific symbol that can be applied within the formatting logic.
     */
    class NKENTSEU_API SymbolStyle {
    public:
        using Code = uint32;
        // _EV = Enum value pour les distinguer de donner de type define
        enum : Code {
            /**
             * @brief No symbol used.
             *
             * This value indicates that no special symbol should be included in the formatting.
             */
            None_ev,
            /**
             * @brief Parentheses symbol '()'.
             */
             Paren_ev,
             /**
             * @brief Square brackets symbol '[]'.
             */
             Bracket_ev,
             /**
             * @brief Curly braces symbol '{}'.
             */
             Brace_ev,
             /**
             * @brief Angle brackets symbol '<>'.
             */
             Angle_ev,
             /**
             * @brief Percent symbol '%%'.
             */
             Percent_ev,
             /**
             * @brief Ampersand symbol '&&'.
             */
             Amp_ev,
             /**
             * @brief Dollar symbol '$$'.
             */
             Dollar_ev
        };

        static std::string GetSymbolStyleString(SymbolStyle::Code style);
    };
} // namespace nkentseu

#endif    // __NKENTSEU_SYMBOLSTYLE_H__