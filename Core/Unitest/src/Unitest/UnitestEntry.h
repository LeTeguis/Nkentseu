//
// File: UnitestEntry.h
// Created by: TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:36:10 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
// This header file defines the `UnitestEntry` class used to represent a single
// unit test entry. A unit test entry encapsulates information about a specific
// test, including its name, description, test function, execution results,
// and detailed information for each test execution.

#ifndef __NKENTSEU_UNITESTENTRY_H__
#define __NKENTSEU_UNITESTENTRY_H__

#pragma once

#include "System/System.h" // Assuming System.h provides necessary includes
#include <System/Nature/Base.h> // Assuming System/Nature/Base.h provides necessary includes
#include <vector> // For std::vector
#include <functional> // For std::function
#include "Unitest/UnitestInfos.h" // Assuming UnitestInfos.h defines the UnitestInfo class

namespace nkentseu {

    /**
     * @typedef UnitestEntryFunction
     * @brief An alias for `std::function<void(const std::string&)>` used to represent
     *        the type of the test function.
     */
    using UnitestEntryFunction = std::function<void(const std::string&)>;

    /**
     * @class UnitestEntry
     * @brief This class represents a single unit test entry. It stores information
     *        about the test, including its name, description, test function,
     *        execution results, and detailed information for each test execution.
     */
    class NKENTSEU_API UnitestEntry {
    public:
        // Constructor to initialize the unit test entry with name, function, and description
        UnitestEntry(const std::string& name, UnitestEntryFunction function, const std::string& description = "");

        // Resets the unit test entry, clearing execution results and information
        void Reset();

        // Runs the test function and captures results, returning true if successful
        bool Run(const std::string& context);

        // Adds detailed information about a single test execution
        void AddUnitestInfo(const UnitestInfo& unitestInfo);

        // Returns the total number of tests executed (passed + failed)
        uint32 GetTotal() const;

        // Getter methods to access member variables
        const std::string& GetName() const; // Returns the name of the test
        const std::string& GetDescription() const; // Returns the description of the test (optional)
        const std::vector<UnitestInfo>& GetUnitestInfos() const; // Returns a reference to the list of execution information

        UnitestInfo& GetUnitestInfo(uint32 index);
        const uint32 GetUnitestInfoCount() const
            ;


        uint32 GetPassedCount() const; // Returns the number of passed tests
        uint32 GetFailedCount() const; // Returns the number of failed tests
        const UnitestEntryFunction& GetFunction() const; // Returns a reference to the test function

    private:
        // Member variables to store test information
        std::string m_Name;  // Name of the test
        UnitestEntryFunction m_Function;  // The test function to be executed
        std::string m_Description;  // Description of the test (optional)
        std::vector<UnitestInfo> m_UnitestInfos;  // List of information for each test execution
        uint32 m_Passeds;  // Counter for the number of passed tests
        uint32 m_Faileds;  // Counter for the number of failed tests
    };

} // namespace nkentseu

#endif // __NKENTSEU_UNITESTENTRY_H__
