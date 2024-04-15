//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 6:32:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_DATE_H__
#define __NKENTSEU_DATE_H__

#pragma once

#include "System/System.h"
#include <System/Nature/Base.h>

namespace nkentseu {
    class NKENTSEU_API Date {
    public:
        // Constructeurs
        Date() noexcept;
        Date(int32 year, int32 month, int32 day) noexcept;

        // Accesseurs
        int32 GetYear() const noexcept { return year; }
        int32 GetMonth() const noexcept { return month; }
        int32 GetDay() const noexcept { return day; }

        // Mutateurs
        void SetYear(int32 year) noexcept;
        void SetMonth(int32 month) noexcept;
        void SetDay(int32 day) noexcept;

        // Static method to get current date
        static Date GetCurrent();

        std::string ToString() const;

        // Méthode ToString
        friend std::string ToString(const Date& date) {
            return date.ToString();
        }

        std::string GetMounName();

        // Méthode pour obtenir le nombre de jours dans un mois donné
        static int32 DaysInMonth(int32 year, int32 month);

        // Méthode pour vérifier si une année est bissextile
        static bool IsLeapYear(int32 year);
    private:
        int32 year;
        int32 month;
        int32 day;
    };

} // namespace nkentseu

#endif    // __NKENTSEU_DATE_H__