//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:23:43 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Matrix3.h"

namespace nkentseu {

    namespace maths
    {
        // Default constructor
        Matrix3f::Matrix3f() {
            std::fill(ptr, ptr + 9, 0.0f);
        }

        // Constructor with a single value
        Matrix3f::Matrix3f(float32 value) {
            std::fill(ptr, ptr + 9, value);
        }

        // Constructor with individual elements
        Matrix3f::Matrix3f(float32 xx, float32 xy, float32 xz, float32 yx, float32 yy, float32 yz, float32 zx, float32 zy, float32 zz) {
            this->xx = xx; this->xy = xy; this->xz = xz;
            this->yx = yx; this->yy = yy; this->yz = yz;
            this->zx = zx; this->zy = zy; this->zz = zz;
        }

        // Constructor with array pointer
        Matrix3f::Matrix3f(const float32* ptr) {
            std::copy(ptr, ptr + 9, this->ptr);
        }

        // Constructor with a diagonal vector
        Matrix3f::Matrix3f(const Vector3f& diagonal) {
            xx = diagonal.x; xy = 0.0f; xz = 0.0f;
            yx = 0.0f; yy = diagonal.y; yz = 0.0f;
            zx = 0.0f; zy = 0.0f; zz = diagonal.z;
        }

        // Constructor with three row vectors
        Matrix3f::Matrix3f(const Vector3f& line1, const Vector3f& line2, const Vector3f& line3) {
            ptr_lines[0] = line1;
            ptr_lines[1] = line2;
            ptr_lines[2] = line3;
        }

        // Copy constructor
        Matrix3f::Matrix3f(const Matrix3f& mat) {
            std::copy(mat.ptr, mat.ptr + 9, ptr);
        }

        // Addition assignment operator (Matrix3f += Matrix3f)
        Matrix3f& Matrix3f::operator+=(const Matrix3f& other) {
            for (int i = 0; i < 9; ++i) {
                ptr[i] += other.ptr[i];
            }
            return *this;
        }

        // Addition operator (Matrix3f + Matrix3f)
        Matrix3f Matrix3f::operator+(const Matrix3f& other) {
            Matrix3f result(*this);
            result += other;
            return result;
        }

        // Friend addition operator (Matrix3f + Matrix3f)
        Matrix3f operator+(const Matrix3f& mat1, const Matrix3f& mat2) {
            Matrix3f result(mat1);
            result += mat2;
            return result;
        }

        // Subtraction assignment operator (Matrix3f -= Matrix3f)
        Matrix3f& Matrix3f::operator-=(const Matrix3f& other) {
            for (int i = 0; i < 9; ++i) {
                ptr[i] -= other.ptr[i];
            }
            return *this;
        }

        // Subtraction operator (Matrix3f - Matrix3f)
        Matrix3f Matrix3f::operator-(const Matrix3f& other) {
            Matrix3f result(*this);
            result -= other;
            return result;
        }

        // Friend subtraction operator (Matrix3f - Matrix3f)
        Matrix3f operator-(const Matrix3f& mat1, const Matrix3f& mat2) {
            Matrix3f result(mat1);
            result -= mat2;
            return result;
        }

        // Multiplication assignment operator (Matrix3f *= Matrix3f)
        Matrix3f& Matrix3f::operator*=(const Matrix3f& other) {
            Matrix3f result;
            result.xx = xx * other.xx + xy * other.yx + xz * other.zx;
            result.xy = xx * other.xy + xy * other.yy + xz * other.zy;
            result.xz = xx * other.xz + xy * other.yz + xz * other.zz;

            result.yx = yx * other.xx + yy * other.yx + yz * other.zx;
            result.yy = yx * other.xy + yy * other.yy + yz * other.zy;
            result.yz = yx * other.xz + yy * other.yz + yz * other.zz;

            result.zx = zx * other.xx + zy * other.yx + zz * other.zx;
            result.zy = zx * other.xy + zy * other.yy + zz * other.zy;
            result.zz = zx * other.xz + zy * other.yz + zz * other.zz;

            *this = result;
            return *this;
        }

        // Multiplication operator (Matrix3f * Matrix3f)
        Matrix3f Matrix3f::operator*(const Matrix3f& other) {
            Matrix3f result(*this);
            result *= other;
            return result;
        }

        // Friend multiplication operator (Matrix3f * Matrix3f)
        Matrix3f operator*(const Matrix3f& mat1, const Matrix3f& mat2) {
            Matrix3f result(mat1);
            result *= mat2;
            return result;
        }

