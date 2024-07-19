//
// Created by TEUGUIA TADJUIDJE Rodolf Sederis on 4/11/2024 at 12:03:04 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_VECTOR2_H__
#define __NKENTSEU_VECTOR2_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include "Ntsm/Utils.h"

namespace nkentseu {

	namespace maths
	{
		class Angle;
		class Vector2f;
		class Vector2i;
		class Vector2u;
		class Vector2d;

		class NKENTSEU_API Vector2f {
		public:
			union {
				struct {
					union { float32 x; float32 u; float32 width;  float32 w; float32 major; }; // cordinate (x, y), texture (u, v), size (width, height)
					union { float32 y; float32 v; float32 height; float32 h; float32 minor; };
				};
				float32 ptr[2];
			};

			Vector2f();
			Vector2f(float32 x, float32 y);
			Vector2f(float32 v);
			Vector2f(float32* v);
			Vector2f(const Vector2f& v);
			Vector2f(const Vector2i& v);
			Vector2f(const Vector2u& v);
			Vector2f(const Vector2d& v);

			operator float32* () {
				return &ptr[0];
			}

			const float32* operator*() {
				return &ptr[0]; // Be cautious of potential aliasing issues and unintended modifications
			}

			operator Vector2i ();
			operator Vector2u ();
			operator Vector2d ();

			Vector2f operator=(const Vector2f& other);
			Vector2f operator=(const Vector2i& other);
			Vector2f operator=(const Vector2u& other);
			Vector2f operator=(const Vector2d& other);

			friend std::ostream& operator<<(std::ostream& os, const Vector2f& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector2f(" << x << ", " << y << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector2f& v) {
				return v.ToString();
			}

			Vector2f operator-() {
				return Vector2f(-x, -y);
			}

			inline float32& operator[] (size_t index) {
				return this->ptr[index % 2];
			}

			inline const float32& operator[](size_t index) const {
				return this->ptr[index % 2];
			}

			Vector2f& operator+=(const Vector2f& other) {
				this->x += other.x;
				this->y += other.y;
				return *this;
			}

			Vector2f operator + (const Vector2f& other) {
				return Vector2f(other.x + this->x, other.y + this->y);
			}

			friend Vector2f operator + (const Vector2f& other1, const Vector2f& other2) {
				return Vector2f(other1.x + other2.x, other1.y + other2.y);
			}

			Vector2f& operator-=(const Vector2f& other) {
				this->x -= other.x;
				this->y -= other.y;
				return *this;
			}

			Vector2f operator - (const Vector2f& other) {
				return Vector2f(this->x - other.x, this->y - other.y);
			}

			friend Vector2f operator - (const Vector2f& other1, const Vector2f& other2) {
				return Vector2f(other1.x - other2.x, other1.y - other2.y);
			}

			Vector2f& operator*=(const Vector2f& other) {
				this->x *= other.x;
				this->y *= other.y;
				return *this;
			}

			Vector2f operator * (const Vector2f& other) {
				return Vector2f(this->x * other.x, this->y * other.y);
			}

			friend Vector2f operator * (const Vector2f& other1, const Vector2f& other2) {
				return Vector2f(other1.x * other2.x, other1.y * other2.y);
			}

			Vector2f& operator/=(const Vector2f& other) {
				this->x /= other.x;
				this->y /= other.y;
				return *this;
			}

			Vector2f operator / (const Vector2f& other) {
				return Vector2f(this->x / other.x, this->y / other.y);
			}

			friend Vector2f operator / (const Vector2f& other1, const Vector2f& other2) {
				return Vector2f(other1.x / other2.x, other1.y / other2.y);
			}

			friend bool operator==(const Vector2f& l, const Vector2f& r) {
				Vector2f diff(l - r);
				return diff.LenSq() < maths::VectorEpsilon;
			}

			friend bool operator!=(const Vector2f& l, const Vector2f& r) {
				return !(l == r);
			}

			// Pre-increment
			Vector2f& operator++() {
				++x;
				++y;
				return *this;
			}

			// Post-increment (returns a copy, then increments)
			Vector2f operator++(int) {
				Vector2f temp = *this;
				++x;
				++y;
				return temp;
			}

			// Pre-decrement
			Vector2f& operator--() {
				--x;
				--y;
				return *this;
			}

			// Post-decrement (returns a copy, then decrements)
			Vector2f operator--(int) {
				Vector2f temp = *this;
				--x;
				--y;
				return temp;
			}

			float32		SumCord();
			float32		Distance(const Vector2f& r);
			float32		Dot(const Vector2f& r);
			float32		LenSq();
			float32		Len();
			void		Normalize();
			Vector2f	Normalized();
			Vector2f	Normal();
			bool		IsUnit();
			Vector2f	Rotate(const Angle& angle);
			Angle		AngleBetween(const Vector2f& b);
			Vector2f	Project(const Vector2f& onto);
			Vector2f	Reject(const Vector2f& b);
			Vector2f	Reflect(const Vector2f& b);
			Vector2f	Lerp(const Vector2f& e, float32 t);
			Vector2f	Slerp(const Vector2f& e, float32 t);
			Vector2f	Nlerp(const Vector2f& e, float32 t);
			Vector2f	Rotate90();
			Vector2f	Rotate180();
			Vector2f	Rotate270();
			bool		Collineaire(const Vector2f& b);


			static Vector2f Zero() { return Vector2f(); }
		};

