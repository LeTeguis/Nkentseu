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

    void FPSTimer::Update(float64 delta) {
        m_NumFrame++;
        m_AccumulateTime += delta;

        if (m_AccumulateTime < 1.0) return;

        m_CurrentFps = maths::Max<uint32>(1, m_NumFrame / m_AccumulateTime);
        m_TimeFps = 1000.0 / (float64)m_CurrentFps;

        m_AccumulateTime = 0.0;
        m_NumFrame = 0;
    }

    float64 FPSTimer::GetFrameTime()
    {
        //std::lock_guard<std::mutex> lock(mutex);
        return m_TimeFps;
    }

    uint32 FPSTimer::GetCurrentFrame()
    {
        //std::lock_guard<std::mutex> lock(mutex);
        return m_NumFrame;
    }

    uint32 FPSTimer::GetFrameRate()
    {
        //std::lock_guard<std::mutex> lock(mutex);
        return m_CurrentFps;
    }

    uint32 FPSTimer::GetFps()
    {
        return GetFrameRate();
    }

}    // namespace nkentseu