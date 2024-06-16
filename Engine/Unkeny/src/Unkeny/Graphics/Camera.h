//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 08:21:18 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __CAMERA_H__
#define __CAMERA_H__

#pragma once

#include <System/System.h>
#include <Ntsm/Vector/Vector3.h>
#include <Ntsm/Matrix/Matrix4.h>

namespace nkentseu {

	enum NKENTSEU_API CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	const Angle YAW = -90.0f;
	const Angle PITCH = 0.0f;
	const float32 SPEED = 3.0f;
	const float32 SENSITIVTY = 0.05f * 10;
	const float32 ZOOM = 45.0f;

	class NKENTSEU_API Camera
	{
	public:
		Vector3f Position;
		Vector3f Front;
		Vector3f Up;
		Vector3f Right;

		Vector3f WorldUp;

		Vector3f fpsFront;
		Vector3f fpsRight;

		Angle Yaw;
		Angle Pitch;

		float32 MovementSpeed;
		float32 MouseSensitivity;
		Angle Zoom;

		bool FPS_Camera;

		Camera(Vector3f pos = Vector3f(0.0f, 0.0f, 0.0f),
			Vector3f up = Vector3f(0.0f, 1.0f, 0.0f),
			Angle yaw = YAW,
			Angle pitch = PITCH);
		Camera(float32 posX, float32 posY, float32 posZ,
			float32 upX, float32 upY, float32 upZ,
			Angle yaw, Angle pitch);
		matrix4f GetViewMatrix();
		matrix4f GetMyViewMatrix(Vector3f pos, Vector3f center, Vector3f up);
		void ProcessKeyboard(CameraMovement direction, float32 deltaTime);
		void ProcessMouseMovement(float32 xoffset, float32 yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float32 yoffset);

		Angle zoom();
	private:
		void updateCameraVectors();
	};



}  //  nkentseu

#endif  // __CAMERA_H__!