		class NKENTSEU_API Vector2d {
		public:
			union {
				struct {
					union { float64 x; float64 u; float64 width; float64 w; float64 major; }; // cordinate (x, y), texture (u, v), size (width, height)
					union { float64 y; float64 v; float64 height; float64 h; float64 minor; };
				};
				float64 ptr[2];
			};

			Vector2d();
			Vector2d(float64 x, float64 y);
			Vector2d(float64 v);
			Vector2d(float64* v);
			Vector2d(const Vector2d& v);
			Vector2d(const Vector2i& v);
			Vector2d(const Vector2u& v);
			Vector2d(const Vector2f& v); // Conversion constructor

			operator float64* () {
				return &ptr[0];
			}

			const float64* operator*() const {
				return &ptr[0]; // Be cautious of potential aliasing issues and unintended modifications
			}

			operator Vector2i ();  // Conversion operator
			operator Vector2u ();  // Conversion operator
			operator Vector2f ();  // Conversion operator

			Vector2d operator=(const Vector2d& other);
			Vector2d operator=(const Vector2i& other);
			Vector2d operator=(const Vector2u& other);
			Vector2d operator=(const Vector2f& other); // Conversion assignment

			friend std::ostream& operator<<(std::ostream& os, const Vector2d& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector2d(" << x << ", " << y << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector2d& v) {
				return v.ToString();
			}

			Vector2d operator-() const {
				return Vector2d(-x, -y);
			}

			inline float64& operator[](size_t index) {
				return this->ptr[index % 2];
			}

			inline const float64& operator[](size_t index) const {
				return this->ptr[index % 2];
			}

			Vector2d& operator+=(const Vector2d& other) {
				this->x += other.x;
				this->y += other.y;
				return *this;
			}

			Vector2d operator + (const Vector2d& other) {
				return Vector2d(other.x + this->x, other.y + this->y);
			}

			friend Vector2d operator + (const Vector2d& other1, const Vector2d& other2) {
				return Vector2d(other1.x + other2.x, other1.y + other2.y);
			}

			Vector2d& operator-=(const Vector2d& other) {
				this->x -= other.x;
				this->y -= other.y;
				return *this;
			}

			Vector2d operator - (const Vector2d& other) {
				return Vector2d(this->x - other.x, this->y - other.y);
			}

			friend Vector2d operator - (const Vector2d& other1, const Vector2d& other2) {
				return Vector2d(other1.x - other2.x, other1.y - other2.y);
			}

			Vector2d& operator*=(const Vector2d& other) {
				this->x *= other.x;
				this->y *= other.y;
				return *this;
			}

			Vector2d operator * (const Vector2d& other) {
				return Vector2d(this->x * other.x, this->y * other.y);
			}

			friend Vector2d operator * (const Vector2d& other1, const Vector2d& other2) {
				return Vector2d(other1.x * other2.x, other1.y * other2.y);
			}

			Vector2d& operator/=(const Vector2d& other) {
				this->x /= other.x;
				this->y /= other.y;
				return *this;
			}

			Vector2d operator / (const Vector2d& other) {
				return Vector2d(this->x / other.x, this->y / other.y);
			}

			friend Vector2d operator / (const Vector2d& other1, const Vector2d& other2) {
				return Vector2d(other1.x / other2.x, other1.y / other2.y);
			}

			friend bool operator==(const Vector2d& l, const Vector2d& r) {
				Vector2d diff(l - r);
				return diff.LenSq() < maths::VectorEpsilon;
			}

			friend bool operator!=(const Vector2d& l, const Vector2d& r) {
				return !(l == r);
			}

			// Pre-increment
			Vector2d& operator++() {
				++x;
				++y;
				return *this;
			}

			// Post-increment (returns a copy, then increments)
			Vector2d operator++(int) {
				Vector2d temp = *this;
				++x;
				++y;
				return temp;
			}

			// Pre-decrement
			Vector2d& operator--() {
				--x;
				--y;
				return *this;
			}

			// Post-decrement (returns a copy, then decrements)
			Vector2d operator--(int) {
				Vector2d temp = *this;
				--x;
				--y;
				return temp;
			}

			float64		SumCord();
			float64		Distance(const Vector2d& r);  // Change return type to float64
			float64		Dot(const Vector2d& r);        // Change return type to float64
			float64		LenSq();
			float64		Len();
			void		Normalize();
			Vector2d	Normalized();
			bool		IsUnit();
			Vector2d	Rotate(const Angle& angle);
			Angle		AngleBetween(const Vector2d& b);
			Vector2d	Project(const Vector2d& onto);
			Vector2d	Reject(const Vector2d& b);
			Vector2d	Reflect(const Vector2d& b);
			Vector2d	Lerp(const Vector2d& e, float64 t);  // Change parameter type to float64
			Vector2d	Slerp(const Vector2d& e, float64 t);  // Change parameter type to float64
			Vector2d	Nlerp(const Vector2d& e, float64 t);  // Change parameter type to float64
			Vector2d	Rotate90();
			Vector2d	Rotate180();
			Vector2d	Rotate270();
			bool		Collineaire(const Vector2d& b);


			static Vector2d Zero() { return Vector2d(); }
		};

