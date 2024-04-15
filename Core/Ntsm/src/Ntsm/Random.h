//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:18:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_RANDOM_H__
#define __NKENTSEU_RANDOM_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <Ntsm/Range.h>

namespace nkentseu {
    class NKENTSEU_API RandomClass {
    private:
        RandomClass();

    public:

        uint32 MaxUInt();
        int32 MaxInt();
        int32 MinInt();

        uint32 NextUInt32();
        uint32 NextUInt32(uint32 max);
        uint32 NextUInt32(uint32 min, uint32 max);
        uint32 NextUInt32(const RangeUInt& range);
        int32 NextInt32();
        int32 NextInt32(int32 limite);
        int32 NextInt32(int32 min, int32 max);
        int32 NextInt32(const RangeInt& range);
        float32 NextFloat32();
        float32 NextFloat32(float32 limite);
        float32 NextFloat32(float32 min, float32 max);
        float32 NextFloat32(const RangeFloat& range);

        static RandomClass& Instance() {
            static RandomClass random;
            return random;
        }
    };
    #define Random RandomClass::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_RANDOM_H__