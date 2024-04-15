//
// File: UnitestInfos.h
// Created by: TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:34:01 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
// This header file defines the `UnitestInfo` class used to store detailed
// information about a single execution of a unit test.

#ifndef __NKENTSEU_UNITESTINFOS_H__
#define __NKENTSEU_UNITESTINFOS_H__

#pragma once

// Assuming System.h and System/Nature/Base.h provide necessary includes
#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {

    /**
     * @class UnitestInfo
     * @brief This class encapsulates detailed information about a single execution
     *        of a unit test.
     */
    class NKENTSEU_API UnitestInfo {
    public:
        // Default constructor (initializes all members with default values)
        UnitestInfo();

        // Constructor with parameters to initialize all members
        UnitestInfo(const std::string& name, bool condition, const std::string& file,
            uint32 line, const std::string& function, const std::string& message);

        // Getter methods to access member variables
        const std::string& GetName() const; // Returns the name of the test
        void SetName(const std::string& name); // Sets the name of the test

        bool IsSuccessfull() const; // Returns true if the test passed, false otherwise
        void SetSuccessfull(bool condition); // Sets the success condition of the test

        const std::string& GetFile() const; // Returns the file where the test is located
        void SetFile(const std::string& file); // Sets the file where the test is located

        uint32 GetLine() const; // Returns the line number where the test is defined
        void SetLine(uint32 line); // Sets the line number where the test is defined

        const std::string& GetFunction() const; // Returns the name of the test function
        void SetFunction(const std::string& function); // Sets the name of the test function

        const std::string& GetMessage() const; // Returns any message associated with the test
        void SetMessage(const std::string& message); // Sets a message associated with the test

    private:
        // Member variables to store test information
        std::string m_Name;  // Name of the test
        bool m_Condition;    // Success condition of the test (true for passed)
        std::string m_File;   // File where the test is located
        uint32 m_Line;      // Line number where the test is defined
        std::string m_Function; // Name of the test function
        std::string m_Message; // Message associated with the test (optional)
    };

} // namespace nkentseu

#endif // __NKENTSEU_UNITESTINFOS_H__
