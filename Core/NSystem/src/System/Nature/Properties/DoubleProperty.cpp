//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:36:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "DoubleProperty.h"

namespace nkentseu {

    DoubleProperty::DoubleProperty() : ValueProperty(0.0f) {}
    DoubleProperty::DoubleProperty(float64 value) : ValueProperty(value) {}

    DoubleProperty::DoubleProperty(const DoubleProperty& other) : ValueProperty(other.m_value) {}

    DoubleProperty& DoubleProperty::operator=(const DoubleProperty& other) {
        if (this != &other) {
            m_value = other.m_value;
        }
        return *this;
    }

    DoubleProperty& DoubleProperty::operator=(float64 other) {
        if (GetValue() != other) {
            m_value = other;
        }
        return *this;
    }

    std::string DoubleProperty::ToString() const {
        return std::to_string(GetValue());
    }

    DoubleProperty DoubleProperty::operator+(const DoubleProperty& other) const {
        return DoubleProperty(GetValue() + other.GetValue());
    }

    DoubleProperty DoubleProperty::operator-(const DoubleProperty& other) const {
        return DoubleProperty(GetValue() - other.GetValue());
    }

    DoubleProperty DoubleProperty::operator*(const DoubleProperty& other) const {
        return DoubleProperty(GetValue() * other.GetValue());
    }

    DoubleProperty DoubleProperty::operator/(const DoubleProperty& other) const {
        if (other.GetValue() != 0.0f) {
            return DoubleProperty(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, returning 0 in this example
            return DoubleProperty(0.0f);
        }
    }

    DoubleProperty& DoubleProperty::operator++() {
        SetValue(GetValue() + 1.0f);
        return *this;
    }

    DoubleProperty DoubleProperty::operator++(int) {
        DoubleProperty temp(*this);
        ++(*this);
        return temp;
    }

    DoubleProperty& DoubleProperty::operator--() {
        SetValue(GetValue() - 1.0f);
        return *this;
    }

    DoubleProperty DoubleProperty::operator--(int) {
        DoubleProperty temp(*this);
        --(*this);
        return temp;
    }

    DoubleProperty::operator float64() const {
        return GetValue();
    }

    DoubleProperty& DoubleProperty::operator+=(const DoubleProperty& other) {
        SetValue(GetValue() + other.GetValue());
        return *this;
    }

    DoubleProperty& DoubleProperty::operator-=(const DoubleProperty& other) {
        SetValue(GetValue() - other.GetValue());
        return *this;
    }

    DoubleProperty& DoubleProperty::operator*=(const DoubleProperty& other) {
        SetValue(GetValue() * other.GetValue());
        return *this;
    }

    DoubleProperty& DoubleProperty::operator/=(const DoubleProperty& other) {
        if (other.GetValue() != 0.0f) {
            SetValue(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, leaving the value unchanged in this example
        }
        return *this;
    }

    DoubleProperty& DoubleProperty::operator+=(float64 value) {
        SetValue(GetValue() + value);
        return *this;
    }

    DoubleProperty& DoubleProperty::operator-=(float64 value) {
        SetValue(GetValue() - value);
        return *this;
    }

    DoubleProperty& DoubleProperty::operator*=(float64 value) {
        SetValue(GetValue() * value);
        return *this;
    }

    DoubleProperty& DoubleProperty::operator/=(float64 value) {
        if (value != 0.0f) {
            SetValue(GetValue() / value);
        }
        else {
        }
        return *this;
    }

    void DoubleProperty::Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) {
        ValueProperty::Receive(this_, oldValue, newValue);
    }

}    // namespace nkentseu