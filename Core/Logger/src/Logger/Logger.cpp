//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 7:34:08 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "LoggerPch/ntspch.h"
#include "Logger.h"
#include "System/Epoch/Date.h"
#include "System/Epoch/Time.h"

#if  defined(NKENTSEU_PLATFORM_ANDROID)
#include <android/log.h>
#elif defined(NKENTSEU_PLATFORM_IOS)
#include <stdio.h>
#include <stdlib.h>
#endif


namespace nkentseu {
    namespace {  
        // Anonymous namespace for helper functions

        /**
        * @brief Gets a color code based on the severity level of a log message
        * @param severity The severity level of the log message
        * @return A string containing the ANSI escape sequence for the color code
        */
        std::string GetColorCode(Severity::Code severity) {
            switch (severity) {
            case Severity::Error_ev:   return "\033[1;31m"; // Red for errors
            case Severity::Warning_ev: return "\033[1;33m"; // Yellow for warnings
            case Severity::Debug_ev:   return "\033[1;34m"; // Blue for debug messages
            case Severity::Trace_ev:   return "\033[1;36m"; // Cyan for trace messages
            case Severity::Critical_ev: return "\033[1;35m"; // Purple for critical messages
            case Severity::Info_ev:    return "\033[1;32m"; // Green for informational messages
            case Severity::Assert_ev:
            case Severity::Fatal_ev:   return "\033[1;41m"; // Dark red for asserts and fatal errors
            default: return "";
            }
        }
    }

    // Implementations of Logger member functions

    Logger::Logger() : m_LoggerInfos()
    {
    }

    Logger::Logger(const std::string& name) : m_LoggerInfos()
    {
        m_LoggerInfos.SetName(name);
    }

    const LoggerInfos& Logger::GetLoggerInfos() const
    {
        return m_LoggerInfos;
    }

    const std::string& Logger::GetName() const
    {
        return m_LoggerInfos.GetName();
    }

    void Logger::SetName(const std::string& name)
    {
        m_LoggerInfos.SetName(name);
    }

    Logger& Logger::Details(const char* file, unsigned int ligne, const char* function) {
        m_LoggerInfos.SetFile(file);
        m_LoggerInfos.SetLine(ligne);
        m_LoggerInfos.SetFunction(function);

        m_LoggerInfos.SetDate(Date::GetCurrent());
        m_LoggerInfos.SetTime(Time::GetCurrent());
        return *this;
    }

    Logger& Logger::Details(const char* file, unsigned int ligne, const char* function, const Date& date, const Time& time)
    {
        m_LoggerInfos.SetFile(file);
        m_LoggerInfos.SetLine(ligne);
        m_LoggerInfos.SetFunction(function);

        m_LoggerInfos.SetDate(date);
        m_LoggerInfos.SetTime(time);
        return *this;
    }

    void Logger::AddObserver(LogObserver observer) {
        // Check for duplicates before adding (avoid unnecessary comparisons later)
        auto it = std::find_if(m_Observers.begin(), m_Observers.end(),
            [&observer](const auto& observer_in) { return CompareFunctions(observer_in, observer); });

        if (it == m_Observers.end()) {
            m_Observers.push_back(observer);
        }
    }

    void Logger::RemoveObserver(LogObserver observer) {
        auto it = std::find_if(m_Observers.begin(), m_Observers.end(),
            [&observer](const auto& observer_in) { return CompareFunctions(observer_in, observer); });

        if (it != m_Observers.end()) {
            m_Observers.erase(it);  // Efficiently remove the matching observer
        }
    }

    void Logger::EnableDefaultWriting()
    {
        m_IsDefaultWritingIsEnable = true;
    }

    void Logger::DisableDefaultWriting()
    {
        m_IsDefaultWritingIsEnable = false;
    }

    void Logger::PreWrite(Severity::Code severity, const std::string& message)
    {
        m_LoggerInfos.SetSeverity(severity);

        m_LoggerInfos.SetMessages(message);

        m_LoggerInfos.SetHeader(
            Formatter::Instance().
            Format("[{0} {1}] | [{2} >> {3} >> {4} ] |",
                m_LoggerInfos.GetDate(), m_LoggerInfos.GetTime(),
                m_LoggerInfos.GetFile(), m_LoggerInfos.GetLine(), m_LoggerInfos.GetFunction()));
        // Notify all registered observers using a range-based for loop
        for (const auto& observer : m_Observers) {
            observer(m_LoggerInfos);
        }
        if (m_IsDefaultWritingIsEnable) {
            Write(m_LoggerInfos);
        }
    }

    bool Logger::CompareFunctions(const std::function<void(const nkentseu::LoggerInfos&)>& f1, const std::function<void(const nkentseu::LoggerInfos&)>& f2)
    {
        return f1.target<void(*)(const nkentseu::LoggerInfos&)>() == f2.target<void(*)(const nkentseu::LoggerInfos&)>();
    }

    void Logger::Write(const LoggerInfos& loggerInfos)
    {
        std::string logType = Severity::ToString(loggerInfos.GetSeverity());
        std::string header = loggerInfos.GetHeader();
        std::string message = loggerInfos.GetMessages();

        std::string beginColor = GetColorCode(loggerInfos.GetSeverity());
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