//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 10:20:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Quaternion.h"

namespace nkentseu {

    Quaternionf::Quaternionf() : x(0), y(0), z(0), w(1) {}
    Quaternionf::Quaternionf(float32 _x, float32 _y, float32 _z, float32 _w) : x(_x), y(_y), z(_z), w(_w) {}
    Quaternionf::Quaternionf(const Quaternionf& quat) : quat(quat.quat) {}
    Quaternionf& Quaternionf::operator=(const Quaternionf& quat) {
        this->quat = quat.quat;
        return *this;
    }

    Quaternionf Quaternionf::AngleAxis(const Angle& angle, const Vector3f& axis) {
        Vector3f norm = Vector3f(axis).Normalized();
        float32 s = maths::Sin(angle * 0.5f);
        return Quaternionf(norm.x * s, norm.y * s, norm.z * s, maths::Cos(0.5f * angle));
    }

    Quaternionf Quaternionf::FromTo(const Vector3f& from, const Vector3f& to) {
        Vector3f f = Vector3f(from).Normalized();
        Vector3f t = Vector3f(to).Normalized();
        if (f == t) {
            return Quaternionf();
        }
        else if (f == t * -1.0f) {
            Vector3f ortho = Vector3f(1, 0, 0);
            if (maths::Abs(f.y) < maths::Abs(f.x)) {
                ortho = Vector3f(0, 1, 0);
            }
            if (maths::Abs(f.z) < maths::Abs(f.y) && maths::Abs(f.z) < maths::Abs(f.x)) {
                ortho = Vector3f(0, 0, 1);
            }
            Vector3f axis = f.Cross(ortho).Normalized();
            return Quaternionf(axis.x, axis.y, axis.z, 0);
        }
        Vector3f half = (f + t).Normalized();
        Vector3f axis = f.Cross(half);
        return Quaternionf(axis.x, axis.y, axis.z, f.Dot(half));
    }

    Vector3f Quaternionf::GetAxis() {
        // return quat.xyz().Normalized();
        return vector().Normalized();
    }

    Angle Quaternionf::GetAngle() {
        Angle angle = maths::ACos(w) * 2.0f;
        return angle;
    }

