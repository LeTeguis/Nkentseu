//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 6:55:22 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_VECTOR3_H__
#define __NKENTSEU_VECTOR3_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Vector/Vector2.h>

namespace nkentseu {

	namespace maths
	{
		class Angle;
		class Vector3f;
		class Vector3i;
		class Vector3u;
		class Vector3d;

		class NKENTSEU_API Vector3f {
		public:
			union {
				struct {
					union { float32 x; float32 u; float32 r; float32 pitch; }; // cordinate (x, y, z), texture (u, v, w), size (r, g, b)
					union { float32 y; float32 v; float32 g; float32 yaw; };
					union { float32 z; float32 t; float32 b; float32 roll; };
				};
				float32 ptr[3];
			};

			Vector3f();
			Vector3f(const Vector2f& xy, float32 z);
			Vector3f(float32 x, const Vector2f& yz);
			Vector3f(float32 x, float32 y);
			Vector3f(float32 x, float32 y, float32 z);
			Vector3f(float32 v);
			Vector3f(const float32* v);
			Vector3f(const Vector3f& v);
			Vector3f(const Vector3i& v);
			Vector3f(const Vector3u& v);
			Vector3f(const Vector3d& v);

			operator float32* () {
				return &ptr[0];
			}

			static Vector3f FromXY(const Vector2f& xy) { return Vector3f(xy.x, xy.y, 0.0f); }
			static Vector3f FromXZ(const Vector2f& xz) { return Vector3f(xz.x, 0.0f, xz.y); }
			static Vector3f FromYZ(const Vector2f& yz) { return Vector3f(0.0f, yz.x, yz.y); }

			static Vector3f FromXYZ(const Vector2f& xy, float32 z) { return Vector3f(xy.x, xy.y, z); }
			static Vector3f FromXZY(const Vector2f& xz, float32 y) { return Vector3f(xz.x, y, xz.y); }
			static Vector3f FromYZX(const Vector2f& yz, float32 x) { return Vector3f(x, yz.x, yz.y); }

			static Vector3f Up() { return Vector3f(0.0f, 1.0f, 0.0f); }
			static Vector3f Down() { return Vector3f(0.0f, -1.0f, 0.0f); }
			static Vector3f Left() { return Vector3f(-1.0f, 0.0f, 0.0f); }
			static Vector3f Right() { return Vector3f(1.0f, 1.0f, 0.0f); }
			static Vector3f Forward() { return Vector3f(0.0f, 0.0f, 1.0f); }
			static Vector3f Backward() { return Vector3f(0.0f, 0.0f, -1.0f); }

			Vector2f xy() const { return Vector2f(x, y); }
			Vector2f xz() const { return Vector2f(x, z); }
			Vector2f yz() const { return Vector2f(y, z); }
			Vector2f yx() const { return Vector2f(y, x); }
			Vector2f zx() const { return Vector2f(z, x); }
			Vector2f zy() const { return Vector2f(z, y); }

			void xy(const Vector2f& v) { x = v.x; y = v.y; }
			void xy(float32 x, float32 y) { this->x = x; this->y = y; }
			void xz(const Vector2f& v) { x = v.x; z = v.y; }
			void xz(float32 x, float32 z) { this->x = x; this->z = z; }

			void yz(const Vector2f& v) { y = v.x; z = v.y; }
			void yz(float32 y, float32 z) { this->y = y; this->z = z; }
			void yx(const Vector2f& v) { y = v.x; x = v.y; }
			void yx(float32 y, float32 x) { this->y = y; this->x = x; }

			void zx(const Vector2f& v) { z = v.x; x = v.y; }
			void zx(float32 z, float32 x) { this->z = z; this->x = x; }
			void zy(const Vector2f& v) { z = v.x; y = v.y; }
			void zy(float32 z, float32 y) { this->z = z; this->y = y; }

			operator Vector3i ();
			operator Vector3u ();
			operator Vector3d ();

