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

namespace nkentseu {

	namespace maths
	{
		class NKENTSEU_API matrix4f {
		public:
			union {
				float32 data[16];
				float32 mat[4][4];
				struct {
					float32 m00, m01, m02, m03;
					float32 m10, m11, m12, m13;
					float32 m20, m21, m22, m23;
					float32 m30, m31, m32, m33;
				};
			};

			matrix4f();
			matrix4f(const Vector3f& V0, const Vector3f& V1, const Vector3f& V2);
			matrix4f(const matrix4f& other);
			matrix4f(float32 value);
			matrix4f(const float32* data);
			matrix4f(float32 m00, float32 m01, float32 m02, float32 m03,
				float32 m10, float32 m11, float32 m12, float32 m13,
				float32 m20, float32 m21, float32 m22, float32 m23,
				float32 m30, float32 m31, float32 m32, float32 m33);
			float32 Get(int row, int col) const;
			void Set(int row, int col, float32 value);
			float32* operator[](int row);
			const float32* operator[](int row) const;
			float32* Ptr();
			const float32* Ptr() const;
			matrix4f& operator=(const matrix4f& other);
			matrix4f& operator=(const float32* otherData);
			bool operator==(const matrix4f& other) const;
			bool operator!=(const matrix4f& other) const;
			matrix4f& operator*=(const matrix4f& other);
			matrix4f& operator*=(float32 scalar);
			//Vector4f operator*(const Vector4f& vector) const;

			friend matrix4f operator*(const matrix4f& left, const matrix4f& right)
			{
				matrix4f result;
				for (uint32 i = 0; i < 4; i++)
				{
					for (uint32 i2 = 0; i2 < 4; i2++)
					{
						float32 Total = 0.0f;
						for (uint32 i3 = 0; i3 < 4; i3++)
						{
							//Total += left[i][i3] * right[i3][i2];
							Total += left[i3][i] * right[i2][i3];
						}
						//result[i][i2] = Total;
						result[i2][i] = Total;
					}
				}
				return result;
			}

			friend matrix4f operator*(const matrix4f& left, float32& right)
			{
				matrix4f result;
				for (uint32 i = 0; i < 4; i++)
				{
					for (uint32 i2 = 0; i2 < 4; i2++)
					{
						result[i][i2] = left[i][i2] * right;
					}
				}
				return result;
			}

			friend matrix4f operator*(float32& left, const matrix4f& right)
			{
				matrix4f result;
				for (uint32 i = 0; i < 4; i++)
				{
					for (uint32 i2 = 0; i2 < 4; i2++)
					{
						result[i][i2] = right[i][i2] * left;
					}
				}
				return result;
			}

			friend Vector4f operator*(const Vector4f& vector, const matrix4f& m) {
				Vector4f result;
				for (int i = 0; i < 4; ++i) {
					result.ptr[i] = 0.0f;
					for (int j = 0; j < 4; ++j) {
						//result.ptr[i] += vector.ptr[j] * m.data[j * 4 + i]; // Notez le changement ici
						result.ptr[i] += vector.ptr[j] * m.data[i * 4 + j]; // Notez le changement ici
					}
				}
				return result;
			}

			friend Vector4f operator*(const matrix4f& m, const Vector4f& vector) {
				Vector4f result;
				for (int i = 0; i < 4; ++i) {
					result.ptr[i] = 0.0f;
					for (int j = 0; j < 4; ++j) {
						//result.ptr[i] += m.data[i * 4 + j] * vector.ptr[j];
						result.ptr[i] += m.data[j * 4 + i] * vector.ptr[j];
					}
				}
				return result;
			}

			friend matrix4f operator+(const matrix4f& left, const matrix4f& right)
			{
				matrix4f result;
				for (uint32 i = 0; i < 4; i++)
				{
					for (uint32 i2 = 0; i2 < 4; i2++)
					{
						result[i][i2] = left[i][i2] + right[i][i2];
					}
				}
				return result;
			}

			friend matrix4f operator-(const matrix4f& left, const matrix4f& right)
			{
				matrix4f result;
				for (uint32 i = 0; i < 4; i++)
				{
					for (uint32 i2 = 0; i2 < 4; i2++)
					{
						result[i][i2] = left[i][i2] - right[i][i2];
					}
				}
				return result;
			}

			matrix4f Transpose() const;
			float32 Determinant() const;
			float32 Cofactor3x3(int row, int col) const;
			float32 Determinant3x3(const float32 submatrix[9]) const;
			Vector3f TransformPoint(const Vector3f& point) const;
			Vector3f TransformVector(const Vector3f& vector) const;
			Vector3f TransformNormal(const Vector3f& normal) const;
			matrix4f Inverse() const;

			Vector3f Forward() const;
			Vector3f Up() const;
			Vector3f Right() const;
			Vector3f Position() const;

