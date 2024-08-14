//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:23:43 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MATRIX3_H__
#define __NKENTSEU_MATRIX3_H__

#pragma once

#include "NTSCore/System.h"
#include <NTSMaths/Vector/Vector3.h>

namespace nkentseu {

    namespace maths
    {
        /*struct NKENTSEU_API Matrix3f {
            union {
                float32 data[9];
                float32 mat[3][3];
                struct {
                    float32 m00, m01, m02; // colonne 0
                    float32 m10, m11, m12; // colonne 1
                    float32 m20, m21, m22; // colonne 2
                };
                struct {
                    float32 vxx, vxy, vxz; // colonne 0
                    float32 vyx, vyy, vyz; // colonne 1
                    float32 vzx, vzy, vzz; // colonne 2
                };
                struct {
                    Vector3f col_1;
                    Vector3f col_2;
                    Vector3f col_3;
                };
                Vector3f component[3];
            };

            // Constructors
            Matrix3f();
            Matrix3f(float32 value);
            Matrix3f(float32 xx, float32 xy, float32 xz, float32 yx, float32 yy, float32 yz, float32 zx, float32 zy, float32 zz);
            Matrix3f(const float32* ptr);
            Matrix3f(const Vector3f& diagonal);
            Matrix3f(const Vector3f& line1, const Vector3f& line2, const Vector3f& line3);
            Matrix3f(const Matrix3f& mat);

            // Addition
            Matrix3f& operator+=(const Matrix3f& other);
            Matrix3f operator+(const Matrix3f& other);
            friend Matrix3f operator+(const Matrix3f& mat1, const Matrix3f& mat2);

            // Subtraction
            Matrix3f& operator-=(const Matrix3f& other);
            Matrix3f operator-(const Matrix3f& other);
            friend Matrix3f operator-(const Matrix3f& mat1, const Matrix3f& mat2);

            // Multiplication
            Matrix3f& operator*=(const Matrix3f& other);
            Matrix3f operator*(const Matrix3f& other);
            friend Matrix3f operator*(const Matrix3f& mat1, const Matrix3f& mat2);
            /*friend Vector3f operator*(const Matrix3f& mat1, const Vector3f& v2) {
                return Vector3f(mat1.xx * v2.x + mat1.xy * v2.y + mat1.xz * v2.z, mat1.yx * v2.x + mat1.yy * v2.y + mat1.yz * v2.z, mat1.zx * v2.x + mat1.zy * v2.y + mat1.zz * v2.z);
            }
            friend Vector3f operator*(const Vector3f& v2, const Matrix3f& mat1) {
                return Vector3f(
                    v2.x * mat1.xx + v2.y * mat1.yx + v2.z * mat1.zx,
                    v2.x * mat1.xy + v2.y * mat1.yy + v2.z * mat1.zy,
                    v2.x * mat1.xz + v2.y * mat1.yz + v2.z * mat1.zz
                );
            }* /

            // Division
            Matrix3f& operator/=(const Matrix3f& other);
            Matrix3f operator/(const Matrix3f& other);
            friend Matrix3f operator/(const Matrix3f& mat1, const Matrix3f& mat2);

            /*float32* operator[](const unsigned& rowNo) {
                if (rowNo % 3 == 0) return &ptr_lines[0][0];
                if (rowNo % 3 == 1) return &ptr_lines[1][0];
                return &ptr_lines[2][0];
            }* /

            Matrix3f& operator=(const Matrix3f& other);


            bool operator==(const Matrix3f& other) const;

            bool operator!=(const Matrix3f& other) const;

            Matrix3f Transpose();
            float32 Determinant();
            Matrix3f Inverse();
            float32 Trace();
            float32 Len();
            float32 LenSq();
            Matrix3f Power(int32 n);
            Vector3f Diagonal();
            std::string ToString() const;

            friend std::ostream& operator<<(std::ostream& out, const Matrix3f& mat) {
                out << mat.ToString();
                return out;
            }

            friend std::string ToString(const Matrix3f& v) {
                return v.ToString();
            }

        };*/

