//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-14 at 08:48:57 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VIEW_H__
#define __VIEW_H__

#pragma once

#include <System/System.h>
#include <Ntsm/Matrix/Matrix4.h>
#include <Ntsm/Quaternion/Quaternion.h>

namespace nkentseu {

	enum class NKENTSEU_API ProjectionInfo {
		Orthographic,
		Perspective
	};
    
    class NKENTSEU_API View {
        public:
            View();
            ~View();

            void LookAt(const Vector3f& target);
            void LookAt(float32 xTarget, float32 yTarget, float32 zTarget);

			void Rotate(float32 axisX, float32 axisY, float32 axisZ, const Angle& angle) {
				Rotate(Vector3f(axisX, axisY, axisZ), angle);
			}

			void Rotate(const Vector3f& axis, const Angle& angle) {
				Rotate(matrix4f::Rotation(axis, angle));
			}

			void Rotate(const Matrix4f& transform) {
				m_Direction = (Vector4f(m_Direction, 1.0f) * transform).xyz();
				m_Up = (Vector4f(m_Up, 1.0f) * transform).xyz();
			}

			void Rotate(const Quaternionf& quat) {
				Quaternionf q(quat);

				m_Direction = q.Transform(m_Direction);
				m_Up = q.Transform(m_Up);
			}

			void RotateAround(const Vector3f& point, const Vector3& axis, const Angle& angle) {
				Vector3f direction = point - m_Position;
				Translate(direction);
				Rotate(axis, angle);
				direction = (Vector4f(direction, 1.0f) * matrix4f::Rotation(axis, angle)).xyz();
				Translate(-direction);
			}

			void Transform(const Matrix4f& transform) {
				m_Position = (Vector4f(m_Position, 1.0f) * transform).xyz();
				Rotate(transform);
			}

			void Translate(float32 x, float32 y, float32 z) {
				m_Position += Vector3f(x, y, z);
			}

			void Translate(const Vector3f& vec) {
				m_Position += vec;
			}



			/*Ray GetPickRay(float32 screenX, float32 screenY, float32 viewportX, float32 viewportY, float32 viewportWidth, float32 viewportHeight) {
				unproject(ray.origin.set(screenX, screenY, 0), viewportX, viewportY, viewportWidth, viewportHeight);
				unproject(ray.direction.set(screenX, screenY, 1), viewportX, viewportY, viewportWidth, viewportHeight);
				ray.direction.sub(ray.origin).nor();
				return ray;
			}

			Ray GetPickRay(float32 screenX, float32 screenY) {
				return GetPickRay(screenX, screenY, 0, 0, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
			}*/
        private:
			Vector3f m_Position = Vector3f();
            Vector3f m_Direction = Vector3f(0, 0, -1);
            Vector3f m_Up = Vector3f(0, 1, 0);

			matrix4f m_Projection = matrix4f::Identity();
            matrix4f m_View = matrix4f::Identity();
            matrix4f m_Combined = matrix4f::Identity();
            matrix4f m_InvProjectionView = matrix4f::Identity();

            float32 m_NearClip = 0.1f;
            float32 m_FarClip = 100.0f;

            void NormalizeUp();
        protected:
    };

}  //  nkentseu

#endif  // __VIEW_H__!