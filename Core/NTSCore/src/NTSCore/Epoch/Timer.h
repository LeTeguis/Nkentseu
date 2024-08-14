//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_TIMER_H__
#define __NKENTSEU_TIMER_H__

#pragma once

// Include guards to prevent multiple inclusions
#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"

#include <chrono>

namespace nkentseu {
    struct NKENTSEU_API ElapsedTime {
        float64 nanoSeconds = 0.0;
        float64 milliSeconds = 0.0;
        float64 seconds = 0.0;
    };

    class NKENTSEU_API Timer {
        public:
            Timer();
            ~Timer();
            const ElapsedTime& Reset();
            const ElapsedTime& Elapsed();
        private:
            std::chrono::high_resolution_clock::time_point m_StartTime;
    };
} // namespace nkentseu

#endif // __NKENTSEU_TIMER_H__