        class NKENTSEU_API Matrix3d {
        public:
            union {
                float64 ptr[9];
                Vector3d ptr_lines[3];

                struct {
                    float64 xx; float64 xy; float64 xz;
                    float64 yx; float64 yy; float64 yz;
                    float64 zx; float64 zy; float64 zz;
                };

                struct {
                    float64 ptr_line1[3];
                    float64 ptr_line2[3];
                    float64 ptr_line3[3];
                };
            };

            // Constructors
            Matrix3d();
            Matrix3d(float64 value);
            Matrix3d(float64 xx, float64 xy, float64 xz, float64 yx, float64 yy, float64 yz, float64 zx, float64 zy, float64 zz);
            Matrix3d(const float64* ptr);
            Matrix3d(const Vector3d& diagonal);
            Matrix3d(const Vector3d& line1, const Vector3d& line2, const Vector3d& line3);
            Matrix3d(const Matrix3d& mat);

            // Addition
            Matrix3d& operator+=(const Matrix3d& other);
            Matrix3d operator+(const Matrix3d& other);
            friend Matrix3d operator+(const Matrix3d& mat1, const Matrix3d& mat2);

            // Subtraction
            Matrix3d& operator-=(const Matrix3d& other);
            Matrix3d operator-(const Matrix3d& other);
            friend Matrix3d operator-(const Matrix3d& mat1, const Matrix3d& mat2);

            // Multiplication
            Matrix3d& operator*=(const Matrix3d& other);
            Matrix3d operator*(const Matrix3d& other);
            friend Matrix3d operator*(const Matrix3d& mat1, const Matrix3d& mat2);
            friend Vector3d operator*(const Matrix3d& mat1, const Vector3d& v2) {
                return Vector3d(mat1.xx * v2.x + mat1.xy * v2.y + mat1.xz * v2.z, mat1.yx * v2.x + mat1.yy * v2.y + mat1.yz * v2.z, mat1.zx * v2.x + mat1.zy * v2.y + mat1.zz * v2.z);
            }
            friend Vector3d operator*(const Vector3d& v2, const Matrix3d& mat1) {
                return Vector3d(
                    v2.x * mat1.xx + v2.y * mat1.yx + v2.z * mat1.zx,
                    v2.x * mat1.xy + v2.y * mat1.yy + v2.z * mat1.zy,
                    v2.x * mat1.xz + v2.y * mat1.yz + v2.z * mat1.zz
                );
            }

            // Division
            Matrix3d& operator/=(const Matrix3d& other);
            Matrix3d operator/(const Matrix3d& other);
            friend Matrix3d operator/(const Matrix3d& mat1, const Matrix3d& mat2);

            float64* operator[](const unsigned& rowNo) {
                if (rowNo % 3 == 0) return &ptr_lines[0][0];
                if (rowNo % 3 == 1) return &ptr_lines[1][0];
                return &ptr_lines[2][0];
            }

            Matrix3d& operator=(const Matrix3d& other);


            bool operator==(const Matrix3d& other) const;

            bool operator!=(const Matrix3d& other) const;

            Matrix3d Transpose();
            float64 Determinant();
            Matrix3d Inverse();
            float64 Trace();
            float64 Len();
            float64 LenSq();
            Matrix3d Power(int32 n);
            Vector3d Diagonal();
            std::string ToString() const;

            friend std::string ToString(const Matrix3d& v) {
                return v.ToString();
            }

            friend std::ostream& operator<<(std::ostream& out, const Matrix3d& mat) {
                out << mat.ToString();
                return out;
            }

        };

        using Matrice3d = Matrix3d;
        using Mat3d = Matrix3d;

        //using Matrix3 = Matrix3f;
        //using Matrice3 = Matrix3f;
        //using Mat3 = Matrix3f;
    }
} // namespace nkentseu

#endif    // __NKENTSEU_MATRIX3_H__