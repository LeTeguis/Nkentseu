//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:31:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "FPSTimer.h"

#include "Ntsm/Utils.h"

namespace nkentseu {
    FPSTimer::FPSTimer(){
    }

    FPSTimer::~FPSTimer(){
    }

    void FPSTimer::Update() {
        //std::lock_guard<std::mutex> lock(mutex);  // Acquire lock before accessing shared data
        float64 delta = m_Timer.Elapsed().seconds;

        if (delta >= 1.0) {
            m_FrameRate = maths::Max<float64>(1, m_CurrentFrame / delta);
            m_CurrentFrame = 0;
            m_FrameTime = 1000.0 / (float64)m_FrameRate;
            m_Timer.Reset();
        }

        m_CurrentFrame++;
    }

    float32 FPSTimer::GetFrameTime()
    {
        //std::lock_guard<std::mutex> lock(mutex);
        return m_FrameTime;
    }

    uint32 FPSTimer::GetCurrentFrame()
    {
        //std::lock_guard<std::mutex> lock(mutex);
        return m_CurrentFrame;
    }

    uint32 FPSTimer::GetFrameRate()
    {
        //std::lock_guard<std::mutex> lock(mutex);
        return m_FrameRate;
    }

    uint32 FPSTimer::GetFps()
    {
        return GetFrameRate();
    }

}    // namespace nkentseu