		class NKENTSEU_API Vector2i {
		public:
			union {
				struct {
					union { int32 x; int32 u; int32 width; int32 w;; int32 major; }; // cordinate (x, y), texture (u, v), size (width, height)
					union { int32 y; int32 v; int32 height; int32 h; int32 minor; };
				};
				int32 ptr[2];
			};

			Vector2i();
			Vector2i(int32 x, int32 y);
			Vector2i(int32 v);
			Vector2i(int32* v);
			Vector2i(const Vector2i& v);
			Vector2i(const Vector2d& v); // Conversion constructor
			Vector2i(const Vector2u& v);
			Vector2i(const Vector2f& v); // Conversion constructor

			operator int32* () {
				return &ptr[0];
			}

			const int32* operator*() const {
				return &ptr[0]; // Be cautious of potential aliasing issues and unintended modifications
			}

			operator Vector2d ();  // Conversion operator
			operator Vector2u ();  // Conversion operator
			operator Vector2f ();  // Conversion operator

			Vector2i operator=(const Vector2i& other);
			Vector2i operator=(const Vector2d& other); // Conversion assignment
			Vector2i operator=(const Vector2u& v);
			Vector2i operator=(const Vector2f& v); // Conversion assignment

			friend std::ostream& operator<<(std::ostream& os, const Vector2i& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector2i(" << x << ", " << y << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector2i& v) {
				return v.ToString();
			}

			Vector2i operator-() const {
				return Vector2i(-x, -y);
			}

			inline int32& operator[](size_t index) {
				return this->ptr[index % 2];
			}

			inline const int32& operator[](size_t index) const {
				return this->ptr[index % 2];
			}

			Vector2i& operator+=(const Vector2i& other) {
				this->x += other.x;
				this->y += other.y;
				return *this;
			}

			Vector2i operator + (const Vector2i& other) {
				return Vector2i(other.x + this->x, other.y + this->y);
			}

			friend Vector2i operator + (const Vector2i& other1, const Vector2i& other2) {
				return Vector2i(other1.x + other2.x, other1.y + other2.y);
			}

			Vector2i& operator-=(const Vector2i& other) {
				this->x -= other.x;
				this->y -= other.y;
				return *this;
			}

			Vector2i operator - (const Vector2i& other) {
				return Vector2i(this->x - other.x, this->y - other.y);
			}

			friend Vector2i operator - (const Vector2i& other1, const Vector2i& other2) {
				return Vector2i(other1.x - other2.x, other1.y - other2.y);
			}

			Vector2i& operator*=(const Vector2i& other) {
				this->x *= other.x;
				this->y *= other.y;
				return *this;
			}

			Vector2i operator * (const Vector2i& other) {
				return Vector2i(this->x * other.x, this->y * other.y);
			}

			friend Vector2i operator * (const Vector2i& other1, const Vector2i& other2) {
				return Vector2i(other1.x * other2.x, other1.y * other2.y);
			}

			Vector2i& operator/=(const Vector2i& other) {
				this->x /= other.x;
				this->y /= other.y;
				return *this;
			}

			Vector2i operator / (const Vector2i& other) {
				return Vector2i(this->x / other.x, this->y / other.y);
			}

			friend Vector2i operator / (const Vector2i& other1, const Vector2i& other2) {
				return Vector2i(other1.x / other2.x, other1.y / other2.y);
			}

			friend bool operator==(const Vector2i& l, const Vector2i& r) {
				return (l.x == r.x) && (l.y == r.y);
			}

			friend bool operator!=(const Vector2i& l, const Vector2i& r) {
				return !(l == r);
			}

			// Pre-increment
			Vector2i& operator++() {
				++x;
				++y;
				return *this;
			}

			// Post-increment (returns a copy, then increments)
			Vector2i operator++(int) {
				Vector2i temp = *this;
				++x;
				++y;
				return temp;
			}

			// Pre-decrement
			Vector2i& operator--() {
				--x;
				--y;
				return *this;
			}

			// Post-decrement (returns a copy, then decrements)
			Vector2i operator--(int) {
				Vector2i temp = *this;
				--x;
				--y;
				return temp;
			}
			int32		SumCord();
			static Vector2i Zero() { return Vector2i(); }
		};

