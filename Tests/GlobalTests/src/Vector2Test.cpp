//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 1:15:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "Vector2Test.h"

//#include "Ntsm/Utils.h"

namespace nkentseu {

    Vector2Test::Vector2Test(){
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_DefaultConstructor, "Test constructor with default values"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_ConstructorFloat, "Test constructor with float values"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_ConstructorSingleFloat, "Test constructor with single float value"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_ConstructorFloatPtr, "Test constructor with float pointer"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_CopyConstructor, "Test copy constructor"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_ConversionConstructor_Vector2i, "Test conversion constructors (Vector2i)"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_AssignmentOperator_Vector2f, "Test assignment operators (Vector2f)"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Negation, "Test vector negation"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_ElementAccess, "Test element access using operator[]"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Addition, "Test addition assignment and operator"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Subtraction, "Test subtraction assignment and operator"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Multiplication, "Test multiplication assignment and operator"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Division, "Test division assignment and operator"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Equality, "Test vector equality and inequality"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_IncrementDecrement, "Test increment and decrement operators"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_SumCord, "Test SumCord() method"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Distance, "Test Distance() method"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Dot, "Test Dot() method"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_LenSq, "Test LenSq() method"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Len, "Test Len() method"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Normalize, "Test Normalize() method"));
        UNITEST_REGISTRY(UnitestRegisterClass(Vector2Test::Test_Normalized, "Test Normalized() method"));//*/
    }

    Vector2Test::~Vector2Test(){
    }

    // Test constructor with default values
    void Vector2Test::Test_DefaultConstructor(const std::string& name) {
        Vector2f vec;
        UNITEST_EQUAL(vec.x, 0.0f);
        UNITEST_EQUAL(vec.y, 0.0f);
    }

    // Test constructor with float values
    void Vector2Test::Test_ConstructorFloat(const std::string& name) {
        Vector2f vec(2.5f, 3.0f);
        UNITEST_EQUAL(vec.x, 2.5f);
        UNITEST_EQUAL(vec.y, 3.0f);
    }

    // Test constructor with single float value
    void Vector2Test::Test_ConstructorSingleFloat(const std::string& name) {
        Vector2f vec(1.0f);
        UNITEST_EQUAL(vec.x, 1.0f);
        UNITEST_EQUAL(vec.y, 1.0f);
    }

    // Test constructor with float pointer
    void Vector2Test::Test_ConstructorFloatPtr(const std::string& name) {
        float values[] = { 5.0f, 7.2f };
        Vector2f vec(values);
        UNITEST_EQUAL(vec.x, 5.0f);
        UNITEST_EQUAL(vec.y, 7.2f);
    }

    // Test copy constructor
    void Vector2Test::Test_CopyConstructor(const std::string& name) {
        Vector2f vec1(1.0f, 2.0f);
        Vector2f vec2(vec1);
        UNITEST_EQUAL(vec1.x, vec2.x);
        UNITEST_EQUAL(vec1.y, vec2.y);
    }

    // Test conversion constructors (Vector2i, Vector2u, Vector2d)
    // You can add similar tests for each conversion constructor
    void Vector2Test::Test_ConversionConstructor_Vector2i(const std::string& name) {
        Vector2i vecInt(3, 5);
        Vector2f vec(vecInt);
        UNITEST_EQUAL(vec.x, static_cast<float32>(vecInt.x));
        UNITEST_EQUAL(vec.y, static_cast<float32>(vecInt.y));
    }

    // Test assignment operators (Vector2f, Vector2i, Vector2u, Vector2d)
    // You can add similar tests for each assignment operator
    void Vector2Test::Test_AssignmentOperator_Vector2f(const std::string& name) {
        Vector2f vec1(1.0f, 2.0f);
        Vector2f vec2;
        vec2 = vec1;
        UNITEST_EQUAL(vec1, vec2);
    }

    // Test vector negation
    void Vector2Test::Test_Negation(const std::string& name) {
        Vector2f vec(2.0f, -3.5f);
        Vector2f negVec = -vec;
        UNITEST_EQUAL(negVec.x, -2.0f);
        UNITEST_EQUAL(negVec.y, 3.5f);
        UNITEST_EQUAL(negVec, Vector2f(-2.1f, 3.5f));
    }

    // Test element access using operator[]
    void Vector2Test::Test_ElementAccess(const std::string& name) {
        Vector2f vec(1.0f, 4.2f);
        UNITEST_EQUAL(vec[0], 1.0f);
        UNITEST_EQUAL(vec[1], 4.2f);
    }

    // Test addition assignment and operator
    void Vector2Test::Test_Addition(const std::string& name) {
        Vector2f vec1(1.0f, 2.0f);
        Vector2f vec2(3.5f, -1.0f);
        vec1 += vec2;
        UNITEST_EQUAL(vec1.x, 4.5f);
        UNITEST_EQUAL(vec1.y, 1.0f);
        Vector2f result = vec1 + vec2;
        UNITEST_EQUAL(result.x, 8.0f);
        UNITEST_EQUAL(result.y, 0.0f);
    }
    // Test subtraction assignment and operator
    void Vector2Test::Test_Subtraction(const std::string& name) {
        Vector2f vec1(5.0f, 3.0f);
        Vector2f vec2(2.0f, 1.5f);
        vec1 -= vec2;
        UNITEST_EQUAL(vec1.x, 3.0f);
        UNITEST_EQUAL(vec1.y, 1.5f);
        Vector2f result = vec1 - vec2;
        UNITEST_EQUAL(result.x, 1.0f);
        UNITEST_EQUAL(result.y, 0.0f);
    }

    // Test multiplication assignment and operator
    void Vector2Test::Test_Multiplication(const std::string& name) {
        Vector2f vec1(2.0f, 4.0f);
        Vector2f vec2(3.0f, 2.0f);
        vec1 *= vec2;
        UNITEST_EQUAL(vec1.x, 6.0f);
        UNITEST_EQUAL(vec1.y, 8.0f);
        Vector2f result = vec1 * vec2;
        UNITEST_EQUAL(result.x, 18.0f);
        UNITEST_EQUAL(result.y, 16.0f);
    }

    // Test division assignment and operator
    void Vector2Test::Test_Division(const std::string& name) {
        Vector2f vec1(6.0f, 8.0f);
        Vector2f vec2(2.0f, 4.0f);
        vec1 /= vec2;
        UNITEST_EQUAL(vec1.x, 3.0f);
        UNITEST_EQUAL(vec1.y, 2.0f);
        Vector2f result = vec1 / vec2;
        UNITEST_EQUAL(result.x, 1.5f);
        UNITEST_EQUAL(result.y, 0.5f);
    }

    // Test vector equality and inequality
    void Vector2Test::Test_Equality(const std::string& name) {
        Vector2f vec1(1.0e-6f, 2.0e-6f);
        Vector2f vec2(1.0e-6f, 2.0e-6f);
        UNITEST_TRUE(vec1 == vec2);

        vec2.x = 3.0e-6f;
        UNITEST_TRUE(vec1 == vec2); // , "{0}-{1}", "bobu", "cayou"
    }

    // Test increment and decrement operators
    void Vector2Test::Test_IncrementDecrement(const std::string& name) {
        Vector2f vec(1.0f, 2.0f);
        Vector2f preInc = ++vec;
        UNITEST_EQUAL(preInc.x, 2.0f);
        UNITEST_EQUAL(preInc.y, 3.0f);
        UNITEST_EQUAL(vec.x, 2.0f);
        UNITEST_EQUAL(vec.y, 3.0f);

        Vector2f postInc = vec++;
        UNITEST_EQUAL(postInc.x, 2.0f);
        UNITEST_EQUAL(postInc.y, 3.0f);
        UNITEST_EQUAL(vec.x, 3.0f);
        UNITEST_EQUAL(vec.y, 4.0f);

        Vector2f preDec = --vec;
        UNITEST_EQUAL(preDec.x, 2.0f);
        UNITEST_EQUAL(preDec.y, 3.0f);
        UNITEST_EQUAL(vec.x, 2.0f);
        UNITEST_EQUAL(vec.y, 3.0f);

        Vector2f postDec = vec--;
        UNITEST_EQUAL(postDec.x, 2.0f);
        UNITEST_EQUAL(postDec.y, 3.0f);
        UNITEST_EQUAL(vec.x, 1.0f);
        UNITEST_EQUAL(vec.y, 2.0f);
    }

    // Test SumCord(const std::string& name)
    void Vector2Test::Test_SumCord(const std::string& name) {
        Vector2f vec(2.5f, 3.7f);
        float sum = vec.SumCord();
        UNITEST_EQUAL(sum, 6.2f);
    }

    // Test Distance(const std::string& name)
    void Vector2Test::Test_Distance(const std::string& name) {
        Vector2f vec1(1.0f, 2.0f);
        Vector2f vec2(4.0f, 6.0f);
        float distance = vec1.Distance(vec2);
        UNITEST_EQUAL(distance, 5.0f); // You might need to adjust for a tolerance depending on your implementation
    }

    // Test Dot(const std::string& name)
    void Vector2Test::Test_Dot(const std::string& name) {
        Vector2f vec1(3.0f, -1.0f);
        Vector2f vec2(2.0f, 4.0f);
        float dotProduct = vec1.Dot(vec2);
        UNITEST_EQUAL(dotProduct, 2.0f);
    }

    // Test LenSq(const std::string& name)
    void Vector2Test::Test_LenSq(const std::string& name) {
        Vector2f vec(4.0f, 3.0f);
        float squaredLength = vec.LenSq();
        UNITEST_EQUAL(squaredLength, 25.0f);
    }

    // Test Len(const std::string& name)
    void Vector2Test::Test_Len(const std::string& name) {
        Vector2f vec(4.0f, 3.0f);
        float length = vec.Len();
        UNITEST_EQUAL(length, 5.0f); // You might need to adjust for a tolerance depending on your implementation
    }

    // Test Normalize(const std::string& name)
    void Vector2Test::Test_Normalize(const std::string& name) {
        Vector2f vec(3.0f, 4.0f);
        vec.Normalize();
        UNITEST_TRUE((maths::Abs(vec.Len() - 1.0f) < maths::Epsilon())); // Check if close to unit length
    }

    // Test Normalized(const std::string& name)
    void Vector2Test::Test_Normalized(const std::string& name) {
        Vector2f vec(3.0f, 4.0f);
        Vector2f normalizedVec = vec.Normalized();
        UNITEST_TRUE((maths::Abs(normalizedVec.Len() - 1.0f) < maths::Epsilon())); // Check if close to unit length
    }
}    // namespace nkentseu