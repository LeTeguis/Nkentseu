//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:23:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_FORMATTER_H__
#define __NKENTSEU_FORMATTER_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"
#include "SymbolStyle.h"
#include "StyleGuide.h"

#include <vector>
#include <sstream>

namespace nkentseu {
    // HAS_METHOD(T, ToString);

    class NKENTSEU_API Formatter {
    public:
        /**
         * @brief Gets a reference to the single instance of the Formatter class.
         *
         * This static method provides a way to access a single instance of the Formatter class.
         * This is useful if you want to avoid creating multiple instances for formatting tasks.
         * @return A reference to the unique Formatter instance.
         */
        static Formatter& Instance();

        /**
         * @brief Constructor for the Formatter class.
         *
         * This constructor initializes the Formatter object with a default StyleGuide::NumBrack and SymbolStyle::Bracket.
         */
        Formatter();

        /**
         * @brief Constructor for the Formatter class.
         *
         * This constructor initializes the Formatter object with a specific StyleGuide and SymbolStyle.
         * @param styleGuide The StyleGuide object defining the formatting rules.
         * @param symbolStyle The SymbolStyle value specifying the symbols to be used.
         */
        Formatter(StyleGuide::Code styleGuide, SymbolStyle::Code symbolStyle);

        /**
         * @brief Destructor for the Formatter class.
         *
         * This destructor releases any resources allocated by the Formatter object.
         */
        ~Formatter();

        /**
         * @brief Reinitializes the Formatter object with new styles.
         *
         * This method allows you to update the StyleGuide and SymbolStyle used by the Formatter object
         * without having to recreate the instance.
         * @param styleGuide The new StyleGuide object.
         * @param symbolStyle The new SymbolStyle value.
         */
        void Initialize(StyleGuide::Code styleGuide, SymbolStyle::Code symbolStyle);

        /**
         * @brief Formats a string with the given arguments.
         *
         * This method takes a format string as input and uses the provided arguments to format the text.
         * The arguments can be of any type supported by the underlying StyleGuide implementation.
         * @param format The format string to be applied.
         * @param args The list of arguments to be used for formatting.
         * @return The formatted string.
         */
        template<typename... Args>
        std::string Format(int add, const std::string& format, Args&&... args) {
            return FormatImpl(add, format, GetArgs(args...));
        }

        /**
         * @brief Formats a string with the given arguments.
         *
         * This method takes a format string and an iterator to an array of arguments as input.
         * The arguments can be of any type supported by the underlying StyleGuide implementation.
         * @param format The format string to be applied.
         * @param args The iterator pointing to the beginning of the argument array.
         * @return The formatted string.
         */
        template<typename... Args>
        std::string Format(const std::string& format, Args&&... args) {
            return Format(0, format, args...);
        }

    private:
        // Helper method for formatting with arguments
        std::string FormatImpl(int add, const std::string& format, const std::vector<std::string>& args);

        template <typename... Args>
        std::vector<std::string> GetArgs(Args&&... args) {
            std::vector<std::string> result;
            (GetArgsHelper(result, std::forward<Args>(args)), ...);
            return result;
        }

        template <typename T>
        void GetArgsHelper(std::vector<std::string>& result, T&& arg) {
            result.push_back(ToString(arg));
        }

    private:
        /**
         * @brief Member variable to store the current SymbolStyle.
         */
        SymbolStyle::Code m_SymboleStyle;

        /**
         * @brief Member variable to store the current StyleGuide.
         */
        StyleGuide::Code m_StyleGuide;
    }; // class Formater

    #define FORMATTER Formatter::Instance()
} // namespace nkentseu

#endif // __NKENTSEU_FORMATTER_H__