    Quaternionf operator+(const Quaternionf& a, const Quaternionf& b) {
        return Quaternionf(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    Quaternionf operator-(const Quaternionf& a, const Quaternionf& b) {
        return Quaternionf(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    Quaternionf operator*(const Quaternionf& a, float32 b) {
        return Quaternionf(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    Quaternionf operator*(float32 b, const Quaternionf& a) {
        return Quaternionf(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    Quaternionf operator-(const Quaternionf& q) {
        return Quaternionf(-q.x, -q.y, -q.z, -q.w);
    }

    bool operator==(const Quaternionf& left, const Quaternionf& right) {
        return (maths::Abs(left.x - right.x) <= maths::QuatEpsilon() && maths::Abs(left.y - right.y) <= maths::QuatEpsilon() &&
            maths::Abs(left.z - right.z) <= maths::QuatEpsilon() && maths::Abs(left.w - right.w) <= maths::QuatEpsilon());
    }

    bool operator!=(const Quaternionf& a, const Quaternionf& b) {
        return !(a == b);
    }

    float32 Quaternionf::Dot(const Quaternionf& b) {
        return x * b.x + y * b.y + z * b.z + w * b.w;
    }

    float32 Quaternionf::LenSq() {
        return x * x + y * y + z * z + w * w;
    }

    float32 Quaternionf::Len() {
        float32 lenSq = x * x + y * y + z * z + w * w;
        if (lenSq < maths::QuatEpsilon()) {
            return 0.0f;
        }
        return maths::Sqrt(lenSq);
    }

    void Quaternionf::Normalize() {
        float32 lenSq = x * x + y * y + z * z + w * w;
        if (lenSq < maths::QuatEpsilon()) {
            return;
        }
        float32 i_len = 1.0f / maths::Sqrt(lenSq);
        x *= i_len;
        y *= i_len;
        z *= i_len;
        w *= i_len;
    }

    Quaternionf Quaternionf::Normalized() {
        float32 lenSq = x * x + y * y + z * z + w * w;
        if (lenSq < maths::QuatEpsilon()) {
            return Quaternionf();
        }
        float32 il = 1.0f / maths::Sqrt(lenSq); // il: inverse length
        return Quaternionf(x * il, y * il, z * il, w * il);
    }

    Quaternionf Quaternionf::Conjugate() {
        return Quaternionf(-x, -y, -z, w);
    }

    Quaternionf Quaternionf::Inverse() {
        float32 lenSq = x * x + y * y + z * z + w * w;
        if (lenSq < maths::QuatEpsilon()) {
            return Quaternionf();
        }
        float32 recip = 1.0f / lenSq;
        return Quaternionf(-x * recip, -y * recip, -z * recip, w * recip);
    }

    Quaternionf operator*(const Quaternionf& Q1, const Quaternionf& Q2) {
        return Quaternionf(
            Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y + Q2.w * Q1.x,
            -Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x + Q2.w * Q1.y,
            Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w + Q2.w * Q1.z,
            -Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z + Q2.w * Q1.w
        );
    }

    /*Quaternionf operator*(const Quaternionf& Q1, const Quaternionf& Q2) {
        Quaternionf result;
        result.scalar = Q2.scalar * Q1.scalar - Vector3f(Q2.vector).Dot(Q1.vector);
        result.vector = (Q1.vector * Q2.scalar) + (Q2.vector * Q1.scalar) + Vector3f(Q2.vector).Cross(Q1.vector);
        return result;
    }*/

    Vector3f operator*(const Quaternionf& q, const Vector3f& v) {
        return q.vector() * 2.0f * q.vector().Dot(v) + v * (q.scalar() * q.scalar() - q.vector().Dot(q.vector())) + q.vector().Cross(v) * 2.0f * q.scalar();
    }

    Vector3f operator*(const Vector3f& v, const Quaternionf& q) {
        return q.vector() * 2.0f * q.vector().Dot(v) + v * (q.scalar() * q.scalar() - q.vector().Dot(q.vector())) + q.vector().Cross(v) * 2.0f * q.scalar();
    }

    /*Vector3f operator*(const Vector3f& v, const Quaternionf& q) {
        return v * 2.0f * Vector3f(v).Dot(q.vector) + q.vector * (q.scalar * q.scalar - Vector3f(v).Dot(v)) + Vector3f(v).Cross(q.vector) * 2.0f * q.scalar;
    }*/

    Quaternionf Quaternionf::Interpolation(const Quaternionf& a, const Quaternionf& b, float32 t) {
        Quaternionf bb(b);
        if (Quaternionf(a).Dot(b) < 0.0f) {
            bb = -b;
        }
        return Quaternionf(a).Slerp(b, t);
    }

    Quaternionf Quaternionf::Mix(const Quaternionf& to, float32 t) {
        return (*this) * (1.0f - t) + to * t;
    }

    Quaternionf Quaternionf::Lerp(const Quaternionf& e, float32 t) {
        return Quaternionf(
            x + (e.x - x) * t,
            y + (e.y - y) * t,
            z + (e.z - z) * t,
            w + (e.w - w) * t
        );
    }

    Quaternionf Quaternionf::Nlerp(const Quaternionf& to, float t) {
        return ((*this) + (to - (*this)) * t).Normalized();
    }

    Quaternionf operator^(const Quaternionf& q, float32 f) {
        Angle angle = (2.0f * maths::ACos(q.scalar())).Rad() * 0.5f * f;
        Vector3f axis = Vector3f(q.vector()).Normalized();
        float32 halfCos = maths::Cos(angle);
        float32 halfSin = maths::Sin(angle);
        return Quaternionf(axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos);
    }

    Quaternionf Quaternionf::Slerp(const Quaternionf& end, float32 t) {
        if (maths::Abs(Dot(end)) > 1.0f - maths::QuatEpsilon()) {
            return Nlerp(end, t);
        }
        Quaternionf delta = Inverse() * end;
        return ((delta ^ t) * (*this)).Normalized();
    }

    Quaternionf Quaternionf::LookAt(const Vector3f& direction, const Vector3f& up) {
        // Find orthonormal basis vectors
        Vector3f f = Vector3f(direction).Normalized(); // Object Forward
        Vector3f u = Vector3f(up).Normalized(); // Desired Up
        Vector3f r = u.Cross(f); // Object Right
        u = f.Cross(r); // Object Up
        // From world forward to object forward
        Quaternionf worldToObject = FromTo(Vector3f(0, 0, 1), f);
        // what direction is the new object up ?
        Vector3f objectUp = worldToObject * Vector3f(0, 1, 0);
        // From object up to desired up
        Quaternionf u2u = FromTo(objectUp, u);
        // Rotate to forward direction first
        // then twist to correct up
        Quaternionf result = worldToObject * u2u;
        // Don't forget to normalize the result
        return result.Normalized();
    }

    Quaternionf Quaternionf::LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up)
    {
        return LookAt((position - target).Normalized(), target, up);
    }

    Matrix4f Quaternionf::ToMat4() {
        Vector3f r = (*this) * Vector3f(1, 0, 0);
        Vector3f u = (*this) * Vector3f(0, 1, 0);
        Vector3f f = (*this) * Vector3f(0, 0, 1);
        return Matrix4f(r.x, r.y, r.z, 0,
            u.x, u.y, u.z, 0,
            f.x, f.y, f.z, 0,
            0, 0, 0, 1
        );
    }

    Quaternionf Quaternionf::FromMat4(const Matrix4f& m) {
        Vector3f up = Vector3f(m.up().x, m.up().y, m.up().z).Normalized();
        Vector3f forward = Vector3f(m.forward().x, m.forward().y, m.forward().z).Normalized();
        Vector3f right = up.Cross(forward);
        up = forward.Cross(right);
        return LookAt(forward, up);
    }

}    // namespace nkentseu