			void Forward(const Vector3f& forward);
			void Up(const Vector3f& up);
			void Right(const Vector3f& right);
			void Position(const Vector3f& position);

			static float32 CompareMatrices(const matrix4f& left, const matrix4f& right);
			static matrix4f Identity();
			static matrix4f Camera(const Vector3f& eye, const Vector3f& _look, const Vector3f& _up, const Vector3f& _right);
			static matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
			static matrix4f Orthogonal(float32 width, float32 height, float32 zNear, float32 zFar);
			static matrix4f Perspective(float32 width, float32 height, float32 zNear, float32 zFar);
			static matrix4f PerspectiveFov(const Angle& fov, float32 aspect, float32 zNear, float32 zFar);
			static matrix4f PerspectiveMultifov(const Angle& fovX, const Angle& fovY, float32 zNear, float32 zFar);

			static matrix4f Rotation(const Vector3f& axis, const Angle& angle);
			static matrix4f Rotation(const Angle& pitch, const Angle& yaw, const Angle& roll);
			static matrix4f Rotation(const Vector3f& axis, const Angle& angle, const Vector3f& Center);
			static matrix4f RotationX(const Angle& angle);
			static matrix4f RotationY(const Angle& angle);
			static matrix4f RotationZ(const Angle& angle);
			static matrix4f Scaling(const Vector3f& scaleFactors);
			static matrix4f Translation(const Vector3f& pos);
			static matrix4f Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 zNear, float32 zFar);

			static matrix4f Rotation(const matrix4f mat, const Vector3f& axis, const Angle& angle);
			static matrix4f Rotation(const matrix4f mat, const Angle& pitch, const Angle& yaw, const Angle& roll);
			static matrix4f Rotation(const matrix4f mat, const Vector3f& axis, const Angle& angle, const Vector3f& center);
			static matrix4f RotationX(const matrix4f mat, const Angle& angle);
			static matrix4f RotationY(const matrix4f mat, const Angle& angle);
			static matrix4f RotationZ(const matrix4f mat, const Angle& angle);
			static matrix4f Scaling(const matrix4f mat, const Vector3f& scaleFactors);
			static matrix4f Translation(const matrix4f mat, const Vector3f& pos);

			static matrix4f FromEuler(const Vector3f& angle);
			static matrix4f FromEuler(const Angle& yaw, const Angle& pitch, const Angle& roll);

			std::string ToStringRow(uint32 i) const;
			std::string ToStringCol(uint32 i) const;
			std::string ToString() const;

			friend std::string ToString(const matrix4f& mat) {
				return mat.ToString();
			}

			friend std::ostream& operator<<(std::ostream& os, const matrix4f& e);
		};

		class NKENTSEU_API matrix4d {
		public:
			union {
				float64 data[16];
				float64 mat[4][4];
				struct {
					float64 m00, m01, m02, m03;
					float64 m10, m11, m12, m13;
					float64 m20, m21, m22, m23;
					float64 m30, m31, m32, m33;
				};
			};

			matrix4d();
			matrix4d(const Vector3d& V0, const Vector3d& V1, const Vector3d& V2);
			matrix4d(const matrix4d& other);
			matrix4d(float64 value);
			matrix4d(const float64* data);
			matrix4d(float64 m00, float64 m01, float64 m02, float64 m03,
				float64 m10, float64 m11, float64 m12, float64 m13,
				float64 m20, float64 m21, float64 m22, float64 m23,
				float64 m30, float64 m31, float64 m32, float64 m33);
			float64 Get(int row, int col) const;
			void Set(int row, int col, float64 value);
			float64* operator[](int row);
			const float64* operator[](int row) const;
			float64* Ptr();
			const float64* Ptr() const;
			matrix4d& operator=(const matrix4d& other);
			matrix4d& operator=(const float64* otherData);
			bool operator==(const matrix4d& other) const;
			bool operator!=(const matrix4d& other) const;
			matrix4d& operator*=(const matrix4d& other);
			matrix4d& operator*=(float64 scalar);
			friend matrix4d operator*(const matrix4d& left, const matrix4d& right);
			friend matrix4d operator*(const matrix4d& left, float64& right);
			friend matrix4d operator*(float64& left, const matrix4d& right);
			friend matrix4d operator+(const matrix4d& left, const matrix4d& right);
			friend matrix4d operator-(const matrix4d& left, const matrix4d& right);
			matrix4d Transpose() const;
			float64 Determinant() const;
			float64 Cofactor3x3(int row, int col) const;
			float64 Determinant3x3(const float64 submatrix[9]) const;
			Vector3d TransformPoint(const Vector3d& point) const;
			Vector3d TransformVector(const Vector3d& vector) const;
			Vector3d TransformNormal(const Vector3d& normal) const;
			matrix4d Inverse() const;
			Vector4d operator*(const Vector4d& vector) const;
			friend Vector4d operator*(const Vector4d& vector, const matrix4d& m);
			friend Vector4d operator*(const matrix4d& m, const Vector4d& vector);
			static float64 CompareMatrices(const matrix4d& left, const matrix4d& right);
			static matrix4d Identity();
			static matrix4d Camera(const Vector3d& eye, const Vector3d& _look, const Vector3d& _up, const Vector3d& _right);
			static matrix4d LookAt(const Vector3d& eye, const Vector3d& center, const Vector3d& up);
			static matrix4d Orthogonal(float64 width, float64 height, float64 zNear, float64 zFar);
			static matrix4d Perspective(float64 width, float64 height, float64 zNear, float64 zFar);
			static matrix4d PerspectiveFov(float64 fov, float64 aspect, float64 zNear, float64 zFar);
			static matrix4d PerspectiveMultifov(float64 fovX, float64 fovY, float64 zNear, float64 zFar);
			static matrix4d Rotation(const Vector3d& Axis, const Angle& angle);
			static matrix4d Rotation(float64 yaw, float64 pitch, float64 roll);
			static matrix4d Rotation(const Vector3d& Axis, const Angle& angle, const Vector3d& Center);
			static matrix4d RotationX(const Angle& angle);
			static matrix4d RotationY(const Angle& angle);
			static matrix4d RotationZ(const Angle& angle);
			static matrix4d Scaling(const Vector3d& scaleFactors);
			static matrix4d Translation(const Vector3d& pos);
			static matrix4d Frustum(float64 left, float64 right, float64 bottom, float64 top, float64 zNear, float64 zFar);

