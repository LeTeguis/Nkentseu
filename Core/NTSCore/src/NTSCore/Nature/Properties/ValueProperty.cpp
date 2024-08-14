//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:31:39 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "ValueProperty.h"

namespace nkentseu {

    bool operator == (const ValueChange& lhs, const ValueChange& rhs) {
        return std::addressof(lhs) == std::addressof(rhs);
    }
    bool operator != (const ValueChange& lhs, const ValueChange& rhs) {
        return !(lhs == rhs);
    }

    // Default constructor
    ValueProperty::ValueProperty() {}

    // Constructor with initial value
    ValueProperty::ValueProperty(const std::any& value) : m_value(value) {}

    std::any ValueProperty::Get() const { return m_value; }

    void ValueProperty::Set(const std::any& value) {
        std::any oldValue = m_value;

        m_value = value;

        SignalValueChanged(this, oldValue, m_value);
    }

    void ValueProperty::AddObserver(const ValueChange& observer) {
        auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);

        if (it == m_Observers.end()) {
            m_Observers.push_back(observer);
        }
    }

    void ValueProperty::Connect(ValueProperty* other) {
        auto it = std::find(m_Connections.begin(), m_Connections.end(), other);
        if (it == m_Connections.end()) {
            m_Connections.push_back(other);
        }
    }

    void ValueProperty::RemoveObserver(const ValueChange& observer) {
        auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
        if (it != m_Observers.end()) {
            m_Observers.erase(it);
        }
    }

    void ValueProperty::Disconnect(ValueProperty* other) {
        auto it = std::find(m_Connections.begin(), m_Connections.end(), other);
        if (it != m_Connections.end()) {
            m_Connections.erase(it);
        }
    }

    void ValueProperty::SignalValueChanged(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) {
        for (auto& handler : m_Connections) {
            if (this_ == handler) continue;

            handler->Receive(this, oldValue, newValue);
        }
    }

    void ValueProperty::Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue) {
        std::any oldValue_ = m_value;

        m_value = newValue;

        SignalValueChanged(this_, oldValue_, m_value);
    }

}    // namespace nkentseu