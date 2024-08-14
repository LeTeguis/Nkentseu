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
    namespace maths
    {
		float32 NKENTSEU_API Sqrt(float32 x);
		float32 NKENTSEU_API Sqrt(float32 x, int32 n);
		float32 NKENTSEU_API Cos(const Angle& angle);
		float32 NKENTSEU_API Sin(const Angle& angle);
		Angle NKENTSEU_API ACos(float32 x);
		Angle NKENTSEU_API ASin(float32 x);
		float32 NKENTSEU_API Tan(const Angle& angle);
		Angle NKENTSEU_API ATan(float32 x);
		Angle NKENTSEU_API ATan(float32 x, float32 y);
		float32 NKENTSEU_API FMod(float32 x, float32 y);
		float32 NKENTSEU_API Floor(float32 x);
		float32 NKENTSEU_API Ceil(float32 x);
		float32 NKENTSEU_API Abs(float32 x);
		float32 NKENTSEU_API Pow(float32 x, float32 y);
		float32 NKENTSEU_API Exp(float32 x);
		float32 NKENTSEU_API Logf(float32 x);
		int32   NKENTSEU_API ILogbf(float32 x);
		float32 NKENTSEU_API LDexp(float32 x, int32 exp);
		Angle NKENTSEU_API Copysign(float32 number, float32 sign);

		bool NKENTSEU_API EqualF32(float32 a, float32 b);
		bool NKENTSEU_API EqualF64(float64 a, float64 b);

		float32 NKENTSEU_API Round(float32 n);

		#define MAX(x, y) ((x) < (y) ? y : x)
		#define MIN(x, y) ((x) < (y) ? x : y)
		#define ABS(x) ((x) < 0 ? -(x) : (x))

		template<typename T> T Min(T a, T b) {
			return (a < b) ? a : b;
		}

		template<typename T> T Max(T a, T b) {
			return (a > b) ? a : b;
		}

		template<typename T> T Abs(T a) {
			return (a < 0) ? -a : a;
		}

		template<typename T> T Min(std::vector<T> list) {
			T min_ = list[0];
			for (int32 i = 1; i < list.size(); i++) {
				if (min_ > list[i]) {
					min_ = list[i];
				}
			}
			return min_;
		}

		template<typename T> T Max(std::vector<T> list) {
			T max_ = list[0];
			for (int32 i = 1; i < list.size(); i++) {
				if (max_ < list[i]) {
					max_ = list[i];
				}
			}
			return max_;
		}

		template<typename T> T Clamp(T value, T min, T max) {
			if (value < min) return min;
			if (value > max) return max;
			return value;
		}

		const float64 Pi = 3.14159265358979323846;
		const float64 Pis2 = 3.14159265358979323846 / 2.0;
		const float64 Pi2 = 3.14159265358979323846 * 2.0;
		const float64 Sqrt2 = 1.41421356237;
		const float64 Sqrt3 = 1.73205080757;

		const float64 DEG2RAD = Pi / 180.0f;
		const float64 RAD2DEG = 180.0f / Pi;

		const float64 PuissanceApprox = 8192.0;

		const float64 Epsilon = 0.000001220703125;
		const float64 VectorEpsilon = 0.000001220703125;
		const float64 MatrixEpsilon = 0.000001220703125;
		const float64 QuatEpsilon = 0.000001220703125;

		enum class NKENTSEU_API PrimitiveType {
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