			std::string ToStringRow(uint32 i);
			std::string ToStringCol(uint32 i);
			std::string ToString() const;
			friend std::ostream& operator<<(std::ostream& os, const matrix4d& e);
		};

		using Matrix4d = matrix4d;
		using Matrice4d = matrix4d;
		using matrice4d = matrix4d;
		using Mat4d = matrix4d;
		using mat4d = matrix4d;

		using Matrix4f = matrix4f;
		using Matrix4 = matrix4f;
		using matrix4 = matrix4f;
		using Matrice4 = matrix4f;
		using matrice4 = matrix4f;
		using Mat4 = matrix4f;
		using mat4 = matrix4f;

		/*
		enum class NKENTSEU_API Direction {
			Forward, Back, Left, Right, Up, Bottom
		};

		enum class NKENTSEU_API AxisTransform {
			X, Y, Z
		};

		matrix4f NKENTSEU_API translate(const matrix4f& mat, const Vector3f& target);
		matrix4f NKENTSEU_API translate(const matrix4f& mat, const Vector3f& direction, float32 distance);
		matrix4f NKENTSEU_API translate(const matrix4f& mat, Direction direction, float32 distance);

		matrix4f NKENTSEU_API scale(const matrix4f& mat, const Vector3f& scale);
		matrix4f NKENTSEU_API scale(const matrix4f& mat, const Vector3f& direction, float32 scale);
		matrix4f NKENTSEU_API scale(const matrix4f& mat, Direction direction, float32 scale);

		matrix4f NKENTSEU_API rotation(const matrix4f& mat, const Angle& pitch, const Angle& yaw, const Angle& roll);
		matrix4f NKENTSEU_API rotation(const matrix4f& mat, const Vector3f& angle);
		matrix4f NKENTSEU_API rotation(const matrix4f& mat, const Vector3f& axis, const Angle& angle);
		matrix4f NKENTSEU_API rotation(const matrix4f& mat, AxisTransform axis, const Angle& angle);
		matrix4f NKENTSEU_API rotation(const matrix4f& mat, const Vector3d& axis, const Angle& angle, const Vector3d& center);

		matrix4f NKENTSEU_API transpose(const matrix4f& mat);
		matrix4f NKENTSEU_API inverse(const matrix4f& mat);
		matrix4f NKENTSEU_API inverse(const matrix4f& mat);

		matrix4f NKENTSEU_API lookAt(const Vector3d& eye, const Vector3d& center, const Vector3d& up);
		matrix4f NKENTSEU_API lookAt(const Vector3d& eye, const Vector3d& look, const Vector3d& up, const Vector3d& right);

		matrix4f NKENTSEU_API orthogonal(float64 width, float64 height, float64 zNear, float64 zFar);
		matrix4f NKENTSEU_API orthogonal(const Vector2u& size, float64 zNear, float64 zFar);
		matrix4f NKENTSEU_API perspective(const Vector2u& size, float64 zNear, float64 zFar);
		matrix4f NKENTSEU_API perspective(float64 fov, float64 aspect, float64 zNear, float64 zFar);
		*/
	}
} // namespace nkentseu

#endif    // __NKENTSEU_MATRIX4_H__