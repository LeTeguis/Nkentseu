//
// File: Unitest.h
// Created by: TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:35:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
// This header file defines the `Unitest` class used to manage and run unit tests.

#ifndef __NKENTSEU_UNITEST_H__
#define __NKENTSEU_UNITEST_H__

#pragma once

#include <memory>

#include "System/System.h" // Assuming System.h provides necessary includes
#include <System/Nature/Base.h> // Assuming System/Nature/Base.h provides necessary includes
#include <Unitest/UnitestState.h> // Assuming UnitestState.h defines the UnitestState enum
#include <unordered_map> // For std::unordered_map
#include "Unitest/UnitestEntry.h" // Assuming UnitestEntry.h defines the UnitestEntry class
#include <Logger/Logger.h>
#include <Logger/Formatter.h>

namespace nkentseu {

    // Type alias for a function registry name (can be a string)
    using FunctionRegistryName = std::string;

    #define UnitestRegisterClass(name, description) #name, std::bind(&name, this, std::placeholders::_1), description
    #define UnitestRegisterFunction(name, description) #name, name, description
    #define UnitestRegisterLambda(name, lambda, description) #name, lambda, description

    /**
     * @class Unitest
     * @brief This class provides an interface for managing and running unit tests.
     *        It allows registering test functions, running all tests, and asserting
     *        test conditions. It also offers options for printing detailed test results.
     */
    class NKENTSEU_API Unitest {
    public:
        // Returns a reference to the singleton instance of the Unitest class
        static Unitest& Instance();

        // Registers a unit test with a name, function, and optional description
        void Register(const std::string& name, UnitestEntryFunction function, const std::string& description = "");

        // Runs all registered unit tests and returns the number of failures (0 for success)
        int32 Run();

        // Asserts a test condition and returns the corresponding UnitestState
        // (passed if condition is true, failed otherwise)
        template<typename... Args>
        UnitestState Verify(bool condition, const std::string& file, uint32 line, const std::string& function, const std::string& errorString, const std::string& pitch, const std::string& format = "", Args&&... args) {
            std::string message = Formatter::Instance().Format(format, args...);
            message = Formatter::Instance().Format("[{0}{1}]; {2}", pitch, errorString, message);
            return VerifyInternal(condition, file, line, function, message);
        }

        bool IsPassedDetailPrint() const;
        void PrintPassedDetails(bool print);

        bool IsFailedDetailPrint() const;
        void PrintFailedDetails(bool print);

        bool IsDetailPrint() const;
        void PrintDetails(bool print);

        Logger& Get() {
            return m_Logger;
        }

    private:
        Logger m_Logger;

        // Type alias for a smart pointer to a UnitestEntry object
        using UnitestEntryPtr = std::shared_ptr<UnitestEntry>;

        Unitest(); // Private constructor (singleton pattern)

        FunctionRegistryName m_CurrentRegister; // Name of the currently registered test
        std::unordered_map<FunctionRegistryName, UnitestEntryPtr> m_UnitestList; // Map of registered tests
        bool m_PrintFailedDetails = true; // Flag for printing details of failed tests
        bool m_PrintPassedDetails = true; // Flag for printing details of passed tests
        bool m_PrintDetails = true; 


        UnitestState VerifyInternal(bool condition, const std::string& file, uint32 line, const std::string& function, const std::string& message = "");
    };

    int32 NKENTSEU_API RunTest();

    #define UNITEST_REGISTRY                                ::nkentseu::Unitest::Instance().Register
    #define UNITEST_VERIFY                                  ::nkentseu::Unitest::Instance().Verify
    #define UNITEST_HEADER                                  __FILE__, __LINE__, __FUNCTION__

    #define STRAUT(a)                                       #a

    #define UNITEST(value, ... )                            UNITEST_VERIFY((value), UNITEST_HEADER, "", #value, ##__VA_ARGS__)
    #define UNITEST_NULL(value, ... )                       UNITEST_VERIFY((value == nullptr), UNITEST_HEADER, " Is Null", #value, ##__VA_ARGS__)
    #define UNITEST_TRUE(value, ... )                       UNITEST_VERIFY(value, UNITEST_HEADER, " Is True", #value, ##__VA_ARGS__)
    #define UNITEST_FALSE(value, ...)                       UNITEST_VERIFY(!(value), UNITEST_HEADER, " Is False", #value, ##__VA_ARGS__)
    #define UNITEST_EQUAL(value1, value2, ... )             UNITEST_VERIFY((value1) == (value2), UNITEST_HEADER, "", STRAUT(value1 == value2), ##__VA_ARGS__)
    #define UNITEST_DIFFERENT(value1, value2, ... )         UNITEST_VERIFY((value1) != (value2), UNITEST_HEADER, "", STRAUT(value1 != value2), ##__VA_ARGS__)
    #define UNITEST_LESS(value1, value2, ... )              UNITEST_VERIFY((value1) < (value2), UNITEST_HEADER, "", STRAUT(value1 < value2), ##__VA_ARGS__)
    #define UNITEST_GREATER(value1, value2, ... )           UNITEST_VERIFY((value1) > (value2), UNITEST_HEADER, "", STRAUT(value1 > value2), ##__VA_ARGS__)
    #define UNITEST_LESS_OR_EQUAL(value1, value2, ... )     UNITEST_VERIFY((value1) <= (value2), UNITEST_HEADER, "", STRAUT(value1 <= value2), ##__VA_ARGS__)
    #define UNITEST_GREATER_OR_EQUAL(value1, value2, ... )  UNITEST_VERIFY((value1) >= (value2), UNITEST_HEADER, "", STRAUT(value1 >= value2), ##__VA_ARGS__)
} // namespace nkentseu

#endif // __NKENTSEU_UNITEST_H__