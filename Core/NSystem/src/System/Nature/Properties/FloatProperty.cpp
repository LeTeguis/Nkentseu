//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:36:06 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "FloatProperty.h"

namespace nkentseu {

    FloatProperty::FloatProperty() : ValueProperty(0.0f) {}
    FloatProperty::FloatProperty(float32 value) : ValueProperty(value) {}

    FloatProperty::FloatProperty(const FloatProperty& other) : ValueProperty(other.m_value) {}

    FloatProperty& FloatProperty::operator=(const FloatProperty& other) {
        if (this != &other) {
            m_value = other.m_value;
        }
        return *this;
    }

    FloatProperty& FloatProperty::operator=(float32 other) {
        if (GetValue() != other) {
            m_value = other;
        }
        return *this;
    }

    std::string FloatProperty::ToString() const {
        return std::to_string(GetValue());
    }

    FloatProperty FloatProperty::operator+(const FloatProperty& other) const {
        return FloatProperty(GetValue() + other.GetValue());
    }

    FloatProperty FloatProperty::operator-(const FloatProperty& other) const {
        return FloatProperty(GetValue() - other.GetValue());
    }

    FloatProperty FloatProperty::operator*(const FloatProperty& other) const {
        return FloatProperty(GetValue() * other.GetValue());
    }

    FloatProperty FloatProperty::operator/(const FloatProperty& other) const {
        if (other.GetValue() != 0.0f) {
            return FloatProperty(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, returning 0 in this example
            return FloatProperty(0.0f);
        }
    }

    FloatProperty& FloatProperty::operator++() {
        SetValue(GetValue() + 1.0f);
        return *this;
    }

    FloatProperty FloatProperty::operator++(int) {
        FloatProperty temp(*this);
        ++(*this);
        return temp;
    }

    FloatProperty& FloatProperty::operator--() {
        SetValue(GetValue() - 1.0f);
        return *this;
    }

    FloatProperty FloatProperty::operator--(int) {
        FloatProperty temp(*this);
        --(*this);
        return temp;
    }

    FloatProperty::operator float32() const {
        return GetValue();
    }

    FloatProperty& FloatProperty::operator+=(const FloatProperty& other) {
        SetValue(GetValue() + other.GetValue());
        return *this;
    }

    FloatProperty& FloatProperty::operator-=(const FloatProperty& other) {
        SetValue(GetValue() - other.GetValue());
        return *this;
    }

    FloatProperty& FloatProperty::operator*=(const FloatProperty& other) {
        SetValue(GetValue() * other.GetValue());
        return *this;
    }

    FloatProperty& FloatProperty::operator/=(const FloatProperty& other) {
        if (other.GetValue() != 0.0f) {
            SetValue(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, leaving the value unchanged in this example
        }
        return *this;
    }

    FloatProperty& FloatProperty::operator+=(float32 value) {
        SetValue(GetValue() + value);
        return *this;
    }

    FloatProperty& FloatProperty::operator-=(float32 value) {
        SetValue(GetValue() - value);
        return *this;
    }

    FloatProperty& FloatProperty::operator*=(float32 value) {
        SetValue(GetValue() * value);
        return *this;
    }

    FloatProperty& FloatProperty::operator/=(float32 value) {
        if (value != 0.0f) {
            SetValue(GetValue() / value);
        }
        else {
        }
        return *this;
    }

    void FloatProperty::Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) {
        ValueProperty::Receive(this_, oldValue, newValue);
    }

}    // namespace nkentseu