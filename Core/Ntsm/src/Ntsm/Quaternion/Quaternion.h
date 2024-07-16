//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 10:20:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_QUATERNION_H__
#define __NKENTSEU_QUATERNION_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector4.h>
#include <Ntsm/Matrix/Matrix4.h>

namespace nkentseu {

    namespace maths
    {
        struct NKENTSEU_API quatf {
            union {
                struct {
                    float32 x;
                    float32 y;
                    float32 z;
                    float32 w;
                };
                struct {
                    Vector3f vector;
                    float32 scalar;
                };
                Vector4f quat;
                float32 data[4];
            };

            // Constructeurs
            quatf();
            quatf(float32 value, bool identity = true);
            quatf(float32 x, float32 y, float32 z, float32 w);
            quatf(float32 x, float32 y, float32 z);
            quatf(const Angle& angle, const Vector3f& axis);
            quatf(const EulerAngle& eulerAngle);
            quatf(const Vector3f& vector, float32 scalar);
            quatf(const mat4f& mat);
            quatf(const quatf& quat);
            quatf(const Vector4f& quat);
            quatf(const Vector3f& from, const Vector3f& to);

            quatf& operator=(const quatf& other);

            operator float32* () {
                return &data[0];
            }

            inline float32& operator[] (size_t index) {
                return this->data[index % 4];
            }

            inline const float32& operator[](size_t index) const {
                return this->data[index % 4];
            }

            quatf operator+(const quatf& right);
            quatf& operator+=(const quatf& right);
            quatf operator-(const quatf& right);
            quatf& operator-=(const quatf& right);
            quatf operator*(const quatf& right);
            quatf operator*(float32 right);
            quatf operator/(float32 right);
            quatf& operator*=(const quatf& right);

            friend quatf operator*(float32 left, const quatf& right);
            friend quatf operator*(const quatf& right, float32 left);
            friend Vector3f operator*(const Vector3f& left, const quatf& right);
            friend Vector3f operator*(const quatf& left, const Vector3f& right);

            friend quatf operator+(const quatf& a, const quatf& b);
            friend quatf operator-(const quatf& a, const quatf& b);

            friend quatf operator^(const quatf& q, float32 f);

            friend bool operator==(const quatf& left, const quatf& right);
            friend bool operator!=(const quatf& a, const quatf& b);

            operator mat4f ();
            operator EulerAngle ();

            static quatf LookAt(const Vector3f& direction, const Vector3f& up);
            static quatf LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up);

            float32 Dot(const quatf& right);
            float32 LenSq();
            float32 Len();
            void Normalize();
            quatf Normalized() const;
            quatf Conjugate();
            quatf Inverse(bool normalize = true);

            quatf Cross(const quatf& quat);

            int32 GimbalPole() const;

            static quatf Reflection(const  Vector3f& normal);

            bool IsNormalize();
            Vector3f Axis() const;
            Angle Angles() const;
            quatf Mix(const quatf& to, float32 t);
            quatf Lerp(const quatf& e, float32 t);
            quatf NLerp(const quatf& to, float t);

            quatf SLerp(const quatf& end, float32 t);

            static quatf identity() {
                return quatf(0, 0, 0, 1);
            }

            EulerAngle eulerAngle() const {
                quatf q(*this);
                return (EulerAngle)q;
            }
            /*Vector3f forward() const {
                return (*this * Vector3f(0, 0, 1));
            }
            Vector3f back()  const {
                return (*this * Vector3f(0, 0, -1));
            }
            Vector3f up()  const {
                return (*this * Vector3f(0, 1, 0));
            }
            Vector3f down()  const {
                return (*this * Vector3f(0, -1, 0));
            }
            Vector3f left()  const {
                return (*this * Vector3f(-1, 0, 0));
            }
            Vector3f right()  const {
                return (*this * Vector3f(1, 0, 0));
            }*/

            static quatf rotateX(const Angle& pitch);
            static quatf rotateY(const Angle& yaw);
            static quatf rotateZ(const Angle& roll);
            Vector3f transformVector(const Vector3f& vector);
            Vector3f reflectVector(const Vector3f& vector);

            friend std::ostream& operator<<(std::ostream& os, const quatf& e) {
                return os << e.ToString();
            }

            std::string ToString() const {
                std::stringstream ss;
                ss << "[" << x << ", " << y << ", " << z << "; " << w << "]";
                return ss.str();
            }

            friend std::string ToString(const quatf& v) {
                return v.ToString();
            }