			Vector3f operator=(const Vector3f& other);
			Vector3f operator=(const Vector3i& other);
			Vector3f operator=(const Vector3u& other);
			Vector3f operator=(const Vector3d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector3f& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector3f(" << x << ", " << y << ", " << z << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector3f& v) {
				return v.ToString();
			}

			Vector3f operator-() const {
				return Vector3f(-x, -y, -z);
			}

			inline float32& operator[] (size_t index) {
				return this->ptr[index % 3];
			}

			inline const float32& operator[](size_t index) const {
				return this->ptr[index % 3];
			}

			Vector3f& operator+=(const Vector3f& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				return *this;
			}

			Vector3f& operator-=(const Vector3f& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				return *this;
			}

			Vector3f& operator*=(const Vector3f& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				return *this;
			}

			Vector3f& operator/=(const Vector3f& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				return *this;
			}

			Vector3f operator + (const Vector3f& other) {
				return Vector3f(other.x + this->x, other.y + this->y, other.z + this->z);
			}

			friend Vector3f operator + (const Vector3f& other1, const Vector3f& other2) {
				return Vector3f(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z);
			}

			Vector3f operator - (const Vector3f& other) {
				return Vector3f(this->x - other.x, this->y - other.y, this->z - other.z);
			}

			friend Vector3f operator - (const Vector3f& other1, const Vector3f& other2) {
				return Vector3f(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z);
			}

			Vector3f operator * (const Vector3f& other) {
				return Vector3f(this->x * other.x, this->y * other.y, this->z * other.z);
			}

			friend Vector3f operator * (const Vector3f& other1, const Vector3f& other2) {
				return Vector3f(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z);
			}

			Vector3f operator / (const Vector3f& other) {
				return Vector3f(this->x / other.x, this->y / other.y, this->z / other.z);
			}

			friend Vector3f operator/(const Vector3f& other1, const Vector3f& other2) {
				return Vector3f(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z);
			}

			friend bool operator==(const Vector3f& l, const Vector3f& r) {
				//Vector3f diff(l - r);
				//return diff.LenSq() < maths::VectorEpsilon();
				return l.x == r.x && l.y == r.y && l.z == r.z;
				//return diff.LenSq() == 0;
			}

			friend bool operator!=(const Vector3f& l, const Vector3f& r) {
				return !(l == r);
			}

			float32	Distance(const Vector3f& r);
			float32 Dot(const Vector3f& r);
			float32 LenSq();
			float32 Len();
			void Normalize();
			Vector3f Normalized();
			Angle AngleBetween(const Vector3f& r);
			Vector3f Project(const Vector3f& b);
			Vector3f Reject(const Vector3f& b);
			Vector3f Reflect(const Vector3f& b);
			Vector3f Cross(const Vector3f& r);
			Vector3f Lerp(const Vector3f& e, float32 t);
			Vector3f Slerp(const Vector3f& e, float32 t);
			Vector3f Nlerp(const Vector3f& e, float32 t);
			Vector3f Unit();
			bool IsUnit();
			bool Collineaire(const Vector3f& b);

			static Vector3f Zero() { return Vector3f(); }
			static Vector3f Exclude(const Vector2f& v2, int32 exclude);
		};

		class NKENTSEU_API Vector3d {
		public:
			union {
				struct {
					union { float64 x; float64 u; float64 r; }; // cordinate (x, y, z), texture (u, v, w), size (r, g, b)
					union { float64 y; float64 v; float64 g; };
					union { float64 z; float64 t; float64 b; };
				};
				float64 ptr[3];
			};

			Vector3d();
			Vector3d(const Vector2d& xy, float64 z);
			Vector3d(float64 x, const Vector2d& yz);
			Vector3d(float64 x, float64 y);
			Vector3d(float64 x, float64 y, float64 z);
			Vector3d(float64 v);
			Vector3d(float64* v);
			Vector3d(const Vector3f& v);
			Vector3d(const Vector3i& v);
			Vector3d(const Vector3u& v);
			Vector3d(const Vector3d& v);

			operator float64* () {
				return &ptr[0];
			}

			static Vector3d FromXY(const Vector2d& xy) { return Vector3d(xy.x, xy.y, 0.0f); }
			static Vector3d FromXZ(const Vector2d& xz) { return Vector3d(xz.x, 0.0f, xz.y); }
			static Vector3d FromYZ(const Vector2d& yz) { return Vector3d(0.0f, yz.x, yz.y); }

