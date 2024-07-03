//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-29 at 12:36:01 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __ENUMERATION_H__
#define __ENUMERATION_H__

#pragma once

#include <System/System.h>

#include <string>
#include <stdexcept>
#include <type_traits>
#include <cstdint>

namespace nkentseu {

    template<typename EnumType, bool EnableBitwiseOps, bool EnableUint32Conversion>
    struct NKENTSEU_API EnumTraits {
        using UnderlyingType = std::underlying_type_t<EnumType>;
        static constexpr bool enableBitwiseOps = EnableBitwiseOps;
        static constexpr bool enableUint32Conversion = EnableUint32Conversion;
    };

    #define ENABLE_ENUM_TRAITS(EnumType, EnableBitwiseOps, EnableUint32Conversion) \
    template<> \
    struct NKENTSEU_API EnumTraits<EnumType, EnableBitwiseOps, EnableUint32Conversion> { \
        using UnderlyingType = std::underlying_type_t<EnumType>; \
        static constexpr bool enableBitwiseOps = EnableBitwiseOps; \
        static constexpr bool enableUint32Conversion = EnableUint32Conversion; \
    };

    template<typename Derived, typename EnumType>
    class NKENTSEU_API EnumBase {
    protected:
        using Traits = EnumTraits<EnumType, EnumTraits<EnumType, false, false>::enableBitwiseOps, EnumTraits<EnumType, false, false>::enableUint32Conversion>;
        typename Traits::UnderlyingType data;

    public:
        using Enum = EnumType;

        EnumBase() : data(static_cast<typename Traits::UnderlyingType>(EnumType::Vertex)) {}
        EnumBase(EnumType s) : data(static_cast<typename Traits::UnderlyingType>(s)) {}
        EnumBase(typename Traits::UnderlyingType s) : data(s) {}

        bool operator==(const EnumBase& other) const {
            return data == other.data;
        }

        bool operator!=(const EnumBase& other) const {
            return data != other.data;
        }

        template<bool Enable = Traits::enableBitwiseOps, typename std::enable_if<Enable, int>::type = 0>
        Derived operator|(EnumType s) const {
            return Derived(data | static_cast<typename Traits::UnderlyingType>(s));
        }

        template<bool Enable = Traits::enableBitwiseOps, typename std::enable_if<Enable, int>::type = 0>
        Derived operator&(EnumType s) const {
            return Derived(data & static_cast<typename Traits::UnderlyingType>(s));
        }

        template<bool Enable = Traits::enableBitwiseOps, typename std::enable_if<Enable, int>::type = 0>
        Derived& operator|=(EnumType s) {
            data |= static_cast<typename Traits::UnderlyingType>(s);
            return static_cast<Derived&>(*this);
        }

        template<bool Enable = Traits::enableBitwiseOps, typename std::enable_if<Enable, int>::type = 0>
        Derived& operator&=(EnumType s) {
            data &= static_cast<typename Traits::UnderlyingType>(s);
            return static_cast<Derived&>(*this);
        }

        template<bool Enable = Traits::enableUint32Conversion, typename std::enable_if<Enable, int>::type = 0>
        operator uint32_t() const {
            return data;
        }

        template<bool Enable = Traits::enableBitwiseOps, typename std::enable_if<Enable, int>::type = 0>
        bool Hasdata(EnumType s) const {
            return (data & static_cast<typename Traits::UnderlyingType>(s)) != 0;
        }
    };

    #define DEFINE_ENUM_BASE_CONSTRUCTOR(class_name, enum_name)     using EnumBase<class_name, enum_name>::EnumBase; \
                                                                    class_name() : EnumBase() {}\
                                                                    class_name(Enum s) : EnumBase(s) {}\
                                                                    class_name(const class_name& other) : EnumBase(other) {}

    #define ENUMERATION(NAME, ...) \
                                    class NAME { \
                                    public: \
                                        enum class Enum : uint32_t { \
                                            __VA_ARGS__, \
                                            NotDefine \
                                        }; \
                                        \
                                    private: \
                                        uint32_t data; \
                                        \
                                    public: \
                                        NAME() : data((uint32_t)NAME::Enum::NotDefine) {} \
                                        NAME(NAME::Enum s) : data((uint32_t)s) {} \
                                        NAME(uint32_t s) : data(s) {} \
                                        \
                                        bool operator==(const NAME& other) const { return data == other.data; } \
                                        bool operator!=(const NAME& other) const { return data != other.data; } \
                                        NAME operator|(NAME::Enum s) const { return NAME(data | (uint32_t)s); } \
                                        NAME operator&(NAME::Enum s) const { return NAME(data & (uint32_t)s); } \
                                        NAME& operator|=(NAME::Enum s) { data |= (uint32_t)s; return *this; } \
                                        NAME& operator&=(NAME::Enum s) { data &= (uint32_t)s; return *this; } \
                                        operator uint32_t() const { return data; } \
                                        operator NAME::Enum() const { return (NAME::Enum)data; } \
                                        bool Hasdata(NAME::Enum s) const { return (data & (uint32_t)s) != 0; } \
                                        bool HasEnum(NAME::Enum s) const { return (data & (uint32_t)s) != 0; } \
                                        std::string ToString() const { \
                                            std::string str(""); \
                                            NAME::ToStringImpl(str, data); \
                                            return str == "" ? "NotDefine" : str; \
                                        } \
                                        static void ToStringImpl(std::string& str, uint32_t data) { \
                                            (void)std::initializer_list<int>{((data & (uint32_t)NAME::Enum::__VA_ARGS__) ? (str += #__VA_ARGS__, 0) : "")...}; \
                                        } \
                                        friend std::ostream& operator<<(std::ostream& os, const NAME& e) { return os << e.ToString(); } \
                                        friend std::string ToString(const NAME& v) { return v.ToString(); } \
                                    };

}  //  nkentseu

#endif  // __ENUMERATION_H__!