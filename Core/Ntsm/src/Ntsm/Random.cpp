//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:18:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Random.h"

namespace nkentseu {

    namespace maths
    {
        RandomClass::RandomClass() {
            srand((uint32)time(NULL));
        }

        uint32 RandomClass::MaxUInt() {
            return RAND_MAX;
        }

        uint32 RandomClass::NextUInt32() {
            return rand();
        }

        uint32 RandomClass::NextUInt32(uint32 max) {
            return NextUInt32() % max;
        }

        uint32 RandomClass::NextUInt32(uint32 min, uint32 max) {
            return NextUInt32(RangeUInt(min, max));
        }

        uint32 RandomClass::NextUInt32(const RangeUInt& range) {
            return (NextUInt32() * range.Len()) + range.GetMin();
        }

        int32 RandomClass::MaxInt() {
            return (RAND_MAX / 2) + 1;
        }

        int32 RandomClass::MinInt() {
            return -RAND_MAX / 2;
        }

        int32 RandomClass::NextInt32() {
            RangeInt rangint(MinInt(), MaxInt());
            return ((int32)NextUInt32() % rangint.Len()) + rangint.GetMin();
        }

        int32 RandomClass::NextInt32(int32 limite) {
            int32 rand_ = NextInt32();
            if (rand_ > 0 && limite > 0) {
                rand_ %= limite;
            }
            else if (rand_ < 0 && limite < 0) {
                rand_ = -((-rand_) % (-limite));
            }
            return (rand_ % limite);
        }

        int32 RandomClass::NextInt32(int32 min, int32 max) {
            return NextInt32(RangeInt(min, max));
        }

        int32 RandomClass::NextInt32(const RangeInt& range) {
            return ((int32)NextUInt32() % range.Len()) + range.GetMin();
        }

        float32 RandomClass::NextFloat32() {
            return (float)rand() / (float)RAND_MAX;
        }

        float32 RandomClass::NextFloat32(float32 limite) {
            return NextFloat32() * limite;
        }

        float32 RandomClass::NextFloat32(float32 min, float32 max) {
            return NextFloat32(RangeFloat(min, max));
        }

        float32 RandomClass::NextFloat32(const RangeFloat& range) {
            return (NextFloat32() * range.Len()) + range.GetMin();
        }
    }
}    // namespace nkentseu