        // Division assignment operator (Matrix3f /= Matrix3f)
        Matrix3f& Matrix3f::operator/=(const Matrix3f& other) {
            Matrix3f result;
            // Check for division by zero
            for (int i = 0; i < 9; ++i) {
                if (other.ptr[i] == 0.0f) {
                    // Handle division by zero as needed, e.g., set result to identity matrix
                    return *this;
                }
            }

            result.xx = xx / other.xx;
            result.xy = xy / other.xy;
            result.xz = xz / other.xz;

            result.yx = yx / other.yx;
            result.yy = yy / other.yy;
            result.yz = yz / other.yz;

            result.zx = zx / other.zx;
            result.zy = zy / other.zy;
            result.zz = zz / other.zz;

            *this = result;
            return *this;
        }

        // Division operator (Matrix3f / Matrix3f)
        Matrix3f Matrix3f::operator/(const Matrix3f& other) {
            Matrix3f result(*this);
            result /= other;
            return result;
        }

        // Friend division operator (Matrix3f / Matrix3f)
        Matrix3f operator/(const Matrix3f& mat1, const Matrix3f& mat2) {
            Matrix3f result(mat1);
            result /= mat2;
            return result;
        }

        // Assignment operator
        Matrix3f& Matrix3f::operator=(const Matrix3f& other) {
            if (this != &other) {
                for (int i = 0; i < 3; i++) {
                    ptr_lines[i] = other.ptr_lines[i];
                }
            }
            return *this;
        }

        // Equality operator
        bool Matrix3f::operator==(const Matrix3f& other) const {
            for (int i = 0; i < 9; ++i) {
                if (maths::Abs(other.ptr[i] - ptr[i]) > maths::MatrixEpsilon) {
                    return false;
                }
            }
            return true;
        }

        // Inequality operator
        bool Matrix3f::operator!=(const Matrix3f& other) const {
            return !(*this == other);
        }

        // Transpose method
        Matrix3f Matrix3f::Transpose() {
            Matrix3f result;
            result.xx = xx;
            result.xy = yx;
            result.xz = zx;

            result.yx = xy;
            result.yy = yy;
            result.yz = zy;

            result.zx = xz;
            result.zy = yz;
            result.zz = zz;

            return result;
        }

        // Determinant method
        float32 Matrix3f::Determinant() {
            return xx * (yy * zz - zy * yz) - xy * (yx * zz - zx * yz) + xz * (yx * zy - zx * yy);
        }

        // Inverse method
        Matrix3f Matrix3f::Inverse() {
            Matrix3f result;

            float32 det = Determinant();
            if (det != 0.0f) {
                float32 invDet = 1.0f / det;

                result.xx = (yy * zz - zy * yz) * invDet;
                result.xy = -(yx * zz - zx * yz) * invDet;
                result.xz = (yx * zy - zx * yy) * invDet;

                result.yx = -(xy * zz - xz * zy) * invDet;
                result.yy = (xx * zz - xz * zx) * invDet;
                result.yz = -(xx * zy - xy * zx) * invDet;

                result.zx = (xy * yz - xz * yy) * invDet;
                result.zy = -(xx * yz - xz * yx) * invDet;
                result.zz = (xx * yy - xy * yx) * invDet;
            }

            return result;
        }

        // Trace method
        float32 Matrix3f::Trace() {
            return xx + yy + zz;
        }

        // Len method
        float32 Matrix3f::Len() {
            return maths::Sqrt(LenSq());
        }

        // LenSq method
        float32 Matrix3f::LenSq() {
            return xx * xx + xy * xy + xz * xz +
                yx * yx + yy * yy + yz * yz +
                zx * zx + zy * zy + zz * zz;
        }

        // Power method
        Matrix3f Matrix3f::Power(int32 n) {
            if (n < 0) {
                return Inverse().Power(-n);
            }

            Matrix3f result(*this);

            if (n == 0) {
                return result;
            }
            else {
                for (int32 i = 1; i < n; ++i) {
                    result *= *this;
                }
            }

            return result;
        }

        // Diagonal method
        Vector3f Matrix3f::Diagonal() {
            return Vector3f(xx, yy, zz);
        }

        // ToString method
        std::string Matrix3f::ToString() const {
            std::ostringstream oss;
            oss << "[[" << xx << ", " << xy << ", " << xz << "], ";
            oss << "[" << yx << ", " << yy << ", " << yz << "], ";
            oss << "[" << zx << ", " << zy << ", " << zz << "]]";
            return oss.str();
        }


