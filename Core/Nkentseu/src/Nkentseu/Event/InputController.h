//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 4:52:28 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INPUTCONTROLLER_H__
#define __NKENTSEU_INPUTCONTROLLER_H__

#pragma once

#include "System/System.h"

#include "Event.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <Nkentseu/Event/ModifierState.h>

namespace nkentseu {

    // Type de fonction pour observer une action
    using ActionSubscriber = std::function<void(const std::string&, EventCategory::Code, int64, bool, bool)>;

    // Type de fonction pour observer un axe
    using AxisSubscriber = std::function<void(const std::string&, EventCategory::Code, int64, float32)>;

    #define REGISTER_ACTION_SUBSCRIBER(x) std::bind(&x, this, STDPH(1), STDPH(2), STDPH(3), STDPH(4), STDPH(5))
    #define REGISTER_AXIS_SUBSCRIBER(x) std::bind(&x, this, STDPH(1), STDPH(2), STDPH(3), STDPH(4))

    class AxisMap;
    class ActionMap;
    class EventObservable;
    class InputManager;

    // Structure représentant une commande d'action
    struct NKENTSEU_API ActionCommand {
        ActionCommand(const std::string& name, const EventCategory::Code& category, int64 code, ModifierState modifierState = {}, bool repeatable = true);
        std::string GetName() const;
        EventCategory::Code GetCategory() const;
        bool IsRepeatable() const;
        int64 GetCode() const;
        ModifierState GetModifierState() const;
        bool IsPrivateRepeatable() const;
        void SetPrivateRepeatable(bool value);
        bool operator==(const ActionCommand& other) const;
        bool operator!=(const ActionCommand& other) const;
    private:
        std::string m_Name;
        EventCategory::Code m_Category;
        bool m_Repeatable;
        bool m_PrivateRepeatable;
        int64 m_Code;
        ModifierState m_ModifierState;
    };

    // Structure représentant une commande d'axe
    struct NKENTSEU_API AxisCommand {
        AxisCommand(const std::string& name, const EventCategory::Code& category, int64 code, float32 scale = 1.0f, float32 minInterval = 0.0f);
        std::string GetName() const;
        EventCategory::Code GetCategory() const;
        int64 GetCode() const;
        float32 GetScale() const;
        float32 GetValue() const;
        float32 GetMinInterval() const;
        bool operator==(const AxisCommand& other) const;
        bool operator!=(const AxisCommand& other) const;
    private:
        std::string m_Name;
        EventCategory::Code m_Category;
        int64 m_Code;
        float32 m_Scale;
        float32 m_Value;
        float32 m_MinInterval;
    };

    // Classe pour gérer les actions et leurs observateurs
    class NKENTSEU_API ActionManager {
        friend class InputManager;
    public:
        void CreateAction(const std::string& actionName, const ActionSubscriber& handler);
        void AddCommand(const ActionCommand& command);
        void RemoveAction(const std::string& actionName);
        void RemoveCommand(const ActionCommand& command);
    private:
        void CheckEventStatus(const std::string& name, bool isValueDown, bool isValueUp, ActionCommand& command);
        void TriggerAction();
        std::unordered_map<std::string, ActionSubscriber> m_Actions;
        std::unordered_map<std::string, std::vector<ActionCommand>> m_Commands;
    };

    // Classe pour gérer les axes et leurs observateurs
    class NKENTSEU_API AxisManager {
        friend class InputManager;
    public:
        void CreateAxis(const std::string& axisName, const AxisSubscriber& handler);
        void AddCommand(const AxisCommand& command);
        void RemoveAxis(const std::string& axisName);
        void RemoveCommand(const AxisCommand& command);
    private:
        bool CheckEventStatus(const std::string& name, bool isType, float32 value, AxisCommand& axis);
        void UpdateAxis();
        std::unordered_map<std::string, AxisSubscriber> m_Axes;
        std::unordered_map<std::string, std::vector<AxisCommand>> m_Commands;
    };

} // namespace nkentseu

#endif    // __NKENTSEU_INPUTCONTROLLER_H__