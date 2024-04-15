//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:36:06 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_FLOAT_PROPERTY_H__
#define __NKENTSEU_FLOAT_PROPERTY_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"
#include "ValueProperty.h"

namespace nkentseu {
    class NKENTSEU_API FloatProperty : public ValueProperty {
    public:
        FloatProperty();
        FloatProperty(float32 value);

        FloatProperty(const FloatProperty& other);

        FloatProperty& operator=(const FloatProperty& other);

        FloatProperty& operator=(float32 other);

        VALUE_CONSTRUCT_DATA(float32)

            std::string ToString() const override;

        FloatProperty operator+(const FloatProperty& other) const;

        FloatProperty operator-(const FloatProperty& other) const;

        FloatProperty operator*(const FloatProperty& other) const;

        FloatProperty operator/(const FloatProperty& other) const;

        bool operator==(const FloatProperty& other) const {
            return GetValue() == other.GetValue();
        }

        bool operator!=(const FloatProperty& other) const {
            return !(*this == other);
        }

        FloatProperty& operator++();

        FloatProperty operator++(int);

        FloatProperty& operator--();

        FloatProperty operator--(int);

        operator float32() const;

        friend std::istream& operator>>(std::istream& is, FloatProperty& floatProp) {
            float32 value;
            is >> value;
            floatProp.SetValue(value);
            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, const FloatProperty& floatProp) {
            os << floatProp.GetValue();
            return os;
        }

        FloatProperty& operator+=(const FloatProperty& other);

        FloatProperty& operator-=(const FloatProperty& other);

        FloatProperty& operator*=(const FloatProperty& other);

        FloatProperty& operator/=(const FloatProperty& other);

        FloatProperty& operator+=(float32 value);

        FloatProperty& operator-=(float32 value);

        FloatProperty& operator*=(float32 value);

        FloatProperty& operator/=(float32 value);

    protected:
        void Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) override;
    };
} // namespace nkentseu

#endif    // __NKENTSEU_FLOAT_PROPERTY_H__