//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:35:53 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Int32Property.h"

namespace nkentseu {

    Int32Property::Int32Property() : ValueProperty(0) {}
    Int32Property::Int32Property(int32 value) : ValueProperty(value) {}

    Int32Property::Int32Property(const Int32Property& other) : ValueProperty(other.m_value) {}

    Int32Property& Int32Property::operator=(const Int32Property& other) {
        if (this != &other) {
            m_value = other.m_value;
        }
        return *this;
    }

    Int32Property& Int32Property::operator=(int32 other) {
        if (GetValue() != other) {
            m_value = other;
        }
        return *this;
    }

    std::string Int32Property::ToString() const {
        return std::to_string(GetValue());
    }

    Int32Property Int32Property::operator+(const Int32Property& other) const {
        return Int32Property(GetValue() + other.GetValue());
    }

    Int32Property Int32Property::operator-(const Int32Property& other) const {
        return Int32Property(GetValue() - other.GetValue());
    }

    Int32Property Int32Property::operator*(const Int32Property& other) const {
        return Int32Property(GetValue() * other.GetValue());
    }

    Int32Property Int32Property::operator/(const Int32Property& other) const {
        if (other.GetValue() != 0) {
            return Int32Property(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, returning 0 in this example
            return Int32Property(0);
        }
    }

    Int32Property Int32Property::operator%(const Int32Property& other) const {
        return Int32Property(GetValue() % other.GetValue());
    }

    Int32Property& Int32Property::operator++() {
        SetValue(GetValue() + 1);
        return *this;
    }

    Int32Property Int32Property::operator++(int) {
        Int32Property temp(*this);
        ++(*this);
        return temp;
    }

    Int32Property& Int32Property::operator--() {
        SetValue(GetValue() - 1);
        return *this;
    }

    Int32Property Int32Property::operator--(int) {
        Int32Property temp(*this);
        --(*this);
        return temp;
    }

    Int32Property::operator int32() const {
        return GetValue();
    }

    Int32Property& Int32Property::operator+=(const Int32Property& other) {
        SetValue(GetValue() + other.GetValue());
        return *this;
    }

    Int32Property& Int32Property::operator-=(const Int32Property& other) {
        SetValue(GetValue() - other.GetValue());
        return *this;
    }

    Int32Property& Int32Property::operator*=(const Int32Property& other) {
        SetValue(GetValue() * other.GetValue());
        return *this;
    }

    Int32Property& Int32Property::operator/=(const Int32Property& other) {
        if (other.GetValue() != 0) {
            SetValue(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, leaving the value unchanged in this example
        }
        return *this;
    }

    Int32Property& Int32Property::operator%=(const Int32Property& other) {
        SetValue(GetValue() % other.GetValue());
        return *this;
    }

    Int32Property& Int32Property::operator+=(int32 value) {
        SetValue(GetValue() + value);
        return *this;
    }

    Int32Property& Int32Property::operator-=(int32 value) {
        SetValue(GetValue() - value);
        return *this;
    }

    Int32Property& Int32Property::operator*=(int32 value) {
        SetValue(GetValue() * value);
        return *this;
    }

    Int32Property& Int32Property::operator/=(int32 value) {
        if (value != 0.0f) {
            SetValue(GetValue() / value);
        }
        else {
        }
        return *this;
    }

    Int32Property& Int32Property::operator%=(int32 value) {
        SetValue(GetValue() % value);
        return *this;
    }

    void Int32Property::Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) {
        ValueProperty::Receive(this_, oldValue, newValue);
    }

}    // namespace nkentseu