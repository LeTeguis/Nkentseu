//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 8:55:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_ASSERTION_H__
#define __NKENTSEU_ASSERTION_H__

#pragma once

#include "System/System.h"
#include <Logger/LoggerInfos.h>
#include "Formatter.h"
#include <functional>

namespace nkentseu {
    using AssertObserver = std::function<void(const LoggerInfos&)>;
    #define ASSERT_BIND_METHOD(obj, method) std::bind(&decltype(obj)::method, &obj)

    class NKENTSEU_API Assertion
    {
    public:
        Assertion();

        Assertion(std::string name);

        virtual ~Assertion() = default;

        const LoggerInfos& GetLoggerInfos() const;

        // Getter (accesseurs) pour les informations de log
        const std::string& GetName() const;

        // Setter (modificateurs) pour les informations de log
        void SetName(const std::string& name);

        Assertion& Details(const char* file, unsigned int ligne, const char* function);
        Assertion& Details(const char* file, unsigned int ligne, const char* function, const Date& date, const Time& time);

        template<typename... Args>
        void Assert(bool compare, const char* format = "", Args&&... args) {
            #ifdef NKENTSEU_DEBUG
                if (compare) {
                    std::string format_new = Formatter::Instance().Format(format, args...);
                    PreWrite(Formatter::Instance().Format("Assertion Echouer: {0}", format_new));
                    NKENTSEU_ASSERT_BREAK
                }
            #endif
        }

        template<typename... Args>
        Assertion& AFalse(bool compare, const char* format = "", Args&&... args) {
            Assert(compare == false, format, args...);
            return *this;
        }

        template<typename... Args>
        Assertion& ATrue(bool compare, const char* format = "", Args&&... args) {
            Assert(compare == true, format, args...);
            return *this;
        }

        template<typename T, typename... Args>
        Assertion& AEqual(T first, T second, const char* format = "", Args&&... args) {
            Assert(first == second, format, args...);
            return *this;
        }

        template<typename... Args>
        Assertion& ANull(void* ptr, const char* format = "", Args&&... args) {
            Assert(ptr == nullptr, format, args...);
            return *this;
        }

        template<typename T, typename... Args>
        Assertion& ADifferent(T first, T second, const char* format = "", Args&&... args) {
            Assert(first != second, format, args...);
            return *this;
        }

        void AddObserver(AssertObserver observer);

        void RemoveObserver(AssertObserver observer);
    private:
        LoggerInfos m_LoggerInfos;
        std::vector<AssertObserver> m_Observers;

        void PreWrite(const std::string& message);

        static bool CompareFunctions(const std::function<void(const nkentseu::LoggerInfos&)>& f1,
            const std::function<void(const nkentseu::LoggerInfos&)>& f2);
    protected:
        /**
         * @brief abstract write method for diffrent logging like consol, file, etc
         *
         * @param loggerType
         * @param entete
         * @param message
         */
        virtual void Write(const LoggerInfos& loggerInfos);

    private:
        template<typename... Args>
        void AssertionCall(bool compare, Args&&... args) {
            #ifdef NKENTSEU_DEBUG
            if (compare) {
                this->Assert("Assertion Echouer: {0}", args...);
                NKENTSEU_ASSERT_BREAK
            }
            #endif
        }
    };
} // namespace nkentseu

#endif    // __NKENTSEU_ASSERTION_H__