        // Default constructor
        Matrix3d::Matrix3d() {
            std::fill(ptr, ptr + 9, 0.0f);
        }

        // Constructor with a single value
        Matrix3d::Matrix3d(float64 value) {
            std::fill(ptr, ptr + 9, value);
        }

        // Constructor with individual elements
        Matrix3d::Matrix3d(float64 xx, float64 xy, float64 xz, float64 yx, float64 yy, float64 yz, float64 zx, float64 zy, float64 zz) {
            this->xx = xx; this->xy = xy; this->xz = xz;
            this->yx = yx; this->yy = yy; this->yz = yz;
            this->zx = zx; this->zy = zy; this->zz = zz;
        }

        // Constructor with array pointer
        Matrix3d::Matrix3d(const float64* ptr) {
            std::copy(ptr, ptr + 9, this->ptr);
        }

        // Constructor with a diagonal vector
        Matrix3d::Matrix3d(const Vector3d& diagonal) {
            xx = diagonal.x; xy = 0.0f; xz = 0.0f;
            yx = 0.0f; yy = diagonal.y; yz = 0.0f;
            zx = 0.0f; zy = 0.0f; zz = diagonal.z;
        }

        // Constructor with three row vectors
        Matrix3d::Matrix3d(const Vector3d& line1, const Vector3d& line2, const Vector3d& line3) {
            ptr_lines[0] = line1;
            ptr_lines[1] = line2;
            ptr_lines[2] = line3;
        }

        // Copy constructor
        Matrix3d::Matrix3d(const Matrix3d& mat) {
            std::copy(mat.ptr, mat.ptr + 9, ptr);
        }

        // Addition assignment operator (Matrix3d += Matrix3d)
        Matrix3d& Matrix3d::operator+=(const Matrix3d& other) {
            for (int i = 0; i < 9; ++i) {
                ptr[i] += other.ptr[i];
            }
            return *this;
        }

        // Addition operator (Matrix3d + Matrix3d)
        Matrix3d Matrix3d::operator+(const Matrix3d& other) {
            Matrix3d result(*this);
            result += other;
            return result;
        }

        // Friend addition operator (Matrix3d + Matrix3d)
        Matrix3d operator+(const Matrix3d& mat1, const Matrix3d& mat2) {
            Matrix3d result(mat1);
            result += mat2;
            return result;
        }

        // Subtraction assignment operator (Matrix3d -= Matrix3d)
        Matrix3d& Matrix3d::operator-=(const Matrix3d& other) {
            for (int i = 0; i < 9; ++i) {
                ptr[i] -= other.ptr[i];
            }
            return *this;
        }

        // Subtraction operator (Matrix3d - Matrix3d)
        Matrix3d Matrix3d::operator-(const Matrix3d& other) {
            Matrix3d result(*this);
            result -= other;
            return result;
        }

        // Friend subtraction operator (Matrix3d - Matrix3d)
        Matrix3d operator-(const Matrix3d& mat1, const Matrix3d& mat2) {
            Matrix3d result(mat1);
            result -= mat2;
            return result;
        }

        // Multiplication assignment operator (Matrix3d *= Matrix3d)
        Matrix3d& Matrix3d::operator*=(const Matrix3d& other) {
            Matrix3d result;
            result.xx = xx * other.xx + xy * other.yx + xz * other.zx;
            result.xy = xx * other.xy + xy * other.yy + xz * other.zy;
            result.xz = xx * other.xz + xy * other.yz + xz * other.zz;

            result.yx = yx * other.xx + yy * other.yx + yz * other.zx;
            result.yy = yx * other.xy + yy * other.yy + yz * other.zy;
            result.yz = yx * other.xz + yy * other.yz + yz * other.zz;

            result.zx = zx * other.xx + zy * other.yx + zz * other.zx;
            result.zy = zx * other.xy + zy * other.yy + zz * other.zy;
            result.zz = zx * other.xz + zy * other.yz + zz * other.zz;

            *this = result;
            return *this;
        }

        // Multiplication operator (Matrix3d * Matrix3d)
        Matrix3d Matrix3d::operator*(const Matrix3d& other) {
            Matrix3d result(*this);
            result *= other;
            return result;
        }

        // Friend multiplication operator (Matrix3d * Matrix3d)
        Matrix3d operator*(const Matrix3d& mat1, const Matrix3d& mat2) {
            Matrix3d result(mat1);
            result *= mat2;
            return result;
        }

