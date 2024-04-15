//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 6:32:21 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NSystemPch/ntspch.h"
#include "Date.h"

#include <ctime>
#include <sstream>

namespace nkentseu {

    Date::Date() noexcept : year(0), month(0), day(0) {}

    Date::Date(int32 year, int32 month, int32 day) noexcept : year(0), month(0), day(0) {
        SetYear(year);
        SetMonth(month);
        SetDay(day);
    }

    std::string Date::ToString() const {
        std::stringstream ss;
        ss << year << "-" << month << "-" << day;
        return ss.str();
    }

    void Date::SetYear(int32 year) noexcept { this->year = year; }

    void Date::SetMonth(int32 month) noexcept {
        if (month < 1 || month > 12) {
            SetYear(year + (month / 12));
        }
        this->month = month % 12;
    }

    void Date::SetDay(int32 day) noexcept {
        if (day < 1 || day > DaysInMonth(year, month)) {
            SetMonth(month + (day / DaysInMonth(year, month)));
        }
        this->day = day % DaysInMonth(year, month);
    }

    Date Date::GetCurrent() {
        // Use std::time to get current time_t
        time_t now = std::time(nullptr);

        // Convert time_t to tm structure for date extraction
        tm* localTime = std::localtime(&now);

        // Extract year, month (0-indexed), and day from tm structure
        return Date(localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday);
    }

    std::string Date::GetMounName()
    {
        if (month == 1) return "Jan"; // Janv
        if (month == 2) return "Feb"; // Fevr
        if (month == 3) return "March"; // Mars
        if (month == 4) return "April"; // Avr
        if (month == 5) return "May"; // Mai
        if (month == 6) return "June"; // Juin
        if (month == 7) return "July"; // Juill
        if (month == 8) return "Aug"; // Aout
        if (month == 9) return "Sept"; // Sept
        if (month == 10) return "Oct"; // Oct
        if (month == 11) return "Nov"; // Nov
        return "Dec"; // Dec
    }

    int32 Date::DaysInMonth(int32 year, int32 month) {
        switch (month) {
        case 2:
            return IsLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
        }
    }

    bool Date::IsLeapYear(int32 year)
    {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

}    // namespace nkentseu