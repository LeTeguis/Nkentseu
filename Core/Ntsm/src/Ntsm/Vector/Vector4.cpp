//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 6:55:30 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Vector4.h"

namespace nkentseu {

	namespace maths
	{
		// [Vector4f]
		Vector4f::Vector4f() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

		Vector4f::Vector4f(const Vector3f& xyz) : x(xyz.x), y(xyz.y), z(xyz.z), w(1.0f) {}

		Vector4f::Vector4f(const Vector3f& xyz, float32 w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

		Vector4f::Vector4f(float32 x, const Vector3f& yzw) : y(yzw.x), z(yzw.y), w(yzw.z), x(x) {}

		Vector4f Vector4f::Exclude(const Vector3f& v3, int32 exclude) {
			Vector4f v;
			switch (exclude % 4) {
			case 0: // Exclude x
				v.yzw(v3);
				break;
			case 1: // Exclude y
				v.xzw(v3);
				break;
			case 2: // Exclude z
				v.xyw(v3);
				break;
			case 3: // Exclude w
				v.xyz(v3);
				break;
			default:
				break;
			}
			return v;
		}

		Vector4f::Vector4f(float32 x, float32 y) : x(x), y(y), z(0.0f), w(1.0f) {}

		Vector4f::Vector4f(float32 x, float32 y, float32 z) : x(x), y(y), z(z), w(1.0f) {}

		Vector4f::Vector4f(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

		Vector4f::Vector4f(float32 v) : x(v), y(v), z(v), w(1.0f) {}

		Vector4f::Vector4f(const float32* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

		Vector4f::Vector4f(const Vector4f& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		Vector4f::Vector4f(const Vector4i& v) : x((float32)v.x), y((float32)v.y), z((float32)v.z), w((float32)v.w) {}

		Vector4f::Vector4f(const Vector4u& v) : x((float32)v.x), y((float32)v.y), z((float32)v.z), w((float32)v.w) {}

		Vector4f::Vector4f(const Vector4d& v) : x((float32)v.x), y((float32)v.y), z((float32)v.z), w((float32)v.w) {}

		Vector4f::operator Vector4i() { return Vector4i((int32)x, (int32)y, (int32)z, (int32)w); }

		Vector4f::operator Vector4u() { return Vector4u((uint32)x, (uint32)y, (uint32)z, (uint32)w); }

		Vector4f::operator Vector4d() { return Vector4d((float64)x, (float64)y, (float64)z, (float64)w); }

		Vector4f Vector4f::operator=(const Vector4f& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		Vector4f Vector4f::operator=(const Vector4i& other) {
			x = static_cast<float32>(other.x);
			y = static_cast<float32>(other.y);
			z = static_cast<float32>(other.z);
			w = static_cast<float32>(other.w);
			return *this;
		}

		Vector4f Vector4f::operator=(const Vector4u& other) {
			x = static_cast<float32>(other.x);
			y = static_cast<float32>(other.y);
			z = static_cast<float32>(other.z);
			w = static_cast<float32>(other.w);
			return *this;
		}

		Vector4f Vector4f::operator=(const Vector4d& other) {
			x = static_cast<float32>(other.x);
			y = static_cast<float32>(other.y);
			z = static_cast<float32>(other.z);
			w = static_cast<float32>(other.w);
			return *this;
		}

		// [Vector4d]
		Vector4d::Vector4d() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

		Vector4d::Vector4d(const Vector3d& xyz) : x(xyz.x), y(xyz.y), z(xyz.z), w(1.0f) {}

		Vector4d::Vector4d(const Vector3d& xyz, float64 w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

		Vector4d::Vector4d(float64 x, const Vector3d& yzw) : y(yzw.x), z(yzw.y), w(yzw.z), x(x) {}

		Vector4d Vector4d::Exclude(const Vector3d& v3, int32 exclude) {
			Vector4d v;
			switch (exclude % 4) {
			case 0: // Exclude x
				v.yzw(v3);
				break;
			case 1: // Exclude y
				v.xzw(v3);
				break;
			case 2: // Exclude z
				v.xyw(v3);
				break;
			case 3: // Exclude w
				v.xyz(v3);
				break;
			default:
				break;
			}
			return v;
		}

		Vector4d::Vector4d(float64 x, float64 y) : x(x), y(y), z(0.0f), w(1.0f) {}

		Vector4d::Vector4d(float64 x, float64 y, float64 z) : x(x), y(y), z(z), w(1.0f) {}

		Vector4d::Vector4d(float64 x, float64 y, float64 z, float64 w) : x(x), y(y), z(z), w(w) {}

		Vector4d::Vector4d(float64 v) : x(v), y(v), z(v), w(1.0f) {}

		Vector4d::Vector4d(const float64* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

		Vector4d::Vector4d(const Vector4d& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		Vector4d::Vector4d(const Vector4i& v) : x((float64)v.x), y((float64)v.y), z((float64)v.z), w((float64)v.w) {}

		Vector4d::Vector4d(const Vector4u& v) : x((float64)v.x), y((float64)v.y), z((float64)v.z), w((float64)v.w) {}

		Vector4d::Vector4d(const Vector4f& v) : x((float64)v.x), y((float64)v.y), z((float64)v.z), w((float64)v.w) {}

		Vector4d::operator Vector4i() { return Vector4i((int32)x, (int32)y, (int32)z, (int32)w); }

		Vector4d::operator Vector4u() { return Vector4u((uint32)x, (uint32)y, (uint32)z, (uint32)w); }

		Vector4d::operator Vector4f() { return Vector4f((float32)x, (float32)y, (float32)z, (float32)w); }

		Vector4d Vector4d::operator=(const Vector4d& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		Vector4d Vector4d::operator=(const Vector4i& other) {
			x = static_cast<float64>(other.x);
			y = static_cast<float64>(other.y);
			z = static_cast<float64>(other.z);
			w = static_cast<float64>(other.w);
			return *this;
		}

		Vector4d Vector4d::operator=(const Vector4u& other) {
			x = static_cast<float64>(other.x);
			y = static_cast<float64>(other.y);
			z = static_cast<float64>(other.z);
			w = static_cast<float64>(other.w);
			return *this;
		}

		Vector4d Vector4d::operator=(const Vector4f& other) {
			x = static_cast<float64>(other.x);
			y = static_cast<float64>(other.y);
			z = static_cast<float64>(other.z);
			w = static_cast<float64>(other.w);
			return *this;
		}

		// [Vector4i]
		Vector4i::Vector4i() : x(0), y(0), z(0), w(1) {}

		Vector4i::Vector4i(const Vector3i& xyz) : x(xyz.x), y(xyz.y), z(xyz.z), w(1) {}

		Vector4i::Vector4i(const Vector3i& xyz, int32 w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

		Vector4i::Vector4i(int32 x, const Vector3i& yzw) : y(yzw.x), z(yzw.y), w(yzw.z), x(x) {}

		Vector4i Vector4i::Exclude(const Vector3i& v3, int32 exclude) {
			Vector4i v;
			switch (exclude % 4) {
			case 0: // Exclude x
				v.yzw(v3);
				break;
			case 1: // Exclude y
				v.xzw(v3);
				break;
			case 2: // Exclude z
				v.xyw(v3);
				break;
			case 3: // Exclude w
				v.xyz(v3);
				break;
			default:
				break;
			}
			return v;
		}

		Vector4i::Vector4i(int32 x, int32 y) : x(x), y(y), z(0), w(1) {}

		Vector4i::Vector4i(int32 x, int32 y, int32 z) : x(x), y(y), z(z), w(1) {}

		Vector4i::Vector4i(int32 x, int32 y, int32 z, int32 w) : x(x), y(y), z(z), w(w) {}

		Vector4i::Vector4i(int32 v) : x(v), y(v), z(v), w(1) {}

		Vector4i::Vector4i(const int32* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

		Vector4i::Vector4i(const Vector4i& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		Vector4i::Vector4i(const Vector4d& v) : x((int32)v.x), y((int32)v.y), z((int32)v.z), w((int32)v.w) {}

		Vector4i::Vector4i(const Vector4u& v) : x((int32)v.x), y((int32)v.y), z((int32)v.z), w((int32)v.w) {}

		Vector4i::Vector4i(const Vector4f& v) : x((int32)v.x), y((int32)v.y), z((int32)v.z), w((int32)v.w) {}

		Vector4i::operator Vector4d() { return Vector4i((int32)x, (int32)y, (int32)z, (int32)w); }

		Vector4i::operator Vector4u() { return Vector4u((uint32)x, (uint32)y, (uint32)z, (uint32)w); }

		Vector4i::operator Vector4f() { return Vector4f((float32)x, (float32)y, (float32)z, (float32)w); }

		Vector4i Vector4i::operator=(const Vector4i& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		Vector4i Vector4i::operator=(const Vector4f& other) {
			x = static_cast<int32>(other.x);
			y = static_cast<int32>(other.y);
			z = static_cast<int32>(other.z);
			w = static_cast<int32>(other.w);
			return *this;
		}

		Vector4i Vector4i::operator=(const Vector4u& other) {
			x = static_cast<int32>(other.x);
			y = static_cast<int32>(other.y);
			z = static_cast<int32>(other.z);
			w = static_cast<int32>(other.w);
			return *this;
		}

		Vector4i Vector4i::operator=(const Vector4d& other) {
			x = static_cast<int32>(other.x);
			y = static_cast<int32>(other.y);
			z = static_cast<int32>(other.z);
			w = static_cast<int32>(other.w);
			return *this;
		}


		// [Vector4u]
		Vector4u::Vector4u() : x(0), y(0), z(0), w(1) {}

		Vector4u::Vector4u(const Vector3u& xyz) : x(xyz.x), y(xyz.y), z(xyz.z), w(1) {}

		Vector4u::Vector4u(const Vector3u& xyz, uint32 w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

		Vector4u::Vector4u(uint32 x, const Vector3u& yzw) : y(yzw.x), z(yzw.y), w(yzw.z), x(x) {}

		Vector4u Vector4u::Exclude(const Vector3u& v3, int32 exclude) {
			Vector4u v;
			switch (exclude % 4) {
			case 0: // Exclude x
				v.yzw(v3);
				break;
			case 1: // Exclude y
				v.xzw(v3);
				break;
			case 2: // Exclude z
				v.xyw(v3);
				break;
			case 3: // Exclude w
				v.xyz(v3);
				break;
			default:
				break;
			}
			return v;
		}

		Vector4u::Vector4u(uint32 x, uint32 y) : x(x), y(y), z(0), w(1) {}

		Vector4u::Vector4u(uint32 x, uint32 y, uint32 z) : x(x), y(y), z(z), w(1) {}

		Vector4u::Vector4u(uint32 x, uint32 y, uint32 z, uint32 w) : x(x), y(y), z(z), w(w) {}

		Vector4u::Vector4u(uint32 v) : x(v), y(v), z(v), w(1) {}

		Vector4u::Vector4u(const uint32* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

		Vector4u::Vector4u(const Vector4u& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

		Vector4u::Vector4u(const Vector4d& v) : x((uint32)v.x), y((uint32)v.y), z((uint32)v.z), w((uint32)v.w) {}

		Vector4u::Vector4u(const Vector4i& v) : x((uint32)v.x), y((uint32)v.y), z((uint32)v.z), w((uint32)v.w) {}

		Vector4u::Vector4u(const Vector4f& v) : x((uint32)v.x), y((uint32)v.y), z((uint32)v.z), w((uint32)v.w) {}

		Vector4u::operator Vector4i() { return Vector4i((int32)x, (int32)y, (int32)z, (int32)w); }

		Vector4u::operator Vector4d() { return Vector4u((uint32)x, (uint32)y, (uint32)z, (uint32)w); }

		Vector4u::operator Vector4f() { return Vector4f((float32)x, (float32)y, (float32)z, (float32)w); }

		Vector4u Vector4u::operator=(const Vector4u& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		Vector4u Vector4u::operator=(const Vector4i& other) {
			x = static_cast<uint32>(other.x);
			y = static_cast<uint32>(other.y);
			z = static_cast<uint32>(other.z);
			w = static_cast<uint32>(other.w);
			return *this;
		}

		Vector4u Vector4u::operator=(const Vector4f& other) {
			x = static_cast<uint32>(other.x);
			y = static_cast<uint32>(other.y);
			z = static_cast<uint32>(other.z);
			w = static_cast<uint32>(other.w);
			return *this;
		}

		Vector4u Vector4u::operator=(const Vector4d& other) {
			x = static_cast<uint32>(other.x);
			y = static_cast<uint32>(other.y);
			z = static_cast<uint32>(other.z);
			w = static_cast<uint32>(other.w);
			return *this;
		}
	}
}    // namespace nkentseu