        // Division assignment operator (Matrix3d /= Matrix3d)
        Matrix3d& Matrix3d::operator/=(const Matrix3d& other) {
            Matrix3d result;
            // Check for division by zero
            for (int i = 0; i < 9; ++i) {
                if (other.ptr[i] == 0.0f) {
                    // Handle division by zero as needed, e.g., set result to identity matrix
                    return *this;
                }
            }

            result.xx = xx / other.xx;
            result.xy = xy / other.xy;
            result.xz = xz / other.xz;

            result.yx = yx / other.yx;
            result.yy = yy / other.yy;
            result.yz = yz / other.yz;

            result.zx = zx / other.zx;
            result.zy = zy / other.zy;
            result.zz = zz / other.zz;

            *this = result;
            return *this;
        }

        // Division operator (Matrix3d / Matrix3d)
        Matrix3d Matrix3d::operator/(const Matrix3d& other) {
            Matrix3d result(*this);
            result /= other;
            return result;
        }

        // Friend division operator (Matrix3d / Matrix3d)
        Matrix3d operator/(const Matrix3d& mat1, const Matrix3d& mat2) {
            Matrix3d result(mat1);
            result /= mat2;
            return result;
        }

        // Assignment operator
        Matrix3d& Matrix3d::operator=(const Matrix3d& other) {
            if (this != &other) {
                for (int i = 0; i < 3; i++) {
                    ptr_lines[i] = other.ptr_lines[i];
                }
            }
            return *this;
        }

        // Equality operator
        bool Matrix3d::operator==(const Matrix3d& other) const {
            for (int i = 0; i < 9; ++i) {
                if (maths::Abs(other.ptr[i] - ptr[i]) > maths::MatrixEpsilon) {
                    return false;
                }
            }
            return true;
        }

        // Inequality operator
        bool Matrix3d::operator!=(const Matrix3d& other) const {
            return !(*this == other);
        }

        // Transpose method
        Matrix3d Matrix3d::Transpose() {
            Matrix3d result;
            result.xx = xx;
            result.xy = yx;
            result.xz = zx;

            result.yx = xy;
            result.yy = yy;
            result.yz = zy;

            result.zx = xz;
            result.zy = yz;
            result.zz = zz;

            return result;
        }

        // Determinant method
        float64 Matrix3d::Determinant() {
            return xx * (yy * zz - zy * yz) - xy * (yx * zz - zx * yz) + xz * (yx * zy - zx * yy);
        }

        // Inverse method
        Matrix3d Matrix3d::Inverse() {
            Matrix3d result;

            float64 det = Determinant();
            if (det != 0.0f) {
                float64 invDet = 1.0f / det;

                result.xx = (yy * zz - zy * yz) * invDet;
                result.xy = -(yx * zz - zx * yz) * invDet;
                result.xz = (yx * zy - zx * yy) * invDet;

                result.yx = -(xy * zz - xz * zy) * invDet;
                result.yy = (xx * zz - xz * zx) * invDet;
                result.yz = -(xx * zy - xy * zx) * invDet;

                result.zx = (xy * yz - xz * yy) * invDet;
                result.zy = -(xx * yz - xz * yx) * invDet;
                result.zz = (xx * yy - xy * yx) * invDet;
            }

            return result;
        }

        // Trace method
        float64 Matrix3d::Trace() {
            return xx + yy + zz;
        }

        // Len method
        float64 Matrix3d::Len() {
            return maths::Sqrt(LenSq());
        }

        // LenSq method
        float64 Matrix3d::LenSq() {
            return xx * xx + xy * xy + xz * xz +
                yx * yx + yy * yy + yz * yz +
                zx * zx + zy * zy + zz * zz;
        }

        // Power method
        Matrix3d Matrix3d::Power(int32 n) {
            if (n < 0) {
                return Inverse().Power(-n);
            }

            Matrix3d result(*this);

            if (n == 0) {
                return result;
            }
            else {
                for (int32 i = 1; i < n; ++i) {
                    result *= *this;
                }
            }

            return result;
        }

        // Diagonal method
        Vector3d Matrix3d::Diagonal() {
            return Vector3d(xx, yy, zz);
        }

        // ToString method
        std::string Matrix3d::ToString() const {
            std::ostringstream oss;
            oss << "[[" << xx << ", " << xy << ", " << xz << "], ";
            oss << "[" << yx << ", " << yy << ", " << yz << "], ";
            oss << "[" << zx << ", " << zy << ", " << zz << "]]";
            return oss.str();
        }
    }
}    // namespace nkentseu