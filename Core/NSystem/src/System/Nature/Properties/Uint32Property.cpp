//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:31:54 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "Uint32Property.h"

namespace nkentseu {

    Uint32Property::Uint32Property() : ValueProperty(0) {}
    Uint32Property::Uint32Property(uint32 value) : ValueProperty(value) {}

    Uint32Property::Uint32Property(const Uint32Property& other) : ValueProperty(other.m_value) {}

    Uint32Property& Uint32Property::operator=(const Uint32Property& other) {
        if (this != &other) {
            m_value = other.m_value;
        }
        return *this;
    }

    Uint32Property& Uint32Property::operator=(uint32 other) {
        if (GetValue() != other) {
            m_value = other;
        }
        return *this;
    }

    std::string Uint32Property::ToString() const {
        return std::to_string(GetValue());
    }

    Uint32Property Uint32Property::operator+(const Uint32Property& other) const {
        return Uint32Property(GetValue() + other.GetValue());
    }

    Uint32Property Uint32Property::operator-(const Uint32Property& other) const {
        return Uint32Property(GetValue() - other.GetValue());
    }

    Uint32Property Uint32Property::operator*(const Uint32Property& other) const {
        return Uint32Property(GetValue() * other.GetValue());
    }

    Uint32Property Uint32Property::operator/(const Uint32Property& other) const {
        if (other.GetValue() != 0) {
            return Uint32Property(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, returning 0 in this example
            return Uint32Property(0);
        }
    }

    Uint32Property Uint32Property::operator%(const Uint32Property& other) const {
        return Uint32Property(GetValue() % other.GetValue());
    }

    Uint32Property& Uint32Property::operator++() {
        SetValue(GetValue() + 1);
        return *this;
    }

    Uint32Property Uint32Property::operator++(int) {
        Uint32Property temp(*this);
        ++(*this);
        return temp;
    }

    Uint32Property& Uint32Property::operator--() {
        SetValue(GetValue() - 1);
        return *this;
    }

    Uint32Property Uint32Property::operator--(int) {
        Uint32Property temp(*this);
        --(*this);
        return temp;
    }

    Uint32Property::operator uint32() const {
        return GetValue();
    }

    Uint32Property& Uint32Property::operator+=(const Uint32Property& other) {
        SetValue(GetValue() + other.GetValue());
        return *this;
    }

    Uint32Property& Uint32Property::operator-=(const Uint32Property& other) {
        SetValue(GetValue() - other.GetValue());
        return *this;
    }

    Uint32Property& Uint32Property::operator*=(const Uint32Property& other) {
        SetValue(GetValue() * other.GetValue());
        return *this;
    }

    Uint32Property& Uint32Property::operator/=(const Uint32Property& other) {
        if (other.GetValue() != 0) {
            SetValue(GetValue() / other.GetValue());
        }
        else {
            // Handle division by zero as needed (maybe throw an exception)
            // For simplicity, leaving the value unchanged in this example
        }
        return *this;
    }

    Uint32Property& Uint32Property::operator%=(const Uint32Property& other) {
        SetValue(GetValue() % other.GetValue());
        return *this;
    }

    Uint32Property& Uint32Property::operator+=(uint32 value) {
        SetValue(GetValue() + value);
        return *this;
    }

    Uint32Property& Uint32Property::operator-=(uint32 value) {
        SetValue(GetValue() - value);
        return *this;
    }

    Uint32Property& Uint32Property::operator*=(uint32 value) {
        SetValue(GetValue() * value);
        return *this;
    }

    Uint32Property& Uint32Property::operator/=(uint32 value) {
        if (value != 0) {
            SetValue(GetValue() / value);
        }
        else {
        }
        return *this;
    }

    Uint32Property& Uint32Property::operator%=(uint32 value) {
        SetValue(GetValue() % value);
        return *this;
    }

    void Uint32Property::Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) {
        ValueProperty::Receive(this_, oldValue, newValue);
    }

}    // namespace nkentseu