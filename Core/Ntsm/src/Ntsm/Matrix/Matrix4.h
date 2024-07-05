//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:26:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MATRIX4_H__
#define __NKENTSEU_MATRIX4_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector4.h>
#include <Ntsm/Utils.h>
#include <Ntsm/EulerAngle.h>

namespace nkentseu {

	namespace maths
	{
		struct quatf;

		struct NKENTSEU_API mat4f {
			union {
				float32 data[16];
				float32 mat[4][4];
				struct {
					float32 m00, m01, m02, m03; // colonne 0
					float32 m10, m11, m12, m13; // colonne 1
					float32 m20, m21, m22, m23; // colonne 2
					float32 m30, m31, m32, m33; // colonne 3
				};
				struct {
					float32 vxx, vxy, vxz, vxw; // colonne 0
					float32 vyx, vyy, vyz, vyw; // colonne 1
					float32 vzx, vzy, vzz, vzw; // colonne 2
					float32 vwx, vwy, vwz, vww; // colonne 3
				};
				struct {
					Vector4f right;
					Vector4f up;
					Vector4f backward;
					Vector4f position;
				};
			};

			// Constructeurs
			mat4f();
			mat4f(const mat4f& other);
			mat4f(const float32* values);
			mat4f(const Vector4f& col0, const Vector4f& col1, const Vector4f& col2, const Vector4f& col3);
			mat4f(float32 m00, float32 m10, float32 m20, float32 m30,
				  float32 m01, float32 m11, float32 m21, float32 m31,
				  float32 m02, float32 m12, float32 m22, float32 m32,
				  float32 m03, float32 m13, float32 m23, float32 m33);
			mat4f(float32 value, bool identity = true);
			mat4f(const Vector3f& right, const Vector3f& up, const Vector3f& back);
			mat4f(const Vector3f& right, const Vector3f& up, const Vector3f& back, const Vector3f& position);

			float32* operator[](int32 col);
			const float32* operator[](int32 col) const;
			float32* operator&();
			operator float32* ();
			operator const float32* () const;

			// Opérateurs d'affectation
			mat4f& operator=(const mat4f& other);

			// Opérateurs d'égalité
			bool operator==(const mat4f& other) const;
			bool operator!=(const mat4f& other) const;

			// Opérateurs arithmétiques
			mat4f operator*(const mat4f& other) const;
			Vector3f operator*(const Vector3f& vec) const;
			Vector4f operator*(const Vector4f& vec) const;
			mat4f operator*(float32 scalar) const;
			friend mat4f operator*(float32 scalar, const mat4f& mat);

			mat4f operator+(const mat4f& other) const;
			mat4f operator-(const mat4f& other) const;


			// Multiplication vec3 x mat
			friend Vector3f operator*(const Vector3f& vec, const mat4f& mat) {
				return Vector3f(
					vec.x * mat.right.x + vec.y * mat.right.y + vec.z * mat.right.z + mat.right.w,	
					vec.x * mat.up.x + vec.y * mat.up.y + vec.z * mat.up.z + mat.up.w,
					vec.x * mat.backward.x + vec.y * mat.backward.y + vec.z * mat.backward.z + mat.backward.w
				);
			}

			// Multiplication vec4 x mat
			friend Vector4f operator*(const Vector4f& vec, const mat4f& mat) {
				return Vector4f(
					vec.x * mat.right.x + vec.y * mat.right.y + vec.z * mat.right.z + vec.w * mat.right.w,
					vec.x * mat.up.x + vec.y * mat.up.y + vec.z * mat.up.z + vec.w * mat.up.w,
					vec.x * mat.backward.x + vec.y * mat.backward.y + vec.z * mat.backward.z + vec.w * mat.backward.w,
					vec.x * mat.position.x + vec.y * mat.position.y + vec.z * mat.position.z + vec.w * mat.position.w
				);
			}

			// Methode pour obtenir une matrice identité
			static mat4f Identity();

			// Méthodes statiques
			static mat4f LookAt(const Vector3f& position, const Vector3f& direction, const Vector3f& up, bool view);
			static mat4f LookAt(const Vector3f& direction, const Vector3f& up);
			static mat4f LookAt(const Vector3f& eye, const Vector3f& look, const Vector3f& up, const Vector3f& right);
			static mat4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
			static mat4f Orthogonal(float32 width, float32 height, float32 zNear, float32 zFar);
			static mat4f Orthogonal(const Vector2f& size, float32 zNear, float32 zFar);
			static mat4f Orthogonal(const Vector2f& bottomLeft, const Vector2f& topRight, float32 zNear, float32 zFar);
			static mat4f Perspective(float32 width, float32 height, float32 zNear, float32 zFar);
			static mat4f Perspective(const Angle& fov, float32 aspect, float32 zNear, float32 zFar);
			static mat4f Perspective(const Angle& fovX, const Angle& fovY, float32 zNear, float32 zFar);
			static mat4f Perspective(const Vector2f& bottomLeft, const Vector2f& topRight, float32 zNear, float32 zFar);

			// pich rotation au tour de l'axe des x, yaw rotation au tour de l'axe des y et roll rotation au tour de l'axe des z
			static mat4f FromEuler(const EulerAngle& euler);
			static mat4f Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar);
			static mat4f Reflection(const Vector3f& normal);
			static mat4f ShearingXY(float32 amountX, float32 amountY);
			static mat4f ShearingXZ(float32 amountX, float32 amountZ);
			static mat4f ShearingYZ(float32 amountY, float32 amountZ);

			static mat4f Rotation(const Vector3f& axis, const Angle& angle);
			static mat4f Rotation(const EulerAngle& euler);
			static mat4f Rotation(const Vector3f& axis, const Angle& angle, const Vector3f& center);
			static mat4f RotationX(const Angle& pitch);
			static mat4f RotationY(const Angle& yaw);
			static mat4f RotationZ(const Angle& roll);
			static mat4f Scaling(const Vector3f& scaleFactors);
			static mat4f Scaling(const Vector3f& axis, const Vector3f& scaleFactors);
			static mat4f Translation(const Vector3f& pos);
			static mat4f Translation(const Vector3f& axis, const Vector3f& move);

			// Méthodes pour l'inverse, cofacteur, déterminant, etc.
			mat4f Inverse() const;
			float32 Cofactor(int32 row, int32 col) const;
			float32 Determinant() const;
			float32 Determinant3x3(float32 m00, float32 m01, float32 m02,
									float32 m10, float32 m11, float32 m12,
									float32 m20, float32 m21, float32 m22) const;

			Vector3f TransformPoint(const Vector3f& point) const;
			Vector3f TransformVector(const Vector3f& vector) const;
			Vector3f TransformNormal(const Vector3f& normal) const;

			// Transposition
			mat4f Transpose() const;

			// to string
			std::string ToStringRow(uint32 i) const;
			std::string ToStringCol(uint32 i) const;
			std::string ToString() const;

			friend std::string ToString(const mat4f& mat) {
				return mat.ToString();
			}

			friend std::ostream& operator<<(std::ostream& os, const mat4f& e);
		};

		using Matrix4f = mat4f;
		using matrix4f = mat4f;
		using Matrix4 = mat4f;
		using matrix4 = mat4f;
		using Matrice4 = mat4f;
		using matrice4 = mat4f;
		using Mat4 = mat4f;
		using mat4 = mat4f;
	}

} // namespace nkentseu

#endif    // __NKENTSEU_MATRIX4_H__