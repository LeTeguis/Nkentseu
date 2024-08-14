//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:36:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_DOUBLE_PROPERTY_H__
#define __NKENTSEU_DOUBLE_PROPERTY_H__

#pragma once

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"
#include "ValueProperty.h"

namespace nkentseu {
    class NKENTSEU_API DoubleProperty : public ValueProperty {
    public:
        DoubleProperty();
        DoubleProperty(float64 value);

        DoubleProperty(const DoubleProperty& other);

        DoubleProperty& operator=(const DoubleProperty& other);

        DoubleProperty& operator=(float64 other);

        VALUE_CONSTRUCT_DATA(float64)

        std::string ToString() const override;

        DoubleProperty operator+(const DoubleProperty& other) const;

        DoubleProperty operator-(const DoubleProperty& other) const;

        DoubleProperty operator*(const DoubleProperty& other) const;

        DoubleProperty operator/(const DoubleProperty& other) const;

        bool operator==(const DoubleProperty& other) const {
            return GetValue() == other.GetValue();
        }

        bool operator!=(const DoubleProperty& other) const {
            return !(*this == other);
        }

        DoubleProperty& operator++();

        DoubleProperty operator++(int);

        DoubleProperty& operator--();

        DoubleProperty operator--(int);

        operator float64() const;

        friend std::istream& operator>>(std::istream& is, DoubleProperty& floatProp) {
            float64 value;
            is >> value;
            floatProp.SetValue(value);
            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, const DoubleProperty& floatProp) {
            os << floatProp.GetValue();
            return os;
        }

        DoubleProperty& operator+=(const DoubleProperty& other);

        DoubleProperty& operator-=(const DoubleProperty& other);

        DoubleProperty& operator*=(const DoubleProperty& other);

        DoubleProperty& operator/=(const DoubleProperty& other);

        DoubleProperty& operator+=(float64 value);

        DoubleProperty& operator-=(float64 value);

        DoubleProperty& operator*=(float64 value);

        DoubleProperty& operator/=(float64 value);

    protected:
        void Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) override;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_DOUBLE_PROPERTY_H__