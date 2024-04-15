//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:26:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MATRIX4_H__
#define __NKENTSEU_MATRIX4_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector4.h>

namespace nkentseu {

	class NKENTSEU_API Matrix4f {
	public:
		union {
			float32 ptr[16];

			struct {
				float32 ptr_right[4];
				float32 ptr_up[4];
				float32 ptr_forward[4];
				float32 ptr_position[4];
			};

			struct {
				// row 1 row 2 row 3 row 4
				/*col 1*/
				float32 xx; float32 xy; float32 xz; float32 xw;
				/*col 2*/
				float32 yx; float32 yy; float32 yz; float32 yw;
				/*col 3*/
				float32 zx; float32 zy; float32 zz; float32 zw;
				/*col 4*/
				float32 wx; float32 wy; float32 wz; float32 ww;
			};

			struct {
				float32 c0r0; float32 c0r1; float32 c0r2; float32 c0r3;
				float32 c1r0; float32 c1r1; float32 c1r2; float32 c1r3;
				float32 c2r0; float32 c2r1; float32 c2r2; float32 c2r3;
				float32 c3r0; float32 c3r1; float32 c3r2; float32 c3r3;
			};

			struct {
				float32 r0c0; float32 r1c0; float32 r2c0; float32 r3c0;
				float32 r0c1; float32 r1c1; float32 r2c1; float32 r3c1;
				float32 r0c2; float32 r1c2; float32 r2c2; float32 r3c2;
				float32 r0c3; float32 r1c3; float32 r2c3; float32 r3c3;
			};
		};

		Matrix4f(float32 a = float32(1));

		Matrix4f(float32* fv);

		Matrix4f(float32 _00, float32 _01, float32 _02, float32 _03,
			float32 _10, float32 _11, float32 _12, float32 _13,
			float32 _20, float32 _21, float32 _22, float32 _23,
			float32 _30, float32 _31, float32 _32, float32 _33);

		Matrix4f(Vector4f _right, Vector4f _up, Vector4f _forward, Vector4f _position);

		Matrix4f(const Matrix4f& mat);

		float32* operator[](const unsigned& rowNo) {
			if (rowNo % 3 == 0) return &ptr_right[0];
			if (rowNo % 3 == 1) return &ptr_up[0];
			if (rowNo % 3 == 1) return &ptr_forward[0];
			return &ptr_position[0];
		}

		operator float32* ();

		friend bool operator==(const Matrix4f& a, const Matrix4f& b);

		friend bool operator!=(const Matrix4f& a, const Matrix4f& b);

		friend Matrix4f operator+(const Matrix4f& a, const Matrix4f& b);

		friend Matrix4f operator*(const Matrix4f& m, float f);

		friend Matrix4f operator*(const Matrix4f& a, const Matrix4f& b);

		//friend Vector4f operator*(const Matrix4f& m, const Vector4f& v);

		// Vector4f * Matrix4f
		friend Vector4f operator*(const Vector4f& v4, const Matrix4f& mat1) {
			return Vector4f(
				v4.x * mat1.xx + v4.y * mat1.yx + v4.z * mat1.zx + v4.w * mat1.wx,
				v4.x * mat1.xy + v4.y * mat1.yy + v4.z * mat1.zy + v4.w * mat1.wy,
				v4.x * mat1.xz + v4.y * mat1.yz + v4.z * mat1.zz + v4.w * mat1.wz,
				v4.x * mat1.xw + v4.y * mat1.yw + v4.z * mat1.zw + v4.w * mat1.ww
			);
		}

		// Matrix4f * Vector4f
		friend Vector4f operator*(const Matrix4f& mat1, const Vector4f& v4) {
			return Vector4f(
				mat1.xx * v4.x + mat1.xy * v4.y + mat1.xz * v4.z + mat1.xw * v4.w,
				mat1.yx * v4.x + mat1.yy * v4.y + mat1.yz * v4.z + mat1.yw * v4.w,
				mat1.zx * v4.x + mat1.zy * v4.y + mat1.zz * v4.z + mat1.zw * v4.w,
				mat1.wx * v4.x + mat1.wy * v4.y + mat1.wz * v4.z + mat1.ww * v4.w
			);
		}

