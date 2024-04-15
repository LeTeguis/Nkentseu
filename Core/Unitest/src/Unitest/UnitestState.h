//
// File: UnitestState.h
// Created by: TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:34:53 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
// This header file defines the `UnitestState` enumeration used to represent
// the various states a unit test can be in.

#ifndef __NKENTSEU_UNITESTSTATE_H__
#define __NKENTSEU_UNITESTSTATE_H__

#pragma once

#include "System/System.h" // Assuming System.h contains necessary includes

namespace nkentseu {

    /**
     * @enum UnitestState
     * @brief Enumeration representing the possible states of a unit test.
     */
    enum class NKENTSEU_API UnitestState {
        /**
         * The state of the unit test is unknown or not yet determined.
         */
        Indeterminate,

        /**
         * The unit test is currently being executed.
         */
        Running,

        /**
        * The unit test has been deliberately ignored and not executed.
        */
        Ignored,

        // Test results
        /**
        * The unit test has passed successfully.
        */
        Passed,

        /**
        * The unit test has failed due to an assertion failure or other error.
        */
        Failed,

        // Errors
        /**
        * An assertion within the unit test has failed.
        */
        AssertionFailed,

        /**
        * The unit test has thrown an unhandled exception.
        */
        UnhandledException,

        /**
        * The unit test execution has timed out.
        */
        Timeout,
    };
} // namespace nkentseu

#endif // __NKENTSEU_UNITESTSTATE_H__
