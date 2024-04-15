//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 6:32:31 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "Time.h"

#include <chrono>
#include <ctime>
#include <sstream>
#include <math.h>

namespace nkentseu {

    Time::Time() noexcept : m_Hour(0), m_Minute(0), m_Second(0), m_Nanoseconds(0), m_Milliseconds(0) {}

    Time::Time(int32 hour, int32 minute, int32 second, int32 milliseconde, int32 nanoseconds) noexcept {
        // Ensure nanoseconds are within the valid range (0-999999999)
        m_Hour = NormalizeHour(hour);
        m_Minute = NormalizeMinute(minute);
        m_Second = NormalizeSecond(second);
        m_Milliseconds = NormalizeMilliseconds(milliseconde);
        m_Nanoseconds = NormalizeNanoseconds(nanoseconds);
    }

    Time::Time(long long total_nanosecondes) noexcept {
        // Conversion des nanosecondes en secondes en évitant le débordement d'entier
        double total_seconds = static_cast<double>(total_nanosecondes) / (NANOSECONDS_PER_MILLISECONDS * MILLISECONDS_PER_SECOND);
        long long secondes = static_cast<long long>(total_seconds); // Conversion en secondes (partie entière)
        double nanosecondes_doubles = total_seconds - secondes; // Nanosecondes restantes (partie décimale)

        // Conversion des nanosecondes restantes en nanosecondes et millisecondes
        long long nanosecondes_local = static_cast<long long>(nanosecondes_doubles * (NANOSECONDS_PER_MILLISECONDS * MILLISECONDS_PER_SECOND));
        int32 millisecondes = static_cast<int32>(nanosecondes_doubles * MILLISECONDS_PER_SECOND);

        // Conversion des secondes en minutes, heures, etc. et extraction des parties entières
        int32 heures = secondes / SECONDS_PER_MINUTE;
        secondes %= SECONDS_PER_MINUTE;
        int32 minutes = secondes / MINUTES_PER_HOUR;
        secondes %= MINUTES_PER_HOUR;

        // Affectation des valeurs aux membres de la classe
        m_Hour = static_cast<int32>(heures);
        m_Minute = static_cast<int32>(minutes);
        m_Second = static_cast<int32>(secondes);
        m_Milliseconds = millisecondes;
        m_Nanoseconds = nanosecondes_local;
    }

    // Constructeur de copie
    Time::Time(const Time& other) noexcept {
        m_Hour = other.m_Hour;
        m_Minute = other.m_Minute;
        m_Second = other.m_Second;
        m_Milliseconds = other.m_Milliseconds;
        m_Nanoseconds = other.m_Nanoseconds;
    }

    // Opérateur d'affectation
    Time& Time::operator=(const Time& other) noexcept {
        if (this != &other) {
            m_Hour = other.m_Hour;
            m_Minute = other.m_Minute;
            m_Second = other.m_Second;
            m_Milliseconds = other.m_Milliseconds;
            m_Nanoseconds = other.m_Nanoseconds;
        }
        return *this;
    }

    // Opérateur d'addition
    Time Time::operator+(const Time& other) const noexcept {
        // Déclaration de variables temporaires
        int32 total_nanoseconds = m_Nanoseconds + other.m_Nanoseconds;
        int32 overflow_seconds = 0;

        int32 nanosecond_per_seconde = (NANOSECONDS_PER_MILLISECONDS * MILLISECONDS_PER_SECOND);

        // Gestion du report des nanosecondes
        if (total_nanoseconds >= nanosecond_per_seconde) {
            total_nanoseconds -= nanosecond_per_seconde;
            overflow_seconds++;
        }

        // Addition des millisecondes et gestion du report
        int32 total_milliseconds = m_Milliseconds + other.m_Milliseconds + overflow_seconds;
        overflow_seconds = total_milliseconds / MILLISECONDS_PER_SECOND;
        total_milliseconds %= MILLISECONDS_PER_SECOND;

        // Addition des secondes et gestion du report
        int32 secondes = m_Second + other.m_Second + overflow_seconds;
        int32 overflow_minutes = secondes / SECONDS_PER_MINUTE;
        secondes %= SECONDS_PER_MINUTE;

        // Addition des minutes et gestion du report
        int32 minutes = m_Minute + other.m_Minute + overflow_minutes;
        int32 heures = minutes / MINUTES_PER_HOUR;
        minutes %= MINUTES_PER_HOUR;

        // Heures
        heures += m_Hour + other.m_Hour;

        // Création d'une nouvelle instance Time avec les valeurs finales
        return Time(heures, minutes, secondes, total_milliseconds, total_nanoseconds);
    }

