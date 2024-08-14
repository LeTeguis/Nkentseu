//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-21 at 05:19:40 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __THEMES_H__
#define __THEMES_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

namespace nkentseu {
    namespace ui {
        class UISkin;
        class UITheme;

        // Classe de base pour les propriétés de thème
        class NKENTSEU_API UIThemeProperty {
        public:
            UIThemeProperty(UITheme* theme, const std::string& name);
            virtual std::string GetName();
            virtual UITheme* GetOwner();
        private:
            std::string name;
            UITheme* theme;
        };

        // Classe de base pour les propriétés de skin
        class NKENTSEU_API UISkinProperty {
        public:
            UISkinProperty(UISkin* theme, const std::string& name);
            virtual std::string GetName();
            virtual UISkin* GetOwner();
        private:
            std::string name;
            UISkin* theme;
        };

        template <typename T, typename U>
        using DefineThemePropertyIf = typename std::enable_if<std::is_base_of<UIThemeProperty, T>::value, U>::type;

        template <typename T, typename U>
        using DefineSkinPropertyIf = typename std::enable_if<std::is_base_of<UISkinProperty, T>::value, U>::type;

        // Classe pour les themes
        class NKENTSEU_API UITheme {
        public:
            UITheme(const std::string& name);

            template <typename T, typename... Args>
            DefineThemePropertyIf<T, Memory::Shared<T>> AddProperty(Args&&... args) {
                // Rechercher la propriété existante
                auto property = GetProperty<T>();
                if (property) {
                    // Retirer la propriété existante
                    properties.erase(std::remove(properties.begin(), properties.end(), property), properties.end());
                }
                // Ajouter la nouvelle propriété
                auto theme = Memory::Alloc<T>(*this, std::forward<Args>(args)...);
                if (theme == nullptr) return nullptr;
                properties.emplace_back(theme);
                return theme;
            }

            template <typename T>
            DefineThemePropertyIf<T, Memory::Shared<T>> GetProperty() const {
                // Rechercher la propriété
                for (const auto& property : properties) {
                    if (T* ptr = dynamic_cast<T*>(property.get())) {
                        return property;
                    }
                }
                return nullptr;
            }

            template <typename T>
            DefineThemePropertyIf<T, bool> HasProperty() const {
                // Rechercher la propriété
                for (const auto& property : properties) {
                    if (T* ptr = dynamic_cast<T*>(property.get())) {
                        return true;
                    }
                }
                return false;
            }

            template <typename T>
            DefineThemePropertyIf<T, void> RemoveProperty() {
                // Rechercher la propriété
                for (auto it = properties.begin(); it != properties.end(); ++it) {
                    if (T* ptr = dynamic_cast<T*>((*it).get())) {
                        properties.erase(it);
                        return;
                    }
                }
            }

            std::vector<Memory::Shared<UIThemeProperty>> GetProperties() const {
                return properties;
            }

        private:
            std::string name;
            std::vector<Memory::Shared<UIThemeProperty>> properties;
        };

        // Classe pour les skins
        class NKENTSEU_API UISkin {
        public:
            UISkin(const std::string& name);

            template <typename T, typename... Args>
            DefineSkinPropertyIf<T, Memory::Shared<T>> AddProperty(Args&&... args) {
                // Rechercher la propriété existante
                auto property = GetProperty<T>();
                if (property) {
                    // Retirer la propriété existante
                    properties.erase(std::remove(properties.begin(), properties.end(), property), properties.end());
                }
                // Ajouter la nouvelle propriété
                auto theme = Memory::Alloc<T>(*this, std::forward<Args>(args)...);
                if (theme == nullptr) return nullptr;
                properties.emplace_back(theme);
                return theme;
            }

            template <typename T>
            DefineSkinPropertyIf<T, Memory::Shared<T>> GetProperty() const {
                // Rechercher la propriété
                for (const auto& property : properties) {
                    if (T* ptr = dynamic_cast<T*>(property.get())) {
                        return property;
                    }
                }
                return nullptr;
            }

            template <typename T>
            DefineSkinPropertyIf<T, bool> HasProperty() const {
                // Rechercher la propriété
                for (const auto& property : properties) {
                    if (T* ptr = dynamic_cast<T*>(property.get())) {
                        return true;
                    }
                }
                return false;
            }

            template <typename T>
            DefineSkinPropertyIf<T, void> RemoveProperty() {
                // Rechercher la propriété
                for (auto it = properties.begin(); it != properties.end(); ++it) {
                    if (T* ptr = dynamic_cast<T*>((*it).get())) {
                        properties.erase(it);
                        return;
                    }
                }
            }

            std::vector<Memory::Shared<UISkinProperty>> GetProperties() const {
                return properties;
            }

        private:
            std::string name;
            std::vector<Memory::Shared<UISkinProperty>> properties;
        };

    }

}  //  nkentseu

#endif  // __THEMES_H__!