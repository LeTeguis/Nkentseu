//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 4:59:35 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Utils.h"
#include <math.h>

namespace nkentseu {
    float32 maths::Sqrt(float32 x) {
        return sqrtf(x);
    }

    float32 maths::Sqrt(float32 x, int32 n) {
        return pow(x, 1.0 / n);
    }

    float32 maths::Cos(const Angle& angle) {
        return cosf(angle.Rad());
    }

    float32 maths::Sin(const Angle& angle) {
        return sinf(angle.Rad());
    }

    Angle maths::ACos(float32 x) {
        return Angle::FromRadian(acosf(x));
    }

    Angle maths::ASin(float32 x) {
        return Angle::FromRadian(asinf(x));
    }

    float32 maths::Tan(const Angle& angle) {
        return tanf(angle.Rad());
    }

    Angle maths::ATan(float32 x) {
        return Angle::FromRadian(atanf(x));
    }

    Angle maths::ATan(float32 x, float32 y) {
        return Angle::FromRadian(atan2f(x, y));
    }

    float32 maths::FMod(float32 x, float32 y) {
        return (float32)fmod(x, y);
    }

    float32 maths::Floor(float32 x) {
        return floor(x);
    }

    float32 maths::Ceil(float32 x) {
        return ceil(x);
    }

    float32 maths::Abs(float32 x) {
        return abs(x);
    }

    float32 maths::Pow(float32 x, float32 y) {
        return pow(x, y);
    }

    float32 maths::Exp(float32 x) {
        return expf(x);
    }

    float32 maths::Logf(float32 x) {
        return logf(x);
    }

    int32 maths::ILogbf(float32 x) {
        return ilogbf(x);
    }

    float32 maths::LDexp(float32 x, int32 exp) {
        return ldexpf(x, exp);
    }

    bool maths::EqualF32(float32 a, float32 b) {
        return Abs(a - b) < Epsilon();
    }

    bool maths::EqualF64(float64 a, float64 b) {
        return Abs(a - b) < Epsilon();
    }

    float32 maths::Round(float32 n) {
        float32 f = Floor(n);
        float32 c = Ceil(n);
        if (n - f > c - n) return c;
        if (n - f < c - n) return f;
        return c;
    }
    const float64 maths::Pi()
    {
        return float64(3.14159265358979323846f);
    }
    const float64 maths::Pi__2()
    {
        return float64(Pi() / 2);
    }
    const float64 maths::Pi_2()
    {
        return float64(Pi() * 2);
    }
    const float64 maths::Sqrt2()
    {
        return float64(1.41421356237f);
    }
    const float64 maths::Sqrt3()
    {
        return float64(1.73205080757f);
    }
    const float64 maths::PuissanceApprox()
    {
        return float64(8192.0f);
    }
    const float64 maths::Epsilon()
    {
        return float64(0.000001220703125f);
    }
    const float64 maths::VectorEpsilon()
    {
        return float64(Epsilon());
    }
    const float64 maths::MatrixEpsilon()
    {
        return float64(Epsilon());
    }
    const float64 maths::QuatEpsilon()
    {
        return float64(Epsilon());
    }
}    // namespace nkentseu