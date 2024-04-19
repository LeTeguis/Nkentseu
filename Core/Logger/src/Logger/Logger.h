//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 7:34:08 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_LOGGER_H__
#define __NKENTSEU_LOGGER_H__

#pragma once

#include "System/System.h" // Include header for System class (assuming this is needed)
#include <Logger/LoggerInfos.h>  // Include header for LoggerInfos class
#include "Formatter.h"           // Include header for Formatter class
#include <functional>             // Include header for std::function

namespace nkentseu {

    using LogObserver = std::function<void(const LoggerInfos&)>;
    #define LOG_BIND_METHOD(obj, method) std::bind(&decltype(obj)::method, &obj)
    /**
     * @brief Interface for a logging system
     */
    class NKENTSEU_API Logger {
    public:
        /**
         * @brief Default constructor for Logger
         */
        Logger();

        /**
         * @brief Constructor for Logger with a specific name
         * @param name The name of the logger
         */
        Logger(const std::string& name);

        /**
         * @brief Virtual destructor for Logger
         */
        virtual ~Logger() = default;

        /**
         * @brief Getter for logger information
         * @return The logger information object
         */
        const LoggerInfos& GetLoggerInfos() const;

        // Getters and Setters for logger information (name)
        const std::string& GetName() const;
        void SetName(const std::string& name);

        /**
         * @brief Adds details to the log message (source file, line number, function name)
         * @param file The source file where the log is generated
         * @param ligne The line number in the source file
         * @param function The function name where the log is generated
         * @return A reference to the Logger object for method chaining
         */
        Logger& Details(const char* file, unsigned int ligne, const char* function);
        Logger& Details(const char* file, unsigned int ligne, const char* function, const Date& date, const Time& time);

        // **Log methods with different severity levels**

        /**
         * @brief Logs a message with a specific format and arguments based on severity
         * @tparam Args The types of arguments for the formatted message
         * @param severity The severity level of the log message
         * @param format The format string for the message (optional)
         * @param args The arguments to be inserted into the format string (optional)
         */
        template<typename... Args>
        void Log(Severity::Code severity, const char* format = "", Args&&... args) {
            PreWrite(severity, Formatter::Instance().Format(format, args...));
        }

        // Specific log methods for different severity levels (examples)
        template<typename... Args>
        void Info(const char* format = "", Args&&... args) {
            Log(Severity::Info, format, args...);
        }

        template<typename... Args>
        void Asserts(const char* format = "", Args&&... args) {
            Log(Severity::Assert, format, args...);
        }

        template<typename... Args>
        void Error(const char* format = "", Args&&... args) {
            Log(Severity::Error, format, args...);
        }

        template<typename... Args>
        void Warning(const char* format = "", Args&&... args) {
            Log(Severity::Warning, format, args...);
        }

        template<typename... Args>
        void Debug(const char* format = "", Args&&... args) {
            Log(Severity::Debug, format, args...);
        }

        template<typename... Args>
        void Trace(const char* format = "", Args&&... args) {
            Log(Severity::Trace, format, args...);
        }

        template<typename... Args>
        void Critical(const char* format = "", Args&&... args) {
            Log(Severity::Critical, format, args...);
        }

        template<typename... Args>
        void Fatal(const char* format = "", Args&&... args) {
            Log(Severity::Fatal, format, args...);
        }
        /**
         * @brief Adds an observer (callback) to be notified when a log message is written
         * @param observer The callback function to be called
         */
        void AddObserver(LogObserver observer);

        /**
         * @brief Removes an observer (callback)
         * @param observer The callback function to be removed
         */
        void RemoveObserver(LogObserver observer);

        /**
         * @brief Enables default writing behavior (presumably writing to console or file)
         */
        void EnableDefaultWriting();

        /**
         * @brief Disables default writing behavior
         */
        void DisableDefaultWriting();

    private:
        LoggerInfos m_LoggerInfos;  // Stores logger information
        std::vector<LogObserver> m_Observers;  // List of registered observers
        bool m_IsDefaultWritingIsEnable = true;  // Flag for default writing behavior

        /**
         * @brief Prepares the log message before writing it (can be used for filtering, etc.)
         * @param severity The severity level of the message
         * @param message The formatted log message
         */
        void PreWrite(Severity::Code severity, const std::string& message);

        /**
         * @brief Compares two observer functions for equality (likely used for removal)
         * @param f1 The first observer function
         * @param f2 The second observer function
         * @return True if the functions are equal, False otherwise
         */
        static bool CompareFunctions(const std::function<void(const nkentseu::LoggerInfos&)>& f1, const std::function<void(const nkentseu::LoggerInfos&)>& f2);

    protected:
        /**
         * @brief Abstract method for writing the log message to a specific destination (console, file, etc.)
         * This method is implemented by derived classes to handle different logging backends.
         * @param loggerInfos The logger information object
         */
        virtual void Write(const LoggerInfos& loggerInfos);
    };
} // namespace nkentseu

#endif // __NKENTSEU_LOGGER_H__