		class NKENTSEU_API Vector2u {
		public:
			union {
				struct {
					union { uint32 x; uint32 u; uint32 width; uint32 w;; uint32 major; }; // cordinate (x, y), texture (u, v), size (width, height)
					union { uint32 y; uint32 v; uint32 height; uint32 h; uint32 minor; };
				};
				uint32 ptr[2];
			};

			Vector2u();
			Vector2u(uint32 x, uint32 y);
			Vector2u(uint32 v);
			Vector2u(uint32* v);
			Vector2u(const Vector2f& v);
			Vector2u(const Vector2u& v);
			Vector2u(const Vector2d& v); // Conversion constructor
			Vector2u(const Vector2i& v); // Conversion constructor

			operator uint32* () {
				return &ptr[0];
			}

			const uint32* operator*() const {
				return &ptr[0]; // Be cautious of potential aliasing issues and unintended modifications
			}

			operator Vector2d ();  // Conversion operator
			operator Vector2i ();  // Conversion operator
			operator Vector2f ();  // Conversion operator

			Vector2u operator=(const Vector2u& other);
			Vector2u operator=(const Vector2d& other); // Conversion assignment
			Vector2u operator=(const Vector2i& v); // Conversion assignment
			Vector2u operator=(const Vector2f& v); // Conversion assignment

			friend std::ostream& operator<<(std::ostream& os, const Vector2u& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector2u(" << x << ", " << y << ")";
				return ss.str();
			}

			friend std::string ToString(const Vector2u& v) {
				return v.ToString();
			}

			inline uint32& operator[](size_t index) {
				return this->ptr[index % 2];
			}

			inline const uint32& operator[](size_t index) const {
				return this->ptr[index % 2];
			}

			Vector2u& operator+=(const Vector2u& other) {
				this->x += other.x;
				this->y += other.y;
				return *this;
			}

			Vector2u operator + (const Vector2u& other) {
				return Vector2u(other.x + this->x, other.y + this->y);
			}

			friend Vector2u operator + (const Vector2u& other1, const Vector2u& other2) {
				return Vector2u(other1.x + other2.x, other1.y + other2.y);
			}

			Vector2u& operator-=(const Vector2u& other) {
				this->x -= other.x;
				this->y -= other.y;
				return *this;
			}

