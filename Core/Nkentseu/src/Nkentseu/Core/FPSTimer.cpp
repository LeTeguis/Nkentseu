//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:31:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "FPSTimer.h"

namespace nkentseu {
    FPSTimer::FPSTimer()
    {
    }
    void FPSTimer::Update() {
        std::lock_guard<std::mutex> lock(mutex);  // Acquire lock before accessing shared data

        m_CurrentFps++;

        if (m_Timer.Elapsed() >= (double)m_UpdateIntervalMs / 1000.0) {
            m_CurrentFps = 0;
            m_Timer.Reset();
        }
    }

    // Set the update interval in milliseconds
    void FPSTimer::SetUpdateIntervalMs(uint32 intervalMs) {
        m_UpdateIntervalMs = intervalMs;
    }

    uint32 FPSTimer::GetFps() {
        std::lock_guard<std::mutex> lock(mutex);  // Acquire lock before reading shared data
        return m_CurrentFps;
    }

}    // namespace nkentseu