//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:23:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Formatter.h"

#include <sstream>
#include <iostream>

namespace nkentseu {
    Formatter& Formatter::Instance()
    {
        // Implementation using static member variable
        static Formatter instance(StyleGuide::Enum::NumBrack, SymbolStyle::Enum::Brace);
        return instance;
    }

    Formatter::Formatter() : m_StyleGuide(StyleGuide::Enum::NumBrack), m_SymboleStyle(SymbolStyle::Enum::Brace)
    {
    }

    Formatter::Formatter(StyleGuide styleGuide, SymbolStyle symbolStyle) : m_StyleGuide(styleGuide), m_SymboleStyle(symbolStyle)
    {
    }

    Formatter::~Formatter()
    {
    }

    void Formatter::Initialize(StyleGuide styleGuide, SymbolStyle symbolStyle)
    {
        m_StyleGuide = styleGuide;
        m_SymboleStyle = symbolStyle;
    }

    std::string Formatter::FormatImpl(int add, const std::string& format_string, const std::vector<std::string>& arguments) {
        // 1. Get symbol style string (opening and closing characters)
        std::string style_format = m_SymboleStyle.GetSymbolStyleString();
        if (style_format == "") {
            return format_string; // No style, return original string
        }

        // 2. Initialize variables
        std::string formatted_string = format_string;
        size_t current_index = 0; // Use size_t for indexing positions in strings
        size_t replacement_index_step = -1; // Index for arguments based on style guide

        // 3. Loop through the format string
        while (current_index < formatted_string.length()) {
            // 3.1 Inside an opened curly brace
            if (formatted_string[current_index] == style_format[0]) {
                int replacement_index = -1;
                size_t search_index = current_index + 1;

                // 3.1.1 Extract replacement index from format string
                while (search_index < formatted_string.length() && formatted_string[search_index] != style_format[1]) {
                    if (formatted_string[search_index] >= '0' && formatted_string[search_index] <= '9') {
                        if (replacement_index == -1) {
                            replacement_index = 0;
                        }
                        replacement_index = replacement_index * 10 + (formatted_string[search_index] - '0');
                    }
                    else if (formatted_string[search_index] != ' ') {
                        replacement_index = -1;
                        break;
                    }
                    search_index++;
                }
                replacement_index -= add;
                // 3.1.2 Handle replacement based on style guide
                if (replacement_index >= 0) {
                    if (m_StyleGuide == StyleGuide::Enum::FullGuide || m_StyleGuide == StyleGuide::Enum::NumBrack) {
                        if (replacement_index < arguments.size()) {
                            formatted_string.erase(current_index, search_index - current_index + 1);
                            formatted_string.insert(current_index, arguments[replacement_index]);
                        }
                        else replacement_index = -1;
                    }
                    else if (m_StyleGuide == StyleGuide::Enum::FullGuide || m_StyleGuide == StyleGuide::Enum::Brack) {
                        replacement_index_step++;
                        if (replacement_index_step < arguments.size()) {
                            formatted_string.erase(current_index, search_index - current_index + 1);
                            formatted_string.insert(current_index, arguments[replacement_index_step]);
                        }
                        else replacement_index = -1;
                    }
                    else replacement_index = -1;
                }

                if (replacement_index < 0) {
                    current_index++;
                }
            }
            else {
                current_index++;
            }
        }

        return formatted_string;
    }

    std::string SymbolStyle::GetSymbolStyleString()
    {
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Paren)) return "()";
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Bracket)) return "[]";
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Brace)) return "{}";
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Angle)) return "<>";
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Percent)) return "%%";
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Amp)) return "&&";
        if (value == static_cast<BASE_TYPE>(SymbolStyle::Enum::Dollar)) return "$$";
        return "";
    }
} // namespace nkentseu