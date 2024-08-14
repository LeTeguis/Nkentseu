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

#include "NTSCore/System.h" // Assuming System.h contains necessary includes

namespace nkentseu {

    ENUMERATION(UnitestState, uint32,
        ENUM_TO_STRING_BEGIN
        ENUM_TO_STRING_SET_CONTENT(Enum::Running)
        ENUM_TO_STRING_SET_CONTENT(Enum::Ignored)
        ENUM_TO_STRING_SET_CONTENT(Enum::Passed)
        ENUM_TO_STRING_SET_CONTENT(Enum::Failed)
        ENUM_TO_STRING_SET_CONTENT(Enum::AssertionFailed)
        ENUM_TO_STRING_SET_CONTENT(Enum::UnhandledException)
        ENUM_TO_STRING_SET_CONTENT(Enum::Timeout)
        ENUM_TO_STRING_END(Enum::Indeterminate),
        ,
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
    );

} // namespace nkentseu

#endif // __NKENTSEU_UNITESTSTATE_H__