            void epsi();
        };
        /*class NKENTSEU_API Quaternionf {
        public:
            union {
                struct {
                    float32 x;
                    float32 y;
                    float32 z;
                    float32 w;
                };
                Vector4f quat;
                float32 ptr[4];
            };

            Quaternionf();
            Quaternionf(float32 _x, float32 _y, float32 _z, float32 _w);
            Quaternionf(const Quaternionf& quat);
            Quaternionf(const matrix4f& m);
            Quaternionf& operator=(const Quaternionf& quat);

            friend std::ostream& operator<<(std::ostream& os, const Quaternionf& quat) {
                return os << quat.ToString();
            }

            std::string ToString() const {
                std::stringstream ss;
                ss << "Quaternionf(" << x << ", " << y << ", " << z << ", " << w << ")";
                return ss.str();
            }

            friend std::string ToString(const Quaternionf& quat) {
                return quat.ToString();
            }

            // Getters
            Vector3f vector() { return Vector3f(x, y, z); }
            float32 scalar() { return w; }
            matrix4f mat4();
            Vector3f euler() { return EulerAngle(); }
            Vector3f axis() { return GetAxis(); }
            Angle angle() { return GetAngle(); }

            // Const getters
            Vector3f vector() const { return Vector3f(x, y, z); }
            const float32 scalar() const { return w; }
            matrix4f mat4() const;
            Vector3f euler() const { return EulerAngle(); }
            Vector3f axis() const { return GetAxis(); }
            Angle angle() const { return GetAngle(); }

            Vector3f forward() const;
            Vector3f up() const;
            Vector3f right() const;

            // Setters
            void vector(const Vector3f& _vector) { x = _vector.x; y = _vector.y; z = _vector.z; }
            void scalar(float32 _scalar) { w = _scalar; }
            static Quaternionf mat4(const matrix4f& m) { return Quaternionf(m); }
            static Quaternionf euler(const Angle& pitch, const Angle& yaw, const Angle& roll) { return FromEuler(pitch, yaw, roll); }
            static Quaternionf euler(const Vector3f angle) { return FromEuler(Angle(angle.pitch), Angle(angle.yaw), Angle(angle.roll)); }

            operator float32* () {
                return &ptr[0];
            }

            inline float32& operator[] (size_t index) {
                return this->ptr[index % 4];
            }

            inline const float32& operator[](size_t index) const {
                return this->ptr[index % 4];
            }

            static Quaternionf AngleAxis(const Angle& angle, const Vector3f& axis);

            static Quaternionf FromTo(const Vector3f& from, const Vector3f& to);

            Vector3f GetAxis();
            Vector3f GetAxis() const;

            Angle GetAngle() const;

            friend Quaternionf operator+(const Quaternionf& a, const Quaternionf& b);

            friend Quaternionf operator-(const Quaternionf& a, const Quaternionf& b);

            friend Quaternionf operator*(const Quaternionf& a, float32 b);

            friend Quaternionf operator*(float32 b, const Quaternionf& a);

            friend Quaternionf operator-(const Quaternionf& q);

            friend bool operator==(const Quaternionf& left, const Quaternionf& right);

            friend bool operator!=(const Quaternionf& a, const Quaternionf& b);

            float32 Dot(const Quaternionf& b);

            float32 LenSq();

            float32 Len();

            void Normalize();

            Quaternionf Normalized();

            Quaternionf Conjugate();

            Quaternionf Inverse();

            friend Quaternionf operator*(const Quaternionf& Q1, const Quaternionf& Q2);

            friend Vector3f operator*(const Quaternionf& q, const Vector3f& v);

            friend Vector3f operator*(const Vector3f& v, const Quaternionf& q);

            Quaternionf Interpolation(const Quaternionf& to, float32 t = 0.5f);
            static Quaternionf Interpolation(const Quaternionf& a, const Quaternionf& b, float32 t = 0.5f);

            Quaternionf Mix(const Quaternionf& to, float32 t);

            Quaternionf Lerp(const Quaternionf& e, float32 t);

            Quaternionf Nlerp(const Quaternionf& to, float t);

            friend Quaternionf operator^(const Quaternionf& q, float32 f);

            Quaternionf Slerp(const Quaternionf& end, float32 t);

            static Quaternionf LookAt(const Vector3f& direction, const Vector3f& up);
            static Quaternionf LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up);

            matrix4f ToMat4() const;

            static Quaternionf FromMat4(const matrix4f& m);

            static Quaternionf FromEuler(const Vector3f& angle);
            static Quaternionf FromEuler(const Angle& pitch, const Angle& yaw, const Angle& roll);

            static Quaternionf Identity();

            Vector3f EulerAngle() const;
            int32 GimbalPole() const;

            Vector3f Transform(const Vector3f& v);
        };

        using Quaternion = Quaternionf;
        using Quat = Quaternionf;

        struct NKENTSEU_API quatf {
            union {
                struct {
                    float32 x;
                    float32 y;
                    float32 z;
                    float32 w;
                };
                struct {
                    Vector3f vector;
                    float32 scalar;
                };
                Vector4f quat;
                float32 ptr[4];
            };

            // Constructeurs
            quatf();
            quatf(float32 value, bool identity = true);
            quatf(float32 x, float32 y, float32 z, float32 w);
            quatf(const Vector3f& axis, const Angle& angle);
            quatf(const Angle& pitch, const Angle& yaw, const Angle& roll);
            quatf(const EulerAngle& eulerAngles);
            quatf(const Vector3f& vector);
            quatf(const Vector3f& vector, float32 scalar);
            quatf(const mat4f& mat);
            quatf(const quatf& quat);
            quatf(const Vector3f& from, const Vector3f& to);

            quatf& operator=(const quatf& auther);

            //operator mat4f ();
            operator float32* ();
            inline float32& operator[] (int32 index);
            inline const float32& operator[](size_t index) const;

            // Opérateurs
            quatf operator*(const quatf& quat) const;
            quatf operator*(float32 scale) const;
            Vector3f operator*(const Vector3f& v) const;
            quatf operator-(const quatf& quat);
            quatf operator+(const quatf& quat);

            //friend quatf operator*(const Vector3f& v, const quatf& quat);
            friend quatf operator*(const Vector3f& v, const quatf& quat) {
                return quat * v;
            }

            friend quatf operator*(float32 scale, const quatf& quat);
            friend bool operator==(const quatf& left, const quatf& right);
            friend bool operator!=(const quatf& left, const quatf& right);

            // Méthodes
            void normalize();
            quatf normalized() const;
            quatf conjugate();
            quatf inverse(bool normalize = true);
            quatf cross(const quatf& quat);
            static quatf fromEuler(const EulerAngle& eulerAngles);
            static quatf fromEuler(const Angle& pitch, const Angle& yaw, const Angle& roll);
            mat4f toMatrix() const;
            static quatf fromMatrix(const mat4f& mat, bool addScale = false);
            static quatf reflection(const  Vector3f& normal);
            bool isNormalize();

            float32 dot(const quatf& b);
            float32 lenSq();
            float32 len();

            Vector3f axis() const;
            Angle angle() const;

            static quatf euler(const EulerAngle& angle);
            static quatf euler(const Angle& pitch, const Angle& yaw, const Angle& roll);

            quatf mix(const quatf& to, float32 t);

            quatf lerp(const quatf& e, float32 t);

            quatf nlerp(const quatf& to, float t);

            friend quatf operator^(const quatf& q, float32 f);

            quatf slerp(const quatf& end, float32 t);

            static quatf lookAt(const Vector3f& eye, const Vector3f& look, const Vector3f& up, const Vector3f& right);
            static quatf lookAt(const Vector3f& direction, const Vector3f& up);
            static quatf lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up);

            static quatf identity();

            EulerAngle eulerAngle() const;
            EulerAngle eulerAngle2() const;
            EulerAngle eulerAngle3() const;
            EulerAngle eulerAngle4() const;
            EulerAngle eulerAngle5() const;
            EulerAngle eulerAngle6() const;
            EulerAngle eulerAngle7() const;
            EulerAngle eulerAngle8() const;
            EulerAngle eulerAngle9() const;
            EulerAngle eulerAngle10() const;
            EulerAngle eulerAngle11() const;
            int32 gimbalPole() const;

            Vector3f forward() const;
            Vector3f back() const;
            Vector3f up() const;
            Vector3f down() const;
            Vector3f right() const;
            Vector3f left() const;

            Vector3f transformVector(const Vector3f& vector);
            Vector3f reflectVector(const Vector3f& vector);

            // Méthodes pour créer des quaternions de rotation autour des axes X, Y et Z
            static quatf rotateX(const Angle& pitch);
            static quatf rotateY(const Angle& yaw);
            static quatf rotateZ(const Angle& roll);

            /*quatf lookAt(Vector3f& lookAt, Vector3f& upDirection) {
                Vector3f forward = lookAt.Normalized();
                Vector3f up = upDirection.Normalized();
                Vector3f right = up.Cross(forward);

                quatf ret;
                ret.w = sqrtf(1.0f + right.x + up.y + forward.z) * 0.5f;
                float32 w4_recip = 1.0f / (4.0f * ret.w);
                ret.x = (up.z - forward.y) * w4_recip;
                ret.y = (forward.x - right.z) * w4_recip;
                ret.z = (right.y - up.x) * w4_recip;

                return ret;
            }

            quatf lookAt(Vector3f& lookAt) {
                Vector3f forward = lookAt.Normalized();
                Vector3f up = Vector3f::Up();
                Vector3f right = up.Cross(forward);

                quatf ret;
                ret.w = sqrtf(1.0f + right.x + up.y + forward.z) * 0.5f;
                float32 w4_recip = 1.0f / (4.0f * ret.w);
                ret.x = (up.z - forward.y) * w4_recip;
                ret.y = (forward.x - right.z) * w4_recip;
                ret.z = (right.y - up.x) * w4_recip;

                return ret;
            }* /

            friend std::ostream& operator<<(std::ostream& os, const quatf& e) {
                return os << e.ToString();
            }

            std::string ToString() const {
                std::stringstream ss;
                ss << "quatf(" << x << ", " << y << ", " << z << ", " << w << ")";
                return ss.str();
            }

            friend std::string ToString(const quatf& v) {
                return v.ToString();
            }
        };*/

    }
} // namespace nkentseu

#endif    // __NKENTSEU_QUATERNION_H__