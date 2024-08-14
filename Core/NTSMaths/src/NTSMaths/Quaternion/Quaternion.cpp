//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 10:20:51 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Quaternion.h"

namespace nkentseu {

    namespace maths
    {
        quatf::quatf() : w(0.0f), x(0.0f), y(0.0f), z(0.0f) {
        }

        quatf::quatf(float32 value, bool identity) : w(value), x(identity ? 0.0f : value), y(identity ? 0.0f : value), z(identity ? 0.0f : value){
            epsi();
        }

        quatf::quatf(float32 x, float32 y, float32 z, float32 w) : w(w), x(x), y(y), z(z) {
            epsi();
        }

        quatf::quatf(float32 x, float32 y, float32 z) : w(1.0f), x(x), y(y), z(z) {
            epsi();
        }

        quatf::quatf(const Angle& angle, const Vector3f& axis) {
            Vector3f normalizedAxis = axis.Normalized();

            if (normalizedAxis.Len() == 0.0f) {
                *this = quatf(1.0f);
                return;
            }

            vector = normalizedAxis * maths::Sin(angle * 0.5f);
            scalar = maths::Cos(angle * 0.5f);
            epsi();
        }

        quatf::quatf(const EulerAngle& eulerAngle) {
            float32 cy = maths::Cos(eulerAngle.yaw * 0.5f);     // yaw (y)
            float32 sy = maths::Sin(eulerAngle.yaw * 0.5f);
            float32 cx = maths::Cos(eulerAngle.pitch * 0.5f);   // pitch (x)
            float32 sx = maths::Sin(eulerAngle.pitch * 0.5f);
            float32 cz = maths::Cos(eulerAngle.roll * 0.5f);    // roll (z)
            float32 sz = maths::Sin(eulerAngle.roll * 0.5f);

            // a revoir pour inverser le roll et le pitch

            w = cz * cy * cx + sz * sy * sx;
            x = cz * cy * sx - sz * sy * cx;
            y = cz * sy * cx + sz * cy * sx;
            z = sz * cy * cx - cz * sy * sx;
            epsi();
        }

        quatf::quatf(const Vector3f& vector, float32 scalar) : vector(vector), scalar(scalar){
            epsi();
        }

        quatf::quatf(const mat4f& mat) {
            Vector3f up = mat.up.xyz().Normalized();
            Vector3f forward = mat.forward.xyz().Normalized();
            Vector3f right = up.Cross(forward);
            up = forward.Cross(right);
            *this = LookAt(forward, up);
            epsi();
        }

        quatf::quatf(const quatf& quat) : quat(quat.quat){
            epsi();
        }

        quatf::quatf(const Vector4f& quat) : quat(quat) {
            epsi();
        }

        quatf::quatf(const Vector3f& from, const Vector3f& to) {
            Vector3f f = from.Normalized();
            Vector3f t = to.Normalized();

            if (f == t) {
                return;
            }

            if (f == t * -1.0f) {
                Vector3f ortho = Vector3f(1, 0, 0);

                if (maths::Abs(f.y) < maths::Abs(f.x)) {
                    ortho = Vector3f(0, 1, 0);
                }

                if (maths::Abs(f.z) < maths::Abs(f.y) && maths::Abs(f.z) < maths::Abs(f.x)) {
                    ortho = Vector3f(0, 0, 1);
                }

                vector = f.Cross(ortho).Normalized();
                scalar = 0.0f;
            }
            else {

                Vector3f half = (f + t).Normalized();
                Vector3f axis = f.Cross(half);

                vector = axis;
                scalar = f.Dot(half);
            }
            epsi();
        }

        quatf& quatf::operator=(const quatf& other)
        {
            w = other.w;
            x = other.x;
            y = other.y;
            z = other.z;
            epsi();
            return *this;
        }

        quatf quatf::operator+(const quatf& right)
        {
            return quatf(x + right.x, y + right.y, z + right.z, w + right.w);
        }

        quatf& quatf::operator+=(const quatf& right)
        {
            *this = *this + right;
            epsi();
            return *this;
        }

        quatf quatf::operator-(const quatf& right)
        {
            return quatf(x - right.x, y - right.y, z - right.z, w - right.w);
        }

        quatf& quatf::operator-=(const quatf& right)
        {
            *this = *this - right;
            epsi();
            return *this;
        }

        quatf quatf::operator*(const quatf& right)
        {
            return quatf(
                right.x * w + right.y * z - right.z * y + right.w * x,
                -right.x * z + right.y * w + right.z * x + right.w * y,
                right.x * y - right.y * x + right.z * w + right.w * z,
                -right.x * x - right.y * y - right.z * z + right.w * w
            );
        }

        quatf quatf::operator*(float32 right)
        {
            return quatf(x * right, y * right, z * right, w * right);
        }

        quatf quatf::operator/(float32 right)
        {
            return quatf(x / right, y / right, z / right, w / right);
        }

        quatf& quatf::operator*=(const quatf& right)
        {
            *this = *this * right;
            epsi();
            return *this;
        }

        quatf operator*(float32 left, const quatf& right) {
            return quatf(right.x * left, right.y * left, right.z * left, right.w * left);
        }

        quatf operator*(const quatf& left, float32 right)
        {
            return quatf(left.x * right, left.y * right, left.z * right, left.w * right);
        }

        Vector3f operator*(const Vector3f& left, const quatf& right) {
            return right * left;
        }

        Vector3f operator*(const quatf& left, const Vector3f& right) {
            return left.vector * 2.0f * left.vector.Dot(right) + 
                   right * (left.scalar * left.scalar - left.vector.Dot(left.vector)) + 
                   left.vector.Cross(right) * 2.0f * left.scalar;
        }

        quatf operator+(const quatf& a, const quatf& b)
        {
            quatf q(a);
            q += b;
            return q;
        }

        quatf operator-(const quatf& a, const quatf& b)
        {
            quatf q(a);
            q -= b;
            return q;
        }

        bool operator==(const quatf& left, const quatf& right) {
            return left.quat == right.quat;
        }

        bool operator!=(const quatf& a, const quatf& b) {
            return !(a == b);
        }

        quatf operator^(const quatf& q, float32 f)
        {
            Angle angle = (2.0f * maths::ACos(q.scalar)).Rad() * 0.5f * f;
            Vector3f axis = Vector3f(q.vector).Normalized();
            float32 halfCos = maths::Cos(angle);
            float32 halfSin = maths::Sin(angle);
            return quatf(axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos);
        }