			static Vector3d FromXYZ(const Vector2d& xy, float64 z) { return Vector3d(xy.x, xy.y, z); }
			static Vector3d FromXZY(const Vector2d& xz, float64 y) { return Vector3d(xz.x, y, xz.y); }
			static Vector3d FromYZX(const Vector2d& yz, float64 x) { return Vector3d(x, yz.x, yz.y); }

			Vector2d xy() const { return Vector2d(x, y); }
			Vector2d xz() const { return Vector2d(x, z); }
			Vector2d yz() const { return Vector2d(y, z); }
			Vector2d yx() const { return Vector2d(y, x); }
			Vector2d zx() const { return Vector2d(z, x); }
			Vector2d zy() const { return Vector2d(z, y); }

			void xy(const Vector2d& v) { x = v.x; y = v.y; }
			void xy(float64 x, float64 y) { this->x = x; this->y = y; }
			void xz(const Vector2d& v) { x = v.x; z = v.y; }
			void xz(float64 x, float64 z) { this->x = x; this->z = z; }

			void yz(const Vector2d& v) { y = v.x; z = v.y; }
			void yz(float64 y, float64 z) { this->y = y; this->z = z; }
			void yx(const Vector2d& v) { y = v.x; x = v.y; }
			void yx(float64 y, float64 x) { this->y = y; this->x = x; }

			void zx(const Vector2d& v) { z = v.x; x = v.y; }
			void zx(float64 z, float64 x) { this->z = z; this->x = x; }
			void zy(const Vector2d& v) { z = v.x; y = v.y; }
			void zy(float64 z, float64 y) { this->z = z; this->y = y; }

			operator Vector3i ();
			operator Vector3u ();
			operator Vector3f ();

			Vector3d operator=(const Vector3f& other);
			Vector3d operator=(const Vector3i& other);
			Vector3d operator=(const Vector3u& other);
			Vector3d operator=(const Vector3d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector3d& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector3d(" << x << ", " << y << ", " << z << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector3d& v) {
				return v.ToString();
			}

			Vector3d operator-() const {
				return Vector3d(-x, -y, -z);
			}

			inline float64& operator[] (size_t index) {
				return this->ptr[index % 3];
			}

			inline const float64& operator[](size_t index) const {
				return this->ptr[index % 3];
			}

			Vector3d& operator+=(const Vector3d& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				return *this;
			}

			Vector3d& operator-=(const Vector3d& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				return *this;
			}

			Vector3d& operator*=(const Vector3d& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				return *this;
			}

			Vector3d& operator/=(const Vector3d& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				return *this;
			}

			Vector3d operator + (const Vector3d& other) {
				return Vector3d(other.x + this->x, other.y + this->y, other.z + this->z);
			}

			friend Vector3d operator + (const Vector3d& other1, const Vector3d& other2) {
				return Vector3d(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z);
			}

			Vector3d operator - (const Vector3d& other) {
				return Vector3d(this->x - other.x, this->y - other.y, this->z - other.z);
			}

			friend Vector3d operator - (const Vector3d& other1, const Vector3d& other2) {
				return Vector3d(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z);
			}

			Vector3d operator * (const Vector3d& other) {
				return Vector3d(this->x * other.x, this->y * other.y, this->z * other.z);
			}

			friend Vector3d operator * (const Vector3d& other1, const Vector3d& other2) {
				return Vector3d(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z);
			}

			Vector3d operator / (const Vector3d& other) {
				return Vector3d(this->x / other.x, this->y / other.y, this->z / other.z);
			}

			friend Vector3d operator/(const Vector3d& other1, const Vector3d& other2) {
				return Vector3d(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z);
			}

			friend bool operator==(const Vector3d& l, const Vector3d& r) {
				Vector3d diff(l - r);
				return diff.LenSq() < maths::VectorEpsilon;
			}

			friend bool operator!=(const Vector3d& l, const Vector3d& r) {
				return !(l == r);
			}

			float64	Distance(const Vector3d& r);
			float64 Dot(const Vector3d& r);
			float64 LenSq();
			float64 Len();
			void Normalize();
			Vector3d Normalized();
			Angle AngleBetween(const Vector3d& r);
			Vector3d Project(const Vector3d& b);
			Vector3d Reject(const Vector3d& b);
			Vector3d Reflect(const Vector3d& b);
			Vector3d Cross(const Vector3d& r);
			Vector3d Lerp(const Vector3d& e, float64 t);
			Vector3d Slerp(const Vector3d& e, float64 t);
			Vector3d Nlerp(const Vector3d& e, float64 t);
			Vector3d Unit();
			bool IsUnit();
			bool Collineaire(const Vector3d& b);

			static Vector3d Zero() { return Vector3d(); }
			static Vector3d Exclude(const Vector2d& v2, int32 exclude);
		};

