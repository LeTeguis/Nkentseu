//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:23:37 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Matrix2.h"

namespace nkentseu {

    Matrix2f::Matrix2f() : xx(1.0f), xy(0.0f), yx(0.0f), yy(1.0f) {}
    Matrix2f::Matrix2f(float32 value) : xx(value), xy(value), yx(value), yy(value) {}
    Matrix2f::Matrix2f(float32 xx, float32 xy, float32 yx, float32 yy) : xx(xx), xy(xy), yx(yx), yy(yy) {}
    Matrix2f::Matrix2f(const float32* ptr) : vec(ptr) {}
    Matrix2f::Matrix2f(const Vector4f& vec) : vec(vec) {}
    Matrix2f::Matrix2f(const Vector2f& ligne1, const Vector2f& ligne2) : xx(ligne1.x), xy(ligne1.y), yx(ligne2.x), yy(ligne2.y) {}
    Matrix2f::Matrix2f(const Matrix2f& mat) : vec(mat.vec) {}

    Matrix2f& Matrix2f::operator+=(const Matrix2f& other) {
        xx += other.xx;
        xy += other.xy;
        yx += other.yx;
        yy += other.yy;
        return *this;
    }

    Matrix2f Matrix2f::operator+(const Matrix2f& other) {
        Matrix2f result(*this);
        result += other;
        return result;
    }

    Matrix2f& Matrix2f::operator-=(const Matrix2f& other) {
        xx -= other.xx;
        xy -= other.xy;
        yx -= other.yx;
        yy -= other.yy;
        return *this;
    }

    Matrix2f Matrix2f::operator-(const Matrix2f& other) {
        Matrix2f result(*this);
        result -= other;
        return result;
    }

    // Matrix2f method implementations

    Matrix2f& Matrix2f::operator*=(const Matrix2f& other) {
        Matrix2f result;
        result.xx = xx * other.xx + xy * other.yx;
        result.xy = xx * other.xy + xy * other.yy;
        result.yx = yx * other.xx + yy * other.yx;
        result.yy = yx * other.xy + yy * other.yy;
        *this = result;
        return *this;
    }

    Matrix2f Matrix2f::operator*(const Matrix2f& other) {
        Matrix2f result(*this);
        result *= other;
        return result;
    }

    // Matrix2f method implementations

    Matrix2f& Matrix2f::operator/=(const Matrix2f& other) {
        Matrix2f m(other);
        // Check if other is invertible
        float32 detOther = m.Determinant();
        if (detOther == 0.0f) {
            return *this;
        }

        *this *= m.Inverse();
        return *this;
    }

    Matrix2f Matrix2f::operator/(const Matrix2f& other) {
        Matrix2f result(*this);
        result /= other;
        return result;
    }

    Matrix2f Matrix2f::Transpose() {
        return Matrix2f(xx, yx, xy, yy);
    }

    float32 Matrix2f::Determinant() {
        return xx * yy - xy * yx;
    }

    Matrix2f Matrix2f::Inverse() {
        float32 det = Determinant();
        if (det == 0.0f) {
            return Matrix2f();
        }

        return Matrix2f(yy / det, -xy / det, -yx / det, xx / det);
    }

    float32 Matrix2f::Trace() {
        return xx + yy;
    }

    float32 Matrix2f::Len() {
        return maths::Sqrt(xx * xx + xy * xy + yx * yx + yy * yy);
    }

    float32 Matrix2f::LenSq() {
        return xx * xx + xy * xy + yx * yx + yy * yy;
    }

    Matrix2f Matrix2f::Power(int32 n) {
        Matrix2f result = Matrix2f();
        if (n == 0.0f) {
            return result;
        }

        if (n == 1.0f) {
            return *this;
        }

        if (n > 0.0f) {
            result = *this;
            for (int i = 1; i < n; i++) {
                result *= (*this);
            }

            return result;
        }

        if (n < 0.0f) {
            return Inverse().Power(-n);
        }

        return result;
    }

    Vector2f Matrix2f::Diagonal() {
        return Vector2f(ptr[0], ptr[3]);
    }

    Matrix2f Matrix2f::Orthogonalize() {
        // Verifie si la matrice est inversible.
        if (Determinant() == 0.0f) {
            return Matrix2f();
        }

        // Calcule les matrices de Householder.
        Vector2f u = Vector2f(xx, xy).Normalized();
        Vector2f v = Vector2f(yy, -yx).Normalized();

        // Construit la matrice orthogonalis�e.
        return Matrix2f(
            u.x * u.x - u.y * u.y, u.x * v.x - u.y * v.y,
            u.y * u.x + u.x * u.y, v.x * v.x - v.y * v.y
        );
    }

