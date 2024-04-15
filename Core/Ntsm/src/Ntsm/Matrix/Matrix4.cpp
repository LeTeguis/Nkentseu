//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 9:26:18 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Matrix4.h"

#include <cstring>

namespace nkentseu {

	Matrix4f::Matrix4f(float32 a) : xx(a), xy(float32(0)), xz(float32(0)), xw(float32(0)),
		yx(float32(0)), yy(a), yz(float32(0)), yw(float32(0)),
		zx(float32(0)), zy(float32(0)), zz(a), zw(float32(0)),
		wx(float32(0)), wy(float32(0)), wz(float32(0)), ww(a) {}

	Matrix4f::Matrix4f(float32* fv) : xx(fv[0]), xy(fv[1]), xz(fv[2]), xw(fv[3]),
		yx(fv[4]), yy(fv[5]), yz(fv[6]), yw(fv[7]),
		zx(fv[8]), zy(fv[9]), zz(fv[10]), zw(fv[11]),
		wx(fv[12]), wy(fv[13]), wz(fv[14]), ww(fv[15]) {}

	Matrix4f::Matrix4f(float32 _00, float32 _01, float32 _02, float32 _03,
		float32 _10, float32 _11, float32 _12, float32 _13,
		float32 _20, float32 _21, float32 _22, float32 _23,
		float32 _30, float32 _31, float32 _32, float32 _33) :
		xx(_00), xy(_01), xz(_02), xw(_03),
		yx(_10), yy(_11), yz(_12), yw(_13),
		zx(_20), zy(_21), zz(_22), zw(_23),
		wx(_30), wy(_31), wz(_32), ww(_33) {}

	Matrix4f::Matrix4f(Vector4f _right, Vector4f _up, Vector4f _forward, Vector4f _position) {
		right(_right);
		up(_up);
		forward(_forward);
		position(_position);
	}

	Matrix4f::Matrix4f(const Matrix4f& mat){
		right(mat.ptr_right);
		up(mat.ptr_up);
		forward(mat.ptr_forward);
		position(mat.ptr_position);
	}

	Matrix4f::operator float32* () {
		return &ptr[0];
	}

