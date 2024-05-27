//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_TIMER_H__
#define __NKENTSEU_TIMER_H__

#pragma once

// Include guards to prevent multiple inclusions
#ifndef __NKENTSEU_SYSTEM_H__
#include "System/System.h"
#endif

#ifndef __NKENTSEU_SYSTEM_NATURE_BASE_H__
#include "System/Nature/Base.h"
#endif

#include <chrono>

namespace nkentseu {

    /**
     * @brief A class for measuring elapsed time.
     *
     * This class provides methods to start, stop, reset, and get the elapsed time
     * in nanoseconds, milliseconds, and seconds.
     * /
    class NKENTSEU_API Timer {
    public:
        /**
         * @brief Starts the timer.
         *
         * This function sets the start time of the timer to the current system time
         * using `std::chrono::system_clock::now()` and sets the internal flag
         * `m_Running` to true to indicate that the timer is running.
         * /
        void Start();

        /**
         * @brief Stops the timer.
         *
         * This function sets the end time of the timer to the current system time
         * using `std::chrono::system_clock::now()` and sets the internal flag
         * `m_Running` to false to indicate that the timer is stopped.
         * /
        void Stop();

        /**
         * @brief Resets the timer and returns the elapsed time in seconds.
         *
         * This function checks if the timer is running. If not, it returns 0.0.
         * Otherwise, it calculates the elapsed time using the `Elapsed()` function,
         * resets the start time to the current system time, and returns the elapsed time.
         *
         * @return The elapsed time in seconds since the timer was last started.
         * /
        float64 Reset();

        /**
         * @brief Calculates the elapsed time in nanoseconds.
         *
         * This function determines the end time based on whether the timer is currently
         * running or not. If running, it uses the current system time. Otherwise, it
         * uses the previously stored end time. It then calculates the difference between
         * the end time and the start time using `std::chrono::duration_cast<std::chrono::nanoseconds>`
         * and returns the number of elapsed nanoseconds.
         *
         * @return The elapsed time in nanoseconds since the timer was last started.
         * /
        float64 ElapsedNanoseconds() const;

        /**
         * @brief Calculates the elapsed time in milliseconds.
         *
         * This function calls `ElapsedNanoseconds()` to get the elapsed time in nanoseconds
         * and then converts it to milliseconds by multiplying by 0.001 * 0.001.
         *
         * @return The elapsed time in milliseconds since the timer was last started.
         * /
        float64 ElapsedMilliseconds() const;

        /**
         * @brief Calculates the elapsed time in seconds.
         *
         * This function calls `ElapsedMilliseconds()` to get the elapsed time in milliseconds
         * and then converts it to seconds by multiplying by 0.001.
         *
         * @return The elapsed time in seconds since the timer was last started.
         * /
        float64 Elapsed() const;

        /**
         * @brief Indique si le chronomètre est en cours d'exécution.
         *
         * @return `true` si le chronomètre est en cours d'exécution, `false` sinon.
         * /
        bool IsRunning() const;
    private:
        /**
         * @brief The start time of the timer.
         * /
        //std::chrono::time_point<std::chrono::system_clock> m_StartTime;
        std::chrono::steady_clock::time_point m_StartTime;

        /**
         * @brief The end time of the timer.
         * /
        //std::chrono::time_point<std::chrono::system_clock> m_EndTime;
        std::chrono::steady_clock::time_point m_EndTime;

        /**
         * @brief Flag indicating whether the timer is running.
         * /
        bool m_Running = false;
    };*/

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
