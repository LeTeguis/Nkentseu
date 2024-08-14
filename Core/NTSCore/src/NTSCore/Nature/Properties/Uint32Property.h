//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:31:54 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_UINT32_PROPERTY_H__
#define __NKENTSEU_UINT32_PROPERTY_H__

#pragma once

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"
#include "ValueProperty.h"

namespace nkentseu {
    class NKENTSEU_API Uint32Property : public ValueProperty {
    public:
        Uint32Property();
        Uint32Property(uint32 value);

        Uint32Property(const Uint32Property& other);

        Uint32Property& operator=(const Uint32Property& other);

        Uint32Property& operator=(uint32 other);

        VALUE_CONSTRUCT_DATA(uint32)

            std::string ToString() const override;

        Uint32Property operator+(const Uint32Property& other) const;

        Uint32Property operator-(const Uint32Property& other) const;

        Uint32Property operator*(const Uint32Property& other) const;

        Uint32Property operator/(const Uint32Property& other) const;
        Uint32Property operator%(const Uint32Property& other) const;

        bool operator==(const Uint32Property& other) const {
            return GetValue() == other.GetValue();
        }

        bool operator!=(const Uint32Property& other) const {
            return !(*this == other);
        }

        Uint32Property& operator++();

        Uint32Property operator++(int);

        Uint32Property& operator--();

        Uint32Property operator--(int);

        operator uint32() const;

        friend std::istream& operator>>(std::istream& is, Uint32Property& floatProp) {
            uint32 value;
            is >> value;
            floatProp.SetValue(value);
            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, const Uint32Property& floatProp) {
            os << floatProp.GetValue();
            return os;
        }

        Uint32Property& operator+=(const Uint32Property& other);

        Uint32Property& operator-=(const Uint32Property& other);

        Uint32Property& operator*=(const Uint32Property& other);

        Uint32Property& operator/=(const Uint32Property& other);
        Uint32Property& operator%=(const Uint32Property& other);

        Uint32Property& operator+=(uint32 value);

        Uint32Property& operator-=(uint32 value);

        Uint32Property& operator*=(uint32 value);

        Uint32Property& operator/=(uint32 value);
        Uint32Property& operator%=(uint32 value);

    protected:
        void Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) override;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_UINT32_PROPERTY_H__