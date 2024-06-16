//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:59:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_MATHS_UTILS_H__
#define __NKENTSEU_MATHS_UTILS_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Angle.h>

#include <vector>

namespace nkentseu {
    struct NKENTSEU_API maths
    {
		static float32 Sqrt(float32 x);
		static float32 Sqrt(float32 x, int32 n);
		static float32 Cos(const Angle& angle);
		static float32 Sin(const Angle& angle);
		static Angle ACos(float32 x);
		static Angle ASin(float32 x);
		static float32 Tan(const Angle& angle);
		static Angle ATan(float32 x);
		static Angle ATan(float32 x, float32 y);
		static float32 FMod(float32 x, float32 y);
		static float32 Floor(float32 x);
		static float32 Ceil(float32 x);
		static float32 Abs(float32 x);
		static float32 Pow(float32 x, float32 y);
		static float32 Exp(float32 x);
		static float32 Logf(float32 x);
		static int32   ILogbf(float32 x);
		static float32 LDexp(float32 x, int32 exp);

		static bool EqualF32(float32 a, float32 b);
		static bool EqualF64(float64 a, float64 b);

		static float32 Round(float32 n);

		#define MAX(x, y) ((x) < (y) ? y : x)
		#define MIN(x, y) ((x) < (y) ? x : y)
		#define ABS(x) ((x) < 0 ? -(x) : (x))

		template<typename T> static T Min(T a, T b) {
			return (a < b) ? a : b;
		}

		template<typename T> static T Max(T a, T b) {
			return (a > b) ? a : b;
		}

		template<typename T> static T Abs(T a) {
			return (a < 0) ? -a : a;
		}

		template<typename T> static T Min(std::vector<T> list) {
			T min_ = list[0];
			for (int32 i = 1; i < list.size(); i++) {
				if (min_ > list[i]) {
					min_ = list[i];
				}
			}
			return min_;
		}

		template<typename T> static T Max(std::vector<T> list) {
			T max_ = list[0];
			for (int32 i = 1; i < list.size(); i++) {
				if (max_ < list[i]) {
					max_ = list[i];
				}
			}
			return max_;
		}

		template<typename T> static T  Clamp(T value, T min, T max) {
			if (value < min) return min;
			if (value > max) return max;
			return value;
		}

		static const float64 Pi();
		static const float64 Pi__2();
		static const float64 Pi_2();
		static const float64 Sqrt2();
		static const float64 Sqrt3();

		static const float64 PuissanceApprox();

		static const float64 Epsilon();
		static const float64 VectorEpsilon();
		static const float64 MatrixEpsilon();
		static const float64 QuatEpsilon();

		#define M4D(aRow, bCol, a, b)	a.ptr[0 * 4 + aRow] * b.ptr[bCol * 4 + 0] + \
										a.ptr[1 * 4 + aRow] * b.ptr[bCol * 4 + 1] + \
										a.ptr[2 * 4 + aRow] * b.ptr[bCol * 4 + 2] + \
										a.ptr[3 * 4 + aRow] * b.ptr[bCol * 4 + 3]

		#define M4V4D(mRow, x, y, z, w, m) \
									x * m.ptr[0 * 4 + mRow] + \
									y * m.ptr[1 * 4 + mRow] + \
									z * m.ptr[2 * 4 + mRow] + \
									w * m.ptr[3 * 4 + mRow]

		#define M4SWAP(x, y) \
									{float64 t = x; x = y; y = t; }

		#define M4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
									(x[c0 * 4 + r0] * (x[c1 * 4 + r1] * x[c2 * 4 + r2] - x[c1 * 4 + r2] * \
									x[c2 * 4 + r1]) - x[c1 * 4 + r0] * (x[c0 * 4 + r1] * x[c2 * 4 + r2] - \
									x[c0 * 4 + r2] * x[c2 * 4 + r1]) + x[ c2 * 4 + r0] * (x[c0 * 4 + r1] * \
									x[c1 * 4 + r2] - x[c0 * 4 + r2] * x[c1 * 4 + r1]))

		enum class PrimitiveType {
			CHAR,
			SIGNED_CHAR,
			UNSIGNED_CHAR,
			SHORT,
			UNSIGNED_SHORT,
			INT,
			UNSIGNED_INT,
			LONG,
			UNSIGNED_LONG,
			LONG_LONG,
			UNSIGNED_LONG_LONG,
			FLOAT,
			DOUBLE,
			LONG_DOUBLE,
			BOOL
		};
	};
} // namespace nkentseu

#endif    // __NKENTSEU_MATHS_UTILS_H__