    Vector2f Matrix2f::GetEigenvalues() {
        // Calcul des valeurs propres de la matrice.
        float32 a = xx + yy;
        float32 b = xx - yy;
        float32 c = 2.0f * xy;

        float32 discriminant = b * b - 4.0f * a * c;

        if (discriminant > 0.0f) {
            // Les valeurs propres sont r�elles et distinctes.
            float32 lambda1 = (-b + maths::Sqrt(discriminant)) / (2.0f * a);
            float32 lambda2 = (-b - maths::Sqrt(discriminant)) / (2.0f * a);

            return Vector2f(lambda1, lambda2);
        }
        else if (discriminant == 0.0f) {
            // Les valeurs propres sont r�elles et �gales.
            float32 lambda = -b / (2.0f * a);

            return Vector2f(lambda, lambda);
        }
        else {
            // Les valeurs propres sont complexes.
            float32 lambda1 = -b / (2.0f * a);
            float32 lambda2 = maths::Sqrt(-discriminant) / (2.0f * a);

            return Vector2f(lambda1, lambda2);
        }
    }

    Vector2f Matrix2f::GetEigenvectors() {
        // Calcul des vecteurs propres de la matrice.
        Vector2f v1, v2;

        if (xx != yy) {
            // Les valeurs propres sont distinctes.
            float32 lambda1 = GetEigenvalues()[0];
            float32 lambda2 = GetEigenvalues()[1];

            v1 = Vector2f(1.0f, -xy / (lambda1 - lambda2));
            v2 = Vector2f(0.0f, 1.0f);

            // Normalisation des vecteurs propres.
            v1 = v1 / v1.Len();
            v2 = v2 - (v1 * v2) * v1;
            v2 = v2 / v2.Len();
        }
        else {
            // Les valeurs propres sont �gales.
            float32 lambda = GetEigenvalues()[0];

            v1 = Vector2f(1.0f, 0.0f);
            v2 = Vector2f(-xy, 1.0f);

            // Normalisation des vecteurs propres.
            v1 = v1 / v1.Len();
            v2 = v2 / v2.Len();
        }

        return v1 - v2;
    }

    Matrix2d::Matrix2d() : xx(1.0f), xy(0.0f), yx(0.0f), yy(1.0f) {}
    Matrix2d::Matrix2d(float64 value) : xx(value), xy(value), yx(value), yy(value) {}
    Matrix2d::Matrix2d(float64 xx, float64 xy, float64 yx, float64 yy) : xx(xx), xy(xy), yx(yx), yy(yy) {}
    Matrix2d::Matrix2d(const float64* ptr) : vec(ptr) {}
    Matrix2d::Matrix2d(const Vector4d& vec) : vec(vec) {}
    Matrix2d::Matrix2d(const Vector2d& ligne1, const Vector2d& ligne2): xx(ligne1.x), xy(ligne1.y), yx(ligne2.x), yy(ligne2.y) {}
    Matrix2d::Matrix2d(const Matrix2d& mat) : vec(mat.vec) {}

    Matrix2d& Matrix2d::operator+=(const Matrix2d& other) {
        xx += other.xx;
        xy += other.xy;
        yx += other.yx;
        yy += other.yy;
        return *this;
    }

    Matrix2d Matrix2d::operator+(const Matrix2d& other) {
        Matrix2d result(*this);
        result += other;
        return result;
    }

    Matrix2d operator+(const Matrix2d& mat1, const Matrix2d& mat2) {
        Matrix2d result(mat1);
        result += mat2;
        return result;
    }

    Matrix2d& Matrix2d::operator-=(const Matrix2d& other) {
        xx -= other.xx;
        xy -= other.xy;
        yx -= other.yx;
        yy -= other.yy;
        return *this;
    }

    Matrix2d Matrix2d::operator-(const Matrix2d& other) {
        Matrix2d result(*this);
        result -= other;
        return result;
    }

    Matrix2d operator-(const Matrix2d& mat1, const Matrix2d& mat2) {
        Matrix2d result(mat1);
        result -= mat2;
        return result;
    }

    // Matrix2d method implementations

    Matrix2d& Matrix2d::operator*=(const Matrix2d& other) {
        Matrix2d result;
        result.xx = xx * other.xx + xy * other.yx;
        result.xy = xx * other.xy + xy * other.yy;
        result.yx = yx * other.xx + yy * other.yx;
        result.yy = yx * other.xy + yy * other.yy;
        *this = result;
        return *this;
    }

    Matrix2d Matrix2d::operator*(const Matrix2d& other) {
        Matrix2d result(*this);
        result *= other;
        return result;
    }

    Matrix2d operator*(const Matrix2d& mat1, const Matrix2d& mat2) {
        Matrix2d result(mat1);
        result *= mat2;
        return result;
    }

    

