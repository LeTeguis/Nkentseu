//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:56:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Severity.h"

#include <unordered_map>

namespace nkentseu {

    // Mapping between Severity::Type enum values and their corresponding strings
    static const std::unordered_map<Severity::Enum, std::string> static_SeverityStringMap = {
        { Severity::Enum::Info, "Info" },
        { Severity::Enum::Trace, "Trace" },
        { Severity::Enum::Debug, "Debug" },
        { Severity::Enum::Warning, "Warning" },
        { Severity::Enum::Error, "Error" },
        { Severity::Enum::Critical, "Critical" },
        { Severity::Enum::Fatal, "Fatal" },
        { Severity::Enum::Assert, "Assert" }
    };

    // Mapping between strings and Severity::Type enum values (reverse lookup)
    static const std::unordered_map<std::string, Severity::Enum> static_SeverityTypeMap = {
        { "Info", Severity::Enum::Info },
        { "Trace", Severity::Enum::Trace },
        { "Debug", Severity::Enum::Debug },
        { "Warning", Severity::Enum::Warning },
        { "Error", Severity::Enum::Error },
        { "Critical", Severity::Enum::Critical },
        { "Fatal", Severity::Enum::Fatal },
        { "Assert", Severity::Enum::Assert }
    };

    // Implementation of Severity::FromString(const std::string& type)
    Severity Severity::FromString(const std::string& type) {
        // Use the reverse lookup table to find the corresponding enum value
        auto it = static_SeverityTypeMap.find(type);
        if (it != static_SeverityTypeMap.end()) {
            return it->second;
        }
        else {
            // Handle unknown string (return default value or throw an exception)
            return Severity::Enum::Default;  // Example (replace with appropriate behavior)
        }
    }

}    // namespace nkentseu