			Vector2u operator - (const Vector2u& other) {
				return Vector2u(this->x - other.x, this->y - other.y);
			}

			friend Vector2u operator - (const Vector2u& other1, const Vector2u& other2) {
				return Vector2u(other1.x - other2.x, other1.y - other2.y);
			}

			// Be cautious with overflow when using multiplication and division with unsigned integers.
			Vector2u& operator*=(const Vector2u& other) {
				this->x *= other.x;
				this->y *= other.y;
				return *this;
			}

			Vector2u operator * (const Vector2u& other) {
				return Vector2u(this->x * other.x, this->y * other.y);
			}

			friend Vector2u operator * (const Vector2u& other1, const Vector2u& other2) {
				return Vector2u(other1.x * other2.x, other1.y * other2.y);
			}

			// Division by zero with unsigned integers results in the maximum value of uint32. Consider adding checks or alternative approaches if needed.
			Vector2u& operator/=(const Vector2u& other) {
				// Handle potential division by zero (check and return special value, throw exception, etc.)
				if (other.x == 0) {
					// Implement your handling here (e.g., return UINT32_MAX, throw exception)
				}
				this->x /= other.x;
				this->y /= other.y;
				return *this;
			}

			Vector2u operator / (const Vector2u& other) {
				// Handle potential division by zero (check and return special value, throw exception, etc.)
				if (other.x == 0) {
					// Implement your handling here (e.g., return UINT32_MAX, throw exception)
				}
				return Vector2u(this->x / other.x, this->y / other.y);
			}

			friend Vector2u operator / (const Vector2u& other1, const Vector2u& other2) {
				return Vector2u(other1.x / other2.x, other1.y / other2.y);
			}

			friend bool operator==(const Vector2u& l, const Vector2u& r) {
				return (l.x == r.x) && (l.y == r.y);
			}

			friend bool operator!=(const Vector2u& l, const Vector2u& r) {
				return !(l == r);
			}

			// Pre-increment
			Vector2u& operator++() {
				++x;
				++y;
				return *this;
			}

			// Post-increment (returns a copy, then increments)
			Vector2u operator++(int) {
				Vector2u temp = *this;
				++x;
				++y;
				return temp;
			}

			// Pre-decrement
			Vector2u& operator--() {
				--x;
				--y;
				return *this;
			}

			// Post-decrement (returns a copy, then decrements)
			Vector2u operator--(int) {
				Vector2u temp = *this;
				--x;
				--y;
				return temp;
			}

			uint32		SumCord();
			static Vector2u Zero() { return Vector2u(); }
		};


		template<typename T>
		class Vector2T {
		public:
			union {
				struct {
					union { T x; T u; T width; T w; T major; }; // cordinate (x, y), texture (u, v), size (width, height)
					union { T y; T v; T height; T h; T minor; };
				};
				T ptr[2];
			};

			Vector2T() : x((T)0), y((T)0) {}

			Vector2T(T val) : x(val), y(val) {}

			template<typename U>
			Vector2T(U val) : x((T)val), y((T)val) {}

			Vector2T(T* fv) : x(fv[0]), y(fv[1]) {}

			template<typename U>
			Vector2T(U* fv) : x((T)fv[0]), y((T)fv[1]) {}

			Vector2T(T x, T y) : x(x), y(y) {}

			template<typename U>
			Vector2T(U x, U y) : x((T)x), y((T)y) {}

			Vector2T(const Vector2T& v) : x(v.x), y(v.y) {}

			template<typename U>
			Vector2T(const Vector2T<U>& v) : x((T)v.x), y((T)v.y) {}

			Vector2T(const Vector2f& v) : x((T)v.x), y((T)v.y) {}
			Vector2T(const Vector2d& v) : x((T)v.x), y((T)v.y) {}
			Vector2T(const Vector2i& v) : x((T)v.x), y((T)v.y) {}
			Vector2T(const Vector2u& v) : x((T)v.x), y((T)v.y) {}

			Vector2T operator=(const Vector2T& other) {
				if (this->x != other.x && this->y != other.y) {
					this->x = other.x;
					this->y = other.y;
				}
				return *this;
			}

			friend std::ostream& operator<<(std::ostream& os, const Vector2T& e) {
				return os << e.ToString();
			}

			std::string ToString() const {
				std::stringstream ss;
				ss << "Vector2T(" << x << ", " << y << ")";
				return ss.str();
			}

