//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 4:33:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "Timer.h"

namespace nkentseu {

    /**
     * @brief Starts the timer.
     *
     * This function sets the start time of the timer to the current system time
     * using `std::chrono::system_clock::now()` and sets the internal flag
     * `m_Running` to true to indicate that the timer is running.
     * /
    void Timer::Start() {
        // m_StartTime = std::chrono::system_clock::now();
        m_StartTime = std::chrono::high_resolution_clock::now();
        m_Running = true;
    }

    /**
     * @brief Stops the timer.
     *
     * This function sets the end time of the timer to the current system time
     * using `std::chrono::system_clock::now()` and sets the internal flag
     * `m_Running` to false to indicate that the timer is stopped.
     * /
    void Timer::Stop() {
        //sm_EndTime = std::chrono::system_clock::now();
        m_EndTime = std::chrono::high_resolution_clock::now();
        m_Running = false;
    }

    /**
     * @brief Resets the timer and returns the elapsed time in seconds.
     *
     * This function checks if the timer is running. If not, it returns 0.0.
     * Otherwise, it calculates the elapsed time using the `Elapsed()` function,
     * resets the start time to the current system time, and returns the elapsed time.
     *
     * @return The elapsed time in seconds since the timer was last started.
     * /
    float64 Timer::Reset() {
        if (!m_Running) return 0.0f;

        float64 elapsed = Elapsed();

        //m_StartTime = std::chrono::system_clock::now();
        m_StartTime = std::chrono::high_resolution_clock::now();

        return elapsed;
    }

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
    float64 Timer::ElapsedNanoseconds() const {
        // std::chrono::time_point<std::chrono::system_clock> endTime;
        std::chrono::steady_clock::time_point endTime;

        if (m_Running) {
            //endTime = std::chrono::system_clock::now();
            endTime = std::chrono::high_resolution_clock::now();
        }
        else {
            endTime = m_EndTime;
        }

        return std::chrono::duration<float64, std::nano>(endTime - m_StartTime).count();
    }

    /**
     * @brief Calculates the elapsed time in milliseconds.
     *
     * This function calls `ElapsedNanoseconds()` to get the elapsed time in nanoseconds
     * and then converts it to milliseconds by multiplying by 0.001 * 0.001.
     *
     * @return The elapsed time in milliseconds since the timer was last started.
     * /
    float64 Timer::ElapsedMilliseconds() const {
        float64 nano = ElapsedNanoseconds();
        return (nano * 0.001 * 0.001);
    }

    /**
     * @brief Calculates the elapsed time in seconds.
     *
     * This function calls `ElapsedMilliseconds()` to get the elapsed time in milliseconds
     * and then converts it to seconds by multiplying by 0.001.
     *
     * @return The elapsed time in seconds since the timer was last started.
     * /
    float64 Timer::Elapsed() const {
        return (ElapsedMilliseconds() * 0.001);
    }

    /**
    * @brief Indique si le chronomètre est en cours d'exécution.
    *
    * @return `true` si le chronomètre est en cours d'exécution, `false` sinon.
    * /
    bool Timer::IsRunning() const
    {
        return m_Running;
    }*/

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
