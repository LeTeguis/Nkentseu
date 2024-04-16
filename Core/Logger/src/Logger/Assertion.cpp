//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 8:55:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "LoggerPch/ntspch.h"
#include "Assertion.h"
#include "Formatter.h"
#include "System/Epoch/Date.h"
#include "System/Epoch/Time.h"

#if  defined(NKENTSEU_PLATFORM_ANDROID)
#include <android/log.h>
#elif defined(NKENTSEU_PLATFORM_IOS)
#include <stdio.h>
#include <stdlib.h>
#endif

namespace nkentseu {
    Assertion::Assertion() : m_LoggerInfos()
    {
    }

    Assertion::Assertion(std::string name) : m_LoggerInfos()
    {
        m_LoggerInfos.SetName(name);
    }

    const LoggerInfos& Assertion::GetLoggerInfos() const
    {
        return m_LoggerInfos;
    }

    const std::string& Assertion::GetName() const
    {
        return m_LoggerInfos.GetName();
    }

    void Assertion::SetName(const std::string& name)
    {
        m_LoggerInfos.SetName(name);
    }

    Assertion& Assertion::Details(const char* file, unsigned int ligne, const char* function) {
        m_LoggerInfos.SetFile(file);
        m_LoggerInfos.SetLine(ligne);
        m_LoggerInfos.SetFunction(function);

        m_LoggerInfos.SetDate(Date::GetCurrent());
        m_LoggerInfos.SetTime(Time::GetCurrent());
        return *this;
    }

    Assertion& Assertion::Details(const char* file, unsigned int ligne, const char* function, const Date& date, const Time& time)
    {
        m_LoggerInfos.SetFile(file);
        m_LoggerInfos.SetLine(ligne);
        m_LoggerInfos.SetFunction(function);

        m_LoggerInfos.SetDate(date);
        m_LoggerInfos.SetTime(time);
        return *this;
    }



    void Assertion::AddObserver(AssertObserver observer) {
        // Check for duplicates before adding (avoid unnecessary comparisons later)
        auto it = std::find_if(m_Observers.begin(), m_Observers.end(),
            [&observer](const auto& observer_in) { return CompareFunctions(observer_in, observer); });

        if (it == m_Observers.end()) {
            m_Observers.push_back(observer);
        }
    }

    void Assertion::RemoveObserver(AssertObserver observer) {
        auto it = std::find_if(m_Observers.begin(), m_Observers.end(),
            [&observer](const auto& observer_in) { return CompareFunctions(observer_in, observer); });

        if (it != m_Observers.end()) {
            m_Observers.erase(it);  // Efficiently remove the matching observer
        }
    }

    void Assertion::PreWrite(const std::string& message)
    {
        m_LoggerInfos.SetSeverity(Severity::Assert_ev);

        m_LoggerInfos.SetMessages(message);

        m_LoggerInfos.SetHeader(
            Formatter::Instance().
            Format("[{0} {1}] | [{2} >> {3} >> {4} ] |",
                m_LoggerInfos.GetDate(), m_LoggerInfos.GetTime(),
                m_LoggerInfos.GetFile(), m_LoggerInfos.GetLine(), m_LoggerInfos.GetFunction(),
                m_LoggerInfos.GetName()));
        Write(m_LoggerInfos);
    }

    bool Assertion::CompareFunctions(const std::function<void(const nkentseu::LoggerInfos&)>& f1, const std::function<void(const nkentseu::LoggerInfos&)>& f2)
    {
        return f1.target<void(*)(const nkentseu::LoggerInfos&)>() == f2.target<void(*)(const nkentseu::LoggerInfos&)>();
    }

    void Assertion::Write(const LoggerInfos& loggerInfos)
    {
        std::string logType = Severity::ToString(loggerInfos.GetSeverity());
        std::string header = loggerInfos.GetHeader();
        std::string message = loggerInfos.GetMessages();

        std::string beginColor = "\033[1;41m"; // "\033[1;41m"
        std::string endColor = "\033[0m";

        #ifdef NKENTSEU_PLATFORM_ANDROID
            __android_log_print(ANDROID_LOG_INFO, "", "(%s%s%s) : %s [%s%s%s] >> %s", beginColor.c_str(), loggerInfos.GetName().c_str(), endColor.c_str(), header.c_str(), beginColor.c_str(), logType.c_str(), endColor.c_str(), message.c_str());
        #elif defined(NKENTSEU_PLATFORM_IOS)
            printf("(%s%s%s) : %s [%s%s%s] >> %s", beginColor.c_str(), loggerInfos.GetName().c_str(), endColor.c_str(), header.c_str(), beginColor.c_str(), logType.c_str(), endColor.c_str(), message.c_str());
        #else
            // Code par défaut pour les autres plates-formes (Linux, macOS, etc.)
            std::cout << " (" << beginColor << loggerInfos.GetName() << endColor << ") : " << header << " [" << beginColor << logType << endColor << "] >> " << message << std::endl;
        #endif
    }

}    // namespace nkentseu