		class NKENTSEU_API Vector3i {
		public:
			union {
				struct {
					union { int32 x; int32 u; int32 r; }; // cordinate (x, y, z), texture (u, v, w), size (r, g, b)
					union { int32 y; int32 v; int32 g; };
					union { int32 z; int32 t; int32 b; };
				};
				int32 ptr[3];
			};

			Vector3i();
			Vector3i(const Vector2i& xy, int32 z);
			Vector3i(int32 x, const Vector2i& yz);
			Vector3i(int32 x, int32 y);
			Vector3i(int32 x, int32 y, int32 z);
			Vector3i(int32 v);
			Vector3i(int32* v);
			Vector3i(const Vector3f& v);
			Vector3i(const Vector3i& v);
			Vector3i(const Vector3u& v);
			Vector3i(const Vector3d& v);

			operator int32* () {
				return &ptr[0];
			}

			static Vector3i FromXY(const Vector2i& xy) { return Vector3i(xy.x, xy.y, 0); }
			static Vector3i FromXZ(const Vector2i& xz) { return Vector3i(xz.x, 0, xz.y); }
			static Vector3i FromYZ(const Vector2i& yz) { return Vector3i(0, yz.x, yz.y); }

			static Vector3i FromXYZ(const Vector2i& xy, int32 z) { return Vector3i(xy.x, xy.y, z); }
			static Vector3i FromXZY(const Vector2i& xz, int32 y) { return Vector3i(xz.x, y, xz.y); }
			static Vector3i FromYZX(const Vector2i& yz, int32 x) { return Vector3i(x, yz.x, yz.y); }

			Vector2i xy() const { return Vector2i(x, y); }
			Vector2i xz() const { return Vector2i(x, z); }
			Vector2i yz() const { return Vector2i(y, z); }
			Vector2i yx() const { return Vector2i(y, x); }
			Vector2i zx() const { return Vector2i(z, x); }
			Vector2i zy() const { return Vector2i(z, y); }

			void xy(const Vector2i& v) { x = v.x; y = v.y; }
			void xy(int32 x, int32 y) { this->x = x; this->y = y; }
			void xz(const Vector2i& v) { x = v.x; z = v.y; }
			void xz(int32 x, int32 z) { this->x = x; this->z = z; }

			void yz(const Vector2i& v) { y = v.x; z = v.y; }
			void yz(int32 y, int32 z) { this->y = y; this->z = z; }
			void yx(const Vector2i& v) { y = v.x; x = v.y; }
			void yx(int32 y, int32 x) { this->y = y; this->x = x; }

			void zx(const Vector2i& v) { z = v.x; x = v.y; }
			void zx(int32 z, int32 x) { this->z = z; this->x = x; }
			void zy(const Vector2i& v) { z = v.x; y = v.y; }
			void zy(int32 z, int32 y) { this->z = z; this->y = y; }

			operator Vector3d ();
			operator Vector3u ();
			operator Vector3f ();

