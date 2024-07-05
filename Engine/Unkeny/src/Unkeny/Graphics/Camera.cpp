//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 08:21:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "Camera.h"
#include <Logger/Formatter.h>
#include "Unkeny/Core/Log.h"

namespace nkentseu {
	using namespace maths;

	Camera::Camera(Vector3f pos, Vector3f up, Angle yaw, Angle pitch)
		: Back(Vector3f::Forward()), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), FPS_Camera(false)
	{
		this->Position = pos;
		this->WorldUp = up;
		Orientation.yaw = yaw;
		Orientation.pitch = pitch;
		this->updateCameraVectors();
	}

	Camera::Camera(float32 posX, float32 posY, float32 posZ, float32 upX, float32 upY, float32 upZ, Angle yaw, Angle pitch)
		: Back(Vector3f::Forward()), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), FPS_Camera(false)
	{
		this->Position = Vector3f(posX, posY, posZ);
		this->WorldUp = Vector3f(upX, upY, upZ);
		Orientation.yaw = yaw;
		Orientation.pitch = pitch;
		this->updateCameraVectors();
	}

	/*matrix4f Camera::GetView()
	{
		return matrix4f::LookAt(this->Position, this->Position + this->Front, this->Up);
	}

	matrix4f Camera::GetProjection(CameraProjection projection, float32 fov_or_orthoSize, float32 aspect, float32 near, float32 far)
	{
		if (projection == CameraProjection::Perspective) {
			return matrix4f::Perspective(Angle(fov_or_orthoSize), aspect, near, far);
		}
		
		return matrix4f::Orthogonal(fov_or_orthoSize * aspect, fov_or_orthoSize, near, far);
	}*/

	mat4f Camera::GetView()
	{
		return mat4f::LookAt(this->Position, this->Position - this->Back, this->Up);
	}

	mat4f Camera::GetProjection(CameraProjection projection, float32 fov_or_orthoSize, float32 aspect, float32 near, float32 far)
	{
		if (projection == CameraProjection::Perspective) {
			return mat4f::Perspective(Angle(fov_or_orthoSize), aspect, near, far);
		}
		
		return mat4f::Orthogonal(fov_or_orthoSize * aspect, fov_or_orthoSize, near, far);
	}

	void Camera::ProcessKeyboard(CameraMovement direction, float32 deltaTime)
	{
		float32 velocity = this->MovementSpeed * deltaTime;

		if (FPS_Camera)
		{
			if (direction == CameraMovement::FORWARD)
				this->Position += fpsFront * velocity;
			if (direction == CameraMovement::BACKWARD)
				this->Position -= fpsFront * velocity;
			if (direction == CameraMovement::LEFT)
				this->Position -= fpsRight * velocity;
			if (direction == CameraMovement::RIGHT)
				this->Position += fpsRight * velocity;
		}
		else
		{
			if (direction == CameraMovement::FORWARD)
				this->Position += this->Back * velocity;
			if (direction == CameraMovement::BACKWARD)
				this->Position -= this->Back * velocity;
			if (direction == CameraMovement::LEFT)
				this->Position += this->Right * velocity;
			if (direction == CameraMovement::RIGHT)
				this->Position -= this->Right * velocity;
		}

		if (direction == CameraMovement::UP)
			this->Position += this->Up * velocity;
		if (direction == CameraMovement::DOWN)
			this->Position -= this->Up * velocity;
	}

	void Camera::ProcessMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		Orientation.yaw += xoffset;
		Orientation.pitch += yoffset;

		if (constrainPitch)
		{
			if (Orientation.pitch > 89.0f)
				Orientation.pitch = 89.0f;
			if (Orientation.pitch < -89.0f)
				Orientation.pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float32 yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;

		Log.Debug("zoom = {0}", this->Zoom);
	}

	Angle Camera::zoom()
	{
		return Zoom;
	}

	void Camera::updateCameraVectors()
	{
		matrix4f mat = matrix4f::Rotation(EulerAngle(Orientation.pitch, Orientation.yaw, Orientation.roll));

		//this->Back = mat.backward.xyz().Normalized();
		//this->Right = -mat.right.xyz().Normalized();
		//this->Up = mat.up.xyz().Normalized();

		this->Right = this->Back.Cross(this->WorldUp).Normalized();
		this->Up = this->Right.Cross(this->Back).Normalized();

		fpsFront = Vector3f(this->Back.x, 0, this->Back.z).Normalized();
		fpsRight = Vector3f(this->Right.x, 0, this->Right.z).Normalized();
	}

}  //  nkentseu