		Vector3f TransformVector(const Vector3f& v);

		Vector3f TransformPoint(const Vector3f& v);

		Vector3f TransformPoint(const Vector3f& v, float32& w);

		void Transpose();

		Matrix4f Transposed();

		float32 Determinant();

		Matrix4f Adjugate();

		Matrix4f Inverse();

		void Invert();

		static Matrix4f Frustum(float32 l, float32 r, float32 b, float32 t, float32 n, float32 f);

		static Matrix4f Perspective(float32 fov, float32 aspect, float32 n, float32 f);

		static Matrix4f Orthogonal(float32 l, float32 r, float32 b, float32 t, float32 n, float32 f);

		Matrix4f LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up);

		virtual std::string ToStringRow(int i);

		virtual std::string ToStringCol(int i);

		virtual std::string ToString() const;

		friend std::ostream& operator<<(std::ostream& os, const Matrix4f& e);

		Matrix4f& operator=(const Matrix4f& other);

		friend std::string ToString(const Matrix4f& v) {
			return v.ToString();
		}

		// Getters
		Vector4f right() { return Vector4f(xx, yx, zx, wx); }
		Vector4f up() { return Vector4f(xy, yy, zy, wy); }
		Vector4f forward() { return Vector4f(xz, yz, zz, wz); }
		Vector4f position() { return Vector4f(xw, yw, zw, ww); }

		// Const getters
		const Vector4f right() const { return Vector4f(xx, yx, zx, wx); }
		const Vector4f up() const { return Vector4f(xy, yy, zy, wy); }
		const Vector4f forward() const { return Vector4f(xz, yz, zz, wz); }
		const Vector4f position() const { return Vector4f(xw, yw, zw, ww); }

