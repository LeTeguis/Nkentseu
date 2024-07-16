//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/11/2024 at 6:55:30 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_VECTOR4_H__
#define __NKENTSEU_VECTOR4_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Vector/Vector3.h>

namespace nkentseu {

	namespace maths
	{
		class Angle;
		class Vector4f;
		class Vector4i;
		class Vector4u;
		class Vector4d;

		class NKENTSEU_API Vector4f {
		public:
			union {
				struct {
					union { float32 x; float32 r; }; // cordinate (x, y, z, w), size (r, g, b, a)
					union { float32 y; float32 g; };
					union { float32 z; float32 b; float32 width; };
					union { float32 w; float32 a; float32 height; };
				};
				float32 ptr[4];
			};

			Vector4f();
			Vector4f(const Vector3f& xyz);
			Vector4f(const Vector3f& xyz, float32 w);
			Vector4f(float32 x, const Vector3f& yzw);
			static Vector4f Exclude(const Vector3f& v3, int32 exclude);
			Vector4f(float32 x, float32 y);
			Vector4f(float32 x, float32 y, float32 z);
			Vector4f(float32 x, float32 y, float32 z, float32 w);
			Vector4f(float32 v);
			Vector4f(const float32* v);
			Vector4f(const Vector4f& v);
			Vector4f(const Vector4i& v);
			Vector4f(const Vector4u& v);
			Vector4f(const Vector4d& v);

			operator float32* () {
				return &ptr[0];
			}

			Vector3f xyz() const { return Vector3f(x, y, z); }
			Vector3f xzy() const { return Vector3f(x, z, y); }
			Vector3f xyw() const { return Vector3f(x, y, w); }
			Vector3f xwy() const { return Vector3f(x, w, y); }
			Vector3f xzw() const { return Vector3f(x, z, w); }
			Vector3f xwz() const { return Vector3f(x, w, z); }

			void xyz(const Vector3f& v) { x = v.x; y = v.y; z = v.z; }
			void xyz(float32 x, float32 y, float32 z) { this->x = x; this->y = y; this->z = z; }
			void xzy(const Vector3f& v) { x = v.x; y = v.z; z = v.y; }
			void xzy(float32 x, float32 z, float32 y) { this->x = x; this->y = y;  this->z = z; }
			void xyw(const Vector3f& v) { this->x = v.x; this->y = v.y;  this->w = v.z; }
			void xyw(float32 x, float32 y, float32 w) { this->x = x; this->y = y;  this->w = w; }
			void xwy(const Vector3f& v) { this->x = v.x; this->y = v.z;  this->w = v.y; }
			void xwy(float32 x, float32 w, float32 y) { this->x = x; this->y = y;  this->w = w; }
			void xzw(const Vector3f& v) { this->x = v.x; this->z = v.y;  this->w = v.z; }
			void xzw(float32 x, float32 z, float32 w) { this->x = x; this->z = z;  this->w = w; }
			void xwz(const Vector3f& v) { this->x = v.x; this->z = v.z;  this->w = v.y; }
			void xwz(float32 x, float32 w, float32 z) { this->x = x; this->z = z;  this->w = w; }

			Vector3f yxz() const { return Vector3f(y, x, z); }
			Vector3f yzx() const { return Vector3f(y, z, x); }
			Vector3f yxw() const { return Vector3f(y, x, w); }
			Vector3f ywx() const { return Vector3f(y, w, x); }
			Vector3f yzw() const { return Vector3f(y, z, w); }
			Vector3f ywz() const { return Vector3f(y, w, z); }

			void yxz(const Vector3f& v) { this->y = v.x; this->x = v.y; this->z = v.z; }
			void yxz(float32 y, float32 x, float32 z) { this->y = y; this->x = x; this->z = z; }
			void yzx(const Vector3f& v) { this->y = v.x; this->x = v.z; this->z = v.y; }
			void yzx(float32 y, float32 z, float32 x) { this->y = y; this->x = x;  this->z = z; }
			void yxw(const Vector3f& v) { this->y = v.x; this->x = v.y;  this->w = v.z; }
			void yxw(float32 y, float32 x, float32 w) { this->y = y; this->x = x;  this->w = w; }
			void ywx(const Vector3f& v) { this->y = v.x; this->x = v.z;  this->w = v.y; }
			void ywx(float32 y, float32 w, float32 x) { this->y = y; this->x = x;  this->w = w; }
			void yzw(const Vector3f& v) { this->y = v.x; this->z = v.y;  this->w = v.z; }
			void yzw(float32 y, float32 z, float32 w) { this->y = y; this->z = z;  this->w = w; }
			void ywz(const Vector3f& v) { this->y = v.x; this->z = v.z;  this->w = v.y; }
			void ywz(float32 y, float32 w, float32 z) { this->y = y; this->z = z;  this->w = w; }

			Vector3f zxy() const { return Vector3f(z, x, y); }
			Vector3f zyx() const { return Vector3f(z, y, x); }
			Vector3f zxw() const { return Vector3f(z, x, w); }
			Vector3f zwx() const { return Vector3f(z, w, x); }
			Vector3f zyw() const { return Vector3f(z, y, w); }
			Vector3f zwy() const { return Vector3f(z, w, y); }

