//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-29 at 12:36:01 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __ENUMERATION_H__
#define __ENUMERATION_H__

#pragma once

#include "NTSCore/Platform/Exports.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <cstdint>

namespace nkentseu {
    // Base template class
    template<typename EnumType, typename BaseType = int>
    class Enumeration {
    public:
        using BASE_TYPE = BaseType;

        Enumeration() : value(0) {}
        Enumeration(EnumType e) : value(static_cast<BASE_TYPE>(e)) {}
        Enumeration(BASE_TYPE v) : value(v) {}

        /*bool operator==(const Enumeration& other) const {
            return value == other.value;
        }

        bool operator!=(const Enumeration& other) const {
            return value != other.value;
        }

        bool operator==(const EnumType& other) const {
            return value == static_cast<BASE_TYPE>(other);
        }

        bool operator!=(const EnumType& other) const {
            return value != static_cast<BASE_TYPE>(other);
        }

        Enumeration operator|(EnumType e) const {
            return Enumeration(value | static_cast<BASE_TYPE>(e));
        }

        Enumeration operator&(EnumType e) const {
            return Enumeration(value & static_cast<BASE_TYPE>(e));
        }

        Enumeration& operator|=(EnumType e) {
            value |= static_cast<BASE_TYPE>(e);
            return *this;
        }

        Enumeration& operator&=(EnumType e) {
            value &= static_cast<BASE_TYPE>(e);
            return *this;
        }*/

        operator BASE_TYPE() const {
            return value;
        }

        operator EnumType() const {
            return static_cast<EnumType>(value);
        }

        bool HasFlag(EnumType e) const {
            return (value & static_cast<BASE_TYPE>(e)) != 0;
        }

        virtual std::string ToString() const {
            return "";
        }

        friend std::ostream& operator<<(std::ostream& os, const Enumeration& e) {
            return os << e.ToString();
        }

    protected:
        BASE_TYPE value;
    };

    #define ENUM_TO_STRING_BEGIN                virtual std::string ToString() const {\
                                                    std::string str = "";
    #define ENUM_TO_STRING_ADD_CONTENT(value_e)         if (value & static_cast<BASE_TYPE>(value_e)) str += #value_e;
    #define ENUM_TO_STRING_SET_CONTENT(value_e)         if (value == static_cast<BASE_TYPE>(value_e)) str = #value_e;
    #define ENUM_TO_STRING_ADD_CONTENT2(value_e)        if (value & static_cast<BASE_TYPE>(Enum::##value_e)) str += #value_e;
    #define ENUM_TO_STRING_SET_CONTENT2(value_e)        if (value == static_cast<BASE_TYPE>(Enum::##value_e)) str = #value_e;
    #define ENUM_TO_STRING_END(not_vlue)            return str.empty() ? #not_vlue : str;\
                                                }

    #define STRING_TO_ENUM_BEGIN()              static virtual enum_name FromString(const std::string& str) const {\
                                                    enum_name e;
    #define STRING_TO_ENUM_ADD_CONTENT(value_e)         if (str.find(#value_e, 0) != std::string::npos()) e.value |= static_cast<BASE_TYPE>(value_e);
    #define STRING_TO_ENUM_SET_CONTENT(value_e)         if (str == #value_e) e.value  = static_cast<BASE_TYPE>(value_e);
    #define STRING_TO_ENUM_END(not_vlue)            return str.empty() ? #not_vlue : str;\
                                                }

    // Macro to define the enumeration and class
    #define ENUMERATION(enum_name, default_type, tostring, methods, ...) \
          enum class NKENTSEU_API Enum##enum_name : default_type { __VA_ARGS__ }; \
          class NKENTSEU_API enum_name { \
                protected:\
                    default_type value;\
                public: \
                    using BASE_TYPE = default_type;\
                    using Enum = Enum##enum_name; \
                    enum_name() : value(0) {} \
                    enum_name(Enum e) : value(static_cast<default_type>(e)) {} \
                    enum_name(default_type v) : value(v) {}\
                    enum_name(const enum_name& v) : value(v.value) {}\
                    tostring\
                    methods\
                    bool operator==(const enum_name& other) const {\
                        return value == other.value;\
                    }\
                    bool operator!=(const enum_name& other) const {\
                        return value != other.value;\
                    }\
                    bool operator==(const Enum& other) const {\
                        return value == static_cast<BASE_TYPE>(other);\
                    }\
                    bool operator!=(const Enum& other) const {\
                        return value != static_cast<BASE_TYPE>(other);\
                    }\
                    bool operator==(const BASE_TYPE& other) const {\
                        return value == static_cast<BASE_TYPE>(other);\
                    }\
                    bool operator!=(const BASE_TYPE& other) const {\
                        return value != static_cast<BASE_TYPE>(other);\
                    }\
                    enum_name& operator=(const enum_name& other) {\
                        this->value = other.value;\
                        return *this;\
                    }\
                    enum_name operator|(Enum e) const {\
                        return enum_name(value | static_cast<BASE_TYPE>(e));\
                    }\
                    enum_name operator&(Enum e) const {\
                        return enum_name(value & static_cast<BASE_TYPE>(e));\
                    }\
                    enum_name& operator|=(Enum e) {\
                        value |= static_cast<BASE_TYPE>(e);\
                        return *this;\
                    }\
                    enum_name& operator&=(Enum e) {\
                        value &= static_cast<BASE_TYPE>(e);\
                        return *this;\
                    }\
                    enum_name operator|(const enum_name& e) const {\
                        return enum_name(value | static_cast<BASE_TYPE>(e.value));\
                    }\
                    enum_name operator&(const enum_name& e) const {\
                        return enum_name(value & static_cast<BASE_TYPE>(e.value));\
                    }\
                    enum_name& operator|=(const enum_name& e) {\
                        value |= static_cast<BASE_TYPE>(e.value);\
                        return *this;\
                    }\
                    enum_name& operator&=(const enum_name& e) {\
                        value &= static_cast<BASE_TYPE>(e.value);\
                        return *this;\
                    }\
                    enum_name operator|(const BASE_TYPE& e) const {\
                        return enum_name(value | static_cast<BASE_TYPE>(e));\
                    }\
                    enum_name operator&(const BASE_TYPE& e) const {\
                        return enum_name(value & static_cast<BASE_TYPE>(e));\
                    }\
                    enum_name& operator|=(const BASE_TYPE& e) {\
                        value |= static_cast<BASE_TYPE>(e);\
                        return *this;\
                    }\
                    enum_name& operator&=(const BASE_TYPE& e) {\
                        value &= static_cast<BASE_TYPE>(e);\
                        return *this;\
                    }\
                    operator BASE_TYPE() const {\
                        return value;\
                    }\
                    operator Enum() const {\
                        return static_cast<Enum>(value);\
                    }\
                    bool HasFlag(Enum e) const {\
                        return (value & static_cast<BASE_TYPE>(e)) != 0;\
                    }\
          }

}  //  nkentseu

#endif  // __ENUMERATION_H__!