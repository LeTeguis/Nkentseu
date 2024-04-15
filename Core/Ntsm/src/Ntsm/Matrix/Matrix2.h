//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:23:37 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MATRIX2_H__
#define __NKENTSEU_MATRIX2_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector4.h>
#include <Ntsm/Vector/Vector2.h>
#include <string>

namespace nkentseu {
    class NKENTSEU_API Matrix2f {
    public:
        union {
            float32 ptr[4];
            Vector2f ptrLigne[2];
            Vector4f vec;

            struct {
                float32 xx; float32 xy;
                float32 yx; float32 yy;
            };

            struct {
                float32 ptr_ligne1[2];
                float32 ptr_ligne2[2];
            };
        };

        // Constructors
        Matrix2f();
        Matrix2f(float32 value);
        Matrix2f(float32 xx, float32 xy, float32 yx, float32 yy);
        Matrix2f(const float32* ptr);
        Matrix2f(const Vector4f& vec);
        Matrix2f(const Vector2f& ligne1, const Vector2f& ligne2);
        Matrix2f(const Matrix2f& mat);

        // Addition
        Matrix2f& operator+=(const Matrix2f& other);
        Matrix2f operator+(const Matrix2f& other);
        friend Matrix2f operator+(const Matrix2f& mat1, const Matrix2f& mat2) {
            Matrix2f m(mat1);
            m += mat2;
            return m;
        }

        // Subtraction
        Matrix2f& operator-=(const Matrix2f& other);
        Matrix2f operator-(const Matrix2f& other);
        friend Matrix2f operator-(const Matrix2f& mat1, const Matrix2f& mat2) {
            Matrix2f m(mat1);
            m -= mat2;
            return m;
        }

        // Multiplication
        Matrix2f& operator*=(const Matrix2f& other);
        Matrix2f operator*(const Matrix2f& other);
        friend Matrix2f operator*(const Matrix2f& mat1, const Matrix2f& mat2) {
            Matrix2f m(mat1);
            m *= mat2;
            return m;
        }

        friend Vector2f operator*(const Matrix2f& mat1, const Vector2f& v2) {
            return Vector2f(mat1.xx * v2.x + mat1.xy * v2.y, mat1.yx * v2.x + mat1.yy * v2.y);
        }
        friend Vector2f operator*(const Vector2f& v2, const Matrix2f& mat1) {
            return Vector2f(
                v2.x * mat1.xx + v2.y * mat1.yx,
                v2.x * mat1.xy + v2.y * mat1.yy
            );
        }

        // Division
        Matrix2f& operator/=(const Matrix2f& other);
        Matrix2f operator/(const Matrix2f& other);
        friend Matrix2f operator/(const Matrix2f& mat1, const Matrix2f& mat2) {
            Matrix2f m(mat1);
            m /= mat2;
            return m;
        }

        float32* operator[](const unsigned& rowNo) {
            return (rowNo % 2 == 0) ? &ptr_ligne1[0] : &ptr_ligne2[0];
        }

        Matrix2f& operator=(const Matrix2f& other) {
            vec = other.vec;
            return *this;
        }


        bool operator==(const Matrix2f& other) const {
            return vec == other.vec;
        }

        bool operator!=(const Matrix2f& other) const {
            return vec != other.vec;
        }

        Matrix2f Transpose();
        float32 Determinant();
        Matrix2f Inverse();
        float32 Trace();
        float32 Len();
        float32 LenSq();
        Matrix2f Power(int32 n);
        Vector2f Diagonal();

        Matrix2f Orthogonalize();
        Vector2f GetEigenvalues();
        Vector2f GetEigenvectors();


        friend std::ostream& operator<<(std::ostream& os, const Matrix2f& e) {
            return os << e.ToString();
        }

        std::string ToString() const {
            std::stringstream ss;
            ss << "Matrix2f[[" << xx << ", " << xy << "]" << ", [" << yx << ", " << yy << "]]";
            return ss.str();
        }

        friend std::string ToString(const Matrix2f& v) {
            return v.ToString();
        }
    };

    class NKENTSEU_API Matrix2d {
    public:
        union {
            float64 ptr[4];
            Vector2d ptrLigne[2];
            Vector4d vec;

            struct {
                float64 xx; float64 xy;
                float64 yx; float64 yy;
            };

            struct {
                float64 ptr_ligne1[2];
                float64 ptr_ligne2[2];
            };
        };

        // Constructors
        Matrix2d();
        Matrix2d(float64 value);
        Matrix2d(float64 xx, float64 xy, float64 yx, float64 yy);
        Matrix2d(const float64* ptr);
        Matrix2d(const Vector4d& vec);
        Matrix2d(const Vector2d& ligne1, const Vector2d& ligne2);
        Matrix2d(const Matrix2d& mat);

        // Addition
        Matrix2d& operator+=(const Matrix2d& other);
        Matrix2d operator+(const Matrix2d& other);
        friend Matrix2d operator+(const Matrix2d& mat1, const Matrix2d& mat2);

        // Subtraction
        Matrix2d& operator-=(const Matrix2d& other);
        Matrix2d operator-(const Matrix2d& other);
        friend Matrix2d operator-(const Matrix2d& mat1, const Matrix2d& mat2);

        // Multiplication
        Matrix2d& operator*=(const Matrix2d& other);
        Matrix2d operator*(const Matrix2d& other);
        friend Matrix2d operator*(const Matrix2d& mat1, const Matrix2d& mat2);

        friend Vector2d operator*(const Matrix2d& mat1, const Vector2d& v2) {
            return Vector2d(mat1.xx * v2.x + mat1.xy * v2.y, mat1.yx * v2.x + mat1.yy * v2.y);
        }
        friend Vector2d operator*(const Vector2d& v2, const Matrix2d& mat1) {
            return Vector2d(
                v2.x * mat1.xx + v2.y * mat1.yx,
                v2.x * mat1.xy + v2.y * mat1.yy
            );
        }

        // Division
        Matrix2d& operator/=(const Matrix2d& other);
        Matrix2d operator/(const Matrix2d& other);
        friend Matrix2d operator/(const Matrix2d& mat1, const Matrix2d& mat2);

        float64* operator[](const unsigned& rowNo) {
            return (rowNo % 2 == 0) ? &ptr_ligne1[0] : &ptr_ligne2[0];
        }

        Matrix2d& operator=(const Matrix2d& other) {
            vec = other.vec;
            return *this;
        }


        bool operator==(const Matrix2d& other) {
            return vec == other.vec;
        }

        bool operator!=(const Matrix2d& other) {
            return vec != other.vec;
        }

        Matrix2d Transpose();
        float64 Determinant();
        Matrix2d Inverse();
        float64 Trace();
        float64 Len();
        float64 LenSq();
        Matrix2d Power(int32 n);
        Vector2d Diagonal();
        Matrix2d Orthogonalize();
        Vector2d GetEigenvalues();
        Vector2d GetEigenvectors();


        friend std::ostream& operator<<(std::ostream& os, const Matrix2d& e) {
            return os << e.ToString();
        }

        std::string ToString() const {
            std::stringstream ss;
            ss << "Matrix2d[[" << xx << ", " << xy << "]" << ", [" << yx << ", " << yy << "]]";
            return ss.str();
        }

        friend std::string ToString(const Matrix2d& v) {
            return v.ToString();
        }
    };

    using Matrice2d = Matrix2d;
    using Mat2d = Matrix2d;

    using Matrix2 = Matrix2f;
    using Matrice2 = Matrix2f;
    using Mat2 = Matrix2f;
} // namespace nkentseu

#endif    // __NKENTSEU_MATRIX2_H__