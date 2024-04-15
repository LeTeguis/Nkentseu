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
        { Severity::None_ev, "None" },
        { Severity::Info_ev, "Info" },
        { Severity::Trace_ev, "Trace" },
        { Severity::Debug_ev, "Debug" },
        { Severity::Warning_ev, "Warning" },
        { Severity::Error_ev, "Error" },
        { Severity::Critical_ev, "Critical" },
        { Severity::Fatal_ev, "Fatal" },
        { Severity::Assert_ev, "Assert" }
    };

    // Mapping between strings and Severity::Type enum values (reverse lookup)
    static const std::unordered_map<std::string, Severity::Code> static_SeverityTypeMap = {
        { "None", Severity::None_ev },
        { "Info", Severity::Info_ev },
        { "Trace", Severity::Trace_ev },
        { "Debug", Severity::Debug_ev },
        { "Warning", Severity::Warning_ev },
        { "Error", Severity::Error_ev },
        { "Critical", Severity::Critical_ev },
        { "Fatal", Severity::Fatal_ev },
        { "Assert", Severity::Assert_ev }
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
            return "NONE";  // Example (replace with appropriate behavior)
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
            return Severity::None_ev;  // Example (replace with appropriate behavior)
        }
    }

}    // namespace nkentseu