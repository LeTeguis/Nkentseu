//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Timer.h"

namespace nkentseu {
    Timer::Timer() {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    Timer::~Timer(){
    }

    const ElapsedTime& Timer::Reset()
    {
        static ElapsedTime elapsed;

        elapsed = Elapsed();

        m_StartTime = std::chrono::high_resolution_clock::now();

        return elapsed;
    }

    const ElapsedTime& Timer::Elapsed()
    {
        static ElapsedTime elapsed;
        std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<float64, std::nano>(endTime - m_StartTime);

        elapsed.nanoSeconds = duration.count();
        elapsed.milliSeconds = elapsed.nanoSeconds * 0.001 * 0.001;
        elapsed.seconds = elapsed.nanoSeconds * 0.001 * 0.001 * 0.001;

        return elapsed;
    }

} // namespace nkentseu