			Vector2T operator-() const {
				return Vector2T(-x, -y);
			}

			template<typename U>
			operator Vector2T<U>() {
				return Vector2T((U)x, (U)y);
			}

			operator Vector2f () {
				return Vector2f(x, y);
			}

			operator Vector2d () {
				return Vector2d(x, y);
			}

			operator Vector2i () {
				return Vector2i(x, y);
			}

			operator Vector2u () {
				return Vector2u(x, y);
			}

			operator T* () {
				T tptr[2] = { x, y };
				return &tptr[0];
			}

			inline T& operator[](size_t index) {
				return this->ptr[index % 2];
			}

			inline const T& operator[](size_t index) const {
				return this->ptr[index % 2];
			}

			Vector2T& operator+=(const Vector2T& other) {
				this->x += other.x;
				this->y += other.y;
				return *this;
			}

			Vector2T& operator-=(const Vector2T& other) {
				this->x -= other.x;
				this->y -= other.y;
				return *this;
			}

			Vector2T& operator*=(const Vector2T& other) {
				this->x *= other.x;
				this->y *= other.y;
				return *this;
			}

			Vector2T operator + (const Vector2T& other) {
				return Vector2T(other.x + this->x, other.y + this->y);
			}

			friend Vector2T operator + (const Vector2T& other1, const Vector2T& other2) {
				return Vector2T(other1.x + other2.x, other1.y + other2.y);
			}

			Vector2T operator - (const Vector2T& other) {
				return Vector2T(this->x - other.x, this->y - other.y);
			}


			friend Vector2T operator - (const Vector2T& other1, const Vector2T& other2) {
				return Vector2T(other1.x - other2.x, other1.y - other2.y);
			}

			Vector2T operator * (const Vector2T& other) {
				return Vector2T(this->x * other.x, this->y * other.y);
			}

			friend Vector2T operator * (const Vector2T& other1, const Vector2T& other2) {
				return Vector2T(other1.x * other2.x, other1.y * other2.y);
			}

			Vector2T operator / (const Vector2T& other) {
				return Vector2T(this->x / other.x, this->y / other.y);
			}

			friend Vector2T operator / (const Vector2T& other1, const Vector2T& other2) {
				return Vector2T(other1.x / other2.x, other1.y / other2.y);
			}

			friend bool operator==(const Vector2T& l, const Vector2T& r) {
				return l.x == r.x && l.y == r.y;
			}

			friend bool operator!=(const Vector2T& l, const Vector2T& r) {
				return !(l == r);
			}
		};

		//using Vector2 = Vector2f;
		using Vec2 = Vector2f;
		using Vec2f = Vector2f;
		using Vec2i = Vector2i;
		using Vec2u = Vector2u;
		using Vec2d = Vector2d;

		template <typename T>
		struct NKENTSEU_API Vector2 {
			union {
				T ptr[2];
				struct {
					union { T x, u, width, major; };
					union { T y, v, height, minor; };
				};
			};

			// Constructeurs
			Vector2() : x(0), y(0) {} // Constructeur par defaut

			Vector2(T x, T y) : x(x), y(y) {} // Constructeur avec paramètres

			Vector2(const T* arr) : x(arr[0]), y(arr[1]) {} // Constructeur avec pointeur

