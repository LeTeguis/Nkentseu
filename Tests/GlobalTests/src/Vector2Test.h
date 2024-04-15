//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 1:15:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_VECTOR2TEST_H__
#define __NKENTSEU_VECTOR2TEST_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector2.h>
#include <Unitest/Unitest.h>

namespace nkentseu {
    class NKENTSEU_API Vector2Test
    {
        public:
            Vector2Test();
            ~Vector2Test();

            // Test constructor with default values
            void Test_DefaultConstructor(const std::string& name);
            // Test constructor with float values
            void Test_ConstructorFloat(const std::string& name);
            // Test constructor with single float value
            void Test_ConstructorSingleFloat(const std::string& name);
            // Test constructor with float pointer
            void Test_ConstructorFloatPtr(const std::string& name);
            // Test copy constructor
            void Test_CopyConstructor(const std::string& name);
            // Test conversion constructors (Vector2i, Vector2u, Vector2d)
            // You can add similar tests for each conversion constructor
            void Test_ConversionConstructor_Vector2i(const std::string& name);
            // Test assignment operators (Vector2f, Vector2i, Vector2u, Vector2d)
            // You can add similar tests for each assignment operator
            void Test_AssignmentOperator_Vector2f(const std::string& name);
            // Test vector negation
            void Test_Negation(const std::string& name);
            // Test element access using operator[]
            void Test_ElementAccess(const std::string& name);
            // Test addition assignment and operator
            void Test_Addition(const std::string& name);
            // Test subtraction assignment and operator
            void Test_Subtraction(const std::string& name);
            // Test multiplication assignment and operator
            void Test_Multiplication(const std::string& name);
            // Test division assignment and operator
            void Test_Division(const std::string& name);
            // Test vector equality and inequality
            void Test_Equality(const std::string& name);
            // Test increment and decrement operators
            void Test_IncrementDecrement(const std::string& name);
            // Test SumCord(const std::string& name)
            void Test_SumCord(const std::string& name);
            // Test Distance(const std::string& name)
            void Test_Distance(const std::string& name);
            // Test Dot(const std::string& name)
            void Test_Dot(const std::string& name);
            // Test LenSq(const std::string& name)
            void Test_LenSq(const std::string& name);
            // Test Len(const std::string& name)
            void Test_Len(const std::string& name);
            // Test Normalize(const std::string& name)
            void Test_Normalize(const std::string& name);
            // Test Normalized(const std::string& name)
            void Test_Normalized(const std::string& name);//*/
        private:
    };
} // namespace nkentseu

#endif    // __NKENTSEU_VECTOR2TEST_H__