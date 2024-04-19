//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 4:52:28 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INPUTCONTROLLER_H__
#define __NKENTSEU_INPUTCONTROLLER_H__

#pragma once

#include "System/System.h"

#include <string>
#include <functional>
#include <unordered_map>

#include "Event.h"
#include "InputCode.h"
#include "Nkentseu/Core/NkentseuLogger.h"

namespace nkentseu {

    #define REGISTER_ACTION_SUBSCRIBER(x) std::bind(&x, this, STDPH(1), STDPH(2), STDPH(3), STDPH(4))
    #define REGISTER_AXIS_SUBSCRIBER(x) std::bind(&x, this, STDPH(1), STDPH(2), STDPH(3))

    class EventObservable;
    class InputManager;

    // Structure représentant une commande d'action
    struct NKENTSEU_API ActionCommand {
        ActionCommand(const std::string& name, ActionCode code, bool repeatable = true);
        std::string GetName() const;
        bool IsRepeatable() const;
        const ActionCode& GetCode() const;
        bool IsPrivateRepeatable() const;
        void SetPrivateRepeatable(bool value);
        bool operator==(const ActionCommand& other) const;
        bool operator!=(const ActionCommand& other) const;
    private:
        std::string m_Name;
        bool m_Repeatable;
        bool m_PrivateRepeatable;
        ActionCode m_Code;
    };

    // Type de fonction pour observer une action
    using ActionSubscriber = std::function<void(const std::string&, const ActionCode&, bool, bool)>;

    // Structure représentant une commande d'axe
    struct NKENTSEU_API AxisCommand {
        AxisCommand(const std::string& name, AxisCode code, float32 scale = 1.0f, float32 minInterval = 0.0f);
        std::string GetName() const;
        const AxisCode& GetCode() const;
        float32 GetScale() const;
        float32 GetValue() const;
        float32 GetMinInterval() const;
        bool operator==(const AxisCommand& other) const;
        bool operator!=(const AxisCommand& other) const;
    private:
        std::string m_Name;
        AxisCode m_Code;
        float32 m_Scale;
        float32 m_Value;
        float32 m_MinInterval;
    };

    // Type de fonction pour observer un axe
    using AxisSubscriber = std::function<void(const std::string&, const AxisCode&, float32)>;

    // Classe pour gérer les actions et leurs observateurs
    class NKENTSEU_API ActionManager {
        friend class InputManager;
    public:
        void CreateAction(const std::string& actionName, const ActionSubscriber& handler);
        void AddCommand(const ActionCommand& command);
        void RemoveAction(const std::string& actionName);
        void RemoveCommand(const ActionCommand& command);

        uint64 GetActionNumber() {
            return m_Actions.size();
        }
        uint64 GetCommandNumber(const std::string& actionName) {
            if (m_Actions.find(actionName) == m_Actions.end()) {
                return 0;
            }
            return m_Commands[actionName].size();
        }
        uint64 GetCommandNumber() {
            uint64 number = 0;
            for (auto&& [axisName, events] : m_Commands) {
                number += events.size();
            }
            return number;
        }
    private:
        void CheckEventStatus(const std::string& name, ButtonState::Code state, ActionCommand& command);
        void TriggerAction(const ActionCode& actionCode, ButtonState::Code state);
        std::unordered_map<std::string, ActionSubscriber> m_Actions;
        std::unordered_map<std::string, std::vector<ActionCommand>> m_Commands;
    };

    using AxisUpdateFromCode = std::function<float(EventCategory::Code, int64)>;

    // Classe pour gérer les axes et leurs observateurs
    class NKENTSEU_API AxisManager {
        friend class InputManager;
    public:
        void CreateAxis(const std::string& axisName, const AxisSubscriber& handler);
        void AddCommand(const AxisCommand& command);
        void RemoveAxis(const std::string& axisName);
        void RemoveCommand(const AxisCommand& command);

        uint64 GetAxisNumber() {
            return m_Axes.size();
        }

        uint64 GetCommandNumber(const std::string& actionName) {
            if (m_Axes.find(actionName) == m_Axes.end()) {
                return 0;
            }
            return m_Commands[actionName].size();
        }

        uint64 GetCommandNumber() {
            uint64 number = 0;
            for (auto&& [axisName, events] : m_Commands) {
                number += events.size();
            }
            return number;
        }
    private:
        void CheckEventStatus(const std::string& name, float32 value, AxisCommand& axis);
        void UpdateAxis(const AxisUpdateFromCode& update);
        std::unordered_map<std::string, AxisSubscriber> m_Axes;
        std::unordered_map<std::string, std::vector<AxisCommand>> m_Commands;
    };

} // namespace nkentseu

#endif    // __NKENTSEU_INPUTCONTROLLER_H__