			Vector3i operator=(const Vector3f& other);
			Vector3i operator=(const Vector3i& other);
			Vector3i operator=(const Vector3u& other);
			Vector3i operator=(const Vector3d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector3i& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector3i(" << x << ", " << y << ", " << z << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector3i& v) {
				return v.ToString();
			}

			Vector3i operator-() const {
				return Vector3i(-x, -y, -z);
			}

			inline int32& operator[] (size_t index) {
				return this->ptr[index % 3];
			}

			inline const int32& operator[](size_t index) const {
				return this->ptr[index % 3];
			}

			Vector3i& operator+=(const Vector3i& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				return *this;
			}

			Vector3i& operator-=(const Vector3i& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				return *this;
			}

			Vector3i& operator*=(const Vector3i& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				return *this;
			}

			Vector3i& operator/=(const Vector3i& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				return *this;
			}

			Vector3i operator + (const Vector3i& other) {
				return Vector3i(other.x + this->x, other.y + this->y, other.z + this->z);
			}

			friend Vector3i operator + (const Vector3i& other1, const Vector3i& other2) {
				return Vector3i(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z);
			}

			Vector3i operator - (const Vector3i& other) {
				return Vector3i(this->x - other.x, this->y - other.y, this->z - other.z);
			}

			friend Vector3i operator - (const Vector3i& other1, const Vector3i& other2) {
				return Vector3i(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z);
			}

			Vector3i operator * (const Vector3i& other) {
				return Vector3i(this->x * other.x, this->y * other.y, this->z * other.z);
			}

			friend Vector3i operator * (const Vector3i& other1, const Vector3i& other2) {
				return Vector3i(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z);
			}

			Vector3i operator / (const Vector3i& other) {
				return Vector3i(this->x / other.x, this->y / other.y, this->z / other.z);
			}

			friend Vector3i operator/(const Vector3i& other1, const Vector3i& other2) {
				return Vector3i(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z);
			}

			friend bool operator==(const Vector3i& l, const Vector3i& r) {
				Vector3i diff(l - r);
				return diff.x == diff.y == 0;
			}

			friend bool operator!=(const Vector3i& l, const Vector3i& r) {
				return !(l == r);
			}

			static Vector3i Zero() { return Vector3i(); }
			static Vector3i Exclude(const Vector2i& v2, int32 exclude);
		};

		class NKENTSEU_API Vector3u {
		public:
			union {
				struct {
					union { uint32 x; uint32 u; uint32 r; }; // cordinate (x, y, z), texture (u, v, w), size (r, g, b)
					union { uint32 y; uint32 v; uint32 g; };
					union { uint32 z; uint32 t; uint32 b; };
				};
				uint32 ptr[3];
			};

			Vector3u();
			Vector3u(const Vector2u& xy, uint32 z);
			Vector3u(uint32 x, const Vector2u& yz);
			Vector3u(uint32 x, uint32 y);
			Vector3u(uint32 x, uint32 y, uint32 z);
			Vector3u(uint32 v);
			Vector3u(uint32* v);
			Vector3u(const Vector3f& v);
			Vector3u(const Vector3i& v);
			Vector3u(const Vector3u& v);
			Vector3u(const Vector3d& v);

			operator uint32* () {
				return &ptr[0];
			}

			Vector2u xy() const { return Vector2u(x, y); }
			Vector2u xz() const { return Vector2u(x, z); }
			Vector2u yz() const { return Vector2u(y, z); }
			Vector2u yx() const { return Vector2u(y, x); }
			Vector2u zx() const { return Vector2u(z, x); }
			Vector2u zy() const { return Vector2u(z, y); }

			void xy(const Vector2u& v) { x = v.x; y = v.y; }
			void xy(uint32 x, uint32 y) { this->x = x; this->y = y; }
			void xz(const Vector2u& v) { x = v.x; z = v.y; }
			void xz(uint32 x, uint32 z) { this->x = x; this->z = z; }

			void yz(const Vector2u& v) { y = v.x; z = v.y; }
			void yz(uint32 y, uint32 z) { this->y = y; this->z = z; }
			void yx(const Vector2u& v) { y = v.x; x = v.y; }
			void yx(uint32 y, uint32 x) { this->y = y; this->x = x; }

			void zx(const Vector2u& v) { z = v.x; x = v.y; }
			void zx(uint32 z, uint32 x) { this->z = z; this->x = x; }
			void zy(const Vector2u& v) { z = v.x; y = v.y; }
			void zy(uint32 z, uint32 y) { this->z = z; this->y = y; }

			operator Vector3d ();
			operator Vector3i ();
			operator Vector3f ();