			template <typename U>
			Vector2(const U& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {} // Constructeur avec type U

			Vector2(const Vector2& other) : x(other.x), y(other.y) {} // Constructeur par copie

			// Operateur d'affectation
			Vector2& operator=(const Vector2& other) {
				if (this != &other) {
					x = other.x;
					y = other.y;
				}
				return *this;
			}

			// Operateur d'affectation pour type U
			template <typename U>
			Vector2& operator=(const Vector2<U>& other) {
				x = static_cast<T>(other.x);
				y = static_cast<T>(other.y);
				return *this;
			}

			// Operateur d'egalite
			bool operator==(const Vector2& other) const {
				return x == other.x && y == other.y;
			}

			// Operateur de difference
			bool operator!=(const Vector2& other) const {
				return !(*this == other);
			}

			// Operateur d'addition
			Vector2 operator+(const Vector2& other) const {
				return Vector2(x + other.x, y + other.y);
			}

			Vector2& operator+=(const Vector2& other) {
				x += other.x;
				y += other.y;
				return *this;
			}

			// Operateur de soustraction
			Vector2 operator-(const Vector2& other) const {
				return Vector2(x - other.x, y - other.y);
			}

			Vector2& operator-=(const Vector2& other) {
				x -= other.x;
				y -= other.y;
				return *this;
			}

			// Operateur de multiplication (scalaire)
			Vector2 operator*(T scalar) const {
				return Vector2(x * scalar, y * scalar);
			}

			Vector2& operator*=(T scalar) {
				x *= scalar;
				y *= scalar;
				return *this;
			}

			// Operateur de division (scalaire)
			Vector2 operator/(T scalar) const {
				return Vector2(x / scalar, y / scalar);
			}

			Vector2& operator/=(T scalar) {
				x /= scalar;
				y /= scalar;
				return *this;
			}

			// Operateur de multiplication (scalaire) pour type U
			template <typename U>
			Vector2<T> operator*(U scalar) const {
				return Vector2<T>(x * static_cast<T>(scalar), y * static_cast<T>(scalar));
			}

			template <typename U>
			Vector2<T>& operator*=(U scalar) {
				x *= static_cast<T>(scalar);
				y *= static_cast<T>(scalar);
				return *this;
			}

			// Operateur de division (scalaire) pour type U
			template <typename U>
			Vector2<T> operator/(U scalar) const {
				return Vector2<T>(x / static_cast<T>(scalar), y / static_cast<T>(scalar));
			}

			template <typename U>
			Vector2<T>& operator/=(U scalar) {
				x /= static_cast<T>(scalar);
				y /= static_cast<T>(scalar);
				return *this;
			}

			// Operateur unaire -
			Vector2 operator-() const {
				return Vector2(-x, -y);
			}

			// Operateurs d'incrementation
			Vector2& operator++() { // Pre-incrementation
				++x;
				++y;
				return *this;
			}

			Vector2 operator++(int) { // Post-incrementation
				Vector2 temp = *this;
				++(*this);
				return temp;
			}

			// Operateurs de decrementation
			Vector2& operator--() { // Pre-decrementation
				--x;
				--y;
				return *this;
			}

			Vector2 operator--(int) { // Post-decrementation
				Vector2 temp = *this;
				--(*this);
				return temp;
			}

			// Operateur de cast vers T*
			operator T* () {
				return ptr;
			}

			// Operateur de cast vers const T*
			operator const T* () const {
				return ptr;
			}

			// Affichage du vecteur
			friend std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
				os << "(" << vec.x << ", " << vec.y << ")";
				return os;
			}

			// Conversion en chaîne de caractères
			std::string ToString() const {
				std::stringstream ss;
				ss << "(" << x << ", " << y << ")";
				return ss.str();
			}

			// Fonction amie pour convertir un Vector2 en chaîne de caractères
			friend std::string ToString(const Vector2& vec) {
				return vec.ToString();
			}
		};

		using vec2i = Vector2<int32>;
		using vec2u = Vector2<uint32>;
		using vec2f = Vector2<float32>;
		using vec2d = Vector2<float64>;

		// Fonction pour calculer la somme des coordonnees
		template <typename T>
		T SumCord(const Vector2<T>& vec) {
			return vec.x + vec.y;
		}

		// Fonction pour calculer la distance entre deux vecteurs
		template <typename T>
		T Distance(const Vector2<T>& vec1, const Vector2<T>& vec2) {
			return std::sqrt(std::pow(vec2.x - vec1.x, 2) + std::pow(vec2.y - vec1.y, 2));
		}

		// Fonction pour calculer le produit scalaire entre deux vecteurs
		template <typename T>
		T Dot(const Vector2<T>& vec1, const Vector2<T>& vec2) {
			return vec1.x * vec2.x + vec1.y * vec2.y;
		}

		// Fonction pour calculer la norme au carre d'un vecteur
		template <typename T>
		T LenSq(const Vector2<T>& vec) {
			return vec.x * vec.x + vec.y * vec.y;
		}

		// Fonction pour calculer la norme d'un vecteur
		template <typename T>
		T Len(const Vector2<T>& vec) {
			return std::sqrt(LenSq(vec));
		}

		// Fonction pour normaliser un vecteur
		template <typename T>
		Vector2<T> Normalize(const Vector2<T>& vec) {
			T length = Len(vec);
			if (length != 0) {
				return vec / length;
			}
			return Vector2<T>(0, 0);
		}

