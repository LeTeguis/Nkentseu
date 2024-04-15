//
// File: UnitestInfos.cpp
// Created by: TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:34:01 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//
// This implementation file defines the member functions of the `UnitestInfo` class.

#include "UnitestPch/ntspch.h"
#include "UnitestInfos.h" // Assuming UnitestInfos.h contains the class definition

namespace nkentseu {

    // Default constructor (initializes all members with default values)
    UnitestInfo::UnitestInfo() : m_Condition(false) {}

    // Constructor with parameters to initialize all members
    UnitestInfo::UnitestInfo(const std::string& name, bool condition, const std::string& file,
        uint32 line, const std::string& function, const std::string& message)
        : m_Name(name), m_Condition(condition), m_File(file), m_Line(line),
        m_Function(function), m_Message(message) {}

    // Getter for `m_Name` (assuming no renaming to `m_NomTest`)
    const std::string& UnitestInfo::GetName() const {
        return m_Name;
    }

    // Setter for `m_Name` (assuming no renaming to `m_NomTest`)
    void UnitestInfo::SetName(const std::string& name) {
        m_Name = name;
    }

    // Getter for `m_Condition` (uses a more descriptive name)
    bool UnitestInfo::IsSuccessfull() const {
        return m_Condition;
    }

    // Setter for `m_Condition` (uses a more descriptive name)
    void UnitestInfo::SetSuccessfull(bool condition) {
        m_Condition = condition;
    }

    // Getter for `m_File`
    const std::string& UnitestInfo::GetFile() const {
        return m_File;
    }

    // Setter for `m_File`
    void UnitestInfo::SetFile(const std::string& file) {
        m_File = file;
    }

    // Getter for `m_Line`
    uint32 UnitestInfo::GetLine() const {
        return m_Line;
    }

    // Setter for `m_Line`
    void UnitestInfo::SetLine(uint32 line) {
        m_Line = line;
    }

    // Getter for `m_Function`
    const std::string& UnitestInfo::GetFunction() const {
        return m_Function;
    }

    // Setter for `m_Function`
    void UnitestInfo::SetFunction(const std::string& function) {
        m_Function = function;
    }

    // Getter for `m_Message`
    const std::string& UnitestInfo::GetMessage() const {
        return m_Message;
    }

    // Setter for `m_Message`
    void UnitestInfo::SetMessage(const std::string& message) {
        m_Message = message;
    }

} // namespace nkentseu
