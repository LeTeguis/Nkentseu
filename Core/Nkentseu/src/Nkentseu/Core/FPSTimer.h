//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 2:31:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_FPSTIMER_H__
#define __NKENTSEU_FPSTIMER_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>
#include <System/Epoch/Timer.h>
#include <mutex>

namespace nkentseu {
    class NKENTSEU_API FPSTimer {
    private:
        uint32 m_CurrentFps;
        Timer m_Timer;
        uint32 m_UpdateIntervalMs;  // Customizable update interval in milliseconds

        // Mutex to protect shared data
        std::mutex mutex;

    public:
        FPSTimer();
        void Update();
        void SetUpdateIntervalMs(uint32 intervalMs);
        uint32 GetFps();
    };

} // namespace nkentseu

#endif    // __NKENTSEU_FPSTIMER_H__