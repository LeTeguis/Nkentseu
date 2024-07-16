//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-06 at 04:36:51 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __CAMERA_EDITOR_H__
#define __CAMERA_EDITOR_H__

#pragma once

#include <System/System.h>
#include <Ntsm/Matrix/Matrix4.h>
#include <Ntsm/Quaternion/Quaternion.h>
#include <Ntsm/Angle.h>

namespace nkentseu {
    enum class ProjectionType {
        Orthographic,
        Perspective
    };

    class CameraEditor {
    public:
        CameraEditor();

        void SetProjectionType(ProjectionType type);

        void SetPosition(const maths::Vector3& newPos);
        maths::Vector3 GetPosition();

        void SetOrientation(float32 pitch, float32 yaw, float32 roll);
        void SetOrientation(const maths::Vector3f& oriantation);
        maths::Vector3 GetOrientation();

        void SetFov(const maths::Angle& newFov);

        void SetAspectRatio(float32 newAspectRatio);

        void SetClippingPlanes(float32 near, float32 far);
        void SetClippingPlanes(const maths::Vector2f& clip);

        void SetOrthoSize(float32 newOrthoSize);

        void Move(const maths::Vector3& direction, float32 distance);

        void Move(const maths::Vector3& speed);

        void Rotate(float32 pitch, float32 yaw, float32 roll);
        void Rotate(const maths::Vector3f& delta);

        maths::matrix4f GetView();
        maths::matrix4f GetProjection();

        maths::Vector3f Forward();
        maths::Vector3f Backward();
        maths::Vector3f Up();
        maths::Vector3f Down();
        maths::Vector3f Left();
        maths::Vector3f Right();

    private:
        void UpdateCameraVectors();

        void UpdateView();
        void UpdateProjection();
        void UpdateRotation();

        ProjectionType projectionType;
        maths::Vector3f position;
        maths::Vector3f front = maths::Vector3f(0, 0, -1);
        maths::Vector3f up = maths::Vector3f(0, 1, 0);
        maths::Vector3f worldUp;
        maths::Vector3f right = maths::Vector3f(1, 0, 0);
        maths::Vector3f oriantation;
        maths::Angle fov;
        float32 aspectRatio;
        maths::Vector2f clippingPlan;
        float32 orthoSize;

        maths::matrix4f view;
        maths::matrix4f projection;
        //maths::Quaternionf rotation;

        bool updateProjection = false;
        bool updateView = false;

    private:
        static inline float32 ClipAngle(float d)
        {
            if (d < -180.0f) return d + 360.0f;
            if (d > +180.0f) return d - 360.f;
            return d;
        }

        static inline maths::Vector3f ClipAngles(const maths::Vector3f& angles)
        {
            return maths::Vector3f(
                maths::FMod(angles.x, 360.0f),
                maths::FMod(angles.y, 360.0f),
                maths::FMod(angles.z, 360.0f)
            );
        }

        static inline maths::Vector3f AngleDelta(const maths::Vector3f& anglesCurrent, const maths::Vector3f& anglesDesired)
        {
            const maths::Vector3f d = ClipAngles(anglesCurrent) - ClipAngles(anglesDesired);
            return maths::Vector3f(ClipAngle(d.x), ClipAngle(d.y), ClipAngle(d.z));
        }
    };

}  //  nkentseu

#endif  // __CAMERA_H__!