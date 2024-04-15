//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 12:03:04 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Vector2.h"

namespace nkentseu {
	// [Vector2f]
	Vector2f::Vector2f() : x(0), y(0) {}
	Vector2f::Vector2f(float32 x, float32 y) : x(x), y(y) {}
	Vector2f::Vector2f(float32 v) : x(v), y(v) {}
	Vector2f::Vector2f(float32* v) : x(v[0]), y(v[1]) {}
	Vector2f::Vector2f(const Vector2f& v) : x(v.x), y(v.y) {}
	Vector2f::Vector2f(const Vector2i& v) : x((float32)v.x), y((float32)v.y) {}
	Vector2f::Vector2f(const Vector2u& v) : x((float32)v.x), y((float32)v.y) {}
	Vector2f::Vector2f(const Vector2d& v) : x((float32)v.x), y((float32)v.y) {}
	Vector2f::operator Vector2i() { return Vector2i(x, y); }
	Vector2f::operator Vector2u() { return Vector2u(x, y); }
	Vector2f::operator Vector2d() { return Vector2d(x, y); }

	Vector2f Vector2f::operator=(const Vector2f& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2f Vector2f::operator=(const Vector2i& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2f Vector2f::operator=(const Vector2u& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2f Vector2f::operator=(const Vector2d& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	float32 Vector2f::SumCord() {
		return x + y;
	}


	float32 Vector2f::Distance(const Vector2f& r) {
		return (r - (*this)).Len();
	}

	float32 Vector2f::Dot(const Vector2f& r) {
		return x * r.x + y * r.y;
	}

	float32 Vector2f::LenSq() {
		return x * x + y * y;
	}

	float32 Vector2f::Len() {
		float32 lenSq = x * x + y * y;
		if (lenSq < maths::VectorEpsilon()) {
			return 0.0f;
		}
		return maths::Sqrt(lenSq);
	}

	void Vector2f::Normalize() {
		float32 lenSq = x * x + y * y;

		if (lenSq < maths::VectorEpsilon()) {
			return;
		}

		float32 invLen = 1.0f / maths::Sqrt(lenSq);

		x *= invLen;
		y *= invLen;
	}

	Vector2f Vector2f::Normalized() {
		float32 lenSq = x * x + y * y;
		if (lenSq < maths::VectorEpsilon()) {
			return Vector2f();
		}

		float32 invLen = 1.0f / maths::Sqrt(lenSq);
		return Vector2f(x * invLen, y * invLen);
	}

	bool Vector2f::IsUnit() {
		return maths::EqualF32(Len(), 1.0);
	}

	Vector2f Vector2f::Rotate(const Angle& angle) {
		float32 radian = angle.Rad();
		float32 sine = maths::Sin(radian);
		float32 cosine = maths::Cos(radian);
		return Vector2f(x * cosine - y * sine, x * sine + y * cosine);
	}

	Angle Vector2f::AngleBetween(const Vector2f& b) {
		Vector2f ua = Normalized();
		Vector2f ub = Vector2f(b).Normalized();

		if (ua == Vector2f() || ub == Vector2f() || ua == ub) return Angle();

		float32 dp = ua.Dot(ub);
		return Angle::FromRadian(maths::ACos(dp));
	}

	Vector2f Vector2f::Project(const Vector2f& onto) {
		float32 magBSq = Vector2f(onto).Len();
		if (magBSq < maths::VectorEpsilon()) {
			return Vector2f();
		}
		float32 scale = Dot(onto) / magBSq;
		return onto * scale;
	}

	Vector2f Vector2f::Reject(const Vector2f& b) {
		Vector2f projection = Project(b);
		return (*this) - projection;
	}

	Vector2f Vector2f::Reflect(const Vector2f& b) {
		float32 magBSq = Vector2f(b).Len();
		if (magBSq < maths::VectorEpsilon()) {
			return Vector2f();
		}
		float32 scale = Dot(b) / magBSq;
		Vector2f proj2 = b * (scale * 2);
		return (*this) - proj2;
	}

	Vector2f Vector2f::Lerp(const Vector2f& e, float32 t) {
		return Vector2f(x + (e.x - x) * t,
			y + (e.y - y) * t);
	}

	Vector2f Vector2f::Slerp(const Vector2f& e, float32 t) {
		if (t < 0.01) {
			return Lerp(e, t);
		}

		Vector2f ee(e);

		Vector2f from = Normalized();
		Vector2f to = ee.Normalized();
		float32 theta = from.AngleBetween(to);
		float32 sin_theta = maths::Sin(theta);
		float32 a = maths::Sin((1.0f - t) * theta) / sin_theta;
		float32 b = maths::Sin(t * theta) / sin_theta;
		return from * a + to * b;
	}

	Vector2f Vector2f::Nlerp(const Vector2f& e, float32 t) {
		Vector2f linear(x + (e.x - x) * t,
			y + (e.y - y) * t);
		return linear.Normalized();
	}

	Vector2f Vector2f::Rotate90() {
		return Vector2f(-y, x);
	}

	Vector2f Vector2f::Rotate180() {
		return (-(*this));
	}

	Vector2f Vector2f::Rotate270() {
		return Vector2d(y, -x);
	}

	bool Vector2f::Collineaire(const Vector2f& b) {
		Vector2f na = Rotate90();
		return maths::EqualF32(0, na.Dot(b));
	}

	// [Vector2d]
	Vector2d::Vector2d() : x(0), y(0) {}
	Vector2d::Vector2d(float64 x, float64 y) : x(x), y(y) {}
	Vector2d::Vector2d(float64 v) : x(v), y(v) {}
	Vector2d::Vector2d(float64* v) : x(v[0]), y(v[1]) {}
	Vector2d::Vector2d(const Vector2f& v) : x(v.x), y(v.y) {}
	Vector2d::Vector2d(const Vector2i& v) : x((float64)v.x), y((float64)v.y) {}
	Vector2d::Vector2d(const Vector2u& v) : x((float64)v.x), y((float64)v.y) {}
	Vector2d::Vector2d(const Vector2d& v) : x((float64)v.x), y((float64)v.y) {}
	Vector2d::operator Vector2i() { return Vector2i(x, y); }
	Vector2d::operator Vector2u() { return Vector2u(x, y); }
	Vector2d::operator Vector2f() { return Vector2f(x, y); }

	Vector2d Vector2d::operator=(const Vector2f& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2d Vector2d::operator=(const Vector2i& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2d Vector2d::operator=(const Vector2u& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2d Vector2d::operator=(const Vector2d& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	float64 Vector2d::SumCord() {
		return x + y;
	}

	float64 Vector2d::Distance(const Vector2d& r) {
		return (r - (*this)).Len();
	}

	float64 Vector2d::Dot(const Vector2d& r) {
		return x * r.x + y * r.y;
	}

	float64 Vector2d::LenSq() {
		return x * x + y * y;
	}

	float64 Vector2d::Len() {
		float64 lenSq = x * x + y * y;
		if (lenSq < maths::VectorEpsilon()) {
			return 0.0f;
		}
		return maths::Sqrt(lenSq);
	}

	void Vector2d::Normalize() {
		float64 lenSq = x * x + y * y;

		if (lenSq < maths::VectorEpsilon()) {
			return;
		}

		float64 invLen = 1.0f / maths::Sqrt(lenSq);

		x *= invLen;
		y *= invLen;
	}

	Vector2d Vector2d::Normalized() {
		float64 lenSq = x * x + y * y;
		if (lenSq < maths::VectorEpsilon()) {
			return Vector2d();
		}

		float64 invLen = 1.0f / maths::Sqrt(lenSq);
		return Vector2d(x * invLen, y * invLen);
	}

	bool Vector2d::IsUnit() {
		return maths::EqualF64(Len(), 1.0);
	}

	Vector2d Vector2d::Rotate(const Angle& angle) {
		float radian = angle.Rad();
		float sine = maths::Sin(radian);
		float cosine = maths::Cos(radian);
		return Vector2d(x * cosine - y * sine,
			x * sine + y * cosine);
	}

	Angle Vector2d::AngleBetween(const Vector2d& b) {
		Vector2d ua = Normalized();
		Vector2d ub = Vector2d(b).Normalized();

		if (ua == Vector2d() || ub == Vector2d() || ua == ub) return Angle();

		float32 dp = ua.Dot(ub);
		return Angle::FromRadian(maths::ACos(dp));
	}

	Vector2d Vector2d::Project(const Vector2d& onto) {
		float64 magBSq = Vector2d(onto).Len();
		if (magBSq < maths::VectorEpsilon()) {
			return Vector2d();
		}
		float64 scale = Dot(onto) / magBSq;
		return onto * scale;
	}

	Vector2d Vector2d::Reject(const Vector2d& b) {
		Vector2d projection = Project(b);
		return (*this) - projection;
	}

	Vector2d Vector2d::Reflect(const Vector2d& b) {
		float64 magBSq = Vector2d(b).Len();
		if (magBSq < maths::VectorEpsilon()) {
			return Vector2d();
		}
		float64 scale = Dot(b) / magBSq;
		Vector2d proj2 = b * (scale * 2);
		return (*this) - proj2;
	}

	Vector2d Vector2d::Lerp(const Vector2d& e, float64 t) {
		return Vector2d(x + (e.x - x) * t,
			y + (e.y - y) * t);
	}

	Vector2d Vector2d::Slerp(const Vector2d& e, float64 t) {
		if (t < 0.01) {
			return Lerp(e, t);
		}

		Vector2d ee(e);

		Vector2d from = Normalized();
		Vector2d to = ee.Normalized();
		float64 theta = from.AngleBetween(to);
		float64 sin_theta = maths::Sin(theta);
		float64 a = maths::Sin((1.0f - t) * theta) / sin_theta;
		float64 b = maths::Sin(t * theta) / sin_theta;
		return from * a + to * b;
	}

	Vector2d Vector2d::Nlerp(const Vector2d& e, float64 t) {
		Vector2d linear(x + (e.x - x) * t,
			y + (e.y - y) * t);
		return linear.Normalized();
	}

	Vector2d Vector2d::Rotate90() {
		return Vector2d(-y, x);
	}

	Vector2d Vector2d::Rotate180() {
		return (-(*this));
	}

	Vector2d Vector2d::Rotate270() {
		return Vector2d(y, -x);
	}

	bool Vector2d::Collineaire(const Vector2d& b) {
		Vector2d na = Rotate90();
		return maths::EqualF64(0, na.Dot(b));
	}

	//[Vector2i]
	Vector2i::Vector2i() : x(0), y(0) {}
	Vector2i::Vector2i(int32 x, int32 y) : x(x), y(y) {}
	Vector2i::Vector2i(int32 v) : x(v), y(v) {}
	Vector2i::Vector2i(int32* v) : x(v[0]), y(v[1]) {}
	Vector2i::Vector2i(const Vector2f& v) : x(v.x), y(v.y) {}
	Vector2i::Vector2i(const Vector2i& v) : x((int32)v.x), y((int32)v.y) {}
	Vector2i::Vector2i(const Vector2u& v) : x((int32)v.x), y((int32)v.y) {}
	Vector2i::Vector2i(const Vector2d& v) : x((int32)v.x), y((int32)v.y) {}
	Vector2i::operator Vector2d() { return Vector2d(x, y); }
	Vector2i::operator Vector2u() { return Vector2u(x, y); }
	Vector2i::operator Vector2f() { return Vector2f(x, y); }

	Vector2i Vector2i::operator=(const Vector2f& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2i Vector2i::operator=(const Vector2i& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2i Vector2i::operator=(const Vector2u& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2i Vector2i::operator=(const Vector2d& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
	int32 Vector2i::SumCord() {
		return x + y;
	}

	//[Vector2u]

	Vector2u::Vector2u() : x(0), y(0) {}
	Vector2u::Vector2u(uint32 x, uint32 y) : x(x), y(y) {}
	Vector2u::Vector2u(uint32 v) : x(v), y(v) {}
	Vector2u::Vector2u(uint32* v) : x(v[0]), y(v[1]) {}
	Vector2u::Vector2u(const Vector2f& v) : x(v.x), y(v.y) {}
	Vector2u::Vector2u(const Vector2i& v) : x((uint32)v.x), y((uint32)v.y) {}
	Vector2u::Vector2u(const Vector2u& v) : x((uint32)v.x), y((uint32)v.y) {}
	Vector2u::Vector2u(const Vector2d& v) : x((uint32)v.x), y((uint32)v.y) {}
	Vector2u::operator Vector2i() { return Vector2i(x, y); }
	Vector2u::operator Vector2d() { return Vector2d(x, y); }
	Vector2u::operator Vector2f() { return Vector2f(x, y); }

	Vector2u Vector2u::operator=(const Vector2f& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2u Vector2u::operator=(const Vector2i& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2u Vector2u::operator=(const Vector2u& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vector2u Vector2u::operator=(const Vector2d& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
	uint32 Vector2u::SumCord() {
		return x + y;
	}
}    // namespace nkentseu