    // Matrix2d method implementations

    Matrix2d& Matrix2d::operator/=(const Matrix2d& other) {
        Matrice2d m(other);
        // Check if other is invertible
        float64 detOther = m.Determinant();
        if (detOther == 0.0f) {
            return *this;
        }

        *this *= m.Inverse();
        return *this;
    }

    Matrix2d Matrix2d::operator/(const Matrix2d& other) {
        Matrix2d result(*this);
        result /= other;
        return result;
    }

    Matrix2d operator/(const Matrix2d& mat1, const Matrix2d& mat2) {
        Matrix2d result(mat1);
        result /= mat2;
        return result;
    }

    Matrix2d Matrix2d::Transpose() {
        return Matrix2d(xx, yx, xy, yy);
    }

    float64 Matrix2d::Determinant() {
        return xx * yy - xy * yx;
    }

    Matrix2d Matrix2d::Inverse() {
        float64 det = Determinant();
        if (det == 0.0f) {
            return Matrix2d();
        }

        return Matrix2d(yy / det, -xy / det, -yx / det, xx / det);
    }

    float64 Matrix2d::Trace() {
        return xx + yy;
    }

    float64 Matrix2d::Len() {
        return maths::Sqrt(xx * xx + xy * xy + yx * yx + yy * yy);
    }

    float64 Matrix2d::LenSq() {
        return xx * xx + xy * xy + yx * yx + yy * yy;
    }

    Matrix2d Matrix2d::Power(int32 n) {
        Matrix2d result = Matrix2d();
        if (n == 0.0f) {
            return result;
        }

        if (n == 1.0f) {
            return *this;
        }

        if (n > 0.0f) {
            result = *this;
            for (int i = 1; i < n; i++) {
                result *= (*this);
            }

            return result;
        }

        if (n < 0.0f) {
            return Inverse().Power(-n);
        }

        return result;
    }

    Vector2d Matrix2d::Diagonal() {
        return Vector2d(ptr[0], ptr[3]);
    }

    Matrix2d Matrix2d::Orthogonalize() {
        // Verifie si la matrice est inversible.
        if (Determinant() == 0.0f) {
            return Matrix2d();
        }

        // Calcule les matrices de Householder.
        Vector2d u = Vector2d(xx, xy).Normalized();
        Vector2d v = Vector2d(yy, -yx).Normalized();

        // Construit la matrice orthogonalis�e.
        return Matrix2d(
            u.x * u.x - u.y * u.y, u.x * v.x - u.y * v.y,
            u.y * u.x + u.x * u.y, v.x * v.x - v.y * v.y
        );
    }

    Vector2d Matrix2d::GetEigenvalues() {
        // Calcul des valeurs propres de la matrice.
        float64 a = xx + yy;
        float64 b = xx - yy;
        float64 c = 2.0f * xy;

        float64 discriminant = b * b - 4.0f * a * c;

        if (discriminant > 0.0f) {
            // Les valeurs propres sont r�elles et distinctes.
            float64 lambda1 = (-b + maths::Sqrt(discriminant)) / (2.0f * a);
            float64 lambda2 = (-b - maths::Sqrt(discriminant)) / (2.0f * a);

            return Vector2d(lambda1, lambda2);
        }
        else if (discriminant == 0.0f) {
            // Les valeurs propres sont r�elles et �gales.
            float64 lambda = -b / (2.0f * a);

            return Vector2d(lambda, lambda);
        }
        else {
            // Les valeurs propres sont complexes.
            float64 lambda1 = -b / (2.0f * a);
            float64 lambda2 = maths::Sqrt(-discriminant) / (2.0f * a);

            return Vector2d(lambda1, lambda2);
        }
    }

    Vector2d Matrix2d::GetEigenvectors() {
        // Calcul des vecteurs propres de la matrice.
        Vector2d v1, v2;

        if (xx != yy) {
            // Les valeurs propres sont distinctes.
            float64 lambda1 = GetEigenvalues()[0];
            float64 lambda2 = GetEigenvalues()[1];

            v1 = Vector2d(1.0f, -xy / (lambda1 - lambda2));
            v2 = Vector2d(0.0f, 1.0f);

            // Normalisation des vecteurs propres.
            v1 = v1 / v1.Len();
            v2 = v2 - (v1 * v2) * v1;
            v2 = v2 / v2.Len();
        }
        else {
            // Les valeurs propres sont �gales.
            float64 lambda = GetEigenvalues()[0];

            v1 = Vector2d(1.0f, 0.0f);
            v2 = Vector2d(-xy, 1.0f);

            // Normalisation des vecteurs propres.
            v1 = v1 / v1.Len();
            v2 = v2 / v2.Len();
        }

        return v1 - v2;
    }
}    // namespace nkentseu