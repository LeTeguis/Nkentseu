//
// File: UnitestEntry.cpp
// Created by: TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:36:10 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
// This implementation file defines the member functions of the `UnitestEntry` class.

#include "UnitestPch/ntspch.h"

#include "UnitestEntry.h" // Assuming UnitestEntry.h contains the class definition
#include <optional> // Not used in this code, but included if needed for future changes

namespace nkentseu {

    // Constructor to initialize the unit test entry with name, function, and description
    UnitestEntry::UnitestEntry(const std::string& name, UnitestEntryFunction function, const std::string& description)
        : m_Name(name), m_Function(function), m_Description(description) {}

    // Resets the unit test entry, clearing execution results and information
    void UnitestEntry::Reset() {
        // Set passed and failed counts to 0
        m_Passeds = 0;
        m_Faileds = 0;
        // Clear the list of execution information
        m_UnitestInfos.clear();
    }

    // Runs the test function and captures results, returning true if successful
    bool UnitestEntry::Run(const std::string& context) {
        // Check if the test function is defined and the context matches the entry name
        if (m_Function != nullptr && context == this->GetName()) {
            // Execute the test function with the provided context
            m_Function(context);
            // Return true indicating successful execution
            return true;
        }
        // Return false if the test function is not defined or the context doesn't match
        return false;
    }

    // Adds detailed information about a single test execution
    void UnitestEntry::AddUnitestInfo(const UnitestInfo& unitestInfo) {
        // Add the execution information to the list
        m_UnitestInfos.push_back(unitestInfo);
        // Update counters based on the test outcome
        if (unitestInfo.IsSuccessfull()) {
            m_Passeds++;
        }
        else {
            m_Faileds++;
        }
    }

    // Returns the total number of tests executed (passed + failed)
    uint32 UnitestEntry::GetTotal() const {
        // Return the sum of passed and failed tests
        return m_Passeds + m_Faileds;
    }

    // Getter methods to access member variables
    const std::string& UnitestEntry::GetName() const {
        return m_Name; // Returns the name of the test
    }
    const std::string& UnitestEntry::GetDescription() const {
        return m_Description; // Returns the description of the test (optional)
    }
    const std::vector<UnitestInfo>& UnitestEntry::GetUnitestInfos() const {
        return m_UnitestInfos; // Returns a reference to the list of execution information
    }

    UnitestInfo& UnitestEntry::GetUnitestInfo(uint32 index) {
        if (index < GetUnitestInfoCount()) {
            return m_UnitestInfos[index];
        }
        else {
            throw std::out_of_range("Index out of bounds for UnitestInfos");
        }
    }

    const uint32 UnitestEntry::GetUnitestInfoCount() const
    {
        return (uint32)m_UnitestInfos.size();
    }
    uint32 UnitestEntry::GetPassedCount() const {
        return m_Passeds; // Returns the number of passed tests
    }
    uint32 UnitestEntry::GetFailedCount() const {
        // Suggesting renaming `GetFailedCount` to `GetFailedTestsCount` for clarity
        return m_Faileds; // Returns the number of failed tests
    }
    const UnitestEntryFunction& UnitestEntry::GetFunction() const {
        return m_Function; // Returns a reference to the test function
    }

} // namespace nkentseu
