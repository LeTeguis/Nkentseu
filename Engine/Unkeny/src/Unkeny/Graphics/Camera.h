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
	const maths::Angle YAW = 45.0f;
	const maths::Angle PITCH = 0.0f;
	const float32 SPEED = 3.0f;
	const float32 SENSITIVTY = 0.05f * 10;
	const float32 ZOOM = 45.0f;

	class NKENTSEU_API Camera
	{
	public:
		maths::Vector3f Position;
		maths::Vector3f Front;
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

		Camera(maths::Vector3f pos = maths::Vector3f(-10.0f, 0.0f, -10.0f), maths::Vector3f up = maths::Vector3f::Up(), maths::Angle yaw = YAW, maths::Angle pitch = PITCH);
		Camera(float32 posX, float32 posY, float32 posZ, float32 upX, float32 upY, float32 upZ, maths::Angle yaw, maths::Angle pitch);
		maths::matrix4f GetView();
		maths::matrix4f GetProjection(CameraProjection projection, float32 fov_or_orthoSize, float32 aspect, float32 near, float32 far);
		void ProcessKeyboard(CameraMovement direction, float32 deltaTime);
		void ProcessMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float32 yoffset);

		maths::Angle zoom();
	private:
		void updateCameraVectors();
	};



}  //  nkentseu

#endif  // __CAMERA_H__!