		// Setters
		void right(const Vector4f& _right) { xx = _right.x; yx = _right.y; zx = _right.z; wx = _right.w; }
		void up(const Vector4f& _up) { xy = _up.x; yy = _up.y; zy = _up.z; wy = _up.w; }
		void forward(const Vector4f& _forward) { xz = _forward.x; yz = _forward.y; zz = _forward.z; wz = _forward.w; }
		void position(const Vector4f& _position) { xw = _position.x; yw = _position.y; zw = _position.z; ww = _position.w; }
	};

	class NKENTSEU_API Matrix4d {
	public:
		union {
			float64 ptr[16];

			struct {
				float64 ptr_right[4];
				float64 ptr_up[4];
				float64 ptr_forward[4];
				float64 ptr_position[4];
			};

			struct {
				// row 1 row 2 row 3 row 4
				/*col 1*/
				float64 xx; float64 xy; float64 xz; float64 xw;
				/*col 2*/
				float64 yx; float64 yy; float64 yz; float64 yw;
				/*col 3*/
				float64 zx; float64 zy; float64 zz; float64 zw;
				/*col 4*/
				float64 wx; float64 wy; float64 wz; float64 ww;
			};

			struct {
				float64 c0r0; float64 c0r1; float64 c0r2; float64 c0r3;
				float64 c1r0; float64 c1r1; float64 c1r2; float64 c1r3;
				float64 c2r0; float64 c2r1; float64 c2r2; float64 c2r3;
				float64 c3r0; float64 c3r1; float64 c3r2; float64 c3r3;
			};

			struct {
				float64 r0c0; float64 r1c0; float64 r2c0; float64 r3c0;
				float64 r0c1; float64 r1c1; float64 r2c1; float64 r3c1;
				float64 r0c2; float64 r1c2; float64 r2c2; float64 r3c2;
				float64 r0c3; float64 r1c3; float64 r2c3; float64 r3c3;
			};
		};

		Matrix4d(float64 a = float64(1));

		Matrix4d(float64* fv);

		Matrix4d(float64 _00, float64 _01, float64 _02, float64 _03,
			float64 _10, float64 _11, float64 _12, float64 _13,
			float64 _20, float64 _21, float64 _22, float64 _23,
			float64 _30, float64 _31, float64 _32, float64 _33);

		Matrix4d(Vector4d _right, Vector4d _up, Vector4d _forward, Vector4d _position);

		Matrix4d(const Matrix4d& mat);

		float64* operator[](const unsigned& rowNo) {
			if (rowNo % 3 == 0) return &ptr_right[0];
			if (rowNo % 3 == 1) return &ptr_up[0];
			if (rowNo % 3 == 1) return &ptr_forward[0];
			return &ptr_position[0];
		}

		operator float64* ();

		friend bool operator==(const Matrix4d& a, const Matrix4d& b);

		friend bool operator!=(const Matrix4d& a, const Matrix4d& b);

		friend Matrix4d operator+(const Matrix4d& a, const Matrix4d& b);

		friend Matrix4d operator*(const Matrix4d& m, float f);

		friend Matrix4d operator*(const Matrix4d& a, const Matrix4d& b);

		friend Vector4d operator*(const Matrix4d& m, const Vector4d& v);

		Vector3d TransformVector(const Vector3d& v);

		Vector3d TransformPoint(const Vector3d& v);

		Vector3d TransformPoint(const Vector3d& v, float64& w);

		void Transpose();

		Matrix4d Transposed();

		float64 Determinant();

		Matrix4d Adjugate();

		Matrix4d Inverse();

		void Invert();

		static Matrix4d Frustum(float64 l, float64 r, float64 b, float64 t, float64 n, float64 f);

		static Matrix4d Perspective(float64 fov, float64 aspect, float64 n, float64 f);

		static Matrix4d Orthogonal(float64 l, float64 r, float64 b, float64 t, float64 n, float64 f);

		Matrix4d LookAt(const Vector3d& position, const Vector3d& target, const Vector3d& up);

		virtual std::string ToStringRow(int i);

		virtual std::string ToStringCol(int i);

		virtual std::string ToString() const;

		friend std::ostream& operator<<(std::ostream& os, const Matrix4d& e);

		Matrix4d& operator=(const Matrix4d& other);

		friend std::string ToString(const Matrix4d& v) {
			return v.ToString();
		}

		// Getters
		Vector4d right() { return Vector4d(xx, yx, zx, wx); }
		Vector4d up() { return Vector4d(xy, yy, zy, wy); }
		Vector4d forward() { return Vector4d(xz, yz, zz, wz); }
		Vector4d position() { return Vector4d(xw, yw, zw, ww); }

		// Const getters
		const Vector4d right() const { return Vector4d(xx, yx, zx, wx); }
		const Vector4d up() const { return Vector4d(xy, yy, zy, wy); }
		const Vector4d forward() const { return Vector4d(xz, yz, zz, wz); }
		const Vector4d position() const { return Vector4d(xw, yw, zw, ww); }

		// Setters
		void right(const Vector4d& _right) { xx = _right.x; yx = _right.y; zx = _right.z; wx = _right.w; }
		void up(const Vector4d& _up) { xy = _up.x; yy = _up.y; zy = _up.z; wy = _up.w; }
		void forward(const Vector4d& _forward) { xz = _forward.x; yz = _forward.y; zz = _forward.z; wz = _forward.w; }
		void position(const Vector4d& _position) { xw = _position.x; yw = _position.y; zw = _position.z; ww = _position.w; }
	};

	using Matrice4d = Matrix4d;
	using Mat4d = Matrix4d;

	using Matrix4 = Matrix4f;
	using Matrice4 = Matrix4f;
	using Mat4 = Matrix4f;
} // namespace nkentseu

#endif    // __NKENTSEU_MATRIX4_H__