		// Fonction pour normaliser un vecteur (version modifiant le vecteur original)
		template <typename T>
		void Normalized(Vector2<T>& vec) {
			T length = Len(vec);
			if (length != 0) {
				vec /= length;
			}
		}

		// Fonction pour verifier si un vecteur est unitaire (norme egale à 1)
		template <typename T>
		bool IsUnit(const Vector2<T>& vec) {
			return std::abs(LenSq(vec) - static_cast<T>(1)) < std::numeric_limits<T>::epsilon();
		}

		// Fonction pour calculer l'angle entre deux vecteurs
		template <typename T>
		T AngleBetween(const Vector2<T>& vec1, const Vector2<T>& vec2) {
			T len1 = Len(vec1);
			T len2 = Len(vec2);
			if (len1 == 0 || len2 == 0) {
				return 0;
			}
			return std::acos(Dot(vec1, vec2) / (len1 * len2));
		}

		// Fonction pour projeter un vecteur sur un autre
		template <typename T>
		Vector2<T> Project(const Vector2<T>& vec, const Vector2<T>& onto) {
			T ontoLenSq = LenSq(onto);
			if (ontoLenSq == 0) {
				return Vector2<T>(0, 0);
			}
			return onto * (Dot(vec, onto) / ontoLenSq);
		}

		// Fonction pour rejeter un vecteur par rapport à un autre
		template <typename T>
		Vector2<T> Reject(const Vector2<T>& vec, const Vector2<T>& onto) {
			return vec - Project(vec, onto);
		}

		// Fonction pour refleter un vecteur par rapport à une normale
		template <typename T>
		Vector2<T> Reflect(const Vector2<T>& vec, const Vector2<T>& normal) {
			return vec - (normal * static_cast<T>(2) * Dot(vec, normal));
		}

		// Fonction pour realiser une interpolation lineaire entre deux vecteurs
		template <typename T>
		Vector2<T> Lerp(const Vector2<T>& start, const Vector2<T>& end, T t) {
			return start + (end - start) * t;
		}

		// Fonction pour realiser une interpolation spherique entre deux vecteurs
		template <typename T>
		Vector2<T> Slerp(const Vector2<T>& start, const Vector2<T>& end, T t) {
			T dot = Dot(start, end);
			dot = (dot < static_cast<T>(-1)) ? static_cast<T>(-1) : ((dot > static_cast<T>(1)) ? static_cast<T>(1) : dot);
			T theta = std::acos(dot) * t;
			Vector2<T> relative = end - start * dot;
			relative = Normalize(relative);
			return ((start * std::cos(theta)) + (relative * std::sin(theta)));
		}

		// Fonction pour realiser une interpolation lineaire normale entre deux vecteurs
		template <typename T>
		Vector2<T> Nlerp(const Vector2<T>& start, const Vector2<T>& end, T t) {
			return Normalize(Lerp(start, end, t));
		}

		// Fonction pour faire pivoter un vecteur de 90 degres dans le sens horaire
		template <typename T>
		Vector2<T> Rotate90(const Vector2<T>& vec) {
			return Vector2<T>(vec.y, -vec.x);
		}

		// Fonction pour faire pivoter un vecteur de 180 degres
		template <typename T>
		Vector2<T> Rotate180(const Vector2<T>& vec) {
			return Vector2<T>(-vec.x, -vec.y);
		}

		// Fonction pour faire pivoter un vecteur de 270 degres dans le sens horaire
		template <typename T>
		Vector2<T> Rotate270(const Vector2<T>& vec) {
			return Vector2<T>(-vec.y, vec.x);
		}

		// Fonction pour verifier si deux vecteurs sont collineaires
		template <typename T>
		bool Collinear(const Vector2<T>& vec1, const Vector2<T>& vec2) {
			T cross = vec1.x * vec2.y - vec1.y * vec2.x;
			return std::abs(cross) < std::numeric_limits<T>::epsilon();
		}

		// Fonction pour creer un vecteur nul
		template <typename T>
		Vector2<T> ZeroVec2() {
			return Vector2<T>(0, 0);
		}
	}
} // namespace nkentseu

#endif    // __NKENTSEU_VECTOR2_H__