//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:59:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Utils.h"

#include <math.h>

namespace nkentseu {

    namespace maths {
        float32 Sqrt(float32 x) {
            return sqrtf(x);
        }

        float32 Sqrt(float32 x, int32 n) {
            return pow(x, 1.0 / n);
        }

        float32 Cos(const Angle& angle) {
            return cosf(angle.Rad());
        }

        float32 Sin(const Angle& angle) {
            return sinf(angle.Rad());
        }

        Angle ACos(float32 x) {
            return Angle::FromRadian(acosf(x));
        }

        Angle ASin(float32 x) {
            return Angle::FromRadian(asinf(x));
        }

        float32 Tan(const Angle& angle) {
            return tanf(angle.Rad());
        }

        Angle ATan(float32 x) {
            return Angle::FromRadian(atanf(x));
        }

        Angle ATan(float32 x, float32 y) {
            return Angle::FromRadian(atan2f(x, y));
        }

        float32 FMod(float32 x, float32 y) {
            return (float32)fmod(x, y);
        }

        float32 Floor(float32 x) {
            return floor(x);
        }

        float32 Ceil(float32 x) {
            return ceil(x);
        }

        float32 Abs(float32 x) {
            return abs(x);
        }

        float32 Pow(float32 x, float32 y) {
            return pow(x, y);
        }

        float32 Exp(float32 x) {
            return expf(x);
        }

        float32 Logf(float32 x) {
            return logf(x);
        }

        int32 ILogbf(float32 x) {
            return ilogbf(x);
        }

        float32 LDexp(float32 x, int32 exp) {
            return ldexpf(x, exp);
        }

        bool EqualF32(float32 a, float32 b) {
            return Abs(a - b) < Epsilon;
        }

        bool EqualF64(float64 a, float64 b) {
            return Abs(a - b) < Epsilon;
        }

        float32 Round(float32 n) {
            float32 f = Floor(n);
            float32 c = Ceil(n);
            if (n - f > c - n) return c;
            if (n - f < c - n) return f;
            return c;
        }
    }
}    // namespace nkentseu