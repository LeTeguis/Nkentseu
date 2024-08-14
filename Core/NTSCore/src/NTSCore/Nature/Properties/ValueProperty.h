//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:31:39 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_VALUE_PROPERTY_H__
#define __NKENTSEU_VALUE_PROPERTY_H__

#pragma once

#include <any>
#include <vector>
#include <functional>
#include <memory>
#include <string>

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"

namespace nkentseu {
    class ValueProperty;

    // Define a type alias for the ValueChange function
    using ValueChange = std::function<void(ValueProperty*, const std::any&, const std::any&)>;
    bool operator == (const ValueChange& lhs, const ValueChange& rhs);
    bool operator != (const ValueChange& lhs, const ValueChange& rhs);

    // Macro to construct the data type
    #define VALUE_CONSTRUCT_DATA(type) virtual std::string GetType() override {return #type;}\
                                            virtual type GetValue() const { return std::any_cast<type>(Get()); }\
                                            virtual void SetValue(type value) {SetValue(value);}

        // Placeholder macro for std::bind
    #define PHD(i) std::placeholders::_##i

    // Macro to bind a method for transmitting value changes
    #define I_TRANSMIT(method_) std::bind(&method_, this, PHD(1), PHD(2), PHD(3))

    // Macro to bind a method for receiving value changes
    #define I_RECEIVE(method_, object_) std::bind(&method_, &object_, PHD(1), PHD(2), PHD(3)) 


    class NKENTSEU_API ValueProperty {
    public:
        // Default constructor
        ValueProperty();

        // Constructor with initial value
        ValueProperty(const std::any& value);

        // Virtual function to get the type of the value property
        virtual std::string GetType() = 0;

        // Get the current value
        std::any Get() const;

        // Set a new value
        void Set(const std::any& value);

        // Add an observer to the value property
        void AddObserver(const ValueChange& observer);

        // Connect two value properties so that they receive updates from each other
        void Connect(ValueProperty* other);

        // Remove an observer from the value property
        void RemoveObserver(const ValueChange& observer);

        // Disconnect two connected value properties
        void Disconnect(ValueProperty* other);

        // Virtual function to convert the value property to a string representation
        virtual std::string ToString() const = 0;

        friend std::string ToString(const ValueProperty& vp) {
            return vp.ToString();
        }

    protected:
        // Signal that the value has changed to all observers except the origin
        void SignalValueChanged(ValueProperty* this_, const std::any& oldValue, const std::any& newValue);

        // Receive a value change from another value property
        virtual void Receive(ValueProperty* this_, const std::any& oldValue, const std::any& newValue);

    protected:
        std::any m_value;

    private:
        std::vector<ValueChange> m_Observers;
        std::vector<ValueProperty*> m_Connections;

        struct ValueChangeComparator {
            bool operator()(const ValueChange& lhs, const ValueChange& rhs) const {
                // Compare les adresses de m�moire des deux fonctions
                return std::addressof(lhs) == std::addressof(rhs);
            }
        };
    };
} // namespace nkentseu

#endif    // __NKENTSEU_VALUE_PROPERTY_H__