	bool operator==(const Matrix4f& a, const Matrix4f& b) {
		for (int i = 0; i < 16; ++i) {
			if (maths::Abs(a.ptr[i] - b.ptr[i]) > maths::MatrixEpsilon()) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Matrix4f& a, const Matrix4f& b) {
		return !(a == b);
	}

	Matrix4f operator+(const Matrix4f& a, const Matrix4f& b) {
		return Matrix4f(
			a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
			a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
			a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
			a.wx + b.wx, a.wy + b.wy, a.wz + b.wz, a.ww + b.ww
		);
	}

	Matrix4f operator*(const Matrix4f& m, float f) {
		return Matrix4f(
			m.xx * f, m.xy * f, m.xz * f, m.xw * f,
			m.yx * f, m.yy * f, m.yz * f, m.yw * f,
			m.zx * f, m.zy * f, m.zz * f, m.zw * f,
			m.wx * f, m.wy * f, m.wz * f, m.ww * f
		);
	}

	Matrix4f operator*(const Matrix4f& a, const Matrix4f& b) {
		return Matrix4f(
			M4D(0, 0, a, b), M4D(1, 0, a, b), M4D(2, 0, a, b), M4D(3, 0, a, b),//Col0
			M4D(0, 1, a, b), M4D(1, 1, a, b), M4D(2, 1, a, b), M4D(3, 1, a, b),//Col1
			M4D(0, 2, a, b), M4D(1, 2, a, b), M4D(2, 2, a, b), M4D(3, 2, a, b),//Col2
			M4D(0, 3, a, b), M4D(1, 3, a, b), M4D(2, 3, a, b), M4D(3, 3, a, b) //Col3
		);
	}

	/*Vector4f operator*(const Matrix4f& m, const Vector4f& v) {
		return Vector4f(
			M4V4D(0, v.x, v.y, v.z, v.w, m),
			M4V4D(1, v.x, v.y, v.z, v.w, m),
			M4V4D(2, v.x, v.y, v.z, v.w, m),
			M4V4D(3, v.x, v.y, v.z, v.w, m)
		);
	}*/

	Vector3f Matrix4f::TransformVector(const Vector3f& v) {
		return Vector3f(
			M4V4D(0, v.x, v.y, v.z, 0.0f, (*this)),
			M4V4D(1, v.x, v.y, v.z, 0.0f, (*this)),
			M4V4D(2, v.x, v.y, v.z, 0.0f, (*this))
		);
	}

	Vector3f Matrix4f::TransformPoint(const Vector3f& v) {
		return Vector3f(
			M4V4D(0, v.x, v.y, v.z, 1.0f, (*this)),
			M4V4D(1, v.x, v.y, v.z, 1.0f, (*this)),
			M4V4D(2, v.x, v.y, v.z, 1.0f, (*this))
		);
	}

	Vector3f Matrix4f::TransformPoint(const Vector3f& v, float32& w) {
		float32 _w = w;
		w = M4V4D(3, v.x, v.y, v.z, _w, (*this));
		return Vector3f(
			M4V4D(0, v.x, v.y, v.z, _w, (*this)),
			M4V4D(1, v.x, v.y, v.z, _w, (*this)),
			M4V4D(2, v.x, v.y, v.z, _w, (*this))
		);
	}

	void Matrix4f::Transpose() {
		M4SWAP(yx, xy);
		M4SWAP(zx, xz);
		M4SWAP(wx, xw);
		M4SWAP(zy, yz);
		M4SWAP(wy, yw);
		M4SWAP(wz, zw);
	}

	Matrix4f Matrix4f::Transposed() {
		return Matrix4f(
			xx, yx, zx, wx,
			xy, yy, zy, wy,
			xz, yz, zz, wz,
			xw, yw, zw, ww
		);
	}

	float32 Matrix4f::Determinant() {
		return  ptr[0] * M4_3X3MINOR(ptr, 1, 2, 3, 1, 2, 3)
			- ptr[4] * M4_3X3MINOR(ptr, 0, 2, 3, 1, 2, 3)
			+ ptr[8] * M4_3X3MINOR(ptr, 0, 1, 3, 1, 2, 3)
			- ptr[12] * M4_3X3MINOR(ptr, 0, 1, 2, 1, 2, 3);
	}

	Matrix4f Matrix4f::Adjugate() {
		//Cof (M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
		Matrix4f cofactor;
		cofactor.ptr[0] = M4_3X3MINOR(ptr, 1, 2, 3, 1, 2, 3);
		cofactor.ptr[1] = -M4_3X3MINOR(ptr, 1, 2, 3, 0, 2, 3);
		cofactor.ptr[2] = M4_3X3MINOR(ptr, 1, 2, 3, 0, 1, 3);
		cofactor.ptr[3] = -M4_3X3MINOR(ptr, 1, 2, 3, 0, 1, 2);
		cofactor.ptr[4] = -M4_3X3MINOR(ptr, 0, 2, 3, 1, 2, 3);
		cofactor.ptr[5] = M4_3X3MINOR(ptr, 0, 2, 3, 0, 2, 3);
		cofactor.ptr[6] = -M4_3X3MINOR(ptr, 0, 2, 3, 0, 1, 3);
		cofactor.ptr[7] = M4_3X3MINOR(ptr, 0, 2, 3, 0, 1, 2);
		cofactor.ptr[8] = M4_3X3MINOR(ptr, 0, 1, 3, 1, 2, 3);
		cofactor.ptr[9] = -M4_3X3MINOR(ptr, 0, 1, 3, 0, 2, 3);
		cofactor.ptr[10] = M4_3X3MINOR(ptr, 0, 1, 3, 0, 1, 3);
		cofactor.ptr[11] = -M4_3X3MINOR(ptr, 0, 1, 3, 0, 1, 2);
		cofactor.ptr[12] = -M4_3X3MINOR(ptr, 0, 1, 2, 1, 2, 3);
		cofactor.ptr[13] = M4_3X3MINOR(ptr, 0, 1, 2, 0, 2, 3);
		cofactor.ptr[14] = -M4_3X3MINOR(ptr, 0, 1, 2, 0, 1, 3);
		cofactor.ptr[15] = M4_3X3MINOR(ptr, 0, 1, 2, 0, 1, 2);
		return cofactor.Transposed();
	}

	Matrix4f Matrix4f::Inverse() {
		float32 det = Determinant();
		if (det == 0.0f) {
			return Matrix4f();
		}
		Matrix4f adj = Adjugate();
		return adj * (1.0f / det);
	}

	void Matrix4f::Invert() {
		float32 det = Determinant();
		if (det == 0.0f) {
			*this = Matrix4f();
			return;
		}
		*this = Adjugate() * (1.0f / det);
	}

	Matrix4f Matrix4f::Frustum(float32 l, float32 r, float32 b, float32 t, float32 n, float32 f) {
		if (l == r || t == b || n == f) {
			return Matrix4f();
		}
		return Matrix4f(
			(2.0f * n) / (r - l), 0, 0, 0,
			0, (2.0f * n) / (t - b), 0, 0,
			(r + l) / (r - l), (t + b) / (t - b), (-(f + n)) / (f - n), -1,
			0, 0, (-2 * f * n) / (f - n), 0
		);
	}

	Matrix4f Matrix4f::Perspective(float32 fov, float32 aspect, float32 n, float32 f) {
		float32 ymax = n * maths::Tan(fov * maths::Pi() / 360.0f);
		float32 xmax = ymax * aspect;
		return Frustum(-xmax, xmax, -ymax, ymax, n, f);
	}

	Matrix4f Matrix4f::Orthogonal(float32 l, float32 r, float32 b, float32 t, float32 n, float32 f) {
		if (l == r || t == b || n == f) {
			return Matrix4f(); // Error
		}
		return Matrix4f(
			2.0f / (r - l), 0, 0, 0,
			0, 2.0f / (t - b), 0, 0,
			0, 0, -2.0f / (f - n), 0,
			-((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / (f - n)), 1
		);
	}

	Matrix4f Matrix4f::LookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up) {
		Vector3f f = (target - position).Normalized() * -1.0f;
		Vector3f r = Vector3f(up).Cross(f); // Right handed

		if (r == Vector3f(0, 0, 0)) {
			return Matrix4f(); // Error
		}
		r.Normalize();
		Vector3f u = f.Cross(r).Normalized(); // Right handed
		Vector3f t = Vector3f(-r.Dot(position), -u.Dot(position), -f.Dot(position));
		return Matrix4f(
			// Transpose upper 3x3 matrix to invert it
			r.x, u.x, f.x, 0,
			r.y, u.y, f.y, 0,
			r.z, u.z, f.z, 0,
			t.x, t.y, t.z, 1
		);
	}

	std::string Matrix4f::ToStringRow(int i) {
		std::stringstream ss;
		if (i == 0)
			ss << "[" << xx << ", " << xy << ", " << xz << ", " << xw << "]";
		if (i == 1)
			ss << "[" << yx << ", " << yy << ", " << yz << ", " << yw << "]";
		if (i == 2)
			ss << "[" << zx << ", " << zy << ", " << zz << ", " << zw << "]";
		if (i == 3)
			ss << "[" << wx << ", " << wy << ", " << wz << ", " << ww << "]";
		return ss.str();
	}

	std::string Matrix4f::ToStringCol(int i) {
		std::stringstream ss;
		if (i == 0)
			ss << "[" << xx << ", " << yx << ", " << zx << ", " << wx << "]";
		if (i == 1)
			ss << "[" << xy << ", " << yy << ", " << zy << ", " << wy << "]";
		if (i == 2)
			ss << "[" << xz << ", " << yz << ", " << zz << ", " << wz << "]";
		if (i == 3)
			ss << "[" << xw << ", " << yw << ", " << zw << ", " << ww << "]";
		return ss.str();
	}

	std::string Matrix4f::ToString() const {
		std::stringstream ss;
		ss << "Matrix4f";
		ss << "[[" << xx << ", " << yx << ", " << zx << ", " << wx << "], ";
		ss << "[" << xy << ", " << yy << ", " << zy << ", " << wy << "], ";
		ss << "[" << xz << ", " << yz << ", " << zz << ", " << wz << "], ";
		ss << "[" << xw << ", " << yw << ", " << zw << ", " << ww << "]]";
		return ss.str();
	}

	std::ostream& operator<<(std::ostream& os, const Matrix4f& e) {
		return os << e.ToString();
	}

	Matrix4f& Matrix4f::operator=(const Matrix4f& other) {
		if (this != &other) {
			memcpy(ptr, other.ptr, sizeof(float32) * 16);
		}
		return *this;
	}


	Matrix4d::Matrix4d(float64 a) : xx(a), xy(float64(0)), xz(float64(0)), xw(float64(0)),
		yx(float64(0)), yy(a), yz(float64(0)), yw(float64(0)),
		zx(float64(0)), zy(float64(0)), zz(a), zw(float64(0)),
		wx(float64(0)), wy(float64(0)), wz(float64(0)), ww(a) {}

	Matrix4d::Matrix4d(float64* fv) : xx(fv[0]), xy(fv[1]), xz(fv[2]), xw(fv[3]),
		yx(fv[4]), yy(fv[5]), yz(fv[6]), yw(fv[7]),
		zx(fv[8]), zy(fv[9]), zz(fv[10]), zw(fv[11]),
		wx(fv[12]), wy(fv[13]), wz(fv[14]), ww(fv[15]) {}

	Matrix4d::Matrix4d(float64 _00, float64 _01, float64 _02, float64 _03,
		float64 _10, float64 _11, float64 _12, float64 _13,
		float64 _20, float64 _21, float64 _22, float64 _23,
		float64 _30, float64 _31, float64 _32, float64 _33) :
		xx(_00), xy(_01), xz(_02), xw(_03),
		yx(_10), yy(_11), yz(_12), yw(_13),
		zx(_20), zy(_21), zz(_22), zw(_23),
		wx(_30), wy(_31), wz(_32), ww(_33) {}

	Matrix4d::Matrix4d(Vector4d _right, Vector4d _up, Vector4d _forward, Vector4d _position) {
		right(_right);
		up(_up);
		forward(_forward);
		position(_position);
	}

	Matrix4d::Matrix4d(const Matrix4d& mat) {
		right(mat.ptr_right);
		up(mat.ptr_up);
		forward(mat.ptr_forward);
		position(mat.ptr_position);
	}

	Matrix4d::operator float64* () {
		return &ptr[0];
	}

	bool operator==(const Matrix4d& a, const Matrix4d& b) {
		for (int i = 0; i < 16; ++i) {
			if (maths::Abs(a.ptr[i] - b.ptr[i]) > maths::MatrixEpsilon()) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Matrix4d& a, const Matrix4d& b) {
		return !(a == b);
	}

	Matrix4d operator+(const Matrix4d& a, const Matrix4d& b) {
		return Matrix4d(
			a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
			a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
			a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
			a.wx + b.wx, a.wy + b.wy, a.wz + b.wz, a.ww + b.ww
		);
	}

	Matrix4d operator*(const Matrix4d& m, float f) {
		return Matrix4d(
			m.xx * f, m.xy * f, m.xz * f, m.xw * f,
			m.yx * f, m.yy * f, m.yz * f, m.yw * f,
			m.zx * f, m.zy * f, m.zz * f, m.zw * f,
			m.wx * f, m.wy * f, m.wz * f, m.ww * f
		);
	}

	Matrix4d operator*(const Matrix4d& a, const Matrix4d& b) {
		return Matrix4d(
			M4D(0, 0, a, b), M4D(1, 0, a, b), M4D(2, 0, a, b), M4D(3, 0, a, b),//Col0
			M4D(0, 1, a, b), M4D(1, 1, a, b), M4D(2, 1, a, b), M4D(3, 1, a, b),//Col1
			M4D(0, 2, a, b), M4D(1, 2, a, b), M4D(2, 2, a, b), M4D(3, 2, a, b),//Col2
			M4D(0, 3, a, b), M4D(1, 3, a, b), M4D(2, 3, a, b), M4D(3, 3, a, b) //Col3
		);
	}

	Vector4d operator*(const Matrix4d& m, const Vector4d& v) {
		return Vector4d(
			M4V4D(0, v.x, v.y, v.z, v.w, m),
			M4V4D(1, v.x, v.y, v.z, v.w, m),
			M4V4D(2, v.x, v.y, v.z, v.w, m),
			M4V4D(3, v.x, v.y, v.z, v.w, m)
		);
	}

	Vector3d Matrix4d::TransformVector(const Vector3d& v) {
		return Vector3d(
			M4V4D(0, v.x, v.y, v.z, 0.0f, (*this)),
			M4V4D(1, v.x, v.y, v.z, 0.0f, (*this)),
			M4V4D(2, v.x, v.y, v.z, 0.0f, (*this))
		);
	}

	Vector3d Matrix4d::TransformPoint(const Vector3d& v) {
		return Vector3d(
			M4V4D(0, v.x, v.y, v.z, 1.0f, (*this)),
			M4V4D(1, v.x, v.y, v.z, 1.0f, (*this)),
			M4V4D(2, v.x, v.y, v.z, 1.0f, (*this))
		);
	}

	Vector3d Matrix4d::TransformPoint(const Vector3d& v, float64& w) {
		float64 _w = w;
		w = M4V4D(3, v.x, v.y, v.z, _w, (*this));
		return Vector3d(
			M4V4D(0, v.x, v.y, v.z, _w, (*this)),
			M4V4D(1, v.x, v.y, v.z, _w, (*this)),
			M4V4D(2, v.x, v.y, v.z, _w, (*this))
		);
	}

	void Matrix4d::Transpose() {
		M4SWAP(yx, xy);
		M4SWAP(zx, xz);
		M4SWAP(wx, xw);
		M4SWAP(zy, yz);
		M4SWAP(wy, yw);
		M4SWAP(wz, zw);
	}

	Matrix4d Matrix4d::Transposed() {
		return Matrix4d(
			xx, yx, zx, wx,
			xy, yy, zy, wy,
			xz, yz, zz, wz,
			xw, yw, zw, ww
		);
	}

	float64 Matrix4d::Determinant() {
		return  ptr[0] * M4_3X3MINOR(ptr, 1, 2, 3, 1, 2, 3)
			- ptr[4] * M4_3X3MINOR(ptr, 0, 2, 3, 1, 2, 3)
			+ ptr[8] * M4_3X3MINOR(ptr, 0, 1, 3, 1, 2, 3)
			- ptr[12] * M4_3X3MINOR(ptr, 0, 1, 2, 1, 2, 3);
	}

	Matrix4d Matrix4d::Adjugate() {
		//Cof (M[i, j]) = Minor(M[i, j]] * pow(-1, i + j)
		Matrix4d cofactor;
		cofactor.ptr[0] = M4_3X3MINOR(ptr, 1, 2, 3, 1, 2, 3);
		cofactor.ptr[1] = -M4_3X3MINOR(ptr, 1, 2, 3, 0, 2, 3);
		cofactor.ptr[2] = M4_3X3MINOR(ptr, 1, 2, 3, 0, 1, 3);
		cofactor.ptr[3] = -M4_3X3MINOR(ptr, 1, 2, 3, 0, 1, 2);
		cofactor.ptr[4] = -M4_3X3MINOR(ptr, 0, 2, 3, 1, 2, 3);
		cofactor.ptr[5] = M4_3X3MINOR(ptr, 0, 2, 3, 0, 2, 3);
		cofactor.ptr[6] = -M4_3X3MINOR(ptr, 0, 2, 3, 0, 1, 3);
		cofactor.ptr[7] = M4_3X3MINOR(ptr, 0, 2, 3, 0, 1, 2);
		cofactor.ptr[8] = M4_3X3MINOR(ptr, 0, 1, 3, 1, 2, 3);
		cofactor.ptr[9] = -M4_3X3MINOR(ptr, 0, 1, 3, 0, 2, 3);
		cofactor.ptr[10] = M4_3X3MINOR(ptr, 0, 1, 3, 0, 1, 3);
		cofactor.ptr[11] = -M4_3X3MINOR(ptr, 0, 1, 3, 0, 1, 2);
		cofactor.ptr[12] = -M4_3X3MINOR(ptr, 0, 1, 2, 1, 2, 3);
		cofactor.ptr[13] = M4_3X3MINOR(ptr, 0, 1, 2, 0, 2, 3);
		cofactor.ptr[14] = -M4_3X3MINOR(ptr, 0, 1, 2, 0, 1, 3);
		cofactor.ptr[15] = M4_3X3MINOR(ptr, 0, 1, 2, 0, 1, 2);
		return cofactor.Transposed();
	}

	Matrix4d Matrix4d::Inverse() {
		float64 det = Determinant();
		if (det == 0.0f) {
			return Matrix4d();
		}
		Matrix4d adj = Adjugate();
		return adj * (1.0f / det);
	}

	void Matrix4d::Invert() {
		float64 det = Determinant();
		if (det == 0.0f) {
			*this = Matrix4d();
			return;
		}
		*this = Adjugate() * (1.0f / det);
	}

	Matrix4d Matrix4d::Frustum(float64 l, float64 r, float64 b, float64 t, float64 n, float64 f) {
		if (l == r || t == b || n == f) {
			return Matrix4d();
		}
		return Matrix4d(
			(2.0f * n) / (r - l), 0, 0, 0,
			0, (2.0f * n) / (t - b), 0, 0,
			(r + l) / (r - l), (t + b) / (t - b), (-(f + n)) / (f - n), -1,
			0, 0, (-2 * f * n) / (f - n), 0
		);
	}

	Matrix4d Matrix4d::Perspective(float64 fov, float64 aspect, float64 n, float64 f) {
		float64 ymax = n * maths::Tan(fov * maths::Pi() / 360.0f);
		float64 xmax = ymax * aspect;
		return Frustum(-xmax, xmax, -ymax, ymax, n, f);
	}

	Matrix4d Matrix4d::Orthogonal(float64 l, float64 r, float64 b, float64 t, float64 n, float64 f) {
		if (l == r || t == b || n == f) {
			return Matrix4d(); // Error
		}
		return Matrix4d(
			2.0f / (r - l), 0, 0, 0,
			0, 2.0f / (t - b), 0, 0,
			0, 0, -2.0f / (f - n), 0,
			-((r + l) / (r - l)), -((t + b) / (t - b)), -((f + n) / (f - n)), 1
		);
	}

	Matrix4d Matrix4d::LookAt(const Vector3d& position, const Vector3d& target, const Vector3d& up) {
		Vector3d f = (target - position).Normalized() * -1.0f;
		Vector3d r = Vector3d(up).Cross(f); // Right handed

		if (r == Vector3d(0, 0, 0)) {
			return Matrix4d(); // Error
		}
		r.Normalize();
		Vector3d u = f.Cross(r).Normalized(); // Right handed
		Vector3d t = Vector3d(-r.Dot(position), -u.Dot(position), -f.Dot(position));
		return Matrix4d(
			// Transpose upper 3x3 matrix to invert it
			r.x, u.x, f.x, 0,
			r.y, u.y, f.y, 0,
			r.z, u.z, f.z, 0,
			t.x, t.y, t.z, 1
		);
	}

	std::string Matrix4d::ToStringRow(int i) {
		std::stringstream ss;
		if (i == 0)
			ss << "[" << xx << ", " << xy << ", " << xz << ", " << xw << "]";
		if (i == 1)
			ss << "[" << yx << ", " << yy << ", " << yz << ", " << yw << "]";
		if (i == 2)
			ss << "[" << zx << ", " << zy << ", " << zz << ", " << zw << "]";
		if (i == 3)
			ss << "[" << wx << ", " << wy << ", " << wz << ", " << ww << "]";
		return ss.str();
	}

	std::string Matrix4d::ToStringCol(int i) {
		std::stringstream ss;
		if (i == 0)
			ss << "[" << xx << ", " << yx << ", " << zx << ", " << wx << "]";
		if (i == 1)
			ss << "[" << xy << ", " << yy << ", " << zy << ", " << wy << "]";
		if (i == 2)
			ss << "[" << xz << ", " << yz << ", " << zz << ", " << wz << "]";
		if (i == 3)
			ss << "[" << xw << ", " << yw << ", " << zw << ", " << ww << "]";
		return ss.str();
	}

	std::string Matrix4d::ToString() const {
		std::stringstream ss;
		ss << "Matrix4d";
		ss << "[[" << xx << ", " << yx << ", " << zx << ", " << wx << "], ";
		ss << "[" << xy << ", " << yy << ", " << zy << ", " << wy << "], ";
		ss << "[" << xz << ", " << yz << ", " << zz << ", " << wz << "], ";
		ss << "[" << xw << ", " << yw << ", " << zw << ", " << ww << "]]";
		return ss.str();
	}

	std::ostream& operator<<(std::ostream& os, const Matrix4d& e) {
		return os << e.ToString();
	}

	Matrix4d& Matrix4d::operator=(const Matrix4d& other) {
		if (this != &other) {
			memcpy(ptr, other.ptr, sizeof(float64) * 16);
		}
		return *this;
	}
}    // namespace nkentseu