        quatf::operator mat4f()
        {
            /*quatf unitQuat = Normalized();
            Vector3f r = *this * Vector3f(1, 0, 0);
            Vector3f u = *this * Vector3f(0, 1, 0);
            Vector3f f = *this * Vector3f(0, 0, 1);
            return mat4f(r, u, f);
            */
            float32 xx = x * x;
            float32 yy = y * y;
            float32 zz = z * z;
            float32 xy = x * y;
            float32 xz = x * z;
            float32 yz = y * z;
            float32 wx = w * x;
            float32 wy = w * y;
            float32 wz = w * z;

            return mat4f(
                1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),        2.0f * (xz + wy),        0.0f,
                2.0f * (xy + wz),        1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),        0.0f,
                2.0f * (xz - wy),        2.0f * (yz + wx),        1.0f - 2.0f * (xx + yy), 0.0f,
                0.0f,                    0.0f,                    0.0f,                    1.0f
            );
        }

        quatf::operator EulerAngle()
        {
            Angle ax, ay, az;

            quatf norm = Normalized();
            //*
            EulerAngle angles;

            float sinp = 2 * (norm.w * norm.y - norm.z * norm.x);
            if (maths::Abs(sinp) >= 1) {
                // Utilisation de PI/2 en cas de singularité
                angles.roll = maths::Copysign(maths::Pi / 2.0f, sinp);
                // En cas de singularité, on peut fixer roll à 0 et calculer pitch + roll
                angles.yaw = 0;
                angles.pitch = maths::ATan(norm.x, norm.w);
                std::cout << "singulariter\n";
            }
            else {
                angles.roll = maths::ASin(sinp);

                float siny_cosp = 2 * (norm.w * norm.z + norm.x * norm.y);
                float cosy_cosp = 1 - 2 * (norm.y * norm.y + norm.z * norm.z);
                angles.yaw = maths::ATan(siny_cosp, cosy_cosp);// -Angle(90.0f);

                // Calcul des termes intermédiaires
                float32 sinr_cosp = 2 * (norm.w * norm.x + norm.y * norm.z);
                float32 cosr_cosp = 1 - 2 * (norm.x * norm.x + norm.y * norm.y);
                angles.pitch = maths::ATan(sinr_cosp, cosr_cosp);// -Angle(90.0f);
            }

            return angles;
            /*/ x axis
            float32 sinx = 2.0f * (norm.w * norm.x + norm.y * norm.z);
            float32 cosx = 1.0f - 2.0f * (norm.x * norm.x + norm.y * norm.y);
            ax = maths::ATan(sinx, cosx);

            // z axis
            float32 sinz = 2.0f * (norm.w * norm.z + norm.y * norm.x);
            float32 cosz = 1.0f - 2.0f * (norm.z * norm.z + norm.y * norm.y);
            az = maths::ATan(sinz, cosz);

            // y axis
            //float32 siny = maths::Sqrt(1.0f + 2.0f * (norm.w * norm.y - norm.z * norm.x));
            //float32 cosy = maths::Sqrt(1.0f - 2.0f * (norm.w * norm.y - norm.z * norm.x));
            //ay = 2.0f * maths::ATan(siny, cosy) - Angle(90.0f);

            /*float32 siny = 2.0f * (norm.w * norm.y - norm.z * norm.x);
            if (fabs(siny) >= 1.0f) {
                ay = siny > 0 ? Angle(90.0f) : Angle(-90.0f);
                //ay = maths::Copysign(maths::Pis2, siny);
            }
            else {
                ay = maths::ASin(siny);
            }*/

            //return EulerAngle(ax, ay, az);
        }

        void quatf::epsi() {
            if (maths::Abs(x) <= maths::Epsilon) x = 0.0f;
            if (maths::Abs(y) <= maths::Epsilon) y = 0.0f;
            if (maths::Abs(z) <= maths::Epsilon) z = 0.0f;
            if (maths::Abs(w) <= maths::Epsilon) w = 0.0f;
        }

        quatf quatf::LookAt(const Vector3f& direction, const Vector3f& up)
        {
            // Find orthonormal basis vectors
            Vector3f f = direction.Normalized(); // Object Forward
            Vector3f u = up.Normalized(); // Desired Up
            Vector3f r = u.Cross(f); // Object Right
            u = f.Cross(r); // Object Up
            // From world forward to object forward
            quatf worldToObject(Vector3f(0, 0, 1), f);
            // what direction is the new object up ?
            Vector3f objectUp = worldToObject * Vector3f(0, 1, 0);
            // From object up to desired up
            quatf u2u(objectUp, u);
            // Rotate to forward direction first
            // then twist to correct up
            quatf result = worldToObject * u2u;
            // Don't forget to normalize the result
            return result.Normalized();
        }

        quatf quatf::LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up)
        {
            return LookAt(target - position, up);
        }

        float32 quatf::Dot(const quatf& right)
        {
            return x * right.x + y * right.y + z * right.z + w * right.w;
        }

        float32 quatf::LenSq()
        {
            return x * x + y * y + z * z + w * w;
        }

        float32 quatf::Len()
        {
            float32 lenSq = x * x + y * y + z * z + w * w;
            if (lenSq < maths::QuatEpsilon) {
                return 0.0f;
            }
            return maths::Sqrt(lenSq);
        }

        void quatf::Normalize()
        {
            float32 lenSq = x * x + y * y + z * z + w * w;
            if (lenSq < maths::QuatEpsilon) {
                return;
            }
            w /= maths::Sqrt(lenSq);
            x /= maths::Sqrt(lenSq);
            y /= maths::Sqrt(lenSq);
            z /= maths::Sqrt(lenSq);
            epsi();
        }

        quatf quatf::Normalized() const
        {
            quatf quat = *this;
            quat.Normalize();
            return quat;
        }

        quatf quatf::Conjugate()
        {
            return quatf(-x, -y, -z, w);
        }

        quatf quatf::Inverse(bool normalize)
        {
            quatf normal(*this);

            if (normalize) normal.Normalize();

            float32 lenSq = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z + normal.w * normal.w;
            if (lenSq < maths::QuatEpsilon) {
                return quatf();
            }
            float32 recip = 1.0f / lenSq;
            return quatf(-normal.x * recip, -normal.y * recip, -normal.z * recip, normal.w * recip);
        }

        int32 quatf::GimbalPole() const {
            float32 t = y * x + z * w;
            return t > 0.499f ? 1 : t < -0.499f ? -1 : 0;
        }

        quatf quatf::Reflection(const Vector3f& normal)
        {
            return quatf();
        }

        bool quatf::IsNormalize()
        {
            return maths::Abs(Len() - 1) < 2.0f * maths::Epsilon;
        }

        Vector3f quatf::Axis() const
        {
            if (maths::Abs(scalar) <= 1.0f + maths::Epsilon && maths::Abs(scalar) >= 1.0f - maths::Epsilon) return Vector3f();

            Angle alpha = Angles();

            float32 sin = maths::Sin(alpha * 0.5f);
            quatf q = Normalized();

            return (q * (1.0f / sin)).vector;
        }

        Angle quatf::Angles() const
        {
            return Angle(2.0f * acos(w));
        }

        quatf quatf::Mix(const quatf& to, float32 t)
        {
            return  (*this) * (1.0f - t) + to * t;
        }

        quatf quatf::Lerp(const quatf& e, float32 t)
        {
            return Mix(e, t);
        }

        quatf quatf::NLerp(const quatf& to, float t)
        {
            return Mix(to, t).Normalized();
        }

        quatf quatf::SLerp(const quatf& end, float32 t)
        {
            if (maths::Abs(Dot(end)) > 1.0f - maths::QuatEpsilon) {
                return NLerp(end, t);
            }
            quatf delta = Inverse() * end;
            return ((delta ^ t) * (*this)).Normalized();
        }

        /*Quaternionf::Quaternionf() : x(0), y(0), z(0), w(1) {}
        Quaternionf::Quaternionf(float32 _x, float32 _y, float32 _z, float32 _w) : x(_x), y(_y), z(_z), w(_w) {}
        Quaternionf::Quaternionf(const Quaternionf& quat) : quat(quat.quat) {}
        Quaternionf::Quaternionf(const matrix4f& m)
        {
            Vector3f up = m.up.xyz().Normalized();
            Vector3f forward = m.forward.xyz().Normalized();
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
            return (maths::Abs(left.x - right.x) <= maths::QuatEpsilon && maths::Abs(left.y - right.y) <= maths::QuatEpsilon &&
                maths::Abs(left.z - right.z) <= maths::QuatEpsilon && maths::Abs(left.w - right.w) <= maths::QuatEpsilon);
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
            if (lenSq < maths::QuatEpsilon) {
                return 0.0f;
            }
            return maths::Sqrt(lenSq);
        }

        void Quaternionf::Normalize() {
            float32 lenSq = x * x + y * y + z * z + w * w;
            if (lenSq < maths::QuatEpsilon) {
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
            if (lenSq < maths::QuatEpsilon) {
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
            if (lenSq < maths::QuatEpsilon) {
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

        Quaternionf Quaternionf::Nlerp(const Quaternionf& to, float32 t) {
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
            if (maths::Abs(Dot(end)) > 1.0f - maths::QuatEpsilon) {
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

        matrix4f Quaternionf::mat4() const { return ToMat4(); }
        matrix4f Quaternionf::mat4() { return ToMat4(); }

        matrix4f Quaternionf::ToMat4() const {
            /*
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
            * /

            Vector3f r = *this * Vector3f(1, 0, 0);
            Vector3f u = *this * Vector3f(0, 1, 0);
            Vector3f f = *this * Vector3f(0, 0, 1);
            return mat4f(r, u, f);
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
                pitch = Angle::FromRadian(((float32)pole * maths::Pi * 0.5f));
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

        // quatf
        /* / Constructeurs
        quatf::quatf() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

        quatf::quatf(float32 value, bool identity)
        {
            if (identity) {
                x = y = z = 0.0f;
                w = 1.0f;
            }
            else {
                x = y = z = w = value;
                normalize();
            }
        }

        quatf::quatf(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

        quatf::quatf(const Vector3f& axis, const Angle& angle) {
            Vector3f normalizedAxis = axis.Normalized();
            vector = normalizedAxis * maths::Sin(angle * 0.5f);
            scalar = maths::Cos(angle * 0.5f);
        }

        quatf::quatf(const Angle& pitch, const Angle& yaw, const Angle& roll)
        {
            *this = fromEuler(pitch, yaw, roll);
        }

        quatf::quatf(const EulerAngle& eulerAngles)
        {
            *this = fromEuler(eulerAngles.pitch, eulerAngles.yaw, eulerAngles.roll);
        }

        quatf::quatf(const Vector3f& vector) : vector(vector), scalar(1.0f){
        }

        quatf::quatf(const Vector3f& vector, float32 scalar) : vector(vector), scalar(scalar)
        {
        }

        quatf::quatf(const mat4f& mat) {
            *this = fromMatrix(mat);
        }

        quatf::quatf(const quatf& quat) : x(quat.x), y(quat.y), z(quat.z), w(quat.w) {}

        quatf::quatf(const Vector3f& from, const Vector3f& to) {
            Vector3f f = from.Normalized();
            Vector3f t = to.Normalized();

            if (f == t) {
                return;
            }

            if (f == t * -1.0f) {
                Vector3f ortho = Vector3f(1, 0, 0);

                if (maths::Abs(f.y) < maths::Abs(f.x)) {
                    ortho = Vector3f(0, 1, 0);
                }
                
                if (maths::Abs(f.z) < maths::Abs(f.y) && maths::Abs(f.z) < maths::Abs(f.x)) {
                    ortho = Vector3f(0, 0, 1);
                }

                vector = f.Cross(ortho).Normalized();
                scalar = 0.0f;
                return;
            }

            /*Vector3f c = f.Cross(t);
            float32 dot = f.Dot(t);

            x = c.x;
            y = c.y;
            z = c.z;
            w = dot + maths::Sqrt(f.LenSq() * t.LenSq());

            normalize();* /
            Vector3f half = (f + t).Normalized();
            Vector3f axis = f.Cross(half);

            vector = axis;
            scalar = f.Dot(half);
        }

        quatf& quatf::operator=(const quatf& auther)
        {
            this->quat = auther.quat;
            return *this;
        }

        /*quatf::operator mat4f() const {
            return toMatrix();
        }* /

        quatf::operator float32* () {
            return ptr;
        }

        float32& quatf::operator[] (int32 index) {
            return ptr[index];
        }

        const float32& quatf::operator[](size_t index) const {
            return ptr[index];
        }

        // Opérateurs
        quatf quatf::operator*(const quatf& quat) const {
            return quatf(
                quat.x * w + quat.y * z - quat.z * y + quat.w * x,
                -quat.x * z + quat.y * w + quat.z * x + quat.w * y,
                quat.x * y - quat.y * x + quat.z * w + quat.w * z,
                -quat.x * x - quat.y * y - quat.z * z + quat.w * w
            );
            /* /
            return quatf(
                w * quat.x + x * quat.w + y * quat.z - z * quat.y,
                w * quat.y - x * quat.z + y * quat.w + z * quat.x,
                w * quat.z + x * quat.y - y * quat.x + z * quat.w,
                w * quat.w - x * quat.x - y * quat.y - z * quat.z
            );
            /* /
            quatf q;
            q.vector = vector * quat.scalar + quat.vector * scalar + quat.vector.Cross(vector);
            q.scalar = scalar * quat.scalar - quat.vector.Dot(vector);
            return q;
            /* /
        }

        quatf quatf::operator*(float32 scale) const
        {
            return quatf(x * scale, y * scale, z * scale, w * scale);
        }

        Vector3f quatf::operator*(const Vector3f& v) const {
            /*Vector3f qv(x, y, z);
            Vector3f uv = qv.Cross(v);
            Vector3f uuv = qv.Cross(uv);
            uv = uv * (2.0f * w);
            uuv = uuv * 2.0f;

            return v + uv + uuv;* /
            return vector * 2.0f * vector.Dot(v) + v * (scalar * scalar - vector.Dot(vector)) + vector.Cross(v) * 2.0f * scalar;
        }

        quatf quatf::operator-(const quatf& quat) {
            return quatf(x - quat.x, y - quat.y, z - quat.z, w - quat.w);
        }

        quatf quatf::operator+(const quatf& quat) {
            return quatf(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
        }

        //Vector3f operator*(const Vector3f& v, const quatf& quat) {
        //    return quat * v;
        //}

        quatf operator*(float32 scale, const quatf& quat)
        {
            return quat * scale;
        }

        bool operator==(const quatf& left, const quatf& right) {
            return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
        }

        bool operator!=(const quatf& left, const quatf& right) {
            return !(left == right);
        }

        // Méthodes
        void quatf::normalize() {
            float32 length = len();
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }

        quatf quatf::normalized() const {
            quatf result = *this;
            result.normalize();
            return result;
        }

        quatf quatf::conjugate() {
            return quatf(-x, -y, -z, w);
        }

        quatf quatf::inverse(bool normalize) {
            if (!normalize)
                return conjugate() * (1.0f / lenSq());
            return normalized().conjugate();
        }

        quatf quatf::cross(const quatf& quat) {
            return *this * quat;
        }

        quatf quatf::fromEuler(const EulerAngle& eulerAngles)
        {
            return fromEuler(eulerAngles.pitch, eulerAngles.yaw, eulerAngles.roll);
        }

        quatf quatf::fromEuler(const Angle& pitch, const Angle& yaw, const Angle& roll) {
            /*float32 cy = maths::Cos(Angle(yaw * 0.5f)); // yaw (y)
            float32 sy = maths::Sin(Angle(yaw * 0.5f));
            float32 cp = maths::Cos(Angle(pitch * 0.5f)); // pitch (x)
            float32 sp = maths::Sin(Angle(pitch * 0.5f));
            float32 cr = maths::Cos(Angle(roll * 0.5f)); // roll (z)
            float32 sr = maths::Sin(Angle(roll * 0.5f));

            quatf quat;
            quat.w = cr * cp * cy + sr * sp * sy;
            quat.x = sr * cp * cy - cr * sp * sy;
            quat.y = cr * sp * cy + sr * cp * sy;
            quat.z = cr * cp * sy - sr * sp * cy;

            float32 cy = maths::Cos(Angle(roll * 0.5f));
            float32 sy = maths::Sin(Angle(roll * 0.5f));
            float32 cp = maths::Cos(Angle(yaw * 0.5f));
            float32 sp = maths::Sin(Angle(yaw * 0.5f));
            float32 cr = maths::Cos(Angle(pitch * 0.5f));
            float32 sr = maths::Sin(Angle(pitch * 0.5f));

            float32 cy = maths::Cos(Angle(yaw * 0.5f)); // yaw (y)
            float32 sy = maths::Sin(Angle(yaw * 0.5f));
            float32 cr = maths::Cos(Angle(pitch * 0.5f)); // pitch (x)
            float32 sr = maths::Sin(Angle(pitch * 0.5f));
            float32 cp = maths::Cos(Angle(roll * 0.5f)); // roll (z)
            float32 sp = maths::Sin(Angle(roll * 0.5f));

            float32 cp = maths::Cos(Angle(yaw * 0.5f)); // yaw (y)
            float32 sp = maths::Sin(Angle(yaw * 0.5f));
            float32 cy = maths::Cos(Angle(pitch * 0.5f)); // pitch (x)
            float32 sy = maths::Sin(Angle(pitch * 0.5f));
            float32 cr = maths::Cos(Angle(roll * 0.5f)); // roll (z)
            float32 sr = maths::Sin(Angle(roll * 0.5f));* /

            float32 cp = maths::Cos(Angle(yaw * 0.5f)); // yaw (y)
            float32 sp = maths::Sin(Angle(yaw * 0.5f));
            float32 cr = maths::Cos(Angle(pitch * 0.5f)); // pitch (x)
            float32 sr = maths::Sin(Angle(pitch * 0.5f));
            float32 cy = maths::Cos(Angle(roll * 0.5f)); // roll (z)
            float32 sy = maths::Sin(Angle(roll * 0.5f));

            quatf quat;
            quat.w = cr * cp * cy + sr * sp * sy;
            quat.x = sr * cp * cy - cr * sp * sy;
            quat.y = cr * sp * cy + sr * cp * sy;
            quat.z = cr * cp * sy - sr * sp * cy;
            float32 cy = maths::Cos(Angle(yaw * 0.5f)); // yaw (y)
            float32 sy = maths::Sin(Angle(yaw * 0.5f));
            float32 cp = maths::Cos(Angle(pitch * 0.5f)); // pitch (x)
            float32 sp = maths::Sin(Angle(pitch * 0.5f));
            float32 cr = maths::Cos(Angle(roll * 0.5f)); // roll (z)
            float32 sr = maths::Sin(Angle(roll * 0.5f));
            quatf quat;
            quat.w = cr * cp * cy + sr * sp * sy;
            quat.x = sr * cp * cy - cr * sp * sy;
            quat.y = cr * sp * cy + sr * cp * sy;
            quat.z = cr * cp * sy - sr * sp * cy;
            return quat;* /

            float32 cy = maths::Cos(Angle(yaw * 0.5f)); // yaw (y)
            float32 sy = maths::Sin(Angle(yaw * 0.5f));
            float32 cp = maths::Cos(Angle(pitch * 0.5f)); // pitch (x)
            float32 sp = maths::Sin(Angle(pitch * 0.5f));
            float32 cr = maths::Cos(Angle(roll * 0.5f)); // roll (z)
            float32 sr = maths::Sin(Angle(roll * 0.5f));

            return quatf(
                sr * cp * cy - sp * sy * cr,
                sp * sr * cy + sy * cp * cr,
                sp * cy * cr - sy * sr * cp,
                sp * sy * sr + cp * cy * cr);
        }

        mat4f quatf::toMatrix() const {

            Vector3f r = *this * Vector3f(1, 0, 0);
            Vector3f u = *this * Vector3f(0, 1, 0);
            Vector3f f = *this * Vector3f(0, 0, 1);
            return mat4f(r, u, f);
            /*mat4f result(1.0f);

            float32 xx = x * x;
            float32 yy = y * y;
            float32 zz = z * z;
            float32 xy = x * y;
            float32 xz = x * z;
            float32 yz = y * z;
            float32 wx = w * x;
            float32 wy = w * y;
            float32 wz = w * z;

            return mat4f(
                            1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz),        2.0f * (xz + wy),        0.0f,
                            2.0f * (xy + wz),        1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx),        0.0f,
                            2.0f * (xz - wy),        2.0f * (yz + wx),        1.0f - 2.0f * (xx + yy), 0.0f,
                            0.0f,                    0.0f,                    0.0f,                    1.0f
                        );* /
        }
        /*float32 trace = m00 + m11 + m22;
        if (trace > 0.0f) {
            float32 s = sqrt(trace + 1.0f) * 2.0f;
            q.w = 0.25f * s;
            q.x = (m21 - m12) / s;
            q.y = (m02 - m20) / s;
            q.z = (m10 - m01) / s;
        }
        else if ((m00 > m11) && (m00 > m22)) {
            float32 s = sqrt(1.0f + m00 - m11 - m22) * 2.0f;
            q.w = (m21 - m12) / s;
            q.x = 0.25f * s;
            q.y = (m01 + m10) / s;
            q.z = (m02 + m20) / s;
        }
        else if (m11 > m22) {
            float32 s = sqrt(1.0f + m11 - m00 - m22) * 2.0f;
            q.w = (m02 - m20) / s;
            q.x = (m01 + m10) / s;
            q.y = 0.25f * s;
            q.z = (m12 + m21) / s;
        }
        else {
            float32 s = sqrt(1.0f + m22 - m00 - m11) * 2.0f;
            q.w = (m10 - m01) / s;
            q.x = (m02 + m20) / s;
            q.y = (m12 + m21) / s;
            q.z = 0.25f * s;
        }

        quatf quatf::fromMatrix(const mat4f& mat) {
            quatf q;

            q.w = maths::Sqrt(maths::Abs(1 + m00 + m11 + m22) * 0.25f);
            q.x = maths::Sqrt(maths::Abs(1 + m00 - m11 - m22) * 0.25f);
            q.y = maths::Sqrt(maths::Abs(1 - m00 + m11 - m22) * 0.25f);
            q.z = maths::Sqrt(maths::Abs(1 - m00 - m11 + m22) * 0.25f);

            if (q.w > q.x && q.w > q.y && q.w > q.z) {
                float32 a = 0.25f / q.w;
                q.x = (m12 - m21) * a;
                q.y = (m20 - m02) * a;
                q.z = (m01 - m10) * a;
            } else if (q.x > q.y && q.x > q.z && q.x > q.w) {
                float32 a = 0.25f / q.x;
                q.w = (m12 - m21) * a;
                q.y = (m10 + m01) * a;
                q.z = (m20 + m02) * a;
            } else if (q.y > q.x && q.y > q.z && q.y > q.w) {
                float32 a = 0.25f / q.y;
                q.w = (m20 - m02) * a;
                q.x = (m10 + m01) * a;
                q.z = (m21 + m11) * a;
            } else if (q.z > q.x && q.z > q.y && q.z > q.w) {
                float32 a = 0.25f / q.z;
                q.w = (m01 - m10) * a;
                q.x = (m20 + m02) * a;
                q.y = (m21 + m12) * a;
            }

            return q;
        }* /

        quatf quatf::fromMatrix(const mat4f& mat, bool addScale) {
            mat4f rotationMatrix = addScale ? mat : mat4f(1.0f);

            if (!addScale) {
                rotationMatrix.right = mat.right.xyz().Normalized();
                rotationMatrix.up = mat.up.xyz().Normalized();
                rotationMatrix.forward = mat.forward.xyz().Normalized();
            }

            float32 m00 = rotationMatrix.data[0], m01 = rotationMatrix.data[4], m02 = rotationMatrix.data[8];
            float32 m10 = rotationMatrix.data[1], m11 = rotationMatrix.data[5], m12 = rotationMatrix.data[9];
            float32 m20 = rotationMatrix.data[2], m21 = rotationMatrix.data[6], m22 = rotationMatrix.data[10];
            
            /*
            // calcul du quaternion avec la methode naive
            quatf q;
            q.w = std::sqrt(1.0f + m00 + m11 + m22) / 2.0f;
            float32 w4 = (4.0f * q.w);
            q.x = (m21 - m12) / w4;
            q.y = (m02 - m20) / w4;
            q.z = (m10 - m01) / w4;

            return q;*/

            // determine le quaternion avec la methode alternative plus sur et securiser
            // Calcul de la trace de la matrice
            /*float32 trace = rotationMatrix.m00 + rotationMatrix.m11 + rotationMatrix.m22;

            quatf q;
            if (trace > 0.0f) {
                float32 s = 0.5f / std::sqrt(trace + 1.0f);
                q.w = 0.25f / s;
                q.x = (rotationMatrix.m21 - rotationMatrix.m12) * s;
                q.y = (rotationMatrix.m02 - rotationMatrix.m20) * s;
                q.z = (rotationMatrix.m10 - rotationMatrix.m01) * s;
            }
            else {
                if (rotationMatrix.m00 > rotationMatrix.m11 && rotationMatrix.m00 > rotationMatrix.m22) {
                    float32 s = 2.0f * std::sqrt(1.0f + rotationMatrix.m00 - rotationMatrix.m11 - rotationMatrix.m22);
                    q.w = (rotationMatrix.m21 - rotationMatrix.m12) / s;
                    q.x = 0.25f * s;
                    q.y = (rotationMatrix.m01 + rotationMatrix.m10) / s;
                    q.z = (rotationMatrix.m02 + rotationMatrix.m20) / s;
                }
                else if (rotationMatrix.m11 > rotationMatrix.m22) {
                    float32 s = 2.0f * std::sqrt(1.0f + rotationMatrix.m11 - rotationMatrix.m00 - rotationMatrix.m22);
                    q.w = (rotationMatrix.m02 - rotationMatrix.m20) / s;
                    q.x = (rotationMatrix.m01 + rotationMatrix.m10) / s;
                    q.y = 0.25f * s;
                    q.z = (rotationMatrix.m12 + rotationMatrix.m21) / s;
                }
                else {
                    float32 s = 2.0f * std::sqrt(1.0f + rotationMatrix.m22 - rotationMatrix.m00 - rotationMatrix.m11);
                    q.w = (rotationMatrix.m10 - rotationMatrix.m01) / s;
                    q.x = (rotationMatrix.m02 + rotationMatrix.m20) / s;
                    q.y = (rotationMatrix.m12 + rotationMatrix.m21) / s;
                    q.z = 0.25f * s;
                }
            }* /

            Vector3f up = rotationMatrix.up.xyz();
            Vector3f forward = rotationMatrix.forward.xyz();
            Vector3f right = up.Cross(forward);
            up = forward.Cross(right);
            return quatf::lookAt(forward, up);

            //return q;
        }

        quatf quatf::reflection(const Vector3f& normal)
        {
            quatf quat;
            quat.vector = normal.Normalized();
            quat.scalar = 0.0f;
            return quat;
        }

        bool quatf::isNormalize()
        {
            return maths::Abs(len() - 1) < 2.0f * maths::Epsilon;
        }

        float32 quatf::dot(const quatf& b) {
            return x * b.x + y * b.y + z * b.z + w * b.w;
        }

        float32 quatf::lenSq() {
            return x * x + y * y + z * z + w * w;
        }

        float32 quatf::len() {
            return sqrt(lenSq());
        }

        Vector3f quatf::axis() const {
            //quatf q = *this;
            //q.normalize();

            /*if (maths::Abs(q.scalar) <= 1.0f + maths::Epsilon && maths::Abs(q.scalar) >= 1.0f - maths::Epsilon) return Vector3f();
            return q.vector / maths::Sqrt(1 - q.scalar * q.scalar);* /
            if (maths::Abs(scalar) <= 1.0f + maths::Epsilon && maths::Abs(scalar) >= 1.0f - maths::Epsilon) return Vector3f();
            Angle alpha = angle();
            float32 sin = maths::Sin(alpha * 0.5f);
            return (normalized() * (1.0f / sin)).vector;
        }

        Angle quatf::angle() const {
            return Angle(2.0f * acos(w));
        }

        quatf quatf::euler(const EulerAngle& angle)
        {
            return fromEuler(angle);
        }

        quatf quatf::euler(const Angle& pitch, const Angle& yaw, const Angle& roll) {
            return fromEuler(pitch, yaw, roll);
        }

        quatf quatf::mix(const quatf& to, float32 t) {
            return *this * (1.0f - t) + to * t;
        }

        quatf quatf::lerp(const quatf& e, float32 t) {
            return mix(e, t);
        }

        quatf quatf::nlerp(const quatf& to, float32 t) {
            return mix(to, t).normalized();
        }

        quatf operator^(const quatf& q, float32 f) {
            Angle angle = (2.0f * maths::ACos(q.scalar)).Rad() * 0.5f * f;
            Vector3f axis = Vector3f(q.vector).Normalized();
            float32 halfCos = maths::Cos(angle);
            float32 halfSin = maths::Sin(angle);
            return quatf(axis.x * halfSin, axis.y * halfSin, axis.z * halfSin, halfCos);
        }

        quatf quatf::slerp(const quatf& end, float32 t) {
            float32 cosTheta = dot(end);
            quatf endCopy = end;

            if (cosTheta < 0.0f) {
                endCopy = quatf(-end.x, -end.y, -end.z, -end.w);
                cosTheta = -cosTheta;
            }

            if (cosTheta > 0.95f) {
                return nlerp(endCopy, t);
            }

            Angle angle = maths::ACos(cosTheta);
            return (*this * maths::Sin((1 - t) * angle) + endCopy * maths::Sin(t * angle)) * (1.0f / maths::Sin(angle));
        }

        quatf quatf::lookAt(const Vector3f& eye, const Vector3f& look, const Vector3f& up, const Vector3f& right) {
            mat4f viewMatrix;// = mat4f::LookAt(eye, look, up, right);
            return fromMatrix(viewMatrix);
        }

        quatf quatf::lookAt(const Vector3f& direction, const Vector3f& up) {
            //mat4f viewMatrix;// = mat4f::LookAt(direction, up);
            /* /return fromMatrix(viewMatrix);
            // Find orthonormal basis vectors
            Vector3f f = direction.Normalized(); // Object Forward
            Vector3f u = up.Normalized(); // Desired Up
            Vector3f r = u.Cross(f); // Object Right
            u = f.Cross(r); // Object Up
            // From world forward to object forward
            quatf worldToObject = quatf(Vector3f(0, 0, 1), f);
            // what direction is the new object up ? 
            Vector3f objectUp = worldToObject * Vector3f(0, 1, 0);
            // From object up to desired up
            quatf u2u = quatf(objectUp, u);
            // Rotate to forward direction first
            // then twist to correct up
            quatf result = worldToObject * u2u;
            // Don't forget to normalize the result
            return result.normalized();* /

            // Find orthonormal basis vectors
            Vector3f f = direction.Normalized(); // Object Forward
            Vector3f u = up.Normalized(); // Desired Up
            Vector3f r = u.Cross(f); // Object Right
            u = f.Cross(r); // Object Up
            // From world forward to object forward
            quatf worldToObject = quatf(Vector3f(0, 0, 1), f);
            // what direction is the new object up ?
            Vector3f objectUp = worldToObject * Vector3f(0, 1, 0);
            // From object up to desired up
            quatf u2u = quatf(objectUp, u);
            // Rotate to forward direction first
            // then twist to correct up
            quatf result = worldToObject * u2u;
            // Don't forget to normalize the result
            return result.normalized();
        }

        quatf quatf::lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up) {
            //mat4f viewMatrix;// = mat4f::LookAt(position, target, up);
            return lookAt(target - position, up);
        }

        quatf quatf::identity() {
            return quatf(0.0f, 0.0f, 0.0f, 1.0f);
        }

        EulerAngle quatf::eulerAngle2() const {
            EulerAngle angles;

            float32 sinr_cosp = 2 * (w * x + y * z);
            float32 cosr_cosp = 1 - 2 * (x * x + y * y);
            angles.pitch = maths::ATan(sinr_cosp, cosr_cosp);

            float32 sinp = 2 * (w * y - z * x);
            if (maths::Abs(sinp) >= 1)
                angles.yaw = maths::Copysign(maths::Pi / 2, sinp);
            else
                angles.yaw = maths::ASin(sinp);

            float32 siny_cosp = 2 * (w * z + x * y);
            float32 cosy_cosp = 1 - 2 * (y * y + z * z);
            angles.roll = maths::ATan(siny_cosp, cosy_cosp);

            return angles;
        }

        EulerAngle quatf::eulerAngle6() const
        {
            EulerAngle angle;

            // Pitch (x-axis rotation)
            float32 sinp = 2 * (w * x - y * z);
            angle.pitch;
            if (std::abs(sinp) >= 1)
                angle.pitch = maths::Copysign(maths::Pi / 2.0f, sinp); // use 90 degrees if out of range
            else
                angle.pitch = maths::ASin(sinp);

            // Yaw (y-axis rotation)
            float32 siny_cosp = 2 * (w * y + z * x);
            float32 cosy_cosp = 1 - 2 * (x * x + y * y);
            angle.yaw = maths::ATan(siny_cosp, cosy_cosp);

            // Roll (z-axis rotation)
            float32 sinr_cosp = 2 * (w * z + x * y);
            float32 cosr_cosp = 1 - 2 * (z * z + y * y);
            angle.roll = maths::ATan(sinr_cosp, cosr_cosp);

            if (angle.roll <= -180.0f || angle.pitch <= -180.0f) {
                angle.yaw += 180.0f;
                angle.roll += angle.roll <= -180.0f ? Angle(180) : Angle(0.0f);
                angle.pitch += angle.pitch <= -180.0f ? Angle(180) : Angle(0.0f);
            }
            if (angle.roll >= 180.0f || angle.pitch >= 180.0f) {
                angle.yaw -= 180.0f;
                angle.roll = angle.roll >= 180.0f ? Angle(-180) : Angle(0.0f);
                angle.pitch = angle.pitch >= 180.0f ? Angle(-180) : Angle(0.0f);
            }

            return angle;
        }

        EulerAngle quatf::eulerAngle3() const
        {
            //int32 pole = gimbalPole();

            EulerAngle euler;

            /*if (pole == 0) {
                euler.yaw = maths::ATan(2.0f * (y * w + x * z), 1.0f - 2.0f * (y * y + x * x)) + Angle(180.0f);
                euler.pitch = maths::ATan(2.0f * (w * z + y * x), 1.0f - 2.0f * (x * x + z * z));
                euler.roll = maths::ASin(maths::Clamp<float32>(2.0f * (w * x - z * y), -1.0f, 1.0f));
            }
            else {
                euler.pitch = (float32)pole * 2.0f * maths::ATan(y, w);
                euler.roll = Angle::FromRadian(((float32)pole * maths::Pi * 0.5f));
            }* /

            euler.yaw = maths::ASin(2.0f * (w * y - x * z));

            if (euler.yaw >= 89.0f && euler.yaw <= 90.0f) {
                euler.roll = 0.0f;
                euler.pitch = -2.0f * maths::ATan(x, w);
            }
            else if (euler.yaw >= -90.0f && euler.yaw <= -89.0f) {
                euler.roll = 0.0f;
                euler.pitch = 2.0f * maths::ATan(x, w);
            }
            else {
                euler.roll = maths::ATan(2.0f * (w * x + y * z), w * w - x * x - y * y + z * z);
                euler.pitch = maths::ATan(2.0f * (w * z + x * y), w * w + x * x - y * y - z * z);
            }

            if (euler.roll <= -180.0f || euler.pitch <= -180.0f) {
                euler.yaw += 180.0f;
                euler.roll += euler.roll <= -180.0f ? Angle(180) : Angle(0.0f);
                euler.pitch += euler.pitch <= -180.0f ? Angle(180) : Angle(0.0f);
            }
            if (euler.roll >= 180.0f || euler.pitch >= 180.0f) {
                euler.yaw -= 180.0f;
                euler.roll = euler.roll >= 180.0f ? Angle(-180) : Angle(0.0f);
                euler.pitch = euler.pitch >= 180.0f ? Angle(-180) : Angle(0.0f);
            }

            return euler;
        }

        EulerAngle quatf::eulerAngle4() const
        {
            EulerAngle angle;

            const float32 sy = float32(2) * ((w * y) - (z * x));

            angle.pitch = maths::ATan(float32(2) * ((w * x) + (y * z)), float32(1) - (float32(2) * ((x * x) + (y * y))));
            angle.yaw = maths::Abs(sy) >= float32(1) ? maths::Copysign(maths::Pi / float32(2), sy) : maths::ASin(sy);
            angle.pitch = maths::ATan(float32(2) * ((w * z) + (x * y)), float32(1) - (float32(2) * ((y * y) + (z * z))));

            return angle;
        }

        EulerAngle quatf::eulerAngle5() const
        {
            EulerAngle ea;
            int32 test = gimbalPole();

            if (test == 1)
            {
                ea.pitch = 2.f * maths::ATan(x, z);
                ea.yaw = maths::Pis2;
                ea.roll = 0.f;
                return ea;
            }
            else if (test == -1)
            {
                ea.pitch = -2.f * maths::ATan(x, z);
                ea.yaw = -maths::Pis2;
                ea.roll = 0.f;
                return ea;
            }

            float32 buf1_2 = x * x;
            float32 buf2_2 = y * y;
            float32 buf3_2 = z * z;

            ea.pitch = maths::ATan(2 * (z * x + y * z), 1 - 2 * (buf1_2 + buf2_2));

            float32 sinp = 2 * (z * y - x * z);
            if (maths::Abs(sinp) > 1)
            {
                ea.yaw = maths::Copysign(maths::Pis2, sinp);
            }
            else
            {
                ea.yaw = maths::ASin(sinp);
            }

            ea.roll = maths::ATan(2 * (z * z + x * y), 1 - 2 * (buf2_2 + buf3_2));
            return ea * 10.0f;
        }

        EulerAngle quatf::eulerAngle() const
        {
            /*EulerAngle eulerAngles;

            // Yaw (Y)
            eulerAngles.yaw = maths::ATan(2.0f * (w * y + x * z), 1.0f - 2.0f * (y * y + z * z));

            // Pitch (X)
            float sinp = 2.0f * (w * z - y * x);
            if (std::abs(sinp) >= 1.0f)
                eulerAngles.pitch = maths::Copysign(maths::Pi / 2.0f, sinp); // Gimbal lock
            else
                eulerAngles.pitch = maths::ASin(sinp);

            // Roll (Z)
            eulerAngles.roll = maths::ATan(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + z * z));

            return eulerAngles;*/

            //EulerAngle eulerAngles;

            // Calcul des angles d'Euler à partir du quaternion

            /*/ Calcul des angles d'Euler
            angle.roll = -maths::ASin(rotScale.m10);            // Rotation autour de Z

            if (std::abs(rotScale.m10) < 0.99999f) { // cos(pitch) != 0
                angle.yaw = maths::ATan(rotScale.m20, rotScale.m00); // Rotation autour de X
                angle.pitch = maths::ATan(rotScale.m12, rotScale.m11);   // Rotation autour de Y
            }
            else {
                // Cas où cos(pitch) est proche de ±1 (gimbal lock)
                // yaw peut être arbitrairement défini à 0
                angle.pitch = 0.0f;
                // Calcul de pitch en fonction de l'orientation de roll
                angle.yaw = maths::ATan(-rotScale.m21, rotScale.m22);
            }* /

            // Yaw (Y)
            float32 sinr_cosp = 2.0f * (w * x + y * z);
            float32 cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
            eulerAngles.yaw = maths::ATan(sinr_cosp, cosr_cosp);

            // Pitch (X)
            float32 sinp = 2.0f * (w * y - z * x);
            if (maths::Abs(sinp) >= 1.0f) {
                // Pitch (X)
                eulerAngles.pitch = maths::Copysign(maths::Pi / 2.0f, sinp); // Gimbal lock

                // Roll (Z)
                float32 siny_cosp = 2.0f * (w * z + x * y);
                float32 cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
                eulerAngles.roll = maths::ATan(siny_cosp, cosy_cosp);
            }
            else {
                // Roll (Z)
                eulerAngles.roll = -maths::ASin(sinp);

                // Pitch (X)
                eulerAngles.pitch = 0.0f;
            }

            return eulerAngles;* /
            EulerAngle eulerAngles;

            float32 sinr_cosp = 2.0f * (w * x + y * z);
            float32 cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
            eulerAngles.pitch = -maths::ATan(sinr_cosp, cosr_cosp);

            float32 sinp = 2.0f * (w * y - z * x);
            if (maths::Abs(sinp) >= 1.0f) {
                eulerAngles.yaw = maths::Copysign(maths::Pi / 2.0f, sinp); // Gimbal lock
                std::cout << "gimbal lock\n";   
            }
            else {
                eulerAngles.yaw = maths::ASin(sinp);
            }

            float32 siny_cosp = 2.0f * (w * z + x * y);
            float32 cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
            eulerAngles.roll = maths::ATan(siny_cosp, cosy_cosp);

            return eulerAngles;
            /* /
            EulerAngle eulerAngles;
            // Pitch (X)
            eulerAngles.roll = -maths::ASin(2.0f * (x * z - w * y)) + Angle(90.0f);

            // Yaw (Y)
            eulerAngles.yaw = maths::ATan(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z) + Angle(90.0f);

            // Roll (Z)
            eulerAngles.pitch = maths::ATan(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z) + Angle(90.0f);
            return eulerAngles;* /
        }

        EulerAngle quatf::eulerAngle7() const
        {
            EulerAngle eulerAngles;

            // Calcul des angles d'Euler à partir du quaternion

            // Roll (Z)
            float32 sinr_cosp = 2.0f * (w * x + y * z);
            float32 cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
            eulerAngles.yaw = maths::ATan(sinr_cosp, cosr_cosp);

            // Pitch (X)
            float32 sinp = 2.0f * (w * y - z * x);
            if (maths::Abs(sinp) >= 1.0f) {
                // Gimbal lock - pitch is at 90 degrees
                eulerAngles.roll = maths::Copysign(maths::Pi / 2.0f, sinp);

                // Yaw (Y)
                float32 siny_cosp = 2.0f * (w * z + x * y);
                float32 cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
                eulerAngles.pitch = maths::ATan(siny_cosp, cosy_cosp);
            }
            else {
                // Normal case
                eulerAngles.roll = -maths::ASin(sinp);

                // Yaw (Y)
                eulerAngles.pitch = 0.0f;  // Set yaw to zero (arbitrary) because roll and pitch are locked

                // Note: There's no need to calculate yaw in gimbal lock case, as it's ambiguous
            }

            return eulerAngles;
        }

        EulerAngle quatf::eulerAngle8() const
        {
            EulerAngle eulerAngles;

            // Calculer le roulis
            float32 sinr_cosp = 2.0f * (w * z + x * y);
            float32 cosr_cosp = 1.0f - 2.0f * (y * y + z * z);
            eulerAngles.roll = Angle::FromRadian(atan2(sinr_cosp, cosr_cosp));

            // Calculer le tangage
            float32 sinp = 2.0f * (w * y - z * x);
            if (fabs(sinp) >= 1.0f) {
                // Verrouillage du cardan
                eulerAngles.pitch = Angle::FromRadian(copysign(maths::Pi / 2.0f, sinp));
                std::cout << "verrouillage du cardan\n";
            }
            else {
                eulerAngles.pitch = Angle::FromRadian(asin(sinp));
            }

            // Calculer le lacet
            float32 siny_cosp = 2.0f * (w * x + y * z);
            float32 cosy_cosp = 1.0f - 2.0f * (x * x + y * y);
            eulerAngles.yaw = Angle::FromRadian(atan2(siny_cosp, cosy_cosp));

            return eulerAngles;
        }

        EulerAngle quatf::eulerAngle9() const
        {
            EulerAngle eulerAngles;

            // Roll (x-axis rotation)
            float32 sinr_cosp = 2.0f * (w * x + y * z);
            float32 cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
            eulerAngles.roll = Angle::FromRadian(atan2(sinr_cosp, cosr_cosp));

            // Pitch (y-axis rotation)
            float32 sinp = 2.0f * (w * y - z * x);
            if (fabs(sinp) >= 1.0f) {
                eulerAngles.pitch = Angle::FromRadian(copysign(maths::Pi / 2.0f, sinp)); // Gimbal lock
                std::cout << "Gimbal lock\n";
            }
            else {
                eulerAngles.pitch = Angle::FromRadian(asin(sinp));
            }

            // Yaw (z-axis rotation)
            float32 siny_cosp = 2.0f * (w * z + x * y);
            float32 cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
            eulerAngles.yaw = Angle::FromRadian(atan2(siny_cosp, cosy_cosp));

            if (eulerAngles.yaw == eulerAngles.roll && eulerAngles.yaw == Angle(-180.0f)) {
                eulerAngles.pitch += Angle(-90.0f) - (eulerAngles.pitch + Angle(90.0f));
                eulerAngles.yaw += 0.0f;
                eulerAngles.roll += 0.0f;
            }

            return eulerAngles;
        }

        EulerAngle quatf::eulerAngle10() const
        {
            EulerAngle eulerAngles;

            // Détecter le verrouillage du cardan
            int32 gp = gimbalPole();

            // Calculer le roulis
            float32 sinr_cosp = 2.0f * (w * z + x * y);
            float32 cosr_cosp = 1.0f - 2.0f * (y * y + z * z);
            eulerAngles.roll = Angle::FromRadian(atan2(sinr_cosp, cosr_cosp));

            // Ajuster le calcul du tangage en fonction du verrouillage du cardan
            if (gp == 1) {
                // Verrouillage du cardan positif sur l'axe X
                eulerAngles.pitch = Angle(90.0f);
            }
            else if (gp == -1) {
                // Verrouillage du cardan négatif sur l'axe X
                eulerAngles.pitch = Angle(-90.0f);
            }
            else {
                // Pas de verrouillage du cardan, calculer le tangage normalement
                float32 sinp = 2.0f * (w * y - z * x);
                eulerAngles.pitch = Angle::FromRadian(asin(sinp));
            }

            // Calculer le lacet
            float32 siny_cosp = 2.0f * (w * x + y * z);
            float32 cosy_cosp = 1.0f - 2.0f * (x * x + y * y);
            eulerAngles.yaw = Angle::FromRadian(atan2(siny_cosp, cosy_cosp));

            return eulerAngles;
        }

        EulerAngle quatf::eulerAngle11() const
        {
            EulerAngle eulerAngles;

            // Determine gimbal pole
            int32 pole = gimbalPole();

            if (pole == 0) {
                // Roll (x-axis rotation)
                float32 sinr_cosp = 2.0f * (w * x + y * z);
                float32 cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
                eulerAngles.pitch = Angle::FromRadian(atan2(sinr_cosp, cosr_cosp));

                // Pitch (y-axis rotation)
                float32 sinp = 2.0f * (w * y - z * x);
                eulerAngles.yaw = Angle::FromRadian(asin(sinp));

                // Yaw (z-axis rotation)
                float32 siny_cosp = 2.0f * (w * z + x * y);
                float32 cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
                eulerAngles.roll = Angle::FromRadian(atan2(siny_cosp, cosy_cosp));
            }
            else {
                // Gimbal lock case
                std::cout << "Gimbal lock\n";
                eulerAngles.roll = (float32)pole * Angle(90.0f);
                eulerAngles.yaw = Angle::FromRadian(atan2(x * z + w * y, 0.5f - y * y - z * z));
                eulerAngles.pitch = 0.0f;
            }

            if (eulerAngles.roll == eulerAngles.pitch && eulerAngles.pitch == Angle(-180.0f)) {
                //eulerAngles.yaw = Angle(180.0f) - eulerAngles.yaw;
                //eulerAngles.roll = 0.0f;
                //eulerAngles.pitch = 0.0f;
            }

            //if (eulerAngles.pitch == Angle(-0.0f)) eulerAngles.pitch = 0.0f;
            //if (eulerAngles.yaw == Angle(-0.0f)) eulerAngles.yaw = 0.0f;
            //if (eulerAngles.roll == Angle(-0.0f)) eulerAngles.roll = 0.0f;

            return eulerAngles;
        }

        int32 quatf::gimbalPole() const {
            float32 t = y * x + z * w;
            return t > 0.499f ? 1 : t < -0.499f ? -1 : 0;
        }

        Vector3f quatf::back() const
        {
            return -forward();
        }

        Vector3f quatf::forward() const
        {
            return *this * Vector3f(0.0f, 0.0f, 1.0f);;
        }

        Vector3f quatf::up() const
        {
            return *this * Vector3f(0.0f, 1.0f, 0.0f);
        }

        Vector3f quatf::down() const
        {
            return -up();
        }

        Vector3f quatf::right() const
        {
            return *this * Vector3f(1.0f, 0.0f, 0.0f);
        }

        Vector3f quatf::left() const
        {
            return -right();
        }

        Vector3f quatf::transformVector(const Vector3f& vector)
        {
            return ((*this * quatf(vector.x, vector.y, vector.z, 0.0f)) * inverse()).vector;
        }

        Vector3f quatf::reflectVector(const Vector3f& vector)
        {
            return ((*this * quatf(vector.x, vector.y, vector.z, 0.0f)) * (*this)).vector;
        }

        quatf quatf::rotateX(const Angle& pitch) {
            float32 halfAngle = pitch * 0.5f;
            return quatf(maths::Sin(halfAngle), 0.0f, 0.0f, maths::Cos(halfAngle));
        }

        quatf quatf::rotateY(const Angle& yaw) {
            float32 halfAngle = yaw * 0.5f;
            return quatf(0.0f, maths::Sin(halfAngle), 0.0f, maths::Cos(halfAngle));
        }

        quatf quatf::rotateZ(const Angle& roll) {
            float32 halfAngle = roll * 0.5f;
            return quatf(0.0f, 0.0f, maths::Sin(halfAngle), maths::Cos(halfAngle));
        }*/
    }
}    // namespace nkentseu