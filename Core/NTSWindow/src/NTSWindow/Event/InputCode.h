//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/18/2024 at 1:32:20 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INPUTCODE_H__
#define __NKENTSEU_INPUTCODE_H__

#pragma once

#include "NTSCore/System.h"
#include "EventType.h"

#include "EventState.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "GenericInput.h"
#include "Gamepad.h"

namespace nkentseu {
    #define IsInputType(inputType_, data, type) if (inputType_ == input) data = static_cast<type>(code)

    struct NKENTSEU_API ActionCode {
        using Code = int64;

        union {
            Keyboard::Code      keyboard;    // si (category == EventCategory::Keyboard)
            Mouse::Code         mouse;    // si (category == EventCategory::MouseButton)
            GenericInput::Code  genericInput;    // si (category == EventCategory::GenericInput)
            Gamepad::Code       gamepadInput;
        };

        EventType::Code inputType;
        ModifierState modifierState;

        ActionCode(EventType::Code input, Code code, ModifierState modifier = {}) : inputType(input), modifierState(modifier) {
            IsInputType(EventType::KeyboardInput, keyboard, Keyboard::Code);
            IsInputType(EventType::MouseInput, mouse, Mouse::Code);
            IsInputType(EventType::GenericInput, genericInput, GenericInput::Code);
            IsInputType(EventType::GamepadInput, gamepadInput, Gamepad::Code);
        }

        bool IsValideModifierState(const ModifierState& mod) const {
            if (modifierState.control && !mod.control) return false;
            if (modifierState.alt && !mod.alt) return false;
            if (modifierState.shift && !mod.shift) return false;
            if (modifierState.super && !mod.super) return false;
            return true;
        }

        // Opérateur d'égalité
        bool operator==(const ActionCode& other) const {
            // if (IsValideModifierState(other.modifierState)) {
            if (other.modifierState == modifierState) {
                if (other.inputType == inputType) {
                    if (inputType == EventType::KeyboardInput && keyboard == other.keyboard) return true;
                    if (inputType == EventType::MouseInput && mouse == other.mouse) return true;
                    if (inputType == EventType::GenericInput && genericInput == other.genericInput) return true;
                    if (inputType == EventType::GamepadInput && gamepadInput == other.gamepadInput) return true;
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
            Gamepad::Code       gamepadInput;
        };

        EventType::Code inputType;
        ModifierState modifierState;

        // Constructeur avec paramètres
        AxisCode(EventType::Code input, Code code, ModifierState modifier = {}) : inputType(input), modifierState(modifier) {
            IsInputType(EventType::KeyboardInput, keyboard, Keyboard::Code);
            IsInputType(EventType::MouseInput, mouse, Mouse::Button);
            IsInputType(EventType::MouseWheel, mouse, Mouse::Wheel);
            IsInputType(EventType::GenericInput, genericInput, GenericInput::Button);
            IsInputType(EventType::GenericAxis, genericInput, GenericInput::Axis);
            IsInputType(EventType::GenericHat, genericInput, GenericInput::Hat);
            IsInputType(EventType::GamepadInput, gamepadInput, Gamepad::Button);
            IsInputType(EventType::GamepadAxis, gamepadInput, Gamepad::Axis);
        }


        // Opérateur d'égalité
        bool operator==(const AxisCode& other) const {

            if (other.modifierState == modifierState) {
                if (other.inputType == inputType) {
                    if (inputType == EventType::KeyboardInput && keyboard == other.keyboard) return true;
                    if (inputType == EventType::MouseInput && mouse == other.mouse) return true;
                    if (inputType == EventType::MouseWheel && mouse == other.mouse) return true;
                    if (inputType == EventType::GenericInput && genericInput == other.genericInput) return true;
                    if (inputType == EventType::GenericAxis && genericInput == other.genericInput) return true;
                    if (inputType == EventType::GenericHat && genericInput == other.genericInput) return true;
                    if (inputType == EventType::GamepadInput && gamepadInput == other.gamepadInput) return true;
                    if (inputType == EventType::GamepadAxis && gamepadInput == other.gamepadInput) return true;
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