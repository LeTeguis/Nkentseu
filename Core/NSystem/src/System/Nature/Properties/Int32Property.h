//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:35:53 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INT32_PROPERTY_H__
#define __NKENTSEU_INT32_PROPERTY_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"
#include "ValueProperty.h"

namespace nkentseu {
    class NKENTSEU_API Int32Property : public ValueProperty {
    public:
        Int32Property();
        Int32Property(int32 value);

        Int32Property(const Int32Property& other);

        Int32Property& operator=(const Int32Property& other);

        Int32Property& operator=(int32 other);

        VALUE_CONSTRUCT_DATA(int32)

            std::string ToString() const override;

        Int32Property operator+(const Int32Property& other) const;

        Int32Property operator-(const Int32Property& other) const;

        Int32Property operator*(const Int32Property& other) const;

        Int32Property operator/(const Int32Property& other) const;
        Int32Property operator%(const Int32Property& other) const;

        bool operator==(const Int32Property& other) const {
            return GetValue() == other.GetValue();
        }

        bool operator!=(const Int32Property& other) const {
            return !(*this == other);
        }

        Int32Property& operator++();

        Int32Property operator++(int);

        Int32Property& operator--();

        Int32Property operator--(int);

        operator int32() const;

        friend std::istream& operator>>(std::istream& is, Int32Property& floatProp) {
            int32 value;
            is >> value;
            floatProp.SetValue(value);
            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, const Int32Property& floatProp) {
            os << floatProp.GetValue();
            return os;
        }

        Int32Property& operator+=(const Int32Property& other);

        Int32Property& operator-=(const Int32Property& other);

        Int32Property& operator*=(const Int32Property& other);

        Int32Property& operator/=(const Int32Property& other);
        Int32Property& operator%=(const Int32Property& other);

        Int32Property& operator+=(int32 value);

        Int32Property& operator-=(int32 value);

        Int32Property& operator*=(int32 value);

        Int32Property& operator/=(int32 value);
        Int32Property& operator%=(int32 value);

    protected:
        void Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) override;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_INT32_PROPERTY_H__