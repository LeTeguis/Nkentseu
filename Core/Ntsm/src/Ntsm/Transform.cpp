//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 1:35:32 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Transform.h"

namespace nkentseu {

    namespace maths
    {
        Transform::Transform(const Vector3f& p, const Quaternion& r, const Vector3f& s) : position(p), rotation(r), scale(s) {}
        Transform::Transform() : position(Vector3f(0, 0, 0)), rotation(Quaternion(0, 0, 0, 1)), scale(Vector3f(1, 1, 1)) {}
        Transform::Transform(const Transform& transform) : position(transform.position), rotation(transform.rotation), scale(transform.scale) {}
        Transform& Transform::operator = (const Transform& transform) {
            position = transform.position;
            rotation = transform.rotation;
            scale = transform.scale;
            return *this;
        }

        Transform Transform::Combine(const Transform& a, const Transform& b) {
            Transform out;
            out.scale = a.scale * b.scale;
            out.rotation = b.rotation * a.rotation;
            out.position = a.rotation * (a.scale * b.position);
            out.position = a.position + out.position;
            return out;
        }

        Transform Transform::Inverse() {
            Transform inv;
            inv.rotation = rotation.Inverse();
            inv.scale.x = maths::Abs(scale.x) < maths::VectorEpsilon ? 0.0f : 1.0f / scale.x;
            inv.scale.y = maths::Abs(scale.y) < maths::VectorEpsilon ? 0.0f : 1.0f / scale.y;
            inv.scale.z = maths::Abs(scale.z) < maths::VectorEpsilon ? 0.0f : 1.0f / scale.z;
            Vector3f invTrans = position * -1.0f;
            inv.position = inv.rotation * (inv.scale * invTrans);
            return inv;
        }

        Transform Transform::Mix(const Transform& b, float32 t) {
            Transform aa(*this);
            Quaternion bRot = b.rotation;
            if (aa.rotation.Dot(bRot) < 0.0f) {
                bRot = -bRot;
            }
            return Transform(aa.position.Lerp(b.position, t), aa.rotation.Nlerp(bRot, t), aa.scale.Lerp(b.scale, t));
        }

        Matrix4f Transform::ToMat4() {
            // First, extract the rotation basis of the transform
            Vector3f x = rotation * Vector3f(1, 0, 0);
            Vector3f y = rotation * Vector3f(0, 1, 0);
            Vector3f z = rotation * Vector3f(0, 0, 1);
            // Next, scale the basis vectors
            x = x * scale.x;
            y = y * scale.y;
            z = z * scale.z;
            // Extract the position of the transform
            Vector3f p = position;
            // Create matrix
            return Matrix4f(
                x.x, y.x, z.x, p.x, // X basis (& Scale)
                x.y, y.y, z.y, p.y, // Y basis (& scale)
                x.x, y.z, z.z, p.z, // Z basis (& scale)
                0.0f, 0.0f, 0.0f, 1.0f  // Position
            );
        }

        Matrix4f Transform::mat4()
        {
            matrix4f scale = matrix4f::Scaling(this->scale);
            matrix4f rot = rotation.mat4() * scale;
            matrix4f translate = matrix4f::Translation(this->position) * rot;
            return translate;
        }
        // t*s*r     s = t_*t*s*r*r_
        Transform Transform::mat4(const Matrix4f& m)
        {
            Transform transform;
            transform.rotation = Quaternionf(m);
            transform.position = m.Position();

            matrix4f tanslate = matrix4f::Translation(transform.position).Inverse();
            matrix4f rotation = transform.rotation.Inverse().mat4();
            matrix4f scale = tanslate * (m * rotation);

            transform.scale = Vector3f(scale.m00, scale.m11, scale.m22);

            return transform;
        }

        Transform Transform::FromMat4(const Matrix4f& m) {
            Transform out;
            out.position = Vector3f(m.data[12], m.data[13], m.data[14]);
            out.rotation = Quaternion::FromMat4(m);
            Matrix4f rotScaleMat(
                m.data[0], m.data[1], m.data[2], 0,
                m.data[4], m.data[5], m.data[6], 0,
                m.data[8], m.data[9], m.data[10], 0,
                0, 0, 0, 1
            );
            Matrix4f invRotMat = out.rotation.Inverse().ToMat4();
            Matrix4f scaleSkewMat = rotScaleMat * invRotMat;
            out.scale = Vector3f(
                scaleSkewMat.data[0],
                scaleSkewMat.data[5],
                scaleSkewMat.data[10]
            );
            return out;
        }

        Vector3f Transform::FormPoint(const Vector3f& b) {
            Vector3f out;
            out = rotation * (scale * b);
            out = position + out;
            return out;
        }

        Vector3f Transform::FormVector(const Vector3f& b) {
            Vector3f out;
            out = rotation * (scale * b);
            return out;
        }
    }
}    // namespace nkentseu