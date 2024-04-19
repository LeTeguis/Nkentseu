//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/18/2024 at 1:32:20 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INPUTCODE_H__
#define __NKENTSEU_INPUTCODE_H__

#pragma once

#include "System/System.h"
#include "ModifierState.h"
#include "EventCategory.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "GenericInput.h"

namespace nkentseu {
    struct NKENTSEU_API ActionCode {
        using Code = int64;

        union {
            Keyboard::Code      keyboard;    // si (category == EventCategory::Keyboard)
            Mouse::Code         mouse;    // si (category == EventCategory::MouseButton)
            GenericInput::Code  genericInput;    // si (category == EventCategory::GenericInput)
        };

        EventCategory::Code category;
        ModifierState modifierState;

        ActionCode(EventCategory::Code cat, Code code, ModifierState modifier = {}) : category(cat), modifierState(modifier) {
            #define IsCategory(category, data, type) if (category == cat) data = static_cast<type::Code>(code);

            IsCategory(EventCategory::Keyboard, keyboard, Keyboard)
                IsCategory(EventCategory::MouseButton, mouse, Mouse)
                IsCategory(EventCategory::GenericInput, genericInput, GenericInput)
        }

        bool IsValideModifierState(const ModifierState& mod) const {
            if (modifierState.Ctrl && !mod.Ctrl) return false;
            if (modifierState.Alt && !mod.Alt) return false;
            if (modifierState.Shift && !mod.Shift) return false;
            if (modifierState.Super && !mod.Super) return false;
            return true;
        }


        // Opérateur d'égalité
        bool operator==(const ActionCode& other) const {

            // if (IsValideModifierState(other.modifierState)) {
            if (other.modifierState == modifierState) {
                if (other.category == category) {
                    if (category == EventCategory::Keyboard && keyboard == other.keyboard) return true;
                    if (category == EventCategory::MouseButton && mouse == other.mouse) return true;
                    if (category == EventCategory::GenericInput && genericInput == other.genericInput) return true;
                }
            }

            return false;
        }

        bool operator!=(const ActionCode& other) const {
            return !(*this == other);
        }
    };

    struct NKENTSEU_API AxisCode {
        using Code = uint64;

        union {
            Keyboard::Code      keyboard;    // si (category == EventCategory::Keyboard)
            Mouse::Code         mouse;    // si (category == EventCategory::MouseButton)
            GenericInput::Code  genericInput;    // si (category == EventCategory::GenericInput)
        };

        EventCategory::Code category;
        ModifierState modifierState;

        // Constructeur avec paramètres
        AxisCode(EventCategory::Code cat, Code code, ModifierState modifier = {}) : category(cat), modifierState(modifier) {
            #define IsCategory(category, data, type) if (category == cat) data = static_cast<type::Code>(code);

            IsCategory(EventCategory::Keyboard, keyboard, Keyboard)
            IsCategory(EventCategory::MouseButton, mouse, Mouse)
            IsCategory(EventCategory::GenericInput, genericInput, GenericInput)
        }


        // Opérateur d'égalité
        bool operator==(const AxisCode& other) const {

            if (other.modifierState == modifierState) {
                if (other.category == category) {
                    if (category == EventCategory::Keyboard && keyboard == other.keyboard) return true;
                    if (category == EventCategory::MouseButton && mouse == other.mouse) return true;
                    if (category == EventCategory::GenericInput && genericInput == other.genericInput) return true;
                }
            }

            return false;
        }

        bool operator!=(const AxisCode& other) const {
            return !(*this == other);
        }
    };
} // namespace nkentseu

#endif    // __NKENTSEU_INPUTCODE_H__