    Time Time::operator-(const Time& other) const noexcept {
        // Déclaration de variables temporaires
        int32 total_nanoseconds = m_Nanoseconds - other.m_Nanoseconds;
        int32 underflow_seconds = 0;

        int32 nanosecond_per_seconde = (NANOSECONDS_PER_MILLISECONDS * MILLISECONDS_PER_SECOND);

        // Gestion du report négatif des nanosecondes
        if (total_nanoseconds < 0) {
            total_nanoseconds += nanosecond_per_seconde;
            underflow_seconds--;
        }

        // Soustraction des millisecondes et gestion du report négatif
        int32 total_milliseconds = m_Milliseconds - other.m_Milliseconds - underflow_seconds;
        underflow_seconds = total_milliseconds < 0 ? 1 : 0;
        total_milliseconds += (total_milliseconds < 0 ? MILLISECONDS_PER_SECOND : 0);

        // Soustraction des secondes et gestion du report négatif
        int32 secondes = m_Second - other.m_Second - underflow_seconds;
        int32 underflow_minutes = secondes < 0 ? 1 : 0;
        secondes += (secondes < 0 ? SECONDS_PER_MINUTE : 0);

        // Soustraction des minutes et gestion du report négatif
        int32 minutes = m_Minute - other.m_Minute - underflow_minutes;
        int32 heures = minutes < 0 ? MINUTES_PER_HOUR : 0;
        minutes += heures;

        // Heures
        heures += m_Hour - other.m_Hour;

        // Création d'une nouvelle instance Time avec les valeurs finales
        return Time(heures, minutes, secondes, total_milliseconds, total_nanoseconds);
    }

    // Fonction pour récupérer l'heure courante
    Time Time::GetCurrent() {
        // Récupère l'heure courante du système à l'aide de l'horloge système
        std::chrono::time_point<std::chrono::system_clock> times_now = std::chrono::system_clock::now();

        // Calcule la durée écoulée depuis l'époque de l'horloge système en nanosecondes
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(times_now.time_since_epoch());
        auto total_nanoseconds = duration.count();

        // Extraction des composants de l'heure en utilisant la division entière et le modulo

        int32 nanoseconds_local = static_cast<int32>(total_nanoseconds % NANOSECONDS_PER_MILLISECONDS); // Conserve les nanosecondes
        total_nanoseconds /= NANOSECONDS_PER_MILLISECONDS; // Conversion en millisecondes (division par 1000)

        int32 milliseconds_local = static_cast<int32>(total_nanoseconds % MILLISECONDS_PER_SECOND); // Millisecondes (reste de la division par 1000)
        total_nanoseconds /= MILLISECONDS_PER_SECOND; // Conversion en secondes (division par 1000)

        int32 second_local = static_cast<int32>(total_nanoseconds % SECONDS_PER_MINUTE); // Secondes (reste de la division par 60)
        total_nanoseconds /= SECONDS_PER_MINUTE; // Conversion en minutes (division par 60)

        int32 minute_local = static_cast<int32>(total_nanoseconds % MINUTES_PER_HOUR); // Minutes (reste de la division par 60)
        int32 hour_local = static_cast<int32>(total_nanoseconds / MINUTES_PER_HOUR); // m_Hour

        // Retourne un objet Time initialisé avec les valeurs extraites
        return Time(hour_local, minute_local, second_local, milliseconds_local, nanoseconds_local);
    }

    std::string Time::ToString() const {
        std::stringstream ss;
        ss << m_Hour << ":" << m_Minute << ":" << m_Second << " " << m_Nanoseconds / MILLISECONDS_PER_SECOND << "." << m_Nanoseconds % MILLISECONDS_PER_SECOND;
        return ss.str();
    }

    int32 Time::NormalizeHour(int32 hour) {
        return (hour % HOURS_PER_DAY); // Wrap hour around to 0-23
    }

    int32 Time::NormalizeMinute(int32 minute) {
        int32 carryover = minute / MINUTES_PER_HOUR;
        m_Hour = NormalizeHour(m_Hour + carryover);
        return minute % MINUTES_PER_HOUR; // Keep minute within 0-59
    }

    // Fonction pour normaliser les secondes (gestion du débordement : ramener entre 0 et 59)
    int32 Time::NormalizeSecond(int32 seconde) {
        int32 retenue = seconde / SECONDS_PER_MINUTE;
        m_Minute = NormalizeMinute(m_Minute + retenue);
        return seconde % SECONDS_PER_MINUTE; // Conserve les secondes entre 0 et 59
    }

    // Fonction pour normaliser les millisecondes (gestion du débordement : ramener entre 0 et 999)
    int32 Time::NormalizeMilliseconds(int32 millisecondes) {
        int32 retenue = millisecondes / MILLISECONDS_PER_SECOND;
        m_Second = NormalizeSecond(m_Second + retenue);
        return millisecondes % MILLISECONDS_PER_SECOND; // Conserve les millisecondes entre 0 et 999
    }

    // Fonction pour normaliser les nanosecondes (gestion du débordement : ramener entre 0 et 999999999)
    int32 Time::NormalizeNanoseconds(int32 nanosecondes) {
        int32 retenue = nanosecondes / NANOSECONDS_PER_MILLISECONDS;
        m_Milliseconds = NormalizeMilliseconds(m_Milliseconds + retenue);
        return nanosecondes % NANOSECONDS_PER_MILLISECONDS; // Conserve les nanosecondes entre 0 et 999999999
    }


} // namespace nkentseu
