//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 8:05:33 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "LoggerPch/ntspch.h"
#include "LoggerInfos.h"

namespace nkentseu {

    LoggerInfos::LoggerInfos() : m_Name("Log"), m_Severity(Severity::Default), m_File(""), m_Function(), m_Line(0), m_Header(""), m_Message("") {}

    LoggerInfos::LoggerInfos(Severity::Code severity, const std::string& name, const std::string& file,
        const std::string& function, uint32 line, const std::string& header,
        const std::string& message) :
        m_Name(name), m_Severity(severity), m_File(file), m_Function(function), m_Line(line),
        m_Header(header), m_Message(message) {}

    LoggerInfos::LoggerInfos(const std::string& file, const std::string& function, uint32 line, Severity::Code severity,
        const std::string& name, const std::string& header, const std::string& message) :
        m_Name(name), m_Severity(severity), m_File(file), m_Function(function), m_Line(line),
        m_Header(header), m_Message(message) {}

    const std::string& LoggerInfos::GetName() const { return m_Name; }
    Severity::Code LoggerInfos::GetSeverity() const { return m_Severity; }
    const std::string& LoggerInfos::GetFile() const { return m_File; }
    const std::string& LoggerInfos::GetFunction() const { return m_Function; }
    uint32 LoggerInfos::GetLine() const { return m_Line; }
    const std::string& LoggerInfos::GetHeader() const { return m_Header; }
    const std::string& LoggerInfos::GetMessages() const { return m_Message; }

    const Date& LoggerInfos::GetDate() const
    {
        // // O: insert return statement here
        return m_Date;
    }

    const Time& LoggerInfos::GetTime() const
    {
        // // O: insert return statement here
        return m_Time;
    }

    void LoggerInfos::SetName(const std::string& name) { m_Name = name; }
    void LoggerInfos::SetSeverity(Severity::Code severity) { m_Severity = severity; }
    void LoggerInfos::SetFile(const std::string& file) { m_File = file; }
    void LoggerInfos::SetFunction(const std::string& function) { m_Function = function; }
    void LoggerInfos::SetLine(uint32 line) { m_Line = line; }
    void LoggerInfos::SetHeader(const std::string& header) { m_Header = header; }
    void LoggerInfos::SetMessages(const std::string& message) { m_Message = message; }

    void LoggerInfos::SetDate(const Date& date)
    {
        m_Date = date;
    }

    void LoggerInfos::SetTime(const Time& time)
    {
        m_Time = time;
    }

} // namespace nkentseu