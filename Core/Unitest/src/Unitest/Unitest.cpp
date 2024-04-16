//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/10/2024 at 7:35:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnitestPch/ntspch.h"
#include "Unitest.h"
#include <Logger/Log.h>

namespace nkentseu {

    Unitest::Unitest() : m_Logger("Unitest") {
    }

    // Returns a reference to the singleton instance of the Unitest class
    Unitest& Unitest::Instance() {
        static Unitest unitest;
        return unitest;
    }

    // Registers a unit test with a name, function, and optional description
    void Unitest::Register(const std::string& name, UnitestEntryFunction function, const std::string& description) {
        // Check for empty name
        if (name.empty()) {
            throw std::invalid_argument("Test name cannot be empty");
        }

        // Create a new UnitestEntry object
        auto entry = std::make_shared<UnitestEntry>(name, function, description);

        // Store the entry in the map with the name as the key
        m_UnitestList[name] = entry;

        // Set the current registered test name (for potential detail logging)
        m_CurrentRegister = name;
    }

    int32 Unitest::Run() {
        uint32 totalRun = 0;
        uint32 totalFailed = 0;
        uint32 totalPassed = 0;

        std::string failedTests(""), passedTests(""), noTests("");

        // Iterate through all registered tests
        for (const auto& [name, entry] : m_UnitestList) {
            // Reset the test entry before running
            entry->Reset();
            m_CurrentRegister = name;

            // Execute the test and update counters if necessary
            if (entry->Run(name)) {
                uint32 contextRun = entry->GetUnitestInfoCount();
                uint32 contextFailed = 0;
                uint32 contextPassed = 0;

                // Process individual test assertions
                for (const auto& unitestInfo : entry->GetUnitestInfos()) {
                    if (unitestInfo.IsSuccessfull()) {
                        contextPassed++;
                        if (m_PrintPassedDetails) {
                            m_Logger.Details(unitestInfo.GetFile().c_str(), unitestInfo.GetLine(), unitestInfo.GetFunction().c_str())
                                .Info("{0}", unitestInfo.GetMessage());
                        }
                    }
                    else {
                        contextFailed++;
                        if (m_PrintFailedDetails) {
                            m_Logger.Details(unitestInfo.GetFile().c_str(), unitestInfo.GetLine(), unitestInfo.GetFunction().c_str())
                                .Asserts("{0}", unitestInfo.GetMessage());
                        }
                    }
                }

                totalRun += contextRun;
                totalFailed += contextFailed;
                totalPassed += contextPassed;

                // Summarize test context results
                if (contextRun > 0) {
                    if (contextFailed > 0) {
                        failedTests += Formatter::Instance().Format("\n\t[{0}] > {1}/{2} Failed;", name, contextFailed, contextRun);
                    }
                    if (contextPassed > 0) {
                        passedTests += Formatter::Instance().Format("\n\t[{0}] > {1}/{2} Passed;", name, contextPassed, contextRun);
                    }
                } else if (contextRun == 0) {
                    noTests += Formatter::Instance().Format("\n\t[{0}] > No test detected;", name);
                }
            }
            else {
                m_Logger.Warning("The current test {0} Is not {1} test research", name, entry);
            }
        }

        // Log final test summary
        if (!failedTests.empty()) {
            m_Logger.Details(__FILE__, __LINE__, __FUNCTION__).Asserts("{0} Failed / {1} Total > \n[{2}\n]", totalFailed, totalRun, failedTests);
        }
        if (!passedTests.empty()) {
            m_Logger.Details(__FILE__, __LINE__, __FUNCTION__).Info("{0} Passed / {1} Total > \n[{2}\n]", totalPassed, totalRun, passedTests);
        }
        if (!noTests.empty()) {
            m_Logger.Details(__FILE__, __LINE__, __FUNCTION__).Warning("No test detected > \n[{0}\n]", noTests);
        }

        return totalFailed;
    }


    UnitestState Unitest::VerifyInternal(bool condition, const std::string& file, uint32 line, const std::string& function, const std::string& message) {
        // Early return for empty or unregistered test name
        if (m_CurrentRegister.empty()) {
            return UnitestState::Indeterminate;
        }

        // Check if the test is registered (avoid unnecessary map lookup)
        if (!m_UnitestList.count(m_CurrentRegister)) {
            // Log a warning only if detail printing is enabled
            if (IsDetailPrint()) {
                m_Logger.Details(file.c_str(), line, function.c_str()).Warning("Test block {0} does not exist", m_CurrentRegister);
            }
            return UnitestState::Indeterminate;
        }

        // Update test details and return result
        m_UnitestList.at(m_CurrentRegister)->AddUnitestInfo(UnitestInfo(m_CurrentRegister, condition, file, line, function, message));
        return condition ? UnitestState::Passed : UnitestState::Failed;
    }

    // Returns true if printing details of passed tests is enabled
    bool Unitest::IsPassedDetailPrint() const {
        return m_PrintPassedDetails;
    }

    // Enables or disables printing details of passed tests
    void Unitest::PrintPassedDetails(bool print) {
        m_PrintPassedDetails = print;
    }

    // Returns true if printing details of failed tests is enabled
    bool Unitest::IsFailedDetailPrint() const {
        return m_PrintFailedDetails;
    }

    // Enables or disables printing details of failed tests
    void Unitest::PrintFailedDetails(bool print) {
        m_PrintFailedDetails = print;
    }

    bool Unitest::IsDetailPrint() const
    {
        return m_PrintDetails;
    }

    void Unitest::PrintDetails(bool print)
    {
        m_PrintDetails = print;
    }

    int32 RunTest() {
        // Affichage d'un message de bienvenue avec des couleurs et des sauts de ligne
        std::string message;
        message = "\n\n";
        message += "********************************************************\n";
        message += "**  Bienvenue dans l'application de tests unitaire !  **\n";
        message += "********************************************************\n\n";
        Unitest::Instance().Get().Trace("{0}", message);
        // Exécution des tests
        int32_t result = Unitest::Instance().Run();

        // Affichage d'un message de fin avec des couleurs et des sauts de ligne
        message = "\n\n";
        message += "********************************************************\n";
        message += "**  Tous les tests ont ete executes.  **\n";
        message += "********************************************************\n\n";

        Unitest::Instance().Get().Trace("{0}", message);

        // Pause avant de continuer
        Unitest::Instance().Get().Trace("Presser sur une touche pour continuer...");
        std::getchar();

        return result;
    }

}    // namespace nkentseu