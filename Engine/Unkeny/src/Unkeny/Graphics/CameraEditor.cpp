//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-06 at 04:36:51 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "CameraEditor.h"
#include <Logger/Formatter.h>
#include <Unkeny/Core/Log.h>

namespace nkentseu {
    using namespace maths;
    
    CameraEditor::CameraEditor()
        : projectionType(ProjectionType::Perspective),
        position(Vector3(0.0f, 0.0f, 0.0f)),
        //front(Vector3(0.0f, -1.0f, 0.0f)),
        //front(Vector3(0.0f, 0.0f, 1.0f)),
        //worldUp(Vector3(0.0f, 0.0f, 1.0f)),
        worldUp(Vector3(0.0f, 1.0f, 0.0f)),
        //right(-front.Cross(worldUp).Normalized()),
        //oriantation(Vector3f(0.0f, -90.0f, 0.0f)),
        oriantation(Vector3f(0.0f, 0.0f, 0.0f)),
        fov(45.0f),
        aspectRatio(4.0f / 3.0f),
        clippingPlan(Vector2f(0.1f, 100.0f)),
        orthoSize(1.0f)
    {
        UpdateRotation();
        UpdateView();
        UpdateProjection();
    }

    void CameraEditor::SetProjectionType(ProjectionType type) {
        projectionType = type;
        updateProjection = true;
    }

    void CameraEditor::SetPosition(const Vector3& newPos) {
        position = newPos;
        updateView = true;
    }

    Vector3 CameraEditor::GetPosition()
    {
        return position;
    }

    void CameraEditor::SetOrientation(float32 pitch, float32 yaw, float32 roll) {
        SetOrientation(Vector3f(pitch, yaw, roll));
    }

    void CameraEditor::SetOrientation(const Vector3f& oriantation)
    {
        this->oriantation = oriantation; 
        UpdateRotation();
        // updateView = true;
    }

    Vector3 CameraEditor::GetOrientation()
    {
        return oriantation;
    }

    void CameraEditor::SetFov(const maths::Angle& newFov) {
        fov = newFov;
        updateProjection = true;
    }

    void CameraEditor::SetAspectRatio(float32 newAspectRatio) {
        aspectRatio = newAspectRatio;
        updateProjection = true;
    }

    void CameraEditor::SetClippingPlanes(float32 near, float32 far) {
        SetClippingPlanes(Vector2f(near, far));
    }

    void CameraEditor::SetClippingPlanes(const Vector2f& clip)
    {
        clippingPlan = clip;
        updateProjection = true;
    }

    void CameraEditor::SetOrthoSize(float32 newOrthoSize) {
        orthoSize = newOrthoSize;
        updateProjection = true;
    }

    void CameraEditor::Move(const Vector3& direction, float32 distance) {
        position += direction * distance;
        updateView = true;
    }

    void CameraEditor::Move(const Vector3& speed) {
        position += speed;
        //position += Vector3f(-speed.x, speed.y, speed.z);
        updateView = true;
    }

    void CameraEditor::Rotate(float32 pitch, float32 yaw, float32 roll) {
        Rotate(Vector3f(pitch, yaw, roll));
    }

    void CameraEditor::Rotate(const Vector3f& delta)
    {
        oriantation += delta;
        UpdateRotation();
        //updateView = true;
    }

    matrix4f CameraEditor::GetView()
    {
        if (updateView) {
            //UpdateCameraVectors();
            //view = matrix4f::LookAt(position, position + front, up);

            UpdateView();
            updateView = false;
        }
        return view;
    }

    matrix4f CameraEditor::GetProjection()
    {
        if (updateProjection) {
            UpdateProjection();
            updateProjection = false;
        }
        return projection;
    }

    Vector3f CameraEditor::Forward()
    {
        return front;
    }

    Vector3f CameraEditor::Backward()
    {
        return -front;
    }

    Vector3f CameraEditor::Up()
    {
        return up;
    }

    Vector3f CameraEditor::Down()
    {
        return -up;
    }

    Vector3f CameraEditor::Left()
    {
        return -right;
    }

    Vector3f CameraEditor::Right()
    {
        return right;
    }

    void CameraEditor::UpdateCameraVectors() {
        Vector3 newFront;
        newFront.x = maths::Cos(oriantation.yaw) * maths::Cos(oriantation.pitch);
        newFront.y = maths::Sin(oriantation.yaw) * maths::Cos(oriantation.pitch);
        newFront.z = maths::Sin(oriantation.pitch);
        front = newFront.Normalized();
        right = front.Cross(worldUp).Normalized();
        up = -right.Cross(front).Normalized();

        if (oriantation.roll != 0.0f) {
            float32 cosRoll = maths::Cos(oriantation.roll);
            float32 sinRoll = maths::Sin(oriantation.roll);

            Vector3f rolledRight = right * cosRoll - up * sinRoll;
            Vector3f rolledUp = right * sinRoll + up * cosRoll;

            right = rolledRight.Normalized();
            up = rolledUp.Normalized();
        }
    }

    void CameraEditor::UpdateView()
    {
        //Quaternionf quat = Quaternion::FromEuler(Angle(oriantation.pitch), Angle(oriantation.yaw), Angle(oriantation.roll));
        

        //if (oriantation != Vector3f()) front = mat.Forward().Normalized();
        //else front = Vector3f(0.0f, 0.0f, 1.0f).Normalized();

        //front = quat.axis();

        //Log.Debug("front = {0}", front);

        //if (oriantation != Vector3f()) up = mat.Up().Normalized();
        //else up = Vector3f(0.0f, 1.0f, 0.0f).Normalized();

        //if (oriantation != Vector3f()) right = mat.Right().Normalized();
        //else right = Vector3f(1.0f, 0.0f, 0.0f).Normalized();

        //view = matrix4f::Translation(-position) * rotation.mat4();
    }

    void CameraEditor::UpdateProjection() {
        if (projectionType == ProjectionType::Perspective) {
            projection = matrix4f::Perspective(fov, aspectRatio, clippingPlan.x, clippingPlan.y);
        }
        else if (projectionType == ProjectionType::Orthographic) {
            projection = matrix4f::Orthogonal(orthoSize * aspectRatio, orthoSize, clippingPlan.x, clippingPlan.y);
        }
    }

    void CameraEditor::UpdateRotation()
    {
        /*/rotation = Quaternion::euler(Angle(oriantation.pitch), Angle(oriantation.yaw), Angle(oriantation.roll));
        matrix4f transform = rotation.mat4();

        //front = mat.Forward();
        //this->right = this->front.Cross(this->worldUp).Normalized();
        //this->up = this->right.Cross(this->front).Normalized();
        //right = mat.Right();
        //up = mat.Up();
        // Extraire les vecteurs de la matrice de transformation
        front = transform.forward.xyz(); // Colonne 2 correspond à l'axe avant
        right = transform.right.xyz();   // Colonne 0 correspond à l'axe droit
        up = transform.up.xyz();     // Colonne 1 correspond à l'axe haut

        // Normaliser les vecteurs pour s'assurer qu'ils ont une longueur de 1
        front.Normalize();
        right.Normalize();
        up.Normalize();

        Log.Debug("front = {0}", front);
        Log.Debug("up = {0}", up);
        Log.Debug("right = {0}", right);

        updateView = true;*/
    }

}  //  nkentseu

