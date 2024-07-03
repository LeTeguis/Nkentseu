//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:26:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Matrix4.h"

#include <cstring>

namespace nkentseu {

    namespace maths
    {
        //*********************************
        // Constructeur par défaut
        matrix4f::matrix4f() {
            // Initialiser la matrice à l'identité
            for (int i = 0; i < 16; ++i) {
                data[i] = 0.0f;
            }
        }

        matrix4f::matrix4f(const Vector3f& V0, const Vector3f& V1, const Vector3f& V2)
        {
            m00 = V0.x;
            m01 = V0.y;
            m02 = V0.z;
            m03 = 0.0f;

            m10 = V1.x;
            m11 = V1.y;
            m12 = V1.z;
            m13 = 0.0f;

            m20 = V2.x;
            m21 = V2.y;
            m22 = V2.z;
            m23 = 0.0f;

            m30 = 0.0f;
            m31 = 0.0f;
            m32 = 0.0f;
            m33 = 1.0f;
        }

        // Constructeur par copie
        matrix4f::matrix4f(const matrix4f& other) {
            std::memcpy(data, other.data, 16 * sizeof(float32));
        }

        // Constructeur avec une valeur initiale
        matrix4f::matrix4f(float32 value) {
            for (int i = 0; i < 16; ++i) {
                data[i] = value;
            }
        }

        // Constructeur à partir d'un tableau de données
        matrix4f::matrix4f(const float32* data) {
            std::memcpy(this->data, data, 16 * sizeof(float32));
        }

        matrix4f::matrix4f(float32 m00, float32 m01, float32 m02, float32 m03,
            float32 m10, float32 m11, float32 m12, float32 m13,
            float32 m20, float32 m21, float32 m22, float32 m23,
            float32 m30, float32 m31, float32 m32, float32 m33) {
            this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
            this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
            this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
            this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
        }

        // Accesseurs aux éléments de la matrice
        float32 matrix4f::Get(int row, int col) const {
            return data[row * 4 + col];
        }

        void matrix4f::Set(int row, int col, float32 value) {
            data[row * 4 + col] = value;
        }

        // Accesseurs pour une ligne entière de la matrice
        float32* matrix4f::operator[](int row) {
            return &data[row * 4];
        }

        const float32* matrix4f::operator[](int row) const {
            return &data[row * 4];
        }

        // Accesseur pour toute la matrice
        float32* matrix4f::Ptr() {
            return data;
        }

        const float32* matrix4f::Ptr() const {
            return data;
        }

        // Opérateurs d'affectation
        matrix4f& matrix4f::operator=(const matrix4f& other) {
            std::memcpy(data, other.data, 16 * sizeof(float32));
            return *this;
        }

        matrix4f& matrix4f::operator=(const float32* otherData) {
            std::memcpy(data, otherData, 16 * sizeof(float32));
            return *this;
        }

        // Opérateurs d'égalité et d'inégalité
        bool matrix4f::operator==(const matrix4f& other) const {
            for (int i = 0; i < 16; ++i) {
                if (data[i] != other.data[i]) {
                    return false;
                }
            }
            return true;
        }

        bool matrix4f::operator!=(const matrix4f& other) const {
            return !(*this == other);
        }

        matrix4f& matrix4f::operator*=(const matrix4f& other) {
            *this = *this * other;
            return *this;
        }

        matrix4f& matrix4f::operator*=(float32 scalar) {
            for (int i = 0; i < 16; ++i) {
                data[i] *= scalar;
            }
            return *this;
        }

        /*Vector4f matrix4f::operator*(const Vector4f& vector) const {
            Vector4f result;
            for (int i = 0; i < 4; ++i) {
                result.ptr[i] = 0.0f;
                for (int j = 0; j < 4; ++j) {
                    result.ptr[i] += data[i * 4 + j] * vector.ptr[j];
                }
            }
            return result;
        }*/

