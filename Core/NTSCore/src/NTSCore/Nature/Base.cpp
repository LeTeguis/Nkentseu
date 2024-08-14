//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 4/7/2024 at 11:29:15 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Base.h"

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

namespace nkentseu {
    std::string ToString(bool value) {
        std::stringstream ss;
        ss << ((value) ? "True" : "False");
        return ss.str();
    }

    std::string UnicodeToUTF8(uint64 unicode_code)
    {
        // Convertir le code Unicode en std::wstring
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wide_str = converter.from_bytes(std::to_string(unicode_code));

        // Convertir la std::wstring en std::string
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wide_to_utf8;
        return wide_to_utf8.to_bytes(wide_str);
    }

    std::string NKENTSEU_API ToLower(std::string str) {
        for (char& c : str)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return str;
    }
}