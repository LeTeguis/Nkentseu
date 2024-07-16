//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-29 at 12:36:01 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __ENUMERATION_H__
#define __ENUMERATION_H__

#pragma once

#include <System/System.h>

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

        bool operator==(const Enumeration& other) const {
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
        }

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

    #define ENUM_TO_STRING_BEGIN                virtual std::string ToString() const override {\
                                                    std::string str = "";
    //#define ENUM_TO_STRING_CONTENT(value_e)         if (value & static_cast<BASE_TYPE>(value_e)) str += #value_e;
    #define ENUM_TO_STRING_CONTENT(value_e)         if (value == static_cast<BASE_TYPE>(value_e)) str = #value_e;
    #define ENUM_TO_STRING_END(not_vlue)            return str.empty() ? #not_vlue : str;\
                                                }

    // Macro to define the enumeration and class
    #define ENUMERATION(enum_name, default_type, tostring, methods, ...) \
          enum class NKENTSEU_API Enum##enum_name : default_type { __VA_ARGS__ }; \
          class NKENTSEU_API enum_name : public Enumeration<Enum##enum_name, default_type> { \
              public: \
                    using Enum = Enum##enum_name; \
                    enum_name() : Enumeration() {} \
                    enum_name(Enum e) : Enumeration(e) {} \
                    enum_name(BASE_TYPE v) : Enumeration(v) {}\
                    tostring\
                    methods\
                    /*bool operator==(const enum_name& other) const {\
                        return value == other.value;\
                    }\
                    bool operator==(const enum_name& other) const {\
                        return !(value == other);\
                    }\*/\
          }

    /*bool operator==(const enum_name& other) const {\
        return value == other.value;\
    }\
    bool operator==(const enum_name& other) const {\
        return !(value == other);\
    }\*/

}  //  nkentseu

#endif  // __ENUMERATION_H__!