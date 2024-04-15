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
        Quaternionf& operator=(const Quaternionf& quat);

        // Getters
        Vector3f vector() { return Vector3f(x, y, z); }
        float32 scalar() { return w; }

        // Const getters
        Vector3f vector() const { return Vector3f(x, y, z); }
        const float32 scalar() const { return w; }

        // Setters
        void vector(const Vector3f& _vector) { x = _vector.x; y = _vector.y; z = _vector.z; }
        void scalar(float32 _scalar) { w = _scalar; }

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

        Angle GetAngle();

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

        //friend Quaternionf operator*(const Quaternionf& Q1, const Quaternionf& Q2);

        friend Vector3f operator*(const Quaternionf& q, const Vector3f& v);

        friend Vector3f operator*(const Vector3f& v, const Quaternionf& q);

        //friend Vector3f operator*(const Vector3f& v, const Quaternionf& q);

        Quaternionf Interpolation(const Quaternionf& a, const Quaternionf& b, float32 t = 0.5f);

        Quaternionf Mix(const Quaternionf& to, float32 t);

        Quaternionf Lerp(const Quaternionf& e, float32 t);

        Quaternionf Nlerp(const Quaternionf& to, float t);

        friend Quaternionf operator^(const Quaternionf& q, float32 f);

        Quaternionf Slerp(const Quaternionf& end, float32 t);

        static Quaternionf LookAt(const Vector3f& direction, const Vector3f& up);
        static Quaternionf LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up);

        Matrix4f ToMat4();

        static Quaternionf FromMat4(const Matrix4f& m);
    };

    using Quaternion = Quaternionf;
    using Quat = Quaternionf;
} // namespace nkentseu

#endif    // __NKENTSEU_QUATERNION_H__