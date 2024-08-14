//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 6:32:31 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEUIME_H__
#define __NKENTSEUIME_H__

#pragma once

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"

namespace nkentseu {

    #define NANOSECONDS_PER_MILLISECONDS 1000000
    #define MILLISECONDS_PER_SECOND 1000
    #define SECONDS_PER_MINUTE 60
    #define MINUTES_PER_HOUR 60
    #define HOURS_PER_DAY 24

    class NKENTSEU_API Time {
    public:
        // Constructors
        Time() noexcept;
        Time(int32 hour, int32 minute, int32 second, int32 milliseconde = 0, int32 nanoseconds = 0) noexcept;
        Time(long long total_nanosecondes) noexcept;
        Time(const Time& other) noexcept;
        Time& operator=(const Time& other) noexcept;

        // Conversion en float (retourne le nombre de secondes en virgule flottante)
        operator float() const noexcept {
            // Calcul du nombre total de secondes en tenant compte des heures, minutes, secondes, millisecondes et nanosecondes
            double total_seconds = static_cast<double>(m_Hour) * (MINUTES_PER_HOUR * SECONDS_PER_MINUTE)
                + static_cast<double>(m_Minute) *  SECONDS_PER_MINUTE
                + static_cast<double>(m_Second)
                + static_cast<double>(m_Milliseconds) / MILLISECONDS_PER_SECOND
                + static_cast<double>(m_Nanoseconds) / (MILLISECONDS_PER_SECOND * NANOSECONDS_PER_MILLISECONDS);
            // Conversion en float et retour du nombre de secondes
            return static_cast<float>(total_seconds);
        }

        // Conversion en double (similaire à la conversion en float)
        operator double() const noexcept {
            // Calcul du nombre total de secondes (similaire à la conversion en float)
            double total_seconds = static_cast<double>(m_Hour) * (MINUTES_PER_HOUR * SECONDS_PER_MINUTE)
                + static_cast<double>(m_Minute) *  SECONDS_PER_MINUTE
                + static_cast<double>(m_Second)
                + static_cast<double>(m_Milliseconds) / MILLISECONDS_PER_SECOND
                + static_cast<double>(m_Nanoseconds) / (MILLISECONDS_PER_SECOND * NANOSECONDS_PER_MILLISECONDS);
            // Conversion en double et retour du nombre de secondes
            return total_seconds;
        }

        // Conversion en long long (retourne le nombre total de nanosecondes)
        operator long long() const noexcept {
            // Somme des nanosecondes de chaque composant du temps
            return (m_Nanoseconds + NANOSECONDS_PER_MILLISECONDS * (m_Milliseconds
                + MILLISECONDS_PER_SECOND * (m_Second
                + SECONDS_PER_MINUTE * (m_Minute
                + m_Hour * MINUTES_PER_HOUR))));
        }

        Time operator+(const Time& other) const noexcept;
        Time operator-(const Time& other) const noexcept;

        // Accesseurs
        int32 GetHour() const noexcept { return m_Hour; }
        int32 GetMinute() const noexcept { return m_Minute; }
        int32 GetSecond() const noexcept { return m_Second; }
        int32 GetNanoseconds() const noexcept { return m_Nanoseconds; }
        int32 GetMilliseconds() const noexcept { return m_Milliseconds; }

        // Mutateurs
        void SetHour(int32 hour) noexcept { m_Hour = NormalizeHour(hour); }
        void SetMinute(int32 minute) noexcept { m_Minute = NormalizeMinute(minute); }
        void SetSecond(int32 second) noexcept { m_Second = NormalizeSecond(second); }
        void SetNanoseconds(int32 nanoseconds) noexcept { m_Nanoseconds = NormalizeNanoseconds(nanoseconds); }
        void SetMilliseconds(int32 milliseconds) noexcept { m_Milliseconds = NormalizeMilliseconds(milliseconds); }

        // Static method to get current time
        static Time GetCurrent();

        std::string ToString() const;

        // Méthode ToString with optional milliseconds and nanoseconds format specifiers
        friend std::string ToString(const Time& t) {
            return t.ToString();
        }

    private:
        int32 m_Hour = 0;
        int32 m_Minute = 0;
        int32 m_Second = 0;
        int32 m_Milliseconds = 0;
        int32 m_Nanoseconds = 0;

        int32 NormalizeHour(int32 hour);

        int32 NormalizeMinute(int32 minute);

        int32 NormalizeSecond(int32 second);

        int32 NormalizeMilliseconds(int32 millis);

        int32 NormalizeNanoseconds(int32 nano);
    };
} // namespace nkentseu

#endif// __NKENTSEUIME_H__