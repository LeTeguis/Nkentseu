//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 6:55:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Vector3.h"

namespace nkentseu {

	namespace maths
	{
		// [Vector3f]
		Vector3f::Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}

		Vector3f::Vector3f(const Vector2f& xy, float32 z) : x(xy.x), y(xy.y), z(z) {}

		Vector3f Vector3f::Exclude(const Vector2f& v2, int32 exclude) {
			Vector3f v;
			switch (exclude % 3) {
			case 0: // Exclude x
				v.yz(v2);
				break;
			case 1: // Exclude y
				v.xz(v2);
				break;
			case 2: // Exclude z
				v.xy(v2);
				break;
			default:
				break;
			}
			return v;
		}

		Vector3f::Vector3f(float32 x, const Vector2f& yz) : x(x), y(yz.x), z(yz.y) {}

		Vector3f::Vector3f(float32 x, float32 y) : x(x), y(y), z(0.0f) {}


		Vector3f::Vector3f(float32 x, float32 y, float32 z) : x(x), y(y), z(z) {}

		Vector3f::Vector3f(float32 v) : x(v), y(v), z(v) {}

		Vector3f::Vector3f(const float32* v) : x(v[0]), y(v[1]), z(v[2]) {}

		Vector3f::Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) {}

		Vector3f::Vector3f(const Vector3i& v) : x(static_cast<float32>(v.x)), y(static_cast<float32>(v.y)), z(static_cast<float32>(v.z)) {}

		Vector3f::Vector3f(const Vector3u& v) : x(static_cast<float32>(v.x)), y(static_cast<float32>(v.y)), z(static_cast<float32>(v.z)) {}

		Vector3f::Vector3f(const Vector3d& v) : x(static_cast<float32>(v.x)), y(static_cast<float32>(v.y)), z(static_cast<float32>(v.z)) {}

		Vector3f::operator Vector3i() {
			return Vector3i(static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z));
		}

		Vector3f::operator Vector3u() {
			return Vector3u(static_cast<uint32>(x), static_cast<uint32>(y), static_cast<uint32>(z));
		}

		Vector3f::operator Vector3d() {
			return Vector3d(static_cast<float64>(x), static_cast<float64>(y), static_cast<float64>(z));
		}

		Vector3f Vector3f::operator=(const Vector3f& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3f Vector3f::operator=(const Vector3i& other) {
			x = static_cast<float32>(other.x);
			y = static_cast<float32>(other.y);
			z = static_cast<float32>(other.z);
			return *this;
		}

		Vector3f Vector3f::operator=(const Vector3u& other) {
			x = static_cast<float32>(other.x);
			y = static_cast<float32>(other.y);
			z = static_cast<float32>(other.z);
			return *this;
		}

		Vector3f Vector3f::operator=(const Vector3d& other) {
			x = static_cast<float32>(other.x);
			y = static_cast<float32>(other.y);
			z = static_cast<float32>(other.z);
			return *this;
		}

		float32 Vector3f::Distance(const Vector3f& r) const {
			return (r - (*this)).Len();
		}

		float32 Vector3f::Dot(const Vector3f& r) const {
			return x * r.x + y * r.y + z * r.z;
		}

		float32 Vector3f::LenSq() const{
			return x * x + y * y + z * z;
		}

		float32 Vector3f::Len() const{
			float32 lenSq = x * x + y * y + z * z;
			if (lenSq < maths::VectorEpsilon) {
				return 0.0f;
			}
			return maths::Sqrt(lenSq);
		}

		void Vector3f::Normalize() {
			float32 lenSq = x * x + y * y + z * z;
			//if (lenSq < maths::VectorEpsilon()) {
			if (lenSq == 0.0f) {
				return;
			}
			float32 invLen = 1.0f / maths::Sqrt(lenSq);
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		Vector3f Vector3f::Normalized() const{
			Vector3f v(*this);
			v.Normalize();
			return v;
		}

		Angle Vector3f::AngleBetween(const Vector3f& r) {
			float32 sqMagL = x * x + y * y + z * z;
			float32 sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;
			if (sqMagL < maths::VectorEpsilon || sqMagR < maths::VectorEpsilon) {
				return 0.0f;
			}
			float32 dot = x * r.x + y * r.y + z * r.z;
			float32 len = maths::Sqrt(sqMagL) * maths::Sqrt(sqMagR);
			return Angle::FromRadian(maths::ACos(dot / len));
		}

		Vector3f Vector3f::Project(const Vector3f& b) {
			float32 magBSq = Vector3f(b).Len();
			if (magBSq < maths::VectorEpsilon) {
				return Vector3f();
			}
			float32 scale = Dot(b) / magBSq;
			return b * scale;
		}

		Vector3f Vector3f::Reject(const Vector3f& b) {
			Vector3f projection = Project(b);
			return *this - projection;
		}

		Vector3f Vector3f::Reflect(const Vector3f& b) {
			float32 magBSq = Vector3f(b).Len();
			if (magBSq < maths::VectorEpsilon) {
				return Vector3f();
			}
			float32 scale = Dot(b) / magBSq;
			Vector3f proj2 = b * (scale * 2.0f);
			return *this - proj2;
		}

		Vector3f Vector3f::Cross(const Vector3f& r) const{
			return Vector3f(
				y * r.z - z * r.y,
				z * r.x - x * r.z,
				x * r.y - y * r.x
			);
		}

		Vector3f Vector3f::Lerp(const Vector3f& e, float32 t) {
			return Vector3f(
				x + (e.x - x) * t,
				y + (e.y - y) * t,
				z + (e.z - z) * t
			);
		}

		Vector3f Vector3f::Slerp(const Vector3f& e, float32 t) {
			if (t < 0.01f) {
				return Lerp(e, t);
			}
			Vector3f from = Normalized();
			Vector3f to = Vector3f(e).Normalized();
			float32 theta = from.AngleBetween(to);
			float32 sin_theta = maths::Sin(theta);
			float32 a = maths::Sin((1.0f - t) * theta) / sin_theta;
			float32 b = maths::Sin(t * theta) / sin_theta;
			return from * a + to * b;
		}

		Vector3f Vector3f::Nlerp(const Vector3f& e, float32 t) {
			Vector3f linear(
				x + (e.x - x) * t,
				y + (e.y - y) * t,
				z + (e.z - z) * t
			);
			return linear.Normalized();
		}

		Vector3f	Vector3f::Unit() {
			float32 len = Len();
			return 0 < len ? *this / len : *this;
		}

		bool		Vector3f::IsUnit() {
			return maths::EqualF32(LenSq(), 1);
		}

		bool		Vector3f::Collineaire(const Vector3f& b) {
			return !Cross(b);
		}


		// [Vector3d]
		Vector3d::Vector3d() : x(0.0f), y(0.0f), z(0.0f) {}

		Vector3d::Vector3d(const Vector2d& xy, float64 z) : x(xy.x), y(xy.y), z(z) {}

		Vector3d Vector3d::Exclude(const Vector2d& v2, int32 exclude) {
			Vector3d v;
			switch (exclude % 3) {
			case 0: // Exclude x
				v.yz(v2);
				break;
			case 1: // Exclude y
				v.xz(v2);
				break;
			case 2: // Exclude z
				v.xy(v2);
				break;
			default:
				break;
			}
			return v;
		}

		Vector3d::Vector3d(float64 x, const Vector2d& yz) : x(x), y(yz.x), z(yz.y) {}

		Vector3d::Vector3d(float64 x, float64 y) : x(x), y(y), z(0.0f) {}


		Vector3d::Vector3d(float64 x, float64 y, float64 z) : x(x), y(y), z(z) {}

		Vector3d::Vector3d(float64 v) : x(v), y(v), z(v) {}

		Vector3d::Vector3d(float64* v) : x(v[0]), y(v[1]), z(v[2]) {}

		Vector3d::Vector3d(const Vector3d& v) : x(v.x), y(v.y), z(v.z) {}

		Vector3d::Vector3d(const Vector3i& v) : x(static_cast<float64>(v.x)), y(static_cast<float64>(v.y)), z(static_cast<float64>(v.z)) {}

		Vector3d::Vector3d(const Vector3u& v) : x(static_cast<float64>(v.x)), y(static_cast<float64>(v.y)), z(static_cast<float64>(v.z)) {}

		Vector3d::Vector3d(const Vector3f& v) : x(static_cast<float64>(v.x)), y(static_cast<float64>(v.y)), z(static_cast<float64>(v.z)) {}

		Vector3d::operator Vector3i() {
			return Vector3i(static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z));
		}

		Vector3d::operator Vector3u() {
			return Vector3u(static_cast<uint32>(x), static_cast<uint32>(y), static_cast<uint32>(z));
		}

		Vector3d::operator Vector3f() {
			return Vector3d(static_cast<float64>(x), static_cast<float64>(y), static_cast<float64>(z));
		}

		Vector3d Vector3d::operator=(const Vector3d& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3d Vector3d::operator=(const Vector3i& other) {
			x = static_cast<float64>(other.x);
			y = static_cast<float64>(other.y);
			z = static_cast<float64>(other.z);
			return *this;
		}

		Vector3d Vector3d::operator=(const Vector3u& other) {
			x = static_cast<float64>(other.x);
			y = static_cast<float64>(other.y);
			z = static_cast<float64>(other.z);
			return *this;
		}

		Vector3d Vector3d::operator=(const Vector3f& other) {
			x = static_cast<float64>(other.x);
			y = static_cast<float64>(other.y);
			z = static_cast<float64>(other.z);
			return *this;
		}

		float64 Vector3d::Distance(const Vector3d& r) {
			return (r - (*this)).Len();
		}

		float64 Vector3d::Dot(const Vector3d& r) {
			return x * r.x + y * r.y + z * r.z;
		}

		float64 Vector3d::LenSq() {
			return x * x + y * y + z * z;
		}

		float64 Vector3d::Len() {
			float64 lenSq = x * x + y * y + z * z;
			if (lenSq < maths::VectorEpsilon) {
				return 0.0f;
			}
			return maths::Sqrt(lenSq);
		}

		void Vector3d::Normalize() {
			float64 lenSq = x * x + y * y + z * z;
			if (lenSq < maths::VectorEpsilon) {
				return;
			}
			float64 invLen = 1.0f / maths::Sqrt(lenSq);
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		Vector3d Vector3d::Normalized() {
			Vector3d v(*this);
			v.Normalize();
			return v;
		}

		Angle Vector3d::AngleBetween(const Vector3d& r) {
			float64 sqMagL = x * x + y * y + z * z;
			float64 sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;
			if (sqMagL < maths::VectorEpsilon || sqMagR < maths::VectorEpsilon) {
				return 0.0f;
			}
			float64 dot = x * r.x + y * r.y + z * r.z;
			float64 len = maths::Sqrt(sqMagL) * maths::Sqrt(sqMagR);
			return Angle::FromRadian(maths::ACos(dot / len));
		}

		Vector3d Vector3d::Project(const Vector3d& b) {
			float64 magBSq = Vector3d(b).Len();
			if (magBSq < maths::VectorEpsilon) {
				return Vector3d();
			}
			float64 scale = Dot(b) / magBSq;
			return b * scale;
		}

		Vector3d Vector3d::Reject(const Vector3d& b) {
			Vector3d projection = Project(b);
			return *this - projection;
		}

		Vector3d Vector3d::Reflect(const Vector3d& b) {
			float64 magBSq = Vector3d(b).Len();
			if (magBSq < maths::VectorEpsilon) {
				return Vector3d();
			}
			float64 scale = Dot(b) / magBSq;
			Vector3d proj2 = b * (scale * 2);
			return *this - proj2;
		}

		Vector3d Vector3d::Cross(const Vector3d& r) {
			return Vector3d(
				y * r.z - z * r.y,
				z * r.x - x * r.z,
				x * r.y - y * r.x
			);
		}

		Vector3d Vector3d::Lerp(const Vector3d& e, float64 t) {
			return Vector3d(
				x + (e.x - x) * t,
				y + (e.y - y) * t,
				z + (e.z - z) * t
			);
		}

		Vector3d Vector3d::Slerp(const Vector3d& e, float64 t) {
			if (t < 0.01f) {
				return Lerp(e, t);
			}
			Vector3d from = Normalized();
			Vector3d to = Vector3d(e).Normalized();
			float64 theta = from.AngleBetween(to);
			float64 sin_theta = maths::Sin(theta);
			float64 a = maths::Sin((1.0f - t) * theta) / sin_theta;
			float64 b = maths::Sin(t * theta) / sin_theta;
			return from * a + to * b;
		}

		Vector3d Vector3d::Nlerp(const Vector3d& e, float64 t) {
			Vector3d linear(
				x + (e.x - x) * t,
				y + (e.y - y) * t,
				z + (e.z - z) * t
			);
			return linear.Normalized();
		}

		Vector3d	Vector3d::Unit() {
			float64 len = Len();
			return 0 < len ? *this / len : *this;
		}

		bool		Vector3d::IsUnit() {
			return maths::EqualF32(LenSq(), 1);
		}

		bool		Vector3d::Collineaire(const Vector3d& b) {
			return !Cross(b);
		}

		// [Vector3i]
		Vector3i::Vector3i() : x(0.0f), y(0.0f), z(0.0f) {}

		Vector3i::Vector3i(const Vector2i& xy, int32 z) : x(xy.x), y(xy.y), z(z) {}

		Vector3i Vector3i::Exclude(const Vector2i& v2, int32 exclude) {
			Vector3i v;
			switch (exclude % 3) {
			case 0: // Exclude x
				v.yz(v2);
				break;
			case 1: // Exclude y
				v.xz(v2);
				break;
			case 2: // Exclude z
				v.xy(v2);
				break;
			default:
				break;
			}
			return v;
		}

		Vector3i::Vector3i(int32 x, const Vector2i& yz) : x(x), y(yz.x), z(yz.y) {}

		Vector3i::Vector3i(int32 x, int32 y) : x(x), y(y), z(0.0f) {}


		Vector3i::Vector3i(int32 x, int32 y, int32 z) : x(x), y(y), z(z) {}

		Vector3i::Vector3i(int32 v) : x(v), y(v), z(v) {}

		Vector3i::Vector3i(int32* v) : x(v[0]), y(v[1]), z(v[2]) {}

		Vector3i::Vector3i(const Vector3i& v) : x(v.x), y(v.y), z(v.z) {}

		Vector3i::Vector3i(const Vector3d& v) : x(static_cast<int32>(v.x)), y(static_cast<int32>(v.y)), z(static_cast<int32>(v.z)) {}

		Vector3i::Vector3i(const Vector3u& v) : x(static_cast<int32>(v.x)), y(static_cast<int32>(v.y)), z(static_cast<int32>(v.z)) {}

		Vector3i::Vector3i(const Vector3f& v) : x(static_cast<int32>(v.x)), y(static_cast<int32>(v.y)), z(static_cast<int32>(v.z)) {}

		Vector3i::operator Vector3d() {
			return Vector3i(static_cast<float64>(x), static_cast<float64>(y), static_cast<float64>(z));
		}

		Vector3i::operator Vector3u() {
			return Vector3u(static_cast<uint32>(x), static_cast<uint32>(y), static_cast<uint32>(z));
		}

		Vector3i::operator Vector3f() {
			return Vector3i(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z));
		}

		Vector3i Vector3i::operator=(const Vector3i& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3i Vector3i::operator=(const Vector3d& other) {
			x = static_cast<int32>(other.x);
			y = static_cast<int32>(other.y);
			z = static_cast<int32>(other.z);
			return *this;
		}

		Vector3i Vector3i::operator=(const Vector3u& other) {
			x = static_cast<int32>(other.x);
			y = static_cast<int32>(other.y);
			z = static_cast<int32>(other.z);
			return *this;
		}

		Vector3i Vector3i::operator=(const Vector3f& other) {
			x = static_cast<int32>(other.x);
			y = static_cast<int32>(other.y);
			z = static_cast<int32>(other.z);
			return *this;
		}

		// [Vector3u]
		Vector3u::Vector3u() : x(0.0f), y(0.0f), z(0.0f) {}

		Vector3u::Vector3u(const Vector2u& xy, uint32 z) : x(xy.x), y(xy.y), z(z) {}

		Vector3u Vector3u::Exclude(const Vector2u& v2, int32 exclude) {
			Vector3u v;
			switch (exclude % 3) {
			case 0: // Exclude x
				v.yz(v2);
				break;
			case 1: // Exclude y
				v.xz(v2);
				break;
			case 2: // Exclude z
				v.xy(v2);
				break;
			default:
				break;
			}
			return v;
		}

		Vector3u::Vector3u(uint32 x, const Vector2u& yz) : x(x), y(yz.x), z(yz.y) {}

		Vector3u::Vector3u(uint32 x, uint32 y) : x(x), y(y), z(0.0f) {}


		Vector3u::Vector3u(uint32 x, uint32 y, uint32 z) : x(x), y(y), z(z) {}

		Vector3u::Vector3u(uint32 v) : x(v), y(v), z(v) {}

		Vector3u::Vector3u(uint32* v) : x(v[0]), y(v[1]), z(v[2]) {}

		Vector3u::Vector3u(const Vector3u& v) : x(v.x), y(v.y), z(v.z) {}

		Vector3u::Vector3u(const Vector3i& v) : x(static_cast<uint32>(v.x)), y(static_cast<uint32>(v.y)), z(static_cast<uint32>(v.z)) {}

		Vector3u::Vector3u(const Vector3d& v) : x(static_cast<uint32>(v.x)), y(static_cast<uint32>(v.y)), z(static_cast<uint32>(v.z)) {}

		Vector3u::Vector3u(const Vector3f& v) : x(static_cast<uint32>(v.x)), y(static_cast<uint32>(v.y)), z(static_cast<uint32>(v.z)) {}

		Vector3u::operator Vector3d() {
			return Vector3u(static_cast<float64>(x), static_cast<float64>(y), static_cast<float64>(z));
		}

		Vector3u::operator Vector3i() {
			return Vector3u(static_cast<int32>(x), static_cast<int32>(y), static_cast<int32>(z));
		}

		Vector3u::operator Vector3f() {
			return Vector3u(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z));
		}

		Vector3u Vector3u::operator=(const Vector3u& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}

		Vector3u Vector3u::operator=(const Vector3d& other) {
			x = static_cast<uint32>(other.x);
			y = static_cast<uint32>(other.y);
			z = static_cast<uint32>(other.z);
			return *this;
		}

		Vector3u Vector3u::operator=(const Vector3i& other) {
			x = static_cast<uint32>(other.x);
			y = static_cast<uint32>(other.y);
			z = static_cast<uint32>(other.z);
			return *this;
		}

		Vector3u Vector3u::operator=(const Vector3f& other) {
			x = static_cast<uint32>(other.x);
			y = static_cast<uint32>(other.y);
			z = static_cast<uint32>(other.z);
			return *this;
		}
	}
}    // namespace nkentseu