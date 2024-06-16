//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-06 at 04:36:51 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __CAMERA_EDITOR_H__
#define __CAMERA_EDITOR_H__

#pragma once

#include <System/System.h>
#include <Ntsm/Matrix/Matrix4.h>
#include <Ntsm/Quaternion/Quaternion.h>

namespace nkentseu {
    enum class ProjectionType {
        Orthographic,
        Perspective
    };

    class CameraEditor {
    public:
        CameraEditor();

        void SetProjectionType(ProjectionType type);

        void SetPosition(const Vector3& newPos);
        Vector3 GetPosition();

        void SetOrientation(float32 pitch, float32 yaw, float32 roll);
        void SetOrientation(const Vector3f& oriantation);
        Vector3 GetOrientation();

        void SetFov(float32 newFov);

        void SetAspectRatio(float32 newAspectRatio);

        void SetClippingPlanes(float32 near, float32 far);
        void SetClippingPlanes(const Vector2f& clip);

        void SetOrthoSize(float32 newOrthoSize);

        void Move(const Vector3& direction, float32 distance);

        void Move(const Vector3& speed);

        void Rotate(float32 pitch, float32 yaw, float32 roll);
        void Rotate(const Vector3f& delta);

        matrix4f GetView();
        matrix4f GetProjection();

        Vector3f Forward();
        Vector3f Backward();
        Vector3f Up();
        Vector3f Down();
        Vector3f Left();
        Vector3f Right();

    private:
        void UpdateCameraVectors();

        void UpdateView();
        void UpdateProjection();
        void UpdateRotation();

        ProjectionType projectionType;
        Vector3f position;
        Vector3f front = Vector3f(0, 0, -1);
        Vector3f up = Vector3f(0, 1, 0);
        Vector3f worldUp;
        Vector3f right = Vector3f(1, 0, 0);
        Vector3f oriantation;
        float32 fov;
        float32 aspectRatio;
        Vector2f clippingPlan;
        float32 orthoSize;

        matrix4f view;
        matrix4f projection;
        Quaternionf rotation;

        bool updateProjection = false;
        bool updateView = false;

    private:
        static inline float32 ClipAngle(float d)
        {
            if (d < -180.0f) return d + 360.0f;
            if (d > +180.0f) return d - 360.f;
            return d;
        }

        static inline Vector3f ClipAngles(const Vector3f& angles)
        {
            return Vector3f(
                maths::FMod(angles.x, 360.0f),
                maths::FMod(angles.y, 360.0f),
                maths::FMod(angles.z, 360.0f)
            );
        }

        static inline Vector3f AngleDelta(const Vector3f& anglesCurrent, const Vector3f& anglesDesired)
        {
            const Vector3f d = ClipAngles(anglesCurrent) - ClipAngles(anglesDesired);
            return Vector3f(ClipAngle(d.x), ClipAngle(d.y), ClipAngle(d.z));
        }
    };

}  //  nkentseu

#endif  // __CAMERA_H__!