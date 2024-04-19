//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:56:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "LoggerPch/ntspch.h"
#include "Severity.h"

#include <unordered_map>

namespace nkentseu {

    // Mapping between Severity::Type enum values and their corresponding strings
    static const std::unordered_map<Severity::Code, std::string> static_SeverityStringMap = {
        { Severity::Info, "Info" },
        { Severity::Trace, "Trace" },
        { Severity::Debug, "Debug" },
        { Severity::Warning, "Warning" },
        { Severity::Error, "Error" },
        { Severity::Critical, "Critical" },
        { Severity::Fatal, "Fatal" },
        { Severity::Assert, "Assert" }
    };

    // Mapping between strings and Severity::Type enum values (reverse lookup)
    static const std::unordered_map<std::string, Severity::Code> static_SeverityTypeMap = {
        { "Info", Severity::Info },
        { "Trace", Severity::Trace },
        { "Debug", Severity::Debug },
        { "Warning", Severity::Warning },
        { "Error", Severity::Error },
        { "Critical", Severity::Critical },
        { "Fatal", Severity::Fatal },
        { "Assert", Severity::Assert }
    };

    // Implementation of Severity::ToString(Severity::Type)
    const std::string Severity::ToString(Severity::Code type) {
        // Use the lookup table to find the corresponding string
        auto it = static_SeverityStringMap.find(type);
        if (it != static_SeverityStringMap.end()) {
            return it->second;
        }
        else {
            // Handle unknown type (return empty string or throw an exception)
            return "Unknow";  // Example (replace with appropriate behavior)
        }
    }

    // Implementation of Severity::FromString(const std::string& type)
    Severity::Code Severity::FromString(const std::string& type) {
        // Use the reverse lookup table to find the corresponding enum value
        auto it = static_SeverityTypeMap.find(type);
        if (it != static_SeverityTypeMap.end()) {
            return it->second;
        }
        else {
            // Handle unknown string (return default value or throw an exception)
            return Severity::Default;  // Example (replace with appropriate behavior)
        }
    }

}    // namespace nkentseu