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
        class NKENTSEU_API Quaternionf {
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
            static quatf fromMatrix(const mat4f& mat);
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
            int32 gimbalPole() const;

            Vector3f forward() const;
            Vector3f backward() const;
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
        };

    }
} // namespace nkentseu

#endif    // __NKENTSEU_QUATERNION_H__