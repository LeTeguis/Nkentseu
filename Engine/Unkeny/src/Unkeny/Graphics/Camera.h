//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-16 at 08:21:18 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __CAMERA_H__
#define __CAMERA_H__

#pragma once

#include <System/System.h>
#include <Ntsm/Vector/Vector3.h>
#include <Ntsm/Matrix/Matrix4.h>
#include <Ntsm/Quaternion/Quaternion.h>
#include <Unkeny/Core/Log.h>

namespace nkentseu {

	enum class NKENTSEU_API CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	enum class NKENTSEU_API CameraProjection {
		Orthographic,
		Perspective
	};

	//const Angle YAW = -90.0f;
	const maths::Angle YAW = 0.0f;
	const maths::Angle PITCH = 0.0f;
	const float32 SPEED = 3.0f;
	const float32 SENSITIVTY = 0.05f * 10;
	const float32 ZOOM = 45.0f;

	class NKENTSEU_API Camera
	{
	public:
		maths::Vector3f Position;
		maths::Vector3f Back;
		maths::Vector3f Up;
		maths::Vector3f Right;

		maths::Vector3f WorldUp;

		maths::Vector3f fpsFront;
		maths::Vector3f fpsRight;

		maths::Vector3T<maths::Angle> Orientation;

		float32 MovementSpeed;
		float32 MouseSensitivity;
		maths::Angle Zoom;

		bool FPS_Camera;

		Camera(maths::Vector3f pos = maths::Vector3f(0.0f, 0.0f, -10.0f), maths::Vector3f up = maths::Vector3f::Up(), maths::Angle yaw = YAW, maths::Angle pitch = PITCH);
		Camera(float32 posX, float32 posY, float32 posZ, float32 upX, float32 upY, float32 upZ, maths::Angle yaw, maths::Angle pitch);
		//maths::matrix4f GetView();
		//maths::matrix4f GetProjection(CameraProjection projection, float32 fov_or_orthoSize, float32 aspect, float32 near, float32 far);
		maths::mat4f GetView();
		maths::mat4f GetProjection(CameraProjection projection, float32 fov_or_orthoSize, float32 aspect, float32 near, float32 far);
		void ProcessKeyboard(CameraMovement direction, float32 deltaTime);
		void ProcessMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float32 yoffset);

		maths::Angle zoom();
	private:
		void updateCameraVectors();
	};

    class NKENTSEU_API CameraView {
    private:
        maths::Vector3f m_forward;
        maths::Vector3f m_right;
        maths::Vector3f m_up;
        maths::Vector3f m_position;
        float32 m_aspectRatio;
        CameraProjection m_projectionType;
        maths::Angle m_fov; // Field of view for perspective projection
        float32 m_orthoSize; // Size for orthographic projection
        float32 m_nearClip;
        float32 m_farClip;
        float32 m_movementSpeed;
        float32 m_rotationSpeed;
        maths::EulerAngle m_euler;   // Yaw angle

    public:
        CameraView()
            : m_position(maths::Vector3f(0, 0, -10)),
            m_aspectRatio(16.0f / 9.0f), // Default aspect ratio
            m_projectionType(CameraProjection::Perspective),
            m_fov(45.0f), // Default field of view
            m_orthoSize(10.0f), // Default orthographic size
            m_nearClip(0.1f),
            m_farClip(1000.0f),
            m_movementSpeed(5.0f), // Default movement speed
            m_rotationSpeed(1.0f), // Default rotation speed
            m_euler(0, 90, 0),
            m_forward(maths::Vector3f::Forward()),
            m_up(maths::Vector3f::Up()),
            m_right(maths::Vector3f::Right())
        {
        }

        // Getters and setters
        const maths::Vector3f& getPosition() const { return m_position; }
        void setPosition(const maths::Vector3f& position) { m_position = position; }

        const maths::EulerAngle& getOrientation() const { return m_euler; }
        void setOrientation(const maths::EulerAngle& rotation) { m_euler = rotation; }
        void MoveOrientation(const maths::EulerAngle& rotation) { m_euler += rotation; }
        void setPitch(const maths::Angle& pitch) { m_euler.pitch = pitch; }
        void setRoll(const maths::Angle& roll) { m_euler.roll = roll; }
        void setYaw(const maths::Angle& yaw) { m_euler.yaw = yaw; }
        void MovePitch(const maths::Angle& pitch) { m_euler.pitch += pitch; }
        void MoveRoll(const maths::Angle& roll) { m_euler.roll += roll; }
        void MoveYaw(const maths::Angle& yaw) { m_euler.yaw += yaw; }

        float32 getAspectRatio() const { return m_aspectRatio; }
        void setAspectRatio(float32 aspectRatio) { m_aspectRatio = aspectRatio; }

        CameraProjection getProjectionType() const { return m_projectionType; }
        void setProjectionType(CameraProjection projectionType) { m_projectionType = projectionType; }

        maths::Angle getFieldOfView() const { return m_fov; }
        void setFieldOfView(maths::Angle fov) { m_fov = fov; }

        float32 getOrthoSize() const { return m_orthoSize; }
        void setOrthoSize(float32 orthoSize) { m_orthoSize = orthoSize; }

        float32 getNearClip() const { return m_nearClip; }
        void setNearClip(float32 nearClip) { m_nearClip = nearClip; }

        float32 getFarClip() const { return m_farClip; }
        void setFarClip(float32 farClip) { m_farClip = farClip; }

        float32 getMovementSpeed() const { return m_movementSpeed; }
        void setMovementSpeed(float32 movementSpeed) { m_movementSpeed = movementSpeed; }

        float32 getRotationSpeed() const { return m_rotationSpeed; }
        void setRotationSpeed(float32 rotationSpeed) { m_rotationSpeed = rotationSpeed; }

        maths::Angle getPitch() const { return m_euler.pitch; }
        void setPitch(maths::Angle pitch) { m_euler.pitch = pitch; }

        maths::Angle getYaw() const { return m_euler.yaw; }
        void setYaw(maths::Angle yaw) { m_euler.yaw = yaw; }

        // Camera movement
        void translate(const maths::Vector3f& translation) {
            m_position += translation;
        }

        void rotate(const maths::EulerAngle& rotation) {
            m_euler += rotation;
        }

        void moveForward(float32 distance) {
            m_position -= m_forward * distance * m_movementSpeed;
        }

        void moveBackward(float32 distance) {
            m_position += m_forward * distance * m_movementSpeed;
        }

        void moveRight(float32 distance) {
            m_position += m_right * distance * m_movementSpeed;
        }

        void moveLeft(float32 distance) {
            m_position -= m_right * distance * m_movementSpeed;
        }

        void moveUp(float32 distance) {
            m_position += m_up * distance * m_movementSpeed;
        }

        void moveDown(float32 distance) {
            m_position -= m_up * distance * m_movementSpeed;
        }

        // Projection matrix
        maths::mat4f getProjection() const {
            if (m_projectionType == CameraProjection::Orthographic) {
                return maths::mat4f::Orthogonal(m_orthoSize * m_aspectRatio, m_orthoSize, m_nearClip, m_farClip);
            }
            else {
                return maths::mat4f::Perspective(m_fov, m_aspectRatio, m_nearClip, m_farClip);
            }
        }

        // Get the view matrix
        maths::mat4f getView() {
            updateDirection3();
            return maths::mat4f::LookAt(m_position, m_position + m_forward, m_up);
        }

        void updateDirection() {
            maths::Vector3f front;

            front.x = maths::Cos(m_euler.yaw) * maths::Cos(m_euler.pitch);
            front.y = maths::Sin(m_euler.pitch);
            front.z = maths::Sin(m_euler.yaw) * maths::Cos(m_euler.pitch);

            m_forward = front.Normalized();
            m_right = m_forward.Cross(maths::Vector3f::Up()).Normalized();
            m_up = m_right.Cross(m_forward).Normalized();
        }

        void updateDirection2() {
            /*/ Calculate rotation quaternion from Euler angles (assuming Z-Y-X convention)
            maths::quatf rotation = maths::quatf::fromEuler(m_euler.roll, m_euler.yaw, m_euler.pitch);

            // Apply rotation to the world's up vector to get the object's up vector
            maths::Vector3f worldUp = maths::Vector3f::Up();
            m_up = rotation * worldUp;

            // Calculate forward vector based on pitch and yaw (similar to existing code)
            m_forward.x = maths::Cos(m_euler.yaw) * maths::Cos(m_euler.pitch);
            m_forward.y = maths::Sin(m_euler.pitch);
            m_forward.z = maths::Sin(m_euler.yaw) * maths::Cos(m_euler.pitch);

            // Normalize forward vector
            m_forward = m_forward.Normalized();

            // Calculate right vector as the cross product of forward and up
            m_right = m_forward.Cross(m_up).Normalized();*/
        }

        void updateDirection3() {
            maths::Vector3f front;

            // Calcul du vecteur de direction sans roulis
            front.x = maths::Cos(m_euler.yaw) * maths::Cos(m_euler.pitch);
            front.y = maths::Sin(m_euler.pitch);
            front.z = maths::Sin(m_euler.yaw) * maths::Cos(m_euler.pitch);

            m_forward = front.Normalized();

            // Calcul du vecteur de droite sans roulis
            maths::Vector3f right = m_forward.Cross(maths::Vector3f::Up()).Normalized();

            // Calcul du vecteur vers le haut sans roulis
            maths::Vector3f up = right.Cross(m_forward).Normalized();

            // Appliquer le roulis aux vecteurs de droite et vers le haut
            float32 cosRoll = maths::Cos(m_euler.roll);
            float32 sinRoll = maths::Sin(m_euler.roll);

            // Calculer les nouveaux vecteurs de droite et vers le haut après l'application du roulis
            m_right = (right * cosRoll + up * sinRoll).Normalized();
            m_up = (up * cosRoll - right * sinRoll).Normalized();
        }


        // Zoom method
        void zoom(float32 amount) {
            if (m_projectionType == CameraProjection::Orthographic) {
                m_orthoSize += amount;
                if (m_orthoSize < 0.1f) m_orthoSize = 0.1f; // Ensure ortho size doesn't go negative
            }
            else {
                m_fov += amount;
                if (m_fov < 1.0f) m_fov = 1.0f; // Ensure fov doesn't go negative or too small
            }
        }

        float32 getZoom() {
            if (m_projectionType == CameraProjection::Orthographic) {
                return m_orthoSize;
            }
            else {
                return m_fov;
            }
        }

        // Process keyboard input for camera movement
        void processKeyboard(CameraMovement direction, float32 deltaTime) {
            float32 velocity = m_movementSpeed * deltaTime;
            switch (direction) {
            case CameraMovement::FORWARD:
                moveBackward(velocity);
                break;
            case CameraMovement::BACKWARD:
                moveForward(velocity);
                break;
            case CameraMovement::RIGHT:
                moveRight(velocity);
                break;
            case CameraMovement::LEFT:
                moveLeft(velocity);
                break;
            case CameraMovement::UP:
                moveUp(velocity);
                break;
            case CameraMovement::DOWN:
                moveDown(velocity);
                break;
            }
        }

        // Process mouse movement to rotate the camera
        void processMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch = false) {
            xoffset *= m_rotationSpeed;
            yoffset *= m_rotationSpeed;

            m_euler += maths::EulerAngle(maths::Angle(yoffset), maths::Angle(xoffset), 0.0f);

            if (constrainPitch) {
                if (m_euler.pitch > maths::Angle(45.0f)) {
                    m_euler.pitch = maths::Angle(45.0f);
                }
                else if (m_euler.pitch < maths::Angle(-45.0f)) {
                    m_euler.pitch = maths::Angle(-45.0f);
                }
            }
        }

        // Process mouse scroll for zooming
        void processMouseScroll(float32 yoffset) {
            zoom(yoffset);
        }
    };

}  //  nkentseu

#endif  // __CAMERA_H__!