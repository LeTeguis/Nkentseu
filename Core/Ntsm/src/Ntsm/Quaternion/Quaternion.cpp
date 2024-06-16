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
    Quaternionf::Quaternionf(const matrix4f& m)
    {
        Vector3f up = m.Up().Normalized();
        Vector3f forward = m.Forward().Normalized();
        Vector3f right = up.Cross(forward);
        up = forward.Cross(right);
        *this = LookAt(forward, up);
    }
    Quaternionf& Quaternionf::operator=(const Quaternionf& quat) {
        this->quat = quat.quat;
        return *this;
    }

    Vector3f Quaternionf::forward() const
    {
        return Vector3f();
    }

    Vector3f Quaternionf::up() const
    {
        return Vector3f();
    }

    Vector3f Quaternionf::right() const
    {
        return Vector3f();
    }

    Quaternionf Quaternionf::AngleAxis(const Angle& angle, const Vector3f& axis) {
        float32 d = Vector3f(axis).Len();
        if (d == 0.0f) return Identity();
        d = 1.0f / d;
        float32 l_sin = (float32)maths::Sin(angle * 0.5f);
        float32 l_cos = (float32)maths::Cos(angle * 0.5f);
        return Quaternionf(d * axis.pitch * l_sin, d * axis.yaw * l_sin, d * axis.roll * l_sin, l_cos).Normalized();
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
        // Normalize the quaternion
        Quaternionf normalizedQuat = Normalized();

        // Calculate the rotation angle
        Angle angle = 2.0f * maths::ACos(Angle::FromRadian(normalizedQuat.w));

        // Calculate the rotation axis (if not zero rotation)
        if (maths::Abs(angle.Rad()) > 0.00001f) {
            return normalizedQuat.vector() / maths::Sin(angle * 0.5f);
        }
        else {
            // Rotation angle is zero, axis is undefined
            // You can handle this as needed (e.g., return an arbitrary unit vector)
            return Vector3f(0.0f, 0.0f, 1.0f); // Positive x-axis
        }
        // return quat.xyz().Normalized();
        //return vector().Normalized();
    }

    Vector3f Quaternionf::GetAxis() const
    {
        Quaternionf normalizedQuat = Quaternionf(*this).Normalized();

        Angle angle = 2.0f * maths::ACos(Angle::FromRadian(normalizedQuat.w));

        if (maths::Abs(angle.Rad()) > 0.00001f) {
            return normalizedQuat.vector() / maths::Sin(angle * 0.5f);
        }
        else {
            return Vector3f(0.0f, 0.0f, 1.0f); // Positive x-axis
        }
    }

    Angle Quaternionf::GetAngle() const {
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

    Vector3f operator*(const Quaternionf& q, const Vector3f& v) {
        return q.vector() * 2.0f * q.vector().Dot(v) + v * (q.scalar() * q.scalar() - q.vector().Dot(q.vector())) + q.vector().Cross(v) * 2.0f * q.scalar();
    }

    Vector3f operator*(const Vector3f& v, const Quaternionf& q) {
        return q.vector() * 2.0f * q.vector().Dot(v) + v * (q.scalar() * q.scalar() - q.vector().Dot(q.vector())) + q.vector().Cross(v) * 2.0f * q.scalar();
    }

    Quaternionf Quaternionf::Interpolation(const Quaternionf& to, float32 t)
    {
        Quaternionf bb(to);
        if (Dot(to) < 0.0f) {
            bb = -to;
        }
        return Slerp(bb, t);
    }

    Quaternionf Quaternionf::Interpolation(const Quaternionf& a, const Quaternionf& b, float32 t) {
        return Quaternionf(a).Interpolation(b, t);
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
        return LookAt((position - target).Normalized(), up);
    }

    Matrix4f Quaternionf::ToMat4() const {

        float32 xx = x * x;
        float32 xy = x * y;
        float32 xz = x * z;
        float32 yy = y * y;
        float32 yz = y * z;
        float32 zz = z * z;
        float32 wx = w * x;
        float32 wy = w * y;
        float32 wz = w * z;

        Matrix4f matrix;
        matrix.m00 = 1.0f - 2.0f * (yy + zz);
        matrix.m01 = 2.0f * (xy - wz);
        matrix.m02 = 2.0f * (xz + wy);
        matrix.m03 = 0.0f;
        matrix.m10 = 2.0f * (xy + wz);
        matrix.m11 = 1.0f - 2.0f * (xx + zz);
        matrix.m12 = 2.0f * (yz - wx);
        matrix.m13 = 0.0f;
        matrix.m20 = 2.0f * (xz - wy);
        matrix.m21 = 2.0f * (yz + wx);
        matrix.m22 = 1.0f - 2.0f * (xx + yy);
        matrix.m23 = 0.0f;
        matrix.m30 = 0.0f;
        matrix.m31 = 0.0f;
        matrix.m32 = 0.0f;
        matrix.m33 = 1.0f;

        return matrix;
    }

    Quaternionf Quaternionf::FromMat4(const Matrix4f& m) {
        return Quaternionf(m);
    }

    Quaternionf Quaternionf::FromEuler(const Vector3f& angle)
    {
        return FromEuler(Angle(angle.pitch), Angle(angle.yaw), Angle(angle.roll));
    }

    Quaternionf Quaternionf::FromEuler(const Angle& pitch, const Angle& yaw, const Angle& roll)
    {
        Angle pitch_ = pitch * 0.5f;
        Angle yaw_ = yaw * 0.5f;
        Angle roll_ = roll * 0.5f;

        Quaternionf quat;

        Vector3f cos;
        cos.pitch = (float32)maths::Cos(pitch_);
        cos.yaw = (float32)maths::Cos(yaw_);
        cos.roll = (float32)maths::Cos(roll_);

        Vector3f sin;
        sin.pitch = (float32)maths::Sin(pitch_);
        sin.yaw = (float32)maths::Sin(yaw_);
        sin.roll = (float32)maths::Sin(roll_);

        quat.w = cos.pitch * cos.yaw * cos.roll + sin.pitch * sin.yaw * sin.roll;
        quat.x = sin.pitch * cos.yaw * cos.roll - cos.pitch * sin.yaw * sin.roll;
        quat.y = cos.pitch * sin.yaw * cos.roll + sin.pitch * cos.yaw * sin.roll;
        quat.z = cos.pitch * cos.yaw * sin.roll - sin.pitch * sin.yaw * cos.roll;

        return quat;
    }

    Quaternionf Quaternionf::Identity()
    {
        return Quaternionf(0.0f, 0.0f, 0.0f, 1.0f);
    }

    Vector3f Quaternionf::EulerAngle() const
    {
        int32 pole = GimbalPole();

        Angle yaw(0.0f);
        Angle roll(0.0f);
        Angle pitch(0.0f);

        if (pole == 0) {
            yaw = maths::ATan(2.0f * (y * w + x * z), 1.0f - 2.0f * (y * y + x * x));
            roll = maths::ATan(2.0f * (w * z + y * x), 1.0f - 2.0f * (x * x + z * z));
            pitch = maths::ASin(maths::Clamp<float32>(2.0f * (w * x - z * y), -1.0f, 1.0f));
        }
        else {
            roll = Angle::FromRadian((float32)pole * 2.0f * maths::ATan(y, w).Rad());
            pitch = Angle::FromRadian(((float32)pole * maths::Pi() * 0.5f));
        }

        return Vector3f(pitch.Deg(), yaw.Deg(), roll.Deg());
    }

    int32 Quaternionf::GimbalPole() const
    {
        float32 t = y * x + z * w;
        return t > 0.499f ? 1 : (t < -0.499f ? -1 : 0);
    }

    Vector3f Quaternionf::Transform(const Vector3f& v) {
        Quaternionf q(*this);

        q = q.Conjugate();
        q = (q * Quaternionf(v.x, v.y, v.z, 0.0f)) * (*this);
        // q = ((*this) * Quaternionf(v.x, v.y, v.z, 0.0f)) * q;

        return Vector3f(q.x, q.y, q.z);
    }

}    // namespace nkentseu