        // Transposition de la matrice
        matrix4f matrix4f::Transpose() const {
            matrix4f result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.mat[j][i] = mat[i][j];
                }
            }
            return result;
        }

        // Calcul du déterminant
        float32 matrix4f::Determinant() const {
            float32 det = 0.0f;
            for (int i = 0; i < 4; ++i) {
                float32 cofactor = data[i] * Cofactor3x3(0, i);
                det += (i % 2 == 0 ? 1 : -1) * cofactor;
            }
            return det;
        }

        float32 matrix4f::Cofactor3x3(int row, int col) const {
            float32 submatrix[9];
            int index = 0;
            for (int i = 0; i < 4; ++i) {
                if (i == row) continue;
                for (int j = 0; j < 4; ++j) {
                    if (j == col) continue;
                    submatrix[index++] = data[i * 4 + j];
                }
            }
            return Determinant3x3(submatrix);
        }

        float32 matrix4f::Determinant3x3(const float32 submatrix[9]) const {
            return submatrix[0] * (submatrix[4] * submatrix[8] - submatrix[5] * submatrix[7])
                - submatrix[1] * (submatrix[3] * submatrix[8] - submatrix[5] * submatrix[6])
                + submatrix[2] * (submatrix[3] * submatrix[7] - submatrix[4] * submatrix[6]);
        }

        Vector3f matrix4f::TransformPoint(const Vector3f& point) const
        {
            Vector4f transformed_point(point.x, point.y, point.z, 1.0f);
            Vector4f result = (*this) * transformed_point;

            if (result.w == 0.0f || result.w == 1.0f) return result.xyz();
            return result.xyz() / result.w;

        }

        Vector3f matrix4f::TransformVector(const Vector3f& vector) const
        {
            Vector4f transformed_vector(vector.x, vector.y, vector.z, 0.0f);
            Vector4f result = (*this) * transformed_vector;

            if (result.w == 0.0f || result.w == 1.0f) return result.xyz();
            return result.xyz() / result.w;
        }

        Vector3f matrix4f::TransformNormal(const Vector3f& normal) const
        {
            Vector4f transformed_normal(normal.x, normal.y, normal.z, 0.0f);
            transformed_normal = transformed_normal * (*this);

            if (transformed_normal.w == 0.0f) return transformed_normal.w = 1.0f;
            if (transformed_normal.w < 0.0f) return transformed_normal.w = -1.0f;

            Vector3f result = transformed_normal.xyz() / transformed_normal.w;

            return result;
        }

        // Calcul de l'inverse de la matrice
        matrix4f matrix4f::Inverse() const {
            float64 result[4][4];
            float64 tmp[12]; /* temp array for pairs */
            float64 src[16]; /* array of transpose source matrix */
            float64 det; /* determinant */

            /* transpose matrix */
            for (uint32 i = 0; i < 4; i++)
            {
                src[i + 0] = (*this)[i][0];
                src[i + 4] = (*this)[i][1];
                src[i + 8] = (*this)[i][2];
                src[i + 12] = (*this)[i][3];
            }
            /* calculate pairs for first 8 elements (cofactors) */
            tmp[0] = src[10] * src[15];
            tmp[1] = src[11] * src[14];
            tmp[2] = src[9] * src[15];
            tmp[3] = src[11] * src[13];
            tmp[4] = src[9] * src[14];
            tmp[5] = src[10] * src[13];
            tmp[6] = src[8] * src[15];
            tmp[7] = src[11] * src[12];
            tmp[8] = src[8] * src[14];
            tmp[9] = src[10] * src[12];
            tmp[10] = src[8] * src[13];
            tmp[11] = src[9] * src[12];

            /* calculate first 8 elements (cofactors) */
            result[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
            result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
            result[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
            result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
            result[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
            result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
            result[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
            result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
            result[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
            result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
            result[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
            result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
            result[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
            result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
            result[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
            result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

            /* calculate pairs for second 8 elements (cofactors) */
            tmp[0] = src[2] * src[7];
            tmp[1] = src[3] * src[6];
            tmp[2] = src[1] * src[7];
            tmp[3] = src[3] * src[5];
            tmp[4] = src[1] * src[6];
            tmp[5] = src[2] * src[5];

            tmp[6] = src[0] * src[7];
            tmp[7] = src[3] * src[4];
            tmp[8] = src[0] * src[6];
            tmp[9] = src[2] * src[4];
            tmp[10] = src[0] * src[5];
            tmp[11] = src[1] * src[4];

            /* calculate second 8 elements (cofactors) */
            result[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
            result[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
            result[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
            result[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
            result[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
            result[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
            result[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
            result[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
            result[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
            result[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
            result[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
            result[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
            result[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
            result[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
            result[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
            result[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

            /* calculate determinant */
            det = src[0] * result[0][0] + src[1] * result[0][1] + src[2] * result[0][2] + src[3] * result[0][3];

            /* calculate matrix inverse */
            det = 1.0f / det;

            matrix4f finalResult;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 j = 0; j < 4; j++)
                {
                    finalResult[i][j] = float32(result[i][j] * det);
                }
            }
            return finalResult;
        }

        Vector3f matrix4f::Forward() const {
            return Vector3f(m02, m12, m22);
        }

        Vector3f matrix4f::Up() const {
            return Vector3f(m01, m11, m21);
        }

        Vector3f matrix4f::Right() const {
            return Vector3f(m00, m10, m20);
        }

        Vector3f matrix4f::Position() const {
            return Vector3f(m03, m13, m23);
        }

        void matrix4f::Forward(const Vector3f& forward) {
            m02 = forward.x;
            m12 = forward.y;
            m22 = forward.z;
        }

        void matrix4f::Up(const Vector3f& up) {
            m01 = up.x;
            m11 = up.y;
            m21 = up.z;
        }

        void matrix4f::Right(const Vector3f& right) {
            m00 = right.x;
            m10 = right.y;
            m20 = right.z;
        }

        void matrix4f::Position(const Vector3f& position) {
            m03 = position.x;
            m13 = position.y;
            m23 = position.z;
        }

        float32 matrix4f::CompareMatrices(const matrix4f& left, const matrix4f& right)
        {
            float32 Sum = 0.0f;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    Sum += maths::Abs(left[i][i2] - right[i][i2]);
                }
            }
            return Sum / 16.0f;
        }

        matrix4f matrix4f::Identity() {
            matrix4f in;
            in.m00 = 1.0f;
            in.m11 = 1.0f;
            in.m22 = 1.0f;
            in.m33 = 1.0f;
            return in;
        }

        matrix4f matrix4f::Camera(const Vector3f& eye, const Vector3f& look, const Vector3f& up, const Vector3f& right)
        {
            Vector3f look_ = Vector3f(look).Normalized();
            Vector3f up_ = Vector3f(up).Normalized();
            Vector3f right_ = Vector3f(right).Normalized();

            matrix4f result;
            result.Right(right_);
            result.m30 = -right_.Dot(eye);

            result.Up(up_);
            result.m31 = -up_.Dot(eye);

            result.Forward(look_);
            result.m32 = -look_.Dot(eye);

            result.Position(Vector3f());
            result.m33 = 1.0f;

            return result;
        }

        matrix4f matrix4f::LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
        {
            Vector3f xAxis, yAxis, zAxis;
            zAxis = (eye - center).Normalized();
            xAxis = Vector3f(up).Cross(zAxis).Normalized();
            yAxis = Vector3f(zAxis).Cross(xAxis).Normalized();

            matrix4f result;
            result.Right(xAxis);
            result.m30 = -xAxis.Dot(eye);

            result.Up(yAxis);
            result.m31 = -yAxis.Dot(eye);

            result.Forward(zAxis);
            result.m32 = -zAxis.Dot(eye);

            result.Position(Vector3f());
            result.m33 = 1.0f;

            /*matrix4f result;
            result[0][0] = xAxis.x;
            result[1][0] = xAxis.y;
            result[2][0] = xAxis.z;
            result[3][0] = -xAxis.Dot(eye);

            result[0][1] = yAxis.x;
            result[1][1] = yAxis.y;
            result[2][1] = yAxis.z;
            result[3][1] = -yAxis.Dot(eye);

            result[0][2] = zAxis.x;
            result[1][2] = zAxis.y;
            result[2][2] = zAxis.z;
            result[3][2] = -zAxis.Dot(eye);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;*/
            return result;
        }

        matrix4f matrix4f::Orthogonal(float32 width, float32 height, float32 zNear, float32 zFar)
        {
            matrix4f result;
            result.m00 = 2.0f / width;
            result.m10 = 0.0f;
            result.m20 = 0.0f;
            result.m30 = 0.0f;

            result.m01 = 0.0f;
            result.m11 = 2.0f / height;
            result.m21 = 0.0f;
            result.m31 = 0.0f;

            result.m02 = 0.0f;
            result.m12 = 0.0f;
            result.m22 = 1.0f / (zNear - zFar);
            result.m32 = zNear / (zNear - zFar);

            result.m03 = 0.0f;
            result.m13 = 0.0f;
            result.m23 = 0.0f;
            result.m33 = 1.0f;
            return result;
        }

        matrix4f matrix4f::Orthogonal(const Vector2f& size, float32 zNear, float32 zFar)
        {
            matrix4f result;
            result[0][0] = 2.0f / size.width;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = 0.0f;

            result[0][1] = 0.0f;
            result[1][1] = 2.0f / size.height;
            result[2][1] = 0.0f;
            result[3][1] = 0.0f;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = 2.0f / (zNear - zFar);
            result[3][2] = (zNear + zFar) / (zNear - zFar);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4f matrix4f::Orthogonal(const Vector2f& bottomLeft, const Vector2f& topRight, float32 zNear, float32 zFar)
        {
            matrix4f result;
            result[0][0] = 2.0f / (topRight.x - bottomLeft.x);
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = -((topRight.x + bottomLeft.x) / (topRight.x - bottomLeft.x));

            result[0][1] = 0.0f;
            result[1][1] = 2.0f / (topRight.y - bottomLeft.y);
            result[2][1] = 0.0f;
            result[3][1] = -((topRight.y + bottomLeft.y) / (topRight.y - bottomLeft.y));

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = 2.0f / (zNear - zFar);
            result[3][2] = (zNear + zFar) / (zNear - zFar);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4f matrix4f::Perspective(float32 width, float32 height, float32 zNear, float32 zFar)
        {
            matrix4f result;
            result.m00 = 2.0f * zNear / width;
            result.m10 = 0.0f;
            result.m20 = 0.0f;
            result.m30 = 0.0f;

            result.m01 = 0.0f;
            result.m11 = 2.0f * zNear / height;
            result.m21 = 0.0f;
            result.m31 = 0.0f;

            result.m02 = 0.0f;
            result.m12 = 0.0f;
            result.m22 = zFar / (zNear - zFar);
            result.m32 = zFar * zNear / (zNear - zFar);

            result.m03 = 0.0f;
            result.m13 = 0.0f;
            result.m23 = -1.0f;
            result.m33 = 0.0f;
            return result;
        }

        matrix4f matrix4f::Perspective(const Angle& fov, float32 aspect, float32 zNear, float32 zFar)
        {
            float32 width = 1.0f / maths::Tan(fov * 0.5f), height = aspect / maths::Tan(fov * 0.5f);

            matrix4f result;
            result.m00 = width;
            result.m10 = 0.0f;
            result.m20 = 0.0f;
            result.m30 = 0.0f;

            result.m01 = 0.0f;
            result.m11 = height;
            result.m21 = 0.0f;
            result.m31 = 0.0f;

            result.m02 = 0.0f;
            result.m12 = 0.0f;
            result.m22 = zFar / (zNear - zFar);
            result.m32 = zFar * zNear / (zNear - zFar);

            result.m03 = 0.0f;
            result.m13 = 0.0f;
            result.m23 = -1.0f;
            result.m33 = 0.0f;
            return result;
        }

        matrix4f matrix4f::Perspective(const Angle& fovX, const Angle& fovY, float32 zNear, float32 zFar)
        {
            float32 width = 1.0f / maths::Tan(fovX * 0.5f), height = 1.0f / maths::Tan(fovY * 0.5f);

            matrix4f result;

            result.m00 = width;
            result.m10 = 0.0f;
            result.m20 = 0.0f;
            result.m30 = 0.0f;

            result.m01 = 0.0f;
            result.m11 = height;
            result.m21 = 0.0f;
            result.m31 = 0.0f;

            result.m02 = 0.0f;
            result.m12 = 0.0f;
            result.m22 = zFar / (zNear - zFar);
            result.m32 = zFar * zNear / (zNear - zFar);

            result.m03 = 0.0f;
            result.m13 = 0.0f;
            result.m23 = -1.0f;
            result.m33 = 0.0f;

            return result;
        }

        matrix4f matrix4f::Rotation(const Vector3f& axis, const Angle& angle)
        {
            float32 theta = angle.Rad();
            float32 c = cosf(theta);
            float32 s = sinf(theta);
            float32 t = 1.0f - c;

            Vector3f NormalizedAxis = Vector3f(axis).Normalized();
            float32 x = NormalizedAxis.x;
            float32 y = NormalizedAxis.y;
            float32 z = NormalizedAxis.z;

            matrix4f result;

            result.m00 = 1 + t * (x * x - 1);
            result.m01 = z * s + t * x * y;
            result.m02 = -y * s + t * x * z;
            result.m03 = 0.0f;

            result.m10 = -z * s + t * x * y;
            result.m11 = 1 + t * (y * y - 1);
            result.m12 = x * s + t * y * z;
            result.m13 = 0.0f;

            result.m20 = y * s + t * x * z;
            result.m21 = -x * s + t * y * z;
            result.m22 = 1 + t * (z * z - 1);
            result.m23 = 0.0f;

            result.m30 = 0.0f;
            result.m31 = 0.0f;
            result.m32 = 0.0f;
            result.m33 = 1.0f;

            return result;
        }

        matrix4f matrix4f::Rotation(const Angle& pitch, const Angle& yaw, const Angle& roll)
        {
            return RotationY(yaw) * RotationX(pitch) * RotationZ(roll);
        }

        matrix4f matrix4f::Rotation(const Vector3f& axis, const Angle& angle, const Vector3f& center)
        {
            return Translation(-center) * Rotation(axis, angle) * Translation(center);
        }

        matrix4f matrix4f::RotationX(const Angle& angle)
        {
            float32 CosT = maths::Cos(angle);
            float32 SinT = maths::Sin(angle);

            matrix4f result = Identity();

            result.m11 = CosT;
            result.m12 = SinT;

            result.m21 = -SinT;
            result.m22 = CosT;

            return result;
        }

        matrix4f matrix4f::RotationY(const Angle& angle)
        {
            float32 CosT = maths::Cos(angle);
            float32 SinT = maths::Sin(angle);

            matrix4f result = Identity();

            result.m00 = CosT;
            result.m02 = SinT;

            result.m20 = -SinT;
            result.m22 = CosT;

            return result;
        }

        matrix4f matrix4f::RotationZ(const Angle& angle)
        {
            float32 CosT = maths::Cos(angle);
            float32 SinT = maths::Sin(angle);

            matrix4f result = Identity();

            result.m00 = CosT;
            result.m01 = SinT;

            result.m10 = -SinT;
            result.m11 = CosT;

            return result;
        }

        matrix4f matrix4f::Scaling(const Vector3f& scaleFactors)
        {
            matrix4f result;

            result.m00 = scaleFactors.x;
            result.m10 = 0.0f;
            result.m20 = 0.0f;
            result.m30 = 0.0f;

            result.m01 = 0.0f;
            result.m11 = scaleFactors.y;
            result.m21 = 0.0f;
            result.m31 = 0.0f;

            result.m02 = 0.0f;
            result.m12 = 0.0f;
            result.m22 = scaleFactors.z;
            result.m32 = 0.0f;

            result.m03 = 0.0f;
            result.m13 = 0.0f;
            result.m23 = 0.0f;
            result.m33 = 1.0f;

            return result;
        }

        matrix4f matrix4f::Translation(const Vector3f& pos)
        {
            matrix4f result;

            result.m00 = 1.0f;
            result.m10 = 0.0f;
            result.m20 = 0.0f;
            result.m30 = pos.x;

            result.m01 = 0.0f;
            result.m11 = 1.0f;
            result.m21 = 0.0f;
            result.m31 = pos.y;

            result.m02 = 0.0f;
            result.m12 = 0.0f;
            result.m22 = 1.0f;
            result.m32 = pos.z;

            result.m03 = 0.0f;
            result.m13 = 0.0f;
            result.m23 = 0.0f;
            result.m33 = 1.0f;

            return result;
        }

        matrix4f matrix4f::Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar) {
            matrix4f result;

            result.m00 = (2.0f * zNear) / (right - left);
            result.m11 = (2.0f * zNear) / (top - bottom);
            result.m20 = (right + left) / (right - left);
            result.m21 = (top + bottom) / (top - bottom);
            result.m22 = -(zFar + zNear) / (zFar - zNear);
            result.m23 = -1.0f;
            result.m32 = -(2.0f * zFar * zNear) / (zFar - zNear);

            return result;
        }

        matrix4f matrix4f::Rotation(const matrix4f mat, const Vector3f& axis, const Angle& angle)
        {
            return Rotation(axis, angle) * mat;
        }

        matrix4f matrix4f::Rotation(const matrix4f mat, const Angle& pitch, const Angle& yaw, const Angle& roll)
        {
            return Rotation(pitch, yaw, roll) * mat;
        }

        matrix4f matrix4f::Rotation(const matrix4f mat, const Vector3f& axis, const Angle& angle, const Vector3f& center)
        {
            return Rotation(axis, angle, center) * mat;
        }

        matrix4f matrix4f::RotationX(const matrix4f mat, const Angle& angle)
        {
            return RotationX(angle) * mat;
        }

        matrix4f matrix4f::RotationY(const matrix4f mat, const Angle& angle)
        {
            return RotationY(angle) * mat;
        }

        matrix4f matrix4f::RotationZ(const matrix4f mat, const Angle& angle)
        {
            return RotationZ(angle) * mat;
        }

        matrix4f matrix4f::Scaling(const matrix4f mat, const Vector3f& scaleFactors)
        {
            return Scaling(scaleFactors) * mat;
        }

        matrix4f matrix4f::Translation(const matrix4f mat, const Vector3f& pos)
        {
            return Translation(pos) * mat;
        }

        matrix4f matrix4f::FromEuler(const Vector3f& angle)
        {
            return FromEuler(angle.yaw, angle.pitch, angle.roll);
        }

        matrix4f matrix4f::FromEuler(const Angle& yaw, const Angle& pitch, const Angle& roll)
        {
            float32 tmp_ch = maths::Cos(yaw);
            float32 tmp_sh = maths::Sin(yaw);
            float32 tmp_cp = maths::Cos(pitch);
            float32 tmp_sp = maths::Sin(pitch);
            float32 tmp_cb = maths::Cos(roll);
            float32 tmp_sb = maths::Sin(roll);

            matrix4f result;
            result.m00 = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
            result.m01 = tmp_sb * tmp_cp;
            result.m02 = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
            result.m03 = static_cast<float32>(0);
            result.m10 = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
            result.m11 = tmp_cb * tmp_cp;
            result.m12 = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
            result.m13 = static_cast<float32>(0);
            result.m20 = tmp_sh * tmp_cp;
            result.m21 = -tmp_sp;
            result.m22 = tmp_ch * tmp_cp;
            result.m23 = static_cast<float32>(0);
            result.m30 = static_cast<float32>(0);
            result.m31 = static_cast<float32>(0);
            result.m32 = static_cast<float32>(0);
            result.m33 = static_cast<float32>(1);
            return result;
        }

        std::string matrix4f::ToStringRow(uint32 i) const {
            std::stringstream ss;

            ss << "[";
            for (uint32 i2 = 0; i2 < 4; i2++)
            {
                ss << mat[i2][i];
                if (i2 < 3) {
                    ss << ", ";
                }
            }
            ss << "]";

            return ss.str();
        }

        std::string matrix4f::ToStringCol(uint32 i) const {
            std::stringstream ss;

            ss << "[";
            for (uint32 i2 = 0; i2 < 4; i2++)
            {
                ss << mat[i][i2];
                if (i2 < 3) {
                    ss << ", ";
                }
            }
            ss << "]";

            return ss.str();
        }

        std::string matrix4f::ToString() const {
            std::stringstream ss;
            ss << "Matrix4f[";
            for (uint32 i1 = 0; i1 < 4; i1++) {
                /*ss << "[";
                for (uint32 i2 = 0; i2 < 4; i2++) {
                    ss << mat[i1][i2];

                    if (i2 < 3) {
                        ss << ", ";
                    }
                }
                ss << "]\n";

                if (i1 < 3) {
                    ss << ", ";
                }*/
                ss << ToStringRow(i1);
                if (i1 < 3) {
                    ss << "\n, ";
                }
            }
            ss << "]";
            return ss.str();
        }

        std::ostream& operator<<(std::ostream& os, const matrix4f& e) {
            return os << e.ToString();
        }

        //*********************************
    // Constructeur par défaut
        matrix4d::matrix4d() {
            // Initialiser la matrice à l'identité
            for (int i = 0; i < 16; ++i) {
                data[i] = 0.0f;
            }
        }

        matrix4d::matrix4d(const Vector3d& V0, const Vector3d& V1, const Vector3d& V2)
        {
            mat[0][0] = V0.x;
            mat[0][1] = V0.y;
            mat[0][2] = V0.z;
            mat[0][3] = 0.0f;

            mat[1][0] = V1.x;
            mat[1][1] = V1.y;
            mat[1][2] = V1.z;
            mat[1][3] = 0.0f;

            mat[2][0] = V2.x;
            mat[2][1] = V2.y;
            mat[2][2] = V2.z;
            mat[2][3] = 0.0f;

            mat[3][0] = 0.0f;
            mat[3][1] = 0.0f;
            mat[3][2] = 0.0f;
            mat[3][3] = 1.0f;
        }

        // Constructeur par copie
        matrix4d::matrix4d(const matrix4d& other) {
            std::memcpy(data, other.data, 16 * sizeof(float64));
        }

        // Constructeur avec une valeur initiale
        matrix4d::matrix4d(float64 value) {
            for (int i = 0; i < 16; ++i) {
                data[i] = value;
            }
        }

        // Constructeur à partir d'un tableau de données
        matrix4d::matrix4d(const float64* data) {
            std::memcpy(this->data, data, 16 * sizeof(float64));
        }

        matrix4d::matrix4d(float64 m00, float64 m01, float64 m02, float64 m03,
            float64 m10, float64 m11, float64 m12, float64 m13,
            float64 m20, float64 m21, float64 m22, float64 m23,
            float64 m30, float64 m31, float64 m32, float64 m33) {
            this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
            this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
            this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
            this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
        }

        // Accesseurs aux éléments de la matrice
        float64 matrix4d::Get(int row, int col) const {
            return data[row * 4 + col];
        }

        void matrix4d::Set(int row, int col, float64 value) {
            data[row * 4 + col] = value;
        }

        // Accesseurs pour une ligne entière de la matrice
        float64* matrix4d::operator[](int row) {
            return &data[row * 4];
        }

        const float64* matrix4d::operator[](int row) const {
            return &data[row * 4];
        }

        // Accesseur pour toute la matrice
        float64* matrix4d::Ptr() {
            return data;
        }

        const float64* matrix4d::Ptr() const {
            return data;
        }

        // Opérateurs d'affectation
        matrix4d& matrix4d::operator=(const matrix4d& other) {
            std::memcpy(data, other.data, 16 * sizeof(float64));
            return *this;
        }

        matrix4d& matrix4d::operator=(const float64* otherData) {
            std::memcpy(data, otherData, 16 * sizeof(float64));
            return *this;
        }

        // Opérateurs d'égalité et d'inégalité
        bool matrix4d::operator==(const matrix4d& other) const {
            for (int i = 0; i < 16; ++i) {
                if (data[i] != other.data[i]) {
                    return false;
                }
            }
            return true;
        }

        bool matrix4d::operator!=(const matrix4d& other) const {
            return !(*this == other);
        }

        matrix4d& matrix4d::operator*=(const matrix4d& other) {
            *this = *this * other;
            return *this;
        }

        matrix4d& matrix4d::operator*=(float64 scalar) {
            for (int i = 0; i < 16; ++i) {
                data[i] *= scalar;
            }
            return *this;
        }

        matrix4d operator*(const matrix4d& left, const matrix4d& right)
        {
            matrix4d result;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    float64 Total = 0.0f;
                    for (uint32 i3 = 0; i3 < 4; i3++)
                    {
                        Total += left[i][i3] * right[i3][i2];
                    }
                    result[i][i2] = Total;
                }
            }
            return result;
        }

        matrix4d operator*(const matrix4d& left, float64& right)
        {
            matrix4d result;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    result[i][i2] = left[i][i2] * right;
                }
            }
            return result;
        }

        matrix4d operator*(float64& left, const matrix4d& right)
        {
            matrix4d result;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    result[i][i2] = right[i][i2] * left;
                }
            }
            return result;
        }

        matrix4d operator+(const matrix4d& left, const matrix4d& right)
        {
            matrix4d result;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    result[i][i2] = left[i][i2] + right[i][i2];
                }
            }
            return result;
        }

        matrix4d operator-(const matrix4d& left, const matrix4d& right)
        {
            matrix4d result;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    result[i][i2] = left[i][i2] - right[i][i2];
                }
            }
            return result;
        }

        // Transposition de la matrice
        matrix4d matrix4d::Transpose() const {
            matrix4d result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.mat[j][i] = mat[i][j];
                }
            }
            return result;
        }

        // Calcul du déterminant
        float64 matrix4d::Determinant() const {
            float64 det = 0.0f;
            for (int i = 0; i < 4; ++i) {
                float64 cofactor = data[i] * Cofactor3x3(0, i);
                det += (i % 2 == 0 ? 1 : -1) * cofactor;
            }
            return det;
        }

        float64 matrix4d::Cofactor3x3(int row, int col) const {
            float64 submatrix[9];
            int index = 0;
            for (int i = 0; i < 4; ++i) {
                if (i == row) continue;
                for (int j = 0; j < 4; ++j) {
                    if (j == col) continue;
                    submatrix[index++] = data[i * 4 + j];
                }
            }
            return Determinant3x3(submatrix);
        }

        float64 matrix4d::Determinant3x3(const float64 submatrix[9]) const {
            return submatrix[0] * (submatrix[4] * submatrix[8] - submatrix[5] * submatrix[7])
                - submatrix[1] * (submatrix[3] * submatrix[8] - submatrix[5] * submatrix[6])
                + submatrix[2] * (submatrix[3] * submatrix[7] - submatrix[4] * submatrix[6]);
        }

        Vector3d matrix4d::TransformPoint(const Vector3d& point) const
        {
            Vector4d Unprojectedresult = Vector4d(point, 1.0f) * (*this);
            if (Unprojectedresult.w == 0.0f || Unprojectedresult.w == 1.0f)
            {
                return Vector3d(Unprojectedresult.x, Unprojectedresult.y, Unprojectedresult.z);
            }
            else
            {
                return Vector3d(Unprojectedresult.x / Unprojectedresult.w,
                    Unprojectedresult.y / Unprojectedresult.w,
                    Unprojectedresult.z / Unprojectedresult.w);
            }
        }

        Vector3d matrix4d::TransformVector(const Vector3d& vector) const
        {
            Vector4d Unprojectedresult = Vector4d(vector, 0.0f) * (*this);
            if (Unprojectedresult.w == 0.0f || Unprojectedresult.w == 1.0f)
            {
                return Vector3d(Unprojectedresult.x, Unprojectedresult.y, Unprojectedresult.z);
            }
            else
            {
                return Vector3d(Unprojectedresult.x / Unprojectedresult.w,
                    Unprojectedresult.y / Unprojectedresult.w,
                    Unprojectedresult.z / Unprojectedresult.w);
            }
        }

        Vector3d matrix4d::TransformNormal(const Vector3d& normal) const
        {
            Vector4d Unprojectedresult = Vector4d(normal, 0.0f) * (*this);
            if (Unprojectedresult.w == 0.0f)
            {
                Unprojectedresult.w = 1.0f;
            }

            Vector3d result(Unprojectedresult.x / Unprojectedresult.w, Unprojectedresult.y / Unprojectedresult.w, Unprojectedresult.z / Unprojectedresult.w);

            if (Unprojectedresult.w < 0.0f)
            {
                result = -result;
            }
            return result;
        }

        // Calcul de l'inverse de la matrice
        matrix4d matrix4d::Inverse() const {
            float64 result[4][4];
            float64 tmp[12]; /* temp array for pairs */
            float64 src[16]; /* array of transpose source matrix */
            float64 det; /* determinant */
            /* transpose matrix */
            for (uint32 i = 0; i < 4; i++)
            {
                src[i + 0] = (*this)[i][0];
                src[i + 4] = (*this)[i][1];
                src[i + 8] = (*this)[i][2];
                src[i + 12] = (*this)[i][3];
            }
            /* calculate pairs for first 8 elements (cofactors) */
            tmp[0] = src[10] * src[15];
            tmp[1] = src[11] * src[14];
            tmp[2] = src[9] * src[15];
            tmp[3] = src[11] * src[13];
            tmp[4] = src[9] * src[14];
            tmp[5] = src[10] * src[13];
            tmp[6] = src[8] * src[15];
            tmp[7] = src[11] * src[12];
            tmp[8] = src[8] * src[14];
            tmp[9] = src[10] * src[12];
            tmp[10] = src[8] * src[13];
            tmp[11] = src[9] * src[12];
            /* calculate first 8 elements (cofactors) */
            result[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
            result[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
            result[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
            result[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
            result[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
            result[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
            result[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
            result[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
            result[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
            result[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
            result[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
            result[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
            result[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
            result[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
            result[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
            result[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
            /* calculate pairs for second 8 elements (cofactors) */
            tmp[0] = src[2] * src[7];
            tmp[1] = src[3] * src[6];
            tmp[2] = src[1] * src[7];
            tmp[3] = src[3] * src[5];
            tmp[4] = src[1] * src[6];
            tmp[5] = src[2] * src[5];

            tmp[6] = src[0] * src[7];
            tmp[7] = src[3] * src[4];
            tmp[8] = src[0] * src[6];
            tmp[9] = src[2] * src[4];
            tmp[10] = src[0] * src[5];
            tmp[11] = src[1] * src[4];

            /* calculate second 8 elements (cofactors) */
            result[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
            result[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
            result[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
            result[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
            result[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
            result[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
            result[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
            result[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
            result[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
            result[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
            result[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
            result[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
            result[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
            result[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
            result[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
            result[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

            /* calculate determinant */
            det = src[0] * result[0][0] + src[1] * result[0][1] + src[2] * result[0][2] + src[3] * result[0][3];

            /* calculate matrix inverse */
            det = 1.0f / det;

            matrix4d finalResult;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 j = 0; j < 4; j++)
                {
                    finalResult[i][j] = float64(result[i][j] * det);
                }
            }
            return finalResult;
        }

        Vector4d matrix4d::operator*(const Vector4d& vector) const {
            Vector4d result;
            for (int i = 0; i < 4; ++i) {
                result.ptr[i] = 0.0f;
                for (int j = 0; j < 4; ++j) {
                    result.ptr[i] += data[i * 4 + j] * vector.ptr[j];
                }
            }
            return result;
        }

        Vector4d operator*(const Vector4d& vector, const matrix4d& m) {
            Vector4d result;
            for (int i = 0; i < 4; ++i) {
                result.ptr[i] = 0.0f;
                for (int j = 0; j < 4; ++j) {
                    result.ptr[i] += vector.ptr[j] * m.data[j * 4 + i]; // Notez le changement ici
                }
            }
            return result;
        }

        Vector4d operator*(const matrix4d& m, const Vector4d& vector) {
            Vector4d result;
            for (int i = 0; i < 4; ++i) {
                result.ptr[i] = 0.0f;
                for (int j = 0; j < 4; ++j) {
                    result.ptr[i] += m.data[i * 4 + j] * vector.ptr[j];
                }
            }
            return result;
        }

        float64 matrix4d::CompareMatrices(const matrix4d& left, const matrix4d& right)
        {
            float64 Sum = 0.0f;
            for (uint32 i = 0; i < 4; i++)
            {
                for (uint32 i2 = 0; i2 < 4; i2++)
                {
                    Sum += maths::Abs(left[i][i2] - right[i][i2]);
                }
            }
            return Sum / 16.0f;
        }

        matrix4d matrix4d::Identity() {
            matrix4d in;
            in[0][0] = 1.0f;
            in[1][1] = 1.0f;
            in[2][2] = 1.0f;
            in[3][3] = 1.0f;
            return in;
        }

        matrix4d matrix4d::Camera(const Vector3d& eye, const Vector3d& _look, const Vector3d& _up, const Vector3d& _right)
        {
            Vector3d look = Vector3d(_look).Normalized();
            Vector3d up = Vector3d(_up).Normalized();
            Vector3d right = Vector3d(_right).Normalized();

            matrix4d result;
            result[0][0] = right.x;
            result[1][0] = right.y;
            result[2][0] = right.z;
            result[3][0] = -right.Dot(eye);

            result[0][1] = up.x;
            result[1][1] = up.y;
            result[2][1] = up.z;
            result[3][1] = -up.Dot(eye);

            result[0][2] = look.x;
            result[1][2] = look.y;
            result[2][2] = look.z;
            result[3][2] = -look.Dot(eye);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4d matrix4d::LookAt(const Vector3d& eye, const Vector3d& center, const Vector3d& up)
        {
            Vector3d xAxis, yAxis, zAxis;
            zAxis = (eye - center).Normalized();
            xAxis = Vector3d(up).Cross(zAxis).Normalized();
            yAxis = Vector3d(zAxis).Cross(xAxis).Normalized();

            matrix4d result;
            result[0][0] = xAxis.x;
            result[1][0] = xAxis.y;
            result[2][0] = xAxis.z;
            result[3][0] = -xAxis.Dot(eye);

            result[0][1] = yAxis.x;
            result[1][1] = yAxis.y;
            result[2][1] = yAxis.z;
            result[3][1] = -yAxis.Dot(eye);

            result[0][2] = zAxis.x;
            result[1][2] = zAxis.y;
            result[2][2] = zAxis.z;
            result[3][2] = -zAxis.Dot(eye);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4d matrix4d::Orthogonal(float64 width, float64 height, float64 zNear, float64 zFar)
        {
            matrix4d result;
            result[0][0] = 2.0f / width;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = 0.0f;

            result[0][1] = 0.0f;
            result[1][1] = 2.0f / height;
            result[2][1] = 0.0f;
            result[3][1] = 0.0f;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = 1.0f / (zNear - zFar);
            result[3][2] = zNear / (zNear - zFar);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4d matrix4d::Perspective(float64 width, float64 height, float64 zNear, float64 zFar)
        {
            matrix4d result;
            result[0][0] = 2.0f * zNear / width;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = 0.0f;

            result[0][1] = 0.0f;
            result[1][1] = 2.0f * zNear / height;
            result[2][1] = 0.0f;
            result[3][1] = 0.0f;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = zFar / (zNear - zFar);
            result[3][2] = zFar * zNear / (zNear - zFar);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = -1.0f;
            result[3][3] = 0.0f;
            return result;
        }

        matrix4d matrix4d::PerspectiveFov(float64 fov, float64 aspect, float64 zNear, float64 zFar)
        {
            float64 width = 1.0f / tanf(fov / 2.0f), height = aspect / tanf(fov / 2.0f);

            matrix4d result;
            result[0][0] = width;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = 0.0f;

            result[0][1] = 0.0f;
            result[1][1] = height;
            result[2][1] = 0.0f;
            result[3][1] = 0.0f;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = zFar / (zNear - zFar);
            result[3][2] = zFar * zNear / (zNear - zFar);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = -1.0f;
            result[3][3] = 0.0f;
            return result;
        }

        matrix4d matrix4d::PerspectiveMultifov(float64 fovX, float64 fovY, float64 zNear, float64 zFar)
        {
            float64 width = 1.0f / tanf(fovX / 2.0f), height = 1.0f / tanf(fovY / 2.0f);

            matrix4d result;
            result[0][0] = width;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = 0.0f;

            result[0][1] = 0.0f;
            result[1][1] = height;
            result[2][1] = 0.0f;
            result[3][1] = 0.0f;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = zFar / (zNear - zFar);
            result[3][2] = zFar * zNear / (zNear - zFar);

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = -1.0f;
            result[3][3] = 0.0f;
            return result;
        }

        matrix4d matrix4d::Rotation(const Vector3d& Axis, const Angle& angle)
        {
            float64 theta = angle.Rad();
            float64 c = cosf(theta);
            float64 s = sinf(theta);
            float64 t = 1.0f - c;

            Vector3d NormalizedAxis = Vector3d(Axis).Normalized();
            float64 x = NormalizedAxis.x;
            float64 y = NormalizedAxis.y;
            float64 z = NormalizedAxis.z;

            matrix4d result;
            result[0][0] = 1 + t * (x * x - 1);
            result[0][1] = z * s + t * x * y;
            result[0][2] = -y * s + t * x * z;
            result[0][3] = 0.0f;

            result[1][0] = -z * s + t * x * y;
            result[1][1] = 1 + t * (y * y - 1);
            result[1][2] = x * s + t * y * z;
            result[1][3] = 0.0f;

            result[2][0] = y * s + t * x * z;
            result[2][1] = -x * s + t * y * z;
            result[2][2] = 1 + t * (z * z - 1);
            result[2][3] = 0.0f;

            result[3][0] = 0.0f;
            result[3][1] = 0.0f;
            result[3][2] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4d matrix4d::Rotation(float64 yaw, float64 pitch, float64 roll)
        {
            return RotationY(yaw) * RotationX(pitch) * RotationZ(roll);
        }

        matrix4d matrix4d::Rotation(const Vector3d& Axis, const Angle& angle, const Vector3d& Center)
        {
            return Translation(-Center) * Rotation(Axis, angle) * Translation(Center);
        }

        matrix4d matrix4d::RotationX(const Angle& angle)
        {
            float64 theta = angle.Rad();
            float64 CosT = cosf(theta);
            float64 SinT = sinf(theta);

            matrix4d result = Identity();
            result[1][1] = CosT;
            result[1][2] = SinT;
            result[2][1] = -SinT;
            result[2][2] = CosT;
            return result;
        }

        matrix4d matrix4d::RotationY(const Angle& angle)
        {
            float64 theta = angle.Rad();
            float64 CosT = cosf(theta);
            float64 SinT = sinf(theta);

            matrix4d result = Identity();
            result[0][0] = CosT;
            result[0][2] = SinT;
            result[2][0] = -SinT;
            result[2][2] = CosT;
            return result;
        }

        matrix4d matrix4d::RotationZ(const Angle& angle)
        {
            float64 theta = angle.Rad();
            float64 CosT = cosf(theta);
            float64 SinT = sinf(theta);

            matrix4d result = Identity();
            result[0][0] = CosT;
            result[0][1] = SinT;
            result[1][0] = -SinT;
            result[1][1] = CosT;
            return result;
        }

        matrix4d matrix4d::Scaling(const Vector3d& scaleFactors)
        {
            matrix4d result;
            result[0][0] = scaleFactors.x;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = 0.0f;

            result[0][1] = 0.0f;
            result[1][1] = scaleFactors.y;
            result[2][1] = 0.0f;
            result[3][1] = 0.0f;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = scaleFactors.z;
            result[3][2] = 0.0f;

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4d matrix4d::Translation(const Vector3d& pos)
        {
            matrix4d result;
            result[0][0] = 1.0f;
            result[1][0] = 0.0f;
            result[2][0] = 0.0f;
            result[3][0] = pos.x;

            result[0][1] = 0.0f;
            result[1][1] = 1.0f;
            result[2][1] = 0.0f;
            result[3][1] = pos.y;

            result[0][2] = 0.0f;
            result[1][2] = 0.0f;
            result[2][2] = 1.0f;
            result[3][2] = pos.z;

            result[0][3] = 0.0f;
            result[1][3] = 0.0f;
            result[2][3] = 0.0f;
            result[3][3] = 1.0f;
            return result;
        }

        matrix4d matrix4d::Frustum(float64 left, float64 right, float64 bottom, float64 top, float64 zNear, float64 zFar) {
            matrix4d result;

            result.m00 = (2.0f * zNear) / (right - left);
            result.m11 = (2.0f * zNear) / (top - bottom);
            result.m20 = (right + left) / (right - left);
            result.m21 = (top + bottom) / (top - bottom);
            result.m22 = -(zFar + zNear) / (zFar - zNear);
            result.m23 = -1.0f;
            result.m32 = -(2.0f * zFar * zNear) / (zFar - zNear);

            return result;
        }

        std::string matrix4d::ToStringRow(uint32 i) {
            std::stringstream ss;

            ss << "[";
            for (uint32 i2 = 0; i2 < 4; i2++)
            {
                ss << mat[i][i2];
            }
            ss << "]";

            return ss.str();
        }

        std::string matrix4d::ToStringCol(uint32 i) {
            std::stringstream ss;

            ss << "[";
            for (uint32 i2 = 0; i2 < 4; i2++)
            {
                ss << mat[i2][i];
            }
            ss << "]";

            return ss.str();
        }

        std::string matrix4d::ToString() const {
            std::stringstream ss;
            ss << "matrix4d[";
            for (uint32 i1 = 0; i1 < 4; i1++) {
                ss << "[";
                for (uint32 i2 = 0; i2 < 4; i2++) {
                    ss << mat[i1][i2];

                    if (i2 < 3) {
                        ss << ", ";
                    }
                }
                ss << "]";

                if (i1 < 3) {
                    ss << ", ";
                }
            }
            return ss.str();
        }

        std::ostream& operator<<(std::ostream& os, const matrix4d& e) {
            return os << e.ToString();
        }
    }
}    // namespace nkentseu