			Vector3u operator=(const Vector3f& other);
			Vector3u operator=(const Vector3i& other);
			Vector3u operator=(const Vector3u& other);
			Vector3u operator=(const Vector3d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector3u& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector3u(" << x << ", " << y << ", " << z << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector3u& v) {
				return v.ToString();
			}

			inline uint32& operator[] (size_t index) {
				return this->ptr[index % 3];
			}

			inline const uint32& operator[](size_t index) const {
				return this->ptr[index % 3];
			}

			Vector3u& operator+=(const Vector3u& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				return *this;
			}

			Vector3u& operator-=(const Vector3u& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				return *this;
			}

			Vector3u& operator*=(const Vector3u& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				return *this;
			}

			Vector3u& operator/=(const Vector3u& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				return *this;
			}

			Vector3u operator + (const Vector3u& other) {
				return Vector3u(other.x + this->x, other.y + this->y, other.z + this->z);
			}

			friend Vector3u operator + (const Vector3u& other1, const Vector3u& other2) {
				return Vector3u(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z);
			}

			Vector3u operator - (const Vector3u& other) {
				return Vector3u(this->x - other.x, this->y - other.y, this->z - other.z);
			}

			friend Vector3u operator - (const Vector3u& other1, const Vector3u& other2) {
				return Vector3u(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z);
			}

			Vector3u operator * (const Vector3u& other) {
				return Vector3u(this->x * other.x, this->y * other.y, this->z * other.z);
			}

			friend Vector3u operator * (const Vector3u& other1, const Vector3u& other2) {
				return Vector3u(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z);
			}

			Vector3u operator / (const Vector3u& other) {
				return Vector3u(this->x / other.x, this->y / other.y, this->z / other.z);
			}

			friend Vector3u operator/(const Vector3u& other1, const Vector3u& other2) {
				return Vector3u(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z);
			}

			friend bool operator==(const Vector3u& l, const Vector3u& r) {
				Vector3u diff(l - r);
				return diff.x == diff.y == 0;
			}

			friend bool operator!=(const Vector3u& l, const Vector3u& r) {
				return !(l == r);
			}

			static Vector3u Zero() { return Vector3u(); }
			static Vector3u Exclude(const Vector2u& v2, int32 exclude);

			static Vector3u FromXY(const Vector2u& xy) { return Vector3u(xy.x, xy.y, 0); }
			static Vector3u FromXZ(const Vector2u& xz) { return Vector3u(xz.x, 0, xz.y); }
			static Vector3u FromYZ(const Vector2u& yz) { return Vector3u(0, yz.x, yz.y); }

			static Vector3u FromXYZ(const Vector2u& xy, uint32 z) { return Vector3u(xy.x, xy.y, z); }
			static Vector3u FromXZY(const Vector2u& xz, uint32 y) { return Vector3u(xz.x, y, xz.y); }
			static Vector3u FromYZX(const Vector2u& yz, uint32 x) { return Vector3u(x, yz.x, yz.y); }
		};

		template<typename T>
		class NKENTSEU_API Vector3T {
		public:
			union {
				struct {
					union { T x; T u; T r; T pitch; };
					union { T y; T v; T g; T yaw; };
					union { T z; T t; T b; T roll; };
				};
				T vec[3];
			};

			Vector3T() {}
			Vector3T(T value) : x(value), y(value), z(value) {}
			Vector3T(T x, T y, T z) : x(x), y(y), z(z) {}
			Vector3T(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}

			Vector3T(const Vector3T& vec) : x(vec.x), y(vec.y), z(vec.z) {}
			Vector3T& operator=(const Vector3T& vec) {
				this->x = vec.x;
				this->y = vec.y;
				this->z = vec.z;
				return *this;
			}

			friend std::ostream& operator<<(std::ostream& os, const Vector3T& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector3T(" << x << ", " << y << ", " << z << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector3T& v) {
				return v.ToString();
			}
		};

		using Vec3 = Vector3f;
		using Vector3 = Vector3f;
		using Vec3i = Vector3i;
		using Vec3u = Vector3u;
		using Vec3d = Vector3d;
	}
} // namespace nkentseu

#endif    // __NKENTSEU_VECTOR3_H__