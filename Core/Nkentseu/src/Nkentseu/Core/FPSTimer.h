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

        float64 m_AccumulateTime = 0.0;
        uint32 m_NumFrame = 0;
        float64 m_CurrentFps = 0.0;
        float64 m_TimeFps = 0.0;

    public:
        FPSTimer();
        ~FPSTimer();

        void Update(float64 delta);

        float64 GetFrameTime();
        uint32 GetCurrentFrame();
        uint32 GetFrameRate();
        uint32 GetFps();
    };

} // namespace nkentseu

#endif    // __NKENTSEU_FPSTIMER_H__