			void zxy(const Vector3f& v) { this->z = v.x; this->x = v.y; this->y = v.z; }
			void zxy(float32 z, float32 x, float32 y) { this->z = z; this->x = x; this->y = y; }
			void zyx(const Vector3f& v) { this->z = v.x; this->x = v.z; this->y = v.y; }
			void zyx(float32 z, float32 y, float32 x) { this->z = z; this->x = x;  this->y = y; }
			void zxw(const Vector3f& v) { this->z = v.x; this->x = v.y;  this->w = v.z; }
			void zxw(float32 z, float32 x, float32 w) { this->z = z; this->x = x;  this->w = w; }
			void zwx(const Vector3f& v) { this->z = v.x; this->x = v.z;  this->w = v.y; }
			void zwx(float32 z, float32 w, float32 x) { this->z = z; this->x = x;  this->w = w; }
			void zyw(const Vector3f& v) { this->z = v.x; this->y = v.y;  this->w = v.z; }
			void zyw(float32 z, float32 y, float32 w) { this->z = z; this->y = y;  this->w = w; }
			void zwy(const Vector3f& v) { this->z = v.x; this->y = v.z;  this->w = v.y; }
			void zwy(float32 z, float32 w, float32 y) { this->z = z; this->y = y;  this->w = w; }

			Vector3f wxy() const { return Vector3f(w, x, y); }
			Vector3f wyx() const { return Vector3f(w, y, x); }
			Vector3f wxz() const { return Vector3f(w, x, z); }
			Vector3f wzx() const { return Vector3f(w, z, x); }
			Vector3f wyz() const { return Vector3f(w, y, z); }
			Vector3f wzy() const { return Vector3f(w, z, y); }

			void wxy(const Vector3f& v) { this->w = v.x; this->x = v.y; this->y = v.z; }
			void wxy(float32 w, float32 x, float32 y) { this->w = w; this->x = x; this->y = y; }
			void wyx(const Vector3f& v) { this->w = v.x; this->x = v.z; this->y = v.y; }
			void wyx(float32 w, float32 y, float32 x) { this->w = w; this->x = x;  this->y = y; }
			void wxz(const Vector3f& v) { this->w = v.x; this->x = v.y;  this->z = v.z; }
			void wxz(float32 w, float32 x, float32 z) { this->w = w; this->x = x;  this->z = z; }
			void wzx(const Vector3f& v) { this->w = v.x; this->x = v.z;  this->z = v.y; }
			void wzx(float32 w, float32 z, float32 x) { this->w = w; this->x = x;  this->z = z; }
			void wyz(const Vector3f& v) { this->w = v.x; this->y = v.y;  this->z = v.z; }
			void wyz(float32 w, float32 y, float32 z) { this->w = w; this->y = y;  this->z = z; }
			void wzy(const Vector3f& v) { this->w = v.x; this->y = v.z;  this->z = v.y; }
			void wzy(float32 w, float32 z, float32 y) { this->w = w; this->y = y;  this->z = z; }

			operator Vector4i ();
			operator Vector4u ();
			operator Vector4d ();

			Vector4f operator=(const Vector4f& other);
			Vector4f operator=(const Vector4i& other);
			Vector4f operator=(const Vector4u& other);
			Vector4f operator=(const Vector4d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector4f& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector4f(" << x << ", " << y << ", " << z << ", " << w << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector4f& v) {
				return v.ToString();
			}

			Vector4f operator-() const {
				return Vector4f(-x, -y, -z, -w);
			}

			inline float32& operator[] (size_t index) {
				return this->ptr[index % 4];
			}

			inline const float32& operator[](size_t index) const {
				return this->ptr[index % 4];
			}

			Vector4f& operator+=(const Vector4f& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				this->w += other.w;
				return *this;
			}

			Vector4f& operator-=(const Vector4f& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				this->w -= other.w;
				return *this;
			}

			Vector4f& operator*=(const Vector4f& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				this->w *= other.w;
				return *this;
			}

			Vector4f& operator/=(const Vector4f& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				this->w /= other.w;
				return *this;
			}

			Vector4f operator + (const Vector4f& other) {
				return Vector4f(other.x + this->x, other.y + this->y, other.z + this->z, other.w + this->w);
			}

			friend Vector4f operator + (const Vector4f& other1, const Vector4f& other2) {
				return Vector4f(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z, other1.w + other2.w);
			}

			Vector4f operator - (const Vector4f& other) {
				return Vector4f(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
			}

			friend Vector4f operator - (const Vector4f& other1, const Vector4f& other2) {
				return Vector4f(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z, other1.w - other2.w);
			}

			Vector4f operator * (const Vector4f& other) {
				return Vector4f(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
			}

			friend Vector4f operator * (const Vector4f& other1, const Vector4f& other2) {
				return Vector4f(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z, other1.w * other2.w);
			}

			Vector4f operator / (const Vector4f& other) {
				return Vector4f(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w);
			}

			friend Vector4f operator/(const Vector4f& other1, const Vector4f& other2) {
				return Vector4f(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z, other1.w / other2.w);
			}

			friend bool operator==(const Vector4f& l, const Vector4f& r) {
				return maths::EqualF32(l.x, r.x) && maths::EqualF32(l.y, r.y) && maths::EqualF32(l.z, r.z) && maths::EqualF32(l.w, r.w);
			}

			friend bool operator!=(const Vector4f& l, const Vector4f& r) {
				return !(l == r);
			}

			float32 Dot(const Vector4f& right) const;
			float32 LenSq() const;
			float32 Len() const;
			void Normalize();
			Vector4f Normalized() const;

			static Vector4f Zero() { return Vector4f(); }
		};

		class NKENTSEU_API Vector4d {
		public:
			union {
				struct {
					union { float64 x; float64 r; }; // cordinate (x, y, z, w), size (r, g, b, a)
					union { float64 y; float64 g; };
					union { float64 z; float64 b; };
					union { float64 w; float64 a; };
				};
				float64 ptr[4];
			};

			Vector4d();
			Vector4d(const Vector3d& xyz);
			Vector4d(const Vector3d& xyz, float64 w);
			Vector4d(float64 x, const Vector3d& yzw);
			static Vector4d Exclude(const Vector3d& v3, int32 exclude);
			Vector4d(float64 x, float64 y);
			Vector4d(float64 x, float64 y, float64 z);
			Vector4d(float64 x, float64 y, float64 z, float64 w);
			Vector4d(float64 v);
			Vector4d(const float64* v);
			Vector4d(const Vector4f& v);
			Vector4d(const Vector4i& v);
			Vector4d(const Vector4u& v);
			Vector4d(const Vector4d& v);

			operator float64* () {
				return &ptr[0];
			}

			Vector3d xyz() const { return Vector3d(x, y, z); }
			Vector3d xzy() const { return Vector3d(x, z, y); }
			Vector3d xyw() const { return Vector3d(x, y, w); }
			Vector3d xwy() const { return Vector3d(x, w, y); }
			Vector3d xzw() const { return Vector3d(x, z, w); }
			Vector3d xwz() const { return Vector3d(x, w, z); }

			void xyz(const Vector3d& v) { x = v.x; y = v.y; z = v.z; }
			void xyz(float64 x, float64 y, float64 z) { this->x = x; this->y = y; this->z = z; }
			void xzy(const Vector3d& v) { x = v.x; y = v.z; z = v.y; }
			void xzy(float64 x, float64 z, float64 y) { this->x = x; this->y = y;  this->z = z; }
			void xyw(const Vector3d& v) { this->x = v.x; this->y = v.y;  this->w = v.z; }
			void xyw(float64 x, float64 y, float64 w) { this->x = x; this->y = y;  this->w = w; }
			void xwy(const Vector3d& v) { this->x = v.x; this->y = v.z;  this->w = v.y; }
			void xwy(float64 x, float64 w, float64 y) { this->x = x; this->y = y;  this->w = w; }
			void xzw(const Vector3d& v) { this->x = v.x; this->z = v.y;  this->w = v.z; }
			void xzw(float64 x, float64 z, float64 w) { this->x = x; this->z = z;  this->w = w; }
			void xwz(const Vector3d& v) { this->x = v.x; this->z = v.z;  this->w = v.y; }
			void xwz(float64 x, float64 w, float64 z) { this->x = x; this->z = z;  this->w = w; }

			Vector3d yxz() const { return Vector3d(y, x, z); }
			Vector3d yzx() const { return Vector3d(y, z, x); }
			Vector3d yxw() const { return Vector3d(y, x, w); }
			Vector3d ywx() const { return Vector3d(y, w, x); }
			Vector3d yzw() const { return Vector3d(y, z, w); }
			Vector3d ywz() const { return Vector3d(y, w, z); }

			void yxz(const Vector3d& v) { this->y = v.x; this->x = v.y; this->z = v.z; }
			void yxz(float64 y, float64 x, float64 z) { this->y = y; this->x = x; this->z = z; }
			void yzx(const Vector3d& v) { this->y = v.x; this->x = v.z; this->z = v.y; }
			void yzx(float64 y, float64 z, float64 x) { this->y = y; this->x = x;  this->z = z; }
			void yxw(const Vector3d& v) { this->y = v.x; this->x = v.y;  this->w = v.z; }
			void yxw(float64 y, float64 x, float64 w) { this->y = y; this->x = x;  this->w = w; }
			void ywx(const Vector3d& v) { this->y = v.x; this->x = v.z;  this->w = v.y; }
			void ywx(float64 y, float64 w, float64 x) { this->y = y; this->x = x;  this->w = w; }
			void yzw(const Vector3d& v) { this->y = v.x; this->z = v.y;  this->w = v.z; }
			void yzw(float64 y, float64 z, float64 w) { this->y = y; this->z = z;  this->w = w; }
			void ywz(const Vector3d& v) { this->y = v.x; this->z = v.z;  this->w = v.y; }
			void ywz(float64 y, float64 w, float64 z) { this->y = y; this->z = z;  this->w = w; }

			Vector3d zxy() const { return Vector3d(z, x, y); }
			Vector3d zyx() const { return Vector3d(z, y, x); }
			Vector3d zxw() const { return Vector3d(z, x, w); }
			Vector3d zwx() const { return Vector3d(z, w, x); }
			Vector3d zyw() const { return Vector3d(z, y, w); }
			Vector3d zwy() const { return Vector3d(z, w, y); }

			void zxy(const Vector3d& v) { this->z = v.x; this->x = v.y; this->y = v.z; }
			void zxy(float64 z, float64 x, float64 y) { this->z = z; this->x = x; this->y = y; }
			void zyx(const Vector3d& v) { this->z = v.x; this->x = v.z; this->y = v.y; }
			void zyx(float64 z, float64 y, float64 x) { this->z = z; this->x = x;  this->y = y; }
			void zxw(const Vector3d& v) { this->z = v.x; this->x = v.y;  this->w = v.z; }
			void zxw(float64 z, float64 x, float64 w) { this->z = z; this->x = x;  this->w = w; }
			void zwx(const Vector3d& v) { this->z = v.x; this->x = v.z;  this->w = v.y; }
			void zwx(float64 z, float64 w, float64 x) { this->z = z; this->x = x;  this->w = w; }
			void zyw(const Vector3d& v) { this->z = v.x; this->y = v.y;  this->w = v.z; }
			void zyw(float64 z, float64 y, float64 w) { this->z = z; this->y = y;  this->w = w; }
			void zwy(const Vector3d& v) { this->z = v.x; this->y = v.z;  this->w = v.y; }
			void zwy(float64 z, float64 w, float64 y) { this->z = z; this->y = y;  this->w = w; }

			Vector3d wxy() const { return Vector3d(w, x, y); }
			Vector3d wyx() const { return Vector3d(w, y, x); }
			Vector3d wxz() const { return Vector3d(w, x, z); }
			Vector3d wzx() const { return Vector3d(w, z, x); }
			Vector3d wyz() const { return Vector3d(w, y, z); }
			Vector3d wzy() const { return Vector3d(w, z, y); }

			void wxy(const Vector3d& v) { this->w = v.x; this->x = v.y; this->y = v.z; }
			void wxy(float64 w, float64 x, float64 y) { this->w = w; this->x = x; this->y = y; }
			void wyx(const Vector3d& v) { this->w = v.x; this->x = v.z; this->y = v.y; }
			void wyx(float64 w, float64 y, float64 x) { this->w = w; this->x = x;  this->y = y; }
			void wxz(const Vector3d& v) { this->w = v.x; this->x = v.y;  this->z = v.z; }
			void wxz(float64 w, float64 x, float64 z) { this->w = w; this->x = x;  this->z = z; }
			void wzx(const Vector3d& v) { this->w = v.x; this->x = v.z;  this->z = v.y; }
			void wzx(float64 w, float64 z, float64 x) { this->w = w; this->x = x;  this->z = z; }
			void wyz(const Vector3d& v) { this->w = v.x; this->y = v.y;  this->z = v.z; }
			void wyz(float64 w, float64 y, float64 z) { this->w = w; this->y = y;  this->z = z; }
			void wzy(const Vector3d& v) { this->w = v.x; this->y = v.z;  this->z = v.y; }
			void wzy(float64 w, float64 z, float64 y) { this->w = w; this->y = y;  this->z = z; }

			operator Vector4i ();
			operator Vector4u ();
			operator Vector4f ();

			Vector4d operator=(const Vector4f& other);
			Vector4d operator=(const Vector4i& other);
			Vector4d operator=(const Vector4u& other);
			Vector4d operator=(const Vector4d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector4d& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector4d(" << x << ", " << y << ", " << z << ", " << w << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector4d& v) {
				return v.ToString();
			}

			Vector4d operator-() const {
				return Vector4d(-x, -y, -z, -w);
			}

			inline float64& operator[] (size_t index) {
				return this->ptr[index % 4];
			}

			inline const float64& operator[](size_t index) const {
				return this->ptr[index % 4];
			}

			Vector4d& operator+=(const Vector4d& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				this->w += other.w;
				return *this;
			}

			Vector4d& operator-=(const Vector4d& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				this->w -= other.w;
				return *this;
			}

			Vector4d& operator*=(const Vector4d& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				this->w *= other.w;
				return *this;
			}

			Vector4d& operator/=(const Vector4d& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				this->w /= other.w;
				return *this;
			}

			Vector4d operator + (const Vector4d& other) {
				return Vector4d(other.x + this->x, other.y + this->y, other.z + this->z, other.w + this->w);
			}

			friend Vector4d operator + (const Vector4d& other1, const Vector4d& other2) {
				return Vector4d(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z, other1.w + other2.w);
			}

			Vector4d operator - (const Vector4d& other) {
				return Vector4d(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
			}

			friend Vector4d operator - (const Vector4d& other1, const Vector4d& other2) {
				return Vector4d(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z, other1.w - other2.w);
			}

			Vector4d operator * (const Vector4d& other) {
				return Vector4d(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
			}

			friend Vector4d operator * (const Vector4d& other1, const Vector4d& other2) {
				return Vector4d(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z, other1.w * other2.w);
			}

			Vector4d operator / (const Vector4d& other) {
				return Vector4d(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w);
			}

			friend Vector4d operator/(const Vector4d& other1, const Vector4d& other2) {
				return Vector4d(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z, other1.w / other2.w);
			}

			friend bool operator==(const Vector4d& l, const Vector4d& r) {
				return maths::EqualF64(l.x, r.x) && maths::EqualF64(l.y, r.y) && maths::EqualF64(l.z, r.z) && maths::EqualF64(l.w, r.w);
			}

			friend bool operator!=(const Vector4d& l, const Vector4d& r) {
				return !(l == r);
			}

			static Vector4d Zero() { return Vector4d(); }
		};

		class NKENTSEU_API Vector4i {
		public:
			union {
				struct {
					union { int32 x; int32 r; }; // cordinate (x, y, z, w), size (r, g, b, a)
					union { int32 y; int32 g; };
					union { int32 z; int32 b; };
					union { int32 w; int32 a; };
				};
				int32 ptr[4];
			};

			Vector4i();
			Vector4i(const Vector3i& xyz);
			Vector4i(const Vector3i& xyz, int32 w);
			Vector4i(int32 x, const Vector3i& yzw);
			static Vector4i Exclude(const Vector3i& v3, int32 exclude);
			Vector4i(int32 x, int32 y);
			Vector4i(int32 x, int32 y, int32 z);
			Vector4i(int32 x, int32 y, int32 z, int32 w);
			Vector4i(int32 v);
			Vector4i(const int32* v);
			Vector4i(const Vector4f& v);
			Vector4i(const Vector4i& v);
			Vector4i(const Vector4u& v);
			Vector4i(const Vector4d& v);

			operator int32* () {
				return &ptr[0];
			}

			Vector3i xyz() const { return Vector3i(x, y, z); }
			Vector3i xzy() const { return Vector3i(x, z, y); }
			Vector3i xyw() const { return Vector3i(x, y, w); }
			Vector3i xwy() const { return Vector3i(x, w, y); }
			Vector3i xzw() const { return Vector3i(x, z, w); }
			Vector3i xwz() const { return Vector3i(x, w, z); }

			void xyz(const Vector3i& v) { x = v.x; y = v.y; z = v.z; }
			void xyz(int32 x, int32 y, int32 z) { this->x = x; this->y = y; this->z = z; }
			void xzy(const Vector3i& v) { x = v.x; y = v.z; z = v.y; }
			void xzy(int32 x, int32 z, int32 y) { this->x = x; this->y = y;  this->z = z; }
			void xyw(const Vector3i& v) { this->x = v.x; this->y = v.y;  this->w = v.z; }
			void xyw(int32 x, int32 y, int32 w) { this->x = x; this->y = y;  this->w = w; }
			void xwy(const Vector3i& v) { this->x = v.x; this->y = v.z;  this->w = v.y; }
			void xwy(int32 x, int32 w, int32 y) { this->x = x; this->y = y;  this->w = w; }
			void xzw(const Vector3i& v) { this->x = v.x; this->z = v.y;  this->w = v.z; }
			void xzw(int32 x, int32 z, int32 w) { this->x = x; this->z = z;  this->w = w; }
			void xwz(const Vector3i& v) { this->x = v.x; this->z = v.z;  this->w = v.y; }
			void xwz(int32 x, int32 w, int32 z) { this->x = x; this->z = z;  this->w = w; }

			Vector3i yxz() const { return Vector3i(y, x, z); }
			Vector3i yzx() const { return Vector3i(y, z, x); }
			Vector3i yxw() const { return Vector3i(y, x, w); }
			Vector3i ywx() const { return Vector3i(y, w, x); }
			Vector3i yzw() const { return Vector3i(y, z, w); }
			Vector3i ywz() const { return Vector3i(y, w, z); }

			void yxz(const Vector3i& v) { this->y = v.x; this->x = v.y; this->z = v.z; }
			void yxz(int32 y, int32 x, int32 z) { this->y = y; this->x = x; this->z = z; }
			void yzx(const Vector3i& v) { this->y = v.x; this->x = v.z; this->z = v.y; }
			void yzx(int32 y, int32 z, int32 x) { this->y = y; this->x = x;  this->z = z; }
			void yxw(const Vector3i& v) { this->y = v.x; this->x = v.y;  this->w = v.z; }
			void yxw(int32 y, int32 x, int32 w) { this->y = y; this->x = x;  this->w = w; }
			void ywx(const Vector3i& v) { this->y = v.x; this->x = v.z;  this->w = v.y; }
			void ywx(int32 y, int32 w, int32 x) { this->y = y; this->x = x;  this->w = w; }
			void yzw(const Vector3i& v) { this->y = v.x; this->z = v.y;  this->w = v.z; }
			void yzw(int32 y, int32 z, int32 w) { this->y = y; this->z = z;  this->w = w; }
			void ywz(const Vector3i& v) { this->y = v.x; this->z = v.z;  this->w = v.y; }
			void ywz(int32 y, int32 w, int32 z) { this->y = y; this->z = z;  this->w = w; }

			Vector3i zxy() const { return Vector3i(z, x, y); }
			Vector3i zyx() const { return Vector3i(z, y, x); }
			Vector3i zxw() const { return Vector3i(z, x, w); }
			Vector3i zwx() const { return Vector3i(z, w, x); }
			Vector3i zyw() const { return Vector3i(z, y, w); }
			Vector3i zwy() const { return Vector3i(z, w, y); }

			void zxy(const Vector3i& v) { this->z = v.x; this->x = v.y; this->y = v.z; }
			void zxy(int32 z, int32 x, int32 y) { this->z = z; this->x = x; this->y = y; }
			void zyx(const Vector3i& v) { this->z = v.x; this->x = v.z; this->y = v.y; }
			void zyx(int32 z, int32 y, int32 x) { this->z = z; this->x = x;  this->y = y; }
			void zxw(const Vector3i& v) { this->z = v.x; this->x = v.y;  this->w = v.z; }
			void zxw(int32 z, int32 x, int32 w) { this->z = z; this->x = x;  this->w = w; }
			void zwx(const Vector3i& v) { this->z = v.x; this->x = v.z;  this->w = v.y; }
			void zwx(int32 z, int32 w, int32 x) { this->z = z; this->x = x;  this->w = w; }
			void zyw(const Vector3i& v) { this->z = v.x; this->y = v.y;  this->w = v.z; }
			void zyw(int32 z, int32 y, int32 w) { this->z = z; this->y = y;  this->w = w; }
			void zwy(const Vector3i& v) { this->z = v.x; this->y = v.z;  this->w = v.y; }
			void zwy(int32 z, int32 w, int32 y) { this->z = z; this->y = y;  this->w = w; }

			Vector3i wxy() const { return Vector3i(w, x, y); }
			Vector3i wyx() const { return Vector3i(w, y, x); }
			Vector3i wxz() const { return Vector3i(w, x, z); }
			Vector3i wzx() const { return Vector3i(w, z, x); }
			Vector3i wyz() const { return Vector3i(w, y, z); }
			Vector3i wzy() const { return Vector3i(w, z, y); }

			void wxy(const Vector3i& v) { this->w = v.x; this->x = v.y; this->y = v.z; }
			void wxy(int32 w, int32 x, int32 y) { this->w = w; this->x = x; this->y = y; }
			void wyx(const Vector3i& v) { this->w = v.x; this->x = v.z; this->y = v.y; }
			void wyx(int32 w, int32 y, int32 x) { this->w = w; this->x = x;  this->y = y; }
			void wxz(const Vector3i& v) { this->w = v.x; this->x = v.y;  this->z = v.z; }
			void wxz(int32 w, int32 x, int32 z) { this->w = w; this->x = x;  this->z = z; }
			void wzx(const Vector3i& v) { this->w = v.x; this->x = v.z;  this->z = v.y; }
			void wzx(int32 w, int32 z, int32 x) { this->w = w; this->x = x;  this->z = z; }
			void wyz(const Vector3i& v) { this->w = v.x; this->y = v.y;  this->z = v.z; }
			void wyz(int32 w, int32 y, int32 z) { this->w = w; this->y = y;  this->z = z; }
			void wzy(const Vector3i& v) { this->w = v.x; this->y = v.z;  this->z = v.y; }
			void wzy(int32 w, int32 z, int32 y) { this->w = w; this->y = y;  this->z = z; }

			operator Vector4d ();
			operator Vector4u ();
			operator Vector4f ();

			Vector4i operator=(const Vector4f& other);
			Vector4i operator=(const Vector4i& other);
			Vector4i operator=(const Vector4u& other);
			Vector4i operator=(const Vector4d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector4i& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector4i(" << x << ", " << y << ", " << z << ", " << w << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector4i& v) {
				return v.ToString();
			}

			Vector4i operator-() const {
				return Vector4i(-x, -y, -z, -w);
			}

			inline int32& operator[] (size_t index) {
				return this->ptr[index % 4];
			}

			inline const int32& operator[](size_t index) const {
				return this->ptr[index % 4];
			}

			Vector4i& operator+=(const Vector4i& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				this->w += other.w;
				return *this;
			}

			Vector4i& operator-=(const Vector4i& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				this->w -= other.w;
				return *this;
			}

			Vector4i& operator*=(const Vector4i& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				this->w *= other.w;
				return *this;
			}

			Vector4i& operator/=(const Vector4i& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				this->w /= other.w;
				return *this;
			}

			Vector4i operator + (const Vector4i& other) {
				return Vector4i(other.x + this->x, other.y + this->y, other.z + this->z, other.w + this->w);
			}

			friend Vector4i operator + (const Vector4i& other1, const Vector4i& other2) {
				return Vector4i(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z, other1.w + other2.w);
			}

			Vector4i operator - (const Vector4i& other) {
				return Vector4i(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
			}

			friend Vector4i operator - (const Vector4i& other1, const Vector4i& other2) {
				return Vector4i(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z, other1.w - other2.w);
			}

			Vector4i operator * (const Vector4i& other) {
				return Vector4i(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
			}

			friend Vector4i operator * (const Vector4i& other1, const Vector4i& other2) {
				return Vector4i(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z, other1.w * other2.w);
			}

			Vector4i operator / (const Vector4i& other) {
				return Vector4i(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w);
			}

			friend Vector4i operator/(const Vector4i& other1, const Vector4i& other2) {
				return Vector4i(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z, other1.w / other2.w);
			}

			friend bool operator==(const Vector4i& l, const Vector4i& r) {
				return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w;
			}

			friend bool operator!=(const Vector4i& l, const Vector4i& r) {
				return !(l == r);
			}

			static Vector4i Zero() { return Vector4i(); }
		};


		class NKENTSEU_API Vector4u {
		public:
			union {
				struct {
					union { uint32 x; uint32 r; }; // cordinate (x, y, z, w), size (r, g, b, a)
					union { uint32 y; uint32 g; };
					union { uint32 z; uint32 b; };
					union { uint32 w; uint32 a; };
				};
				uint32 ptr[4];
			};

			Vector4u();
			Vector4u(const Vector3u& xyz);
			Vector4u(const Vector3u& xyz, uint32 w);
			Vector4u(uint32 x, const Vector3u& yzw);
			static Vector4u Exclude(const Vector3u& v3, int32 exclude);
			Vector4u(uint32 x, uint32 y);
			Vector4u(uint32 x, uint32 y, uint32 z);
			Vector4u(uint32 x, uint32 y, uint32 z, uint32 w);
			Vector4u(uint32 v);
			Vector4u(const uint32* v);
			Vector4u(const Vector4f& v);
			Vector4u(const Vector4i& v);
			Vector4u(const Vector4u& v);
			Vector4u(const Vector4d& v);

			operator uint32* () {
				return &ptr[0];
			}

			Vector3u xyz() const { return Vector3u(x, y, z); }
			Vector3u xzy() const { return Vector3u(x, z, y); }
			Vector3u xyw() const { return Vector3u(x, y, w); }
			Vector3u xwy() const { return Vector3u(x, w, y); }
			Vector3u xzw() const { return Vector3u(x, z, w); }
			Vector3u xwz() const { return Vector3u(x, w, z); }

			void xyz(const Vector3u& v) { x = v.x; y = v.y; z = v.z; }
			void xyz(uint32 x, uint32 y, uint32 z) { this->x = x; this->y = y; this->z = z; }
			void xzy(const Vector3u& v) { x = v.x; y = v.z; z = v.y; }
			void xzy(uint32 x, uint32 z, uint32 y) { this->x = x; this->y = y;  this->z = z; }
			void xyw(const Vector3u& v) { this->x = v.x; this->y = v.y;  this->w = v.z; }
			void xyw(uint32 x, uint32 y, uint32 w) { this->x = x; this->y = y;  this->w = w; }
			void xwy(const Vector3u& v) { this->x = v.x; this->y = v.z;  this->w = v.y; }
			void xwy(uint32 x, uint32 w, uint32 y) { this->x = x; this->y = y;  this->w = w; }
			void xzw(const Vector3u& v) { this->x = v.x; this->z = v.y;  this->w = v.z; }
			void xzw(uint32 x, uint32 z, uint32 w) { this->x = x; this->z = z;  this->w = w; }
			void xwz(const Vector3u& v) { this->x = v.x; this->z = v.z;  this->w = v.y; }
			void xwz(uint32 x, uint32 w, uint32 z) { this->x = x; this->z = z;  this->w = w; }

			Vector3u yxz() const { return Vector3u(y, x, z); }
			Vector3u yzx() const { return Vector3u(y, z, x); }
			Vector3u yxw() const { return Vector3u(y, x, w); }
			Vector3u ywx() const { return Vector3u(y, w, x); }
			Vector3u yzw() const { return Vector3u(y, z, w); }
			Vector3u ywz() const { return Vector3u(y, w, z); }

			void yxz(const Vector3u& v) { this->y = v.x; this->x = v.y; this->z = v.z; }
			void yxz(uint32 y, uint32 x, uint32 z) { this->y = y; this->x = x; this->z = z; }
			void yzx(const Vector3u& v) { this->y = v.x; this->x = v.z; this->z = v.y; }
			void yzx(uint32 y, uint32 z, uint32 x) { this->y = y; this->x = x;  this->z = z; }
			void yxw(const Vector3u& v) { this->y = v.x; this->x = v.y;  this->w = v.z; }
			void yxw(uint32 y, uint32 x, uint32 w) { this->y = y; this->x = x;  this->w = w; }
			void ywx(const Vector3u& v) { this->y = v.x; this->x = v.z;  this->w = v.y; }
			void ywx(uint32 y, uint32 w, uint32 x) { this->y = y; this->x = x;  this->w = w; }
			void yzw(const Vector3u& v) { this->y = v.x; this->z = v.y;  this->w = v.z; }
			void yzw(uint32 y, uint32 z, uint32 w) { this->y = y; this->z = z;  this->w = w; }
			void ywz(const Vector3u& v) { this->y = v.x; this->z = v.z;  this->w = v.y; }
			void ywz(uint32 y, uint32 w, uint32 z) { this->y = y; this->z = z;  this->w = w; }

			Vector3u zxy() const { return Vector3u(z, x, y); }
			Vector3u zyx() const { return Vector3u(z, y, x); }
			Vector3u zxw() const { return Vector3u(z, x, w); }
			Vector3u zwx() const { return Vector3u(z, w, x); }
			Vector3u zyw() const { return Vector3u(z, y, w); }
			Vector3u zwy() const { return Vector3u(z, w, y); }

			void zxy(const Vector3u& v) { this->z = v.x; this->x = v.y; this->y = v.z; }
			void zxy(uint32 z, uint32 x, uint32 y) { this->z = z; this->x = x; this->y = y; }
			void zyx(const Vector3u& v) { this->z = v.x; this->x = v.z; this->y = v.y; }
			void zyx(uint32 z, uint32 y, uint32 x) { this->z = z; this->x = x;  this->y = y; }
			void zxw(const Vector3u& v) { this->z = v.x; this->x = v.y;  this->w = v.z; }
			void zxw(uint32 z, uint32 x, uint32 w) { this->z = z; this->x = x;  this->w = w; }
			void zwx(const Vector3u& v) { this->z = v.x; this->x = v.z;  this->w = v.y; }
			void zwx(uint32 z, uint32 w, uint32 x) { this->z = z; this->x = x;  this->w = w; }
			void zyw(const Vector3u& v) { this->z = v.x; this->y = v.y;  this->w = v.z; }
			void zyw(uint32 z, uint32 y, uint32 w) { this->z = z; this->y = y;  this->w = w; }
			void zwy(const Vector3u& v) { this->z = v.x; this->y = v.z;  this->w = v.y; }
			void zwy(uint32 z, uint32 w, uint32 y) { this->z = z; this->y = y;  this->w = w; }

			Vector3u wxy() const { return Vector3u(w, x, y); }
			Vector3u wyx() const { return Vector3u(w, y, x); }
			Vector3u wxz() const { return Vector3u(w, x, z); }
			Vector3u wzx() const { return Vector3u(w, z, x); }
			Vector3u wyz() const { return Vector3u(w, y, z); }
			Vector3u wzy() const { return Vector3u(w, z, y); }

			void wxy(const Vector3u& v) { this->w = v.x; this->x = v.y; this->y = v.z; }
			void wxy(uint32 w, uint32 x, uint32 y) { this->w = w; this->x = x; this->y = y; }
			void wyx(const Vector3u& v) { this->w = v.x; this->x = v.z; this->y = v.y; }
			void wyx(uint32 w, uint32 y, uint32 x) { this->w = w; this->x = x;  this->y = y; }
			void wxz(const Vector3u& v) { this->w = v.x; this->x = v.y;  this->z = v.z; }
			void wxz(uint32 w, uint32 x, uint32 z) { this->w = w; this->x = x;  this->z = z; }
			void wzx(const Vector3u& v) { this->w = v.x; this->x = v.z;  this->z = v.y; }
			void wzx(uint32 w, uint32 z, uint32 x) { this->w = w; this->x = x;  this->z = z; }
			void wyz(const Vector3u& v) { this->w = v.x; this->y = v.y;  this->z = v.z; }
			void wyz(uint32 w, uint32 y, uint32 z) { this->w = w; this->y = y;  this->z = z; }
			void wzy(const Vector3u& v) { this->w = v.x; this->y = v.z;  this->z = v.y; }
			void wzy(uint32 w, uint32 z, uint32 y) { this->w = w; this->y = y;  this->z = z; }

			operator Vector4d ();
			operator Vector4i ();
			operator Vector4f ();

			Vector4u operator=(const Vector4f& other);
			Vector4u operator=(const Vector4i& other);
			Vector4u operator=(const Vector4u& other);
			Vector4u operator=(const Vector4d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector4u& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector4u(" << x << ", " << y << ", " << z << ", " << w << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector4u& v) {
				return v.ToString();
			}

			inline uint32& operator[] (size_t index) {
				return this->ptr[index % 4];
			}

			inline const uint32& operator[](size_t index) const {
				return this->ptr[index % 4];
			}

			Vector4u& operator+=(const Vector4u& other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;
				this->w += other.w;
				return *this;
			}

			Vector4u& operator-=(const Vector4u& other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;
				this->w -= other.w;
				return *this;
			}

			Vector4u& operator*=(const Vector4u& other) {
				this->x *= other.x;
				this->y *= other.y;
				this->z *= other.z;
				this->w *= other.w;
				return *this;
			}

			Vector4u& operator/=(const Vector4u& other) {
				this->x /= other.x;
				this->y /= other.y;
				this->z /= other.z;
				this->w /= other.w;
				return *this;
			}

			Vector4u operator + (const Vector4u& other) {
				return Vector4u(other.x + this->x, other.y + this->y, other.z + this->z, other.w + this->w);
			}

			friend Vector4u operator + (const Vector4u& other1, const Vector4u& other2) {
				return Vector4u(other1.x + other2.x, other1.y + other2.y, other1.z + other2.z, other1.w + other2.w);
			}

			Vector4u operator - (const Vector4u& other) {
				return Vector4u(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
			}

			friend Vector4u operator - (const Vector4u& other1, const Vector4u& other2) {
				return Vector4u(other1.x - other2.x, other1.y - other2.y, other1.z - other2.z, other1.w - other2.w);
			}

			Vector4u operator * (const Vector4u& other) {
				return Vector4u(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
			}

			friend Vector4u operator * (const Vector4u& other1, const Vector4u& other2) {
				return Vector4u(other1.x * other2.x, other1.y * other2.y, other1.z * other2.z, other1.w * other2.w);
			}

			Vector4u operator / (const Vector4u& other) {
				return Vector4u(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w);
			}

			friend Vector4u operator/(const Vector4u& other1, const Vector4u& other2) {
				return Vector4u(other1.x / other2.x, other1.y / other2.y, other1.z / other2.z, other1.w / other2.w);
			}

			friend bool operator==(const Vector4u& l, const Vector4u& r) {
				return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w;
			}

			friend bool operator!=(const Vector4u& l, const Vector4u& r) {
				return !(l == r);
			}

			static Vector4u Zero() { return Vector4u(); }
		};

		using Vector4 = Vector4f;
		using Vec4 = Vector4f;
		using Vec4d = Vector4d;
		using Vec4i = Vector4i;
		using Vec4u = Vector4u;
	}
} // namespace nkentseu

#endif    // __NKENTSEU_VECTOR4_H__