//
// Créé par TEUGUIA TADJUIDJE Rodolf Séderis le 7/04/2024 à 20:34:36.
// Copyright (c) 2024 Rihen. Tous droits réservés.
//

#ifndef __NKENTSEU_LOGGER_INFOS_H__
#define __NKENTSEU_LOGGER_INFOS_H__

#pragma once

#include "NTSCore/System.h"
#include "Severity.h"

namespace nkentseu {

    /**
     * @brief Classe pour stocker les informations de log.
     *
     * Cette classe stocke les informations relatives à un message de log,
     * y compris le niveau de sévérité, le nom du logger, le fichier d'origine,
     * la fonction d'origine, le numéro de ligne, l'entête (optionnel) et le message.
     * Elle fournit des méthodes pour accéder et modifier ces informations.
     */
    class NKENTSEU_API LoggerInfos {
    public:
        /**
         * @brief Constructeur par défaut.
         *
         * Initialise les informations de log avec des valeurs par défaut.
         */
        LoggerInfos();

        /**
         * @brief Constructeur complet.
         *
         * Initialise les informations de log avec les paramètres spécifiés.
         *
         * @param severity Niveau de sévérité du message de log.
         * @param name Nom du logger.
         * @param file Fichier d'origine du message de log.
         * @param function Fonction d'origine du message de log.
         * @param line Numéro de ligne d'origine du message de log.
         * @param header Entête optionnel du message de log.
         * @param message Message de log.
         */
        LoggerInfos(Severity severity, const std::string& name, const std::string& file,
            const std::string& function, uint32 line, const std::string& header,
            const std::string& message);

        /**
         * @brief Constructeur alternatif.
         *
         * Initialise les informations de log avec les paramètres spécifiés
         * dans un ordre différent.
         *
         * @param file Fichier d'origine du message de log.
         * @param function Fonction d'origine du message de log.
         * @param line Numéro de ligne d'origine du message de log.
         * @param severity Niveau de sévérité du message de log.
         * @param name Nom du logger.
         * @param header Entête optionnel du message de log.
         * @param message Message de log.
         */
        LoggerInfos(const std::string& file, const std::string& function, uint32 line, Severity severity,
            const std::string& name, const std::string& header, const std::string& message);

        // Getter (accesseurs) pour les informations de log
        const std::string& GetName() const;
        Severity GetSeverity() const;
        const std::string& GetFile() const;
        const std::string& GetFunction() const;
        uint32 GetLine() const;
        const std::string& GetHeader() const;
        const std::string& GetMessages() const;
        const Date& GetDate() const;
        const Time& GetTime() const;

        // Setter (modificateurs) pour les informations de log
        void SetName(const std::string& name);
        void SetSeverity(Severity severity);
        void SetFile(const std::string& file);
        void SetFunction(const std::string& function);
        void SetLine(uint32 line);
        void SetHeader(const std::string& header);
        void SetMessages(const std::string& message);
        void SetDate(const Date& date);
        void SetTime(const Time& time);

    private:
        // Membres privés stockant les informations de log
        std::string m_Name;       // Nom du logger
        Severity m_Severity;  // Niveau de sévérité
        std::string m_File;        // Fichier d'origine
        std::string m_Function;    // Fonction d'origine
        uint32 m_Line;          // Numéro de ligne
        std::string m_Header;      // Entête (optionnel)
        std::string m_Message;    // Message de log
        Date m_Date;    // Message de log
        Time m_Time;    // Message de log
    };

} // namespace nkentseu

#endif // __NKENTSEU_LOGGER_INFOS
