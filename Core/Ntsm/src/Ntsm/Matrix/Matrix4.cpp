//
// Created by TEUGUIA TADJUIDJE Rodolf Sederis on 4/12/2024 at 9:26:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Matrix4.h"
#include "Ntsm/Quaternion/Quaternion.h"

#include <cstring>

namespace nkentseu {

    namespace maths
    {
        // mat4f
        // Constructeur par defaut
        mat4f::mat4f() {
            for (int32 index = 0; index < 16; index++) {
                data[index] = 0.0f;
            }
        }

        // Constructeur par copie
        mat4f::mat4f(const mat4f& other) {
            for (int32 index = 0; index < 16; index++) {
                data[index] = other.data[index];
            }
        }

        // Constructeur avec un tableau de valeurs
        mat4f::mat4f(const float32* values) {
            std::memcpy(data, values, sizeof(data));
        }

        // Constructeur avec des vecteurs colonne
        mat4f::mat4f(const Vector4f& col0, const Vector4f& col1, const Vector4f& col2, const Vector4f& col3) {
            right = col0;
            up = col1;
            backward = col2;
            position = col3;
        }

        // Constructeur avec 16 reels
        mat4f::mat4f(float32 m00, float32 m10, float32 m20, float32 m30,
                     float32 m01, float32 m11, float32 m21, float32 m31,
                     float32 m02, float32 m12, float32 m22, float32 m32,
                     float32 m03, float32 m13, float32 m23, float32 m33) {
            this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
            this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
            this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
            this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
        }

        // Constructeur avec un seul reel
        mat4f::mat4f(float32 value, bool identity)
        {
            if (!identity) {
                for (int32 index = 0; index < 16; index++) {
                    data[index] = 0.0f;
                }
            }
            else {
                for (int32 column = 0; column < 4; column++) {
                    for (int32 row = 0; row < 4; row++) {
                        if (row == column) mat[column][row] = value;
                        else mat[column][row] = 0.0f;
                    }
                }

                mat[0][0] = value;
                mat[1][1] = value;
                mat[2][2] = value;
                mat[3][3] = value;
            }
        }

        // Constructeur avec trois vecteurs 3D
        mat4f::mat4f(const Vector3f& right, const Vector3f& up, const Vector3f& back) {
            this->right = Vector4f(right, 0.0f);
            this->up = Vector4f(up, 0.0f);
            this->backward = Vector4f(back, 0.0f);
            this->position = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
        }

        // Constructeur avec trois vecteurs 3D et une position
        mat4f::mat4f(const Vector3f& right, const Vector3f& up, const Vector3f& back, const Vector3f& position) {
            this->right = Vector4f(right, 0.0f);
            this->up = Vector4f(up, 0.0f);
            this->backward = Vector4f(back, 0.0f);
            this->position = Vector4f(position, 1.0f);
        }

        float32* mat4f::operator[](int32 col)
        {
            return &data[col * 4];
        }

        const float32* mat4f::operator[](int32 col) const
        {
            return &data[col * 4];
        }

        float32* mat4f::operator&()
        {
            return &(this->data[0]);
        }

        mat4f::operator float32* ()
        {
            return &(this->data[0]);
        }

        mat4f::operator const float32* () const
        {
            return &(this->data[0]);
        }

        // Methode pour obtenir une matrice identite
        mat4f mat4f::Identity() {
            return mat4f(1.0f);
        }

        // Operateur d'affectation
        mat4f& mat4f::operator=(const mat4f& other) {
            if (this != &other) {
                for (int32 index = 0; index < 16; index++) {
                    data[index] = other.data[index];
                }
            }
            return *this;
        }

        // Operateur d'egalite
        bool mat4f::operator==(const mat4f& other) const {
            for (int32 index = 0; index < 16; index++) {
                if (data[index] != other.data[index]) return false;
            }
            return true;
        }

        bool mat4f::operator!=(const mat4f& other) const {
            return !(*this == other);
        }

        // Multiplication mat x mat
        mat4f mat4f::operator*(const mat4f& other) const {
            mat4f result;
            for (int32 row = 0; row < 4; ++row) {
                for (int32 col = 0; col < 4; ++col) {
                    result.mat[col][row] = mat[0][row] * other.mat[col][0] +
                                           mat[1][row] * other.mat[col][1] +
                                           mat[2][row] * other.mat[col][2] +
                                           mat[3][row] * other.mat[col][3];
                }
            }
            return result;
        }

        // Multiplication mat x vec3
        Vector3f mat4f::operator*(const Vector3f& vec) const {
            return Vector3f(
                m00 * vec.x + m10 * vec.y + m20 * vec.z + m30,
                m01 * vec.x + m11 * vec.y + m21 * vec.z + m31,
                m02 * vec.x + m12 * vec.y + m22 * vec.z + m23
            );
        }

        // Multiplication mat x vec4
        Vector4f mat4f::operator*(const Vector4f& vec) const {
            return Vector4f(
                m00 * vec.x + m10 * vec.y + m20 * vec.z + m30 * vec.w,
                m01 * vec.x + m11 * vec.y + m21 * vec.z + m31 * vec.w,
                m02 * vec.x + m12 * vec.y + m22 * vec.z + m32 * vec.w,
                m03 * vec.x + m13 * vec.y + m23 * vec.z + m33 * vec.w
            );
        }

        // Multiplication mat x float32
        mat4f mat4f::operator*(float32 scalar) const {
            mat4f result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = data[i] * scalar;
            }
            return result;
        }

        // Multiplication float32 x mat
        mat4f operator*(float32 scalar, const mat4f& mat) {
            return mat * scalar;
        }

        // Addition mat + mat
        mat4f mat4f::operator+(const mat4f& other) const {
            mat4f result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = data[i] + other.data[i];
            }
            return result;
        }

        // Soustraction mat - mat
        mat4f mat4f::operator-(const mat4f& other) const {
            mat4f result;
            for (int i = 0; i < 16; ++i) {
                result.data[i] = data[i] - other.data[i];
            }
            return result;
        }

        // Transposition de la matrice
        mat4f mat4f::Transpose() const {
            mat4f result;
            result.m00 = m00; result.m01 = m10; result.m02 = m20; result.m03 = m30;
            result.m10 = m01; result.m11 = m11; result.m12 = m21; result.m13 = m31;
            result.m20 = m02; result.m21 = m12; result.m22 = m22; result.m23 = m32;
            result.m30 = m03; result.m31 = m13; result.m32 = m23; result.m33 = m33;
            return result;
        }

        /*EulerAngle mat4f::eulerAngles() const
        {
            EulerAngle angle;
            
            angle.yaw = maths::ASin(m02);

            if (angle.yaw >= 89.0f && angle.yaw <= 90.0f) {
                angle.roll = 0.0f;
                angle.pitch = maths::ATan(-m10, -m20);
            } else if (angle.yaw >= -90.0f && angle.yaw <= -89.0f) {
                angle.roll = 0.0f;
                angle.pitch = maths::ATan(m10, m20);
            }
            else {
                angle.roll = maths::ATan(m01, m00);
                angle.pitch = maths::ATan(m12, m22);
            }

            if (angle.roll >= 180.0f || angle.pitch >= 180.0f) {
                angle.yaw += 180.0f;
                angle.roll = angle.roll >= 180.0f ? Angle(0.0f) : angle.roll;
                angle.pitch = angle.pitch >= 180.0f ? Angle(0.0f) : angle.pitch;
            }

            return angle;
        }*/

        mat4f mat4f::LookAt(const Vector3f& position, const Vector3f& direction, const Vector3f& up, bool view) {
            Vector3f forward, right, upward;

            if (view) {
                // Si c'est une vue, le vecteur forward est dans la direction opposée
                forward = -direction.Normalized();
            }
            else {
                forward = direction.Normalized();
            }

            // Calcul du vecteur right comme produit vectoriel de up et forward
            right = up.Cross(forward).Normalized();

            // Recalcul du vecteur up pour qu'il soit orthogonal aux deux autres
            upward = right.Cross(forward);

            // Initialisation de la matrice résultat avec la matrice identité
            mat4f result(1.0f);

            result.right = Vector4f(right, 0.0f);
            result.up = Vector4f(upward, 0.0f);
            result.backward = Vector4f(forward, 0.0f);
            result.position = Vector4f(-position.Dot(right), -position.Dot(upward), -position.Dot(forward), 1.0f);

            return result;
        }

        // Méthode lookAt utilisant les vecteurs direction et up
        mat4f mat4f::LookAt(const Vector3f& direction, const Vector3f& up) {
            // Calculer le vecteur right comme le produit vectoriel de up et direction
            Vector3f right = up.Cross(direction).Normalized();

            // Recalculer le vecteur up pour qu'il soit orthogonal aux deux autres
            Vector3f upward = direction.Cross(right).Normalized();

            // Appeler la méthode lookAt déjà définie avec eye à l'origine
            return LookAt(Vector3f(0.0f, 0.0f, 0.0f), direction, upward, right);
        }

        // Methode lookAt utilisant les vecteurs right, up et back
        mat4f mat4f::LookAt(const Vector3f& eye, const Vector3f& look, const Vector3f& up, const Vector3f& right) {
            mat4f result(1.0f);

            result.right = Vector4f(right, 0.0f);
            result.up = Vector4f(up, 0.0f);
            result.backward = Vector4f(look, 0.0f);
            result.position = Vector4f(-eye.Dot(right), -eye.Dot(up), -eye.Dot(look), 1.0f);

            return result;
        }

        // Methode lookAt utilisant le centre et up
        mat4f mat4f::LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) {
            Vector3f right_transpose, upward_transpose, forward_transpose;
            forward_transpose = (eye - center).Normalized();
            right_transpose = up.Cross(forward_transpose).Normalized();
            upward_transpose = forward_transpose.Cross(right_transpose).Normalized();

            mat4f result;
            result.right = Vector4f(right_transpose.x, upward_transpose.x, forward_transpose.x, 0.0f);
            result.up = Vector4f(right_transpose.y, upward_transpose.y, forward_transpose.y, 0.0f);
            result.backward = Vector4f(right_transpose.z, upward_transpose.z, forward_transpose.z, 0.0f);
            result.position = Vector4f(-right_transpose.Dot(eye), -upward_transpose.Dot(eye), -forward_transpose.Dot(eye), 1.0f);
            return result;
        }

        // Methode orthogonal
        mat4f mat4f::Orthogonal(float32 width, float32 height, float32 zNear, float32 zFar) {
            mat4f result(1.0f);

            result.m00 = 2.0f / width;
            result.m11 = 2.0f / height;
            result.m22 = 2.0f / (zNear - zFar);
            result.m32 = (zFar + zNear) / (zNear - zFar);

            return result;
        }

        // Methode orthogonal utilisant un Vector2f pour la taille
        mat4f mat4f::Orthogonal(const Vector2f& size, float32 zNear, float32 zFar) {
            return Orthogonal(size.x, size.y, zNear, zFar);
        }

        // Methode orthogonal utilisant bottomLeft et topRight
        mat4f mat4f::Orthogonal(const Vector2f& bottomLeft, const Vector2f& topRight, float32 zNear, float32 zFar) {
            mat4f result(1.0f);

            result.m00 = 2.0f / (topRight.x - bottomLeft.x);
            result.m11 = 2.0f / (topRight.y - bottomLeft.y);
            result.m22 = 2.0f / (zNear - zFar);
            result.m30 = -(topRight.x + bottomLeft.x) / (topRight.x - bottomLeft.x);
            result.m31 = -(topRight.y + bottomLeft.y) / (topRight.y - bottomLeft.y);
            result.m32 = (zFar + zNear) / (zNear - zFar);

            return result;
        }

        // Methode perspective utilisant width et height
        mat4f mat4f::Perspective(float32 width, float32 height, float32 zNear, float32 zFar) {
            mat4f result;

            result.m00 = 2.0f * zNear / width;
            result.m11 = 2.0f * zNear / height;
            result.m22 = (zFar + zNear) / (zNear - zFar);
            result.m32 = 2.0f * zFar * zNear / (zNear - zFar);
            result.m23 = -1.0f;

            return result;
        }

        // Methode perspective utilisant un angle fov et un aspect ratio
        mat4f mat4f::Perspective(const Angle& fov, float32 aspect, float32 zNear, float32 zFar) {
            float32 tanHalfFovy = maths::Tan(fov * 0.5f);

            mat4f result;

            result.m00 = 1.0f / (aspect * tanHalfFovy);
            result.m11 = 1.0f / tanHalfFovy;
            result.m22 = (zFar + zNear) / (zNear - zFar);
            result.m32 = (2.0f * zFar * zNear) / (zNear - zFar);
            result.m23 = -1.0f;

            return result;
        }

        // Methode perspective utilisant deux angles fovX et fovY
        mat4f mat4f::Perspective(const Angle& fovX, const Angle& fovY, float32 zNear, float32 zFar) {
            float32 tanHalfFovX = maths::Tan(fovX * 0.5f);
            float32 tanHalfFovY = maths::Tan(fovY * 0.5f);

            mat4f result(0.0f);

            result.m00 = 1.0f / tanHalfFovX;
            result.m11 = 1.0f / tanHalfFovY;
            result.m22 = (zFar + zNear) / (zNear - zFar);
            result.m32 = (2.0f * zFar * zNear) / (zNear - zFar);
            result.m23 = -1.0f;

            return result;
        }

        // perspective avec Vector2f pour les coins
        mat4f mat4f::Perspective(const Vector2f& bottomLeft, const Vector2f& topRight, float32 zNear, float32 zFar) {
            return Frustum(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, zNear, zFar);
        }

        // fromEuler avec trois Angle
        mat4f mat4f::FromEuler(const EulerAngle& euler) {
            float32 cosPitch = maths::Cos(euler.pitch);
            float32 sinPitch = maths::Sin(euler.pitch);
            float32 cosYaw = maths::Cos(euler.yaw);
            float32 sinYaw = maths::Sin(euler.yaw);
            float32 cosRoll = maths::Cos(euler.roll);
            float32 sinRoll = maths::Sin(euler.roll);

            mat4f rotX(1.0f);
            rotX.m11 = cosPitch;
            rotX.m12 = -sinPitch;
            rotX.m21 = sinPitch;
            rotX.m22 = cosPitch;

            mat4f rotY(1.0f);
            rotY.m00 = cosYaw;
            rotY.m02 = sinYaw;
            rotY.m20 = -sinYaw;
            rotY.m22 = cosYaw;

            mat4f rotZ(1.0f);
            rotZ.m00 = cosRoll;
            rotZ.m01 = -sinRoll;
            rotZ.m10 = sinRoll;
            rotZ.m11 = cosRoll;

            return rotZ * rotY * rotX;
        }

        // frustum
        mat4f mat4f::Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar) {
            mat4f result(0.0f);

            result.m00 = (2.0f * zNear) / (right - left);
            result.m11 = (2.0f * zNear) / (top - bottom);
            result.m20 = (right + left) / (right - left);
            result.m21 = (top + bottom) / (top - bottom);
            result.m22 = -(zFar + zNear) / (zFar - zNear);
            result.m23 = -1.0f;
            result.m32 = -(2.0f * zFar * zNear) / (zFar - zNear);

            return result;
        }

        // reflection
        mat4f mat4f::Reflection(const Vector3f& normal) {
            Vector3f n = normal.Normalized();

            mat4f result(1.0f);

            result.m00 = 1 - 2 * n.x * n.x;
            result.m01 = -2 * n.x * n.y;
            result.m02 = -2 * n.x * n.z;

            result.m10 = -2 * n.y * n.x;
            result.m11 = 1 - 2 * n.y * n.y;
            result.m12 = -2 * n.y * n.z;

            result.m20 = -2 * n.z * n.x;
            result.m21 = -2 * n.z * n.y;
            result.m22 = 1 - 2 * n.z * n.z;

            return result;
        }

        // shearingXY
        mat4f mat4f::ShearingXY(float32 amountX, float32 amountY) {
            mat4f result(1.0f);
            result.m20 = amountX;
            result.m21 = amountY;
            return result;
        }

        // shearingXZ
        mat4f mat4f::ShearingXZ(float32 amountX, float32 amountZ) {
            mat4f result(1.0f);
            result.m10 = amountX;
            result.m12 = amountZ;
            return result;
        }

        // shearingYZ
        mat4f mat4f::ShearingYZ(float32 amountY, float32 amountZ) {
            mat4f result(1.0f);
            result.m01 = amountY;
            result.m02 = amountZ;
            return result;
        }

        // Méthode de rotation autour d'un axe
        mat4f mat4f::Rotation(const Vector3f& axis, const Angle& angle) {
            float32 c = maths::Cos(angle);
            float32 s = maths::Sin(angle);
            float32 oneMinusC = 1.0f - c;
            Vector3f v = axis.Normalized();

            mat4f result(1.0f);

            result.m00 = c + v.x * v.x * oneMinusC;
            result.m01 = v.x * v.y * oneMinusC + v.z * s;
            result.m02 = v.x * v.z * oneMinusC - v.y * s;

            result.m10 = v.y * v.x * oneMinusC - v.z * s;
            result.m11 = c + v.y * v.y * oneMinusC;
            result.m12 = v.y * v.z * oneMinusC + v.x * s;

            result.m20 = v.z * v.x * oneMinusC + v.y * s;
            result.m21 = v.z * v.y * oneMinusC - v.x * s;
            result.m22 = c + v.z * v.z * oneMinusC;

            return result;
        }

        // Méthode de rotation à partir des angles pitch, yaw, roll
        mat4f mat4f::Rotation(const EulerAngle& euler) {

            // Create a rotation matrix for each Euler angle
            mat4f rotX = RotationX(euler.pitch);
            mat4f rotY = RotationY(euler.yaw);
            mat4f rotZ = RotationZ(euler.roll);

            // Combine rotations (intrinsic rotations - ZYX order)
            //return rotY * rotX * rotZ;
            return rotZ * rotY * rotX;
        }

        // Méthode de rotation autour d'un axe avec un centre de rotation
        mat4f mat4f::Rotation(const Vector3f& axis, const Angle& angle, const Vector3f& center) {

            mat4f translateToOrigin = Translation(-center);
            mat4f rotate = Rotation(axis, angle);
            mat4f translateBack = Translation(center);

            //return translateToOrigin * rotate * translateBack;
            return translateBack * rotate * translateToOrigin;
        }

        // Méthode de rotation autour de l'axe X
        mat4f mat4f::RotationX(const Angle& pitch) {
            float32 cosA = cosf(pitch.Rad());
            float32 sinA = sinf(pitch.Rad());
            return mat4f(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cosA, -sinA, 0.0f,
                0.0f, sinA, cosA, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        // Méthode de rotation autour de l'axe Y
        mat4f mat4f::RotationY(const Angle& yaw) {
            float cosA = cosf(yaw.Rad());
            float sinA = sinf(yaw.Rad());
            return mat4f(
                cosA, 0.0f, sinA, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                -sinA, 0.0f, cosA, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        // Méthode de rotation autour de l'axe Z
        mat4f mat4f::RotationZ(const Angle& roll) {
            float cosA = cosf(roll.Rad());
            float sinA = sinf(roll.Rad());
            return mat4f(
                cosA, -sinA, 0.0f, 0.0f,
                sinA, cosA, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        // Méthode de mise à l'échelle
        mat4f mat4f::Scaling(const Vector3f& scaleFactors) {
            mat4f result(1.0f);

            result.m00 = scaleFactors.x;
            result.m11 = scaleFactors.y;
            result.m22 = scaleFactors.z;

            return result;
        }

        // Méthode de mise à l'échelle avec un axe spécifique
        mat4f mat4f::Scaling(const Vector3f& axis, const Vector3f& scaleFactors) {
            mat4f result(1.0f);

            result.m00 = scaleFactors.x * axis.x * axis.x + scaleFactors.y * (1 - axis.x * axis.x);
            result.m11 = scaleFactors.y * axis.y * axis.y + scaleFactors.x * (1 - axis.y * axis.y);
            result.m22 = scaleFactors.z * axis.z * axis.z + scaleFactors.x * (1 - axis.z * axis.z);

            return result;
        }

        // Méthode de translation
        mat4f mat4f::Translation(const Vector3f& pos) {
            mat4f result(1.0f);

            result.m30 = pos.x;
            result.m31 = pos.y;
            result.m32 = pos.z;

            return result;
        }

        // Méthode de translation avec un axe spécifique
        mat4f mat4f::Translation(const Vector3f& axis, const Vector3f& move) {
            mat4f result(1.0f);

            result.m00 = 1.0f + move.x * axis.x;
            result.m11 = 1.0f + move.y * axis.y;
            result.m22 = 1.0f + move.z * axis.z;
            result.m30 = move.x;
            result.m31 = move.y;
            result.m32 = move.z;

            return result;
        }

        // Méthode pour calculer le cofacteur d'un élément spécifique de la matrice
        float32 mat4f::Cofactor(int32 row, int32 col) const {
            // Compute minors
            float32 minors[9];
            int32 minorIndex = 0;
            for (int32 i = 0; i < 4; ++i) {
                if (i == row) continue;
                for (int32 j = 0; j < 4; ++j) {
                    if (j == col) continue;
                    minors[minorIndex++] = mat[i][j];
                }
            }

            // Compute determinant of the 3x3 matrix formed by minors
            float32 cofactorSign = ((row + col) % 2 == 0) ? 1.0f : -1.0f;
            return cofactorSign * Determinant3x3(minors[0], minors[1], minors[2],
                minors[3], minors[4], minors[5],
                minors[6], minors[7], minors[8]);
        }

        // Méthode pour calculer le déterminant d'une matrice 4x4
        float32 mat4f::Determinant() const {
            float32 det = 0.0f;
            for (int32 j = 0; j < 4; ++j) {
                det += mat[0][j] * Cofactor(0, j);
            }
            return det;
        }

        // Méthode pour calculer le déterminant d'une matrice 3x3
        float32 mat4f::Determinant3x3(float32 m00, float32 m01, float32 m02,
            float32 m10, float32 m11, float32 m12,
            float32 m20, float32 m21, float32 m22) const {
            return m00 * (m11 * m22 - m12 * m21) -
                m01 * (m10 * m22 - m12 * m20) +
                m02 * (m10 * m21 - m11 * m20);
        }

        // Méthode pour calculer l'inverse de la matrice
        mat4f mat4f::Inverse() const {
            mat4f inv;

            // Calculate matrix of minors and cofactors
            for (int32 i = 0; i < 4; ++i) {
                for (int32 j = 0; j < 4; ++j) {
                    inv.mat[i][j] = Cofactor(i, j);
                }
            }

            // Calculate determinant of the original matrix
            float32 det = Determinant();

            // Check if matrix is singular (det == 0)
            if (maths::EqualF32(det, 0.0f)) {
                return mat4f(1.0f);
            }

            // Transpose and scale by determinant to get the inverse
            float32 invDet = 1.0f / det;
            for (int32 i = 0; i < 4; ++i) {
                for (int32 j = 0; j < 4; ++j) {
                    inv.mat[i][j] *= invDet;
                }
            }

            return inv;
        }

        Vector3f mat4f::TransformPoint(const Vector3f& point) const {
            Vector4f result = (*this) * Vector4f(point, 1.0f);

            if (result.w == 0.0f || result.w == 1.0f) return result.xyz();
            return result.xyz() / result.w;
        }

        Vector3f mat4f::TransformVector(const Vector3f& vector) const {
            Vector4f result = (*this) * Vector4f(vector, 0.0f);

            if (result.w == 0.0f || result.w == 1.0f) return result.xyz();
            return result.xyz() / result.w;
        }

        Vector3f mat4f::TransformNormal(const Vector3f& normal) const {
            // Calcul de l'inverse de la matrice
            mat4f inverse = this->Inverse(); // Assurez-vous que la méthode inverse() est bien définie
            mat4f normalMatrix = inverse.Transpose();

            float32 x = normal.x * normalMatrix.m00 + normal.y * normalMatrix.m10 + normal.z * normalMatrix.m20;
            float32 y = normal.x * normalMatrix.m01 + normal.y * normalMatrix.m11 + normal.z * normalMatrix.m21;
            float32 z = normal.x * normalMatrix.m02 + normal.y * normalMatrix.m12 + normal.z * normalMatrix.m22;

            return Vector3f(x, y, z).Normalized();
        }

        std::string mat4f::ToStringRow(uint32 i) const
        {
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

        std::string mat4f::ToStringCol(uint32 i) const
        {
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

        std::string mat4f::ToString() const
        {
            std::stringstream ss;
            ss << "mat4f[";
            for (uint32 i1 = 0; i1 < 4; i1++) {
                ss << ToStringRow(i1);
                if (i1 < 3) {
                    ss << "\n, ";
                }
            }
            ss << "]";
            return ss.str();
        }

        std::ostream& operator<<(std::ostream& os, const mat4f& e)
        {
            return os << e.ToString();
        }
    }
}    // namespace nkentseu