//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 08:21:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "Camera.h"
#include <Logger/Formatter.h>
#include "Unkeny/Core/Log.h"

namespace nkentseu {

	Camera::Camera(Vector3f pos, Vector3f up, Angle yaw, Angle pitch)
		: Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), FPS_Camera(false)
	{
		this->Position = pos;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	Camera::Camera(float32 posX, float32 posY, float32 posZ, float32 upX, float32 upY, float32 upZ, Angle yaw, Angle pitch)
		: Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), FPS_Camera(false)
	{
		this->Position = Vector3f(posX, posY, posZ);
		this->WorldUp = Vector3f(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	matrix4f Camera::GetViewMatrix()
	{
		//return GetMyViewMatrix(this->Position, this->Position + this->Front, this->Up);
		return matrix4f::LookAt(this->Position, this->Position + this->Front, this->Up);
	}

	matrix4f Camera::GetMyViewMatrix(Vector3f pos, Vector3f center, Vector3f up)
	{
		Vector3f zAxis = (pos - center).Normalized();
		Vector3f xAxis = (up.Normalized().Cross(zAxis)).Normalized();
		Vector3f yAxis = zAxis.Cross(xAxis);

		matrix4f rotation = matrix4f(1.0f);
		rotation[0][0] = xAxis.x;
		rotation[1][0] = xAxis.y;
		rotation[2][0] = xAxis.z;
		rotation[0][1] = yAxis.x;
		rotation[1][1] = yAxis.y;
		rotation[2][1] = yAxis.z;
		rotation[0][2] = zAxis.x;
		rotation[1][2] = zAxis.y;
		rotation[2][2] = zAxis.z;

		matrix4f translation = matrix4f(1.0f);
		translation[3][0] = -this->Position.x;
		translation[3][1] = -this->Position.y;
		translation[3][2] = -this->Position.z;

		return rotation * translation;
	}



	void Camera::ProcessKeyboard(CameraMovement direction, float32 deltaTime)
	{
		float32 velocity = this->MovementSpeed * deltaTime;

		if (FPS_Camera)
		{
			if (direction == FORWARD)
				this->Position += fpsFront * velocity;
			if (direction == BACKWARD)
				this->Position -= fpsFront * velocity;
			if (direction == LEFT)
				this->Position -= fpsRight * velocity;
			if (direction == RIGHT)
				this->Position += fpsRight * velocity;
		}
		else
		{
			if (direction == FORWARD)
				this->Position += this->Front * velocity;
			if (direction == BACKWARD)
				this->Position -= this->Front * velocity;
			if (direction == LEFT)
				this->Position -= this->Right * velocity;
			if (direction == RIGHT)
				this->Position += this->Right * velocity;
		}

		if (direction == UP)
			this->Position += this->Up * velocity;
		if (direction == DOWN)
			this->Position -= this->Up * velocity;
	}

	void Camera::ProcessMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
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
		Vector3f front;

		front.x = maths::Cos(this->Yaw) * maths::Cos(this->Pitch);
		front.y = maths::Sin(this->Pitch);
		front.z = maths::Sin(this->Yaw) * maths::Cos(this->Pitch);

		this->Front = front.Normalized();
		this->Right = this->Front.Cross(this->WorldUp).Normalized();
		this->Up = this->Right.Cross(this->Front).Normalized();

		fpsFront = Vector3f(this->Front.x, 0, this->Front.z).Normalized();
		fpsRight = Vector3f(this->Right.x, 0, this->Right.z).Normalized();
	}

}  //  nkentseu