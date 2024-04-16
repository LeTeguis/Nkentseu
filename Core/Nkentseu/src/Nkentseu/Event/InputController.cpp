//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 4:52:28 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InputController.h"
#include <Nkentseu/Core/NkentseuLogger.h>

#include "Keyboard.h"
#include "Mouse.h"
#include "GenericInput.h"
#include "InputManager.h"

namespace nkentseu {
    ActionCommand::ActionCommand(const std::string& name, const EventCategory::Code& category, int64 code, ModifierState modifierState, bool repeatable)
        : m_Name(name), m_Category(category), m_Repeatable(repeatable), m_Code(code), m_ModifierState(modifierState) {
        m_PrivateRepeatable = m_Repeatable;
    }

    std::string ActionCommand::GetName() const
    {
        return m_Name;
    }

    EventCategory::Code ActionCommand::GetCategory() const
    {
        return m_Category;
    }

    bool ActionCommand::IsRepeatable() const
    {
        return m_Repeatable;
    }

    int64 ActionCommand::GetCode() const
    {
        return m_Code;
    }

    ModifierState ActionCommand::GetModifierState() const
    {
        return m_ModifierState;
    }

    bool ActionCommand::IsPrivateRepeatable() const
    {
        return m_PrivateRepeatable;
    }

    void ActionCommand::SetPrivateRepeatable(bool value)
    {
        m_PrivateRepeatable = value;
    }

    bool ActionCommand::operator==(const ActionCommand& other) const
    {
        return m_Category == other.m_Category && m_Code == other.m_Code && m_ModifierState == other.m_ModifierState;
    }

    bool ActionCommand::operator!=(const ActionCommand& other) const
    {
        return !(*this == other);
    }

    //************************************************
    AxisCommand::AxisCommand(const std::string& name, const EventCategory::Code& category, int64 code, float32 scale, float32 minInterval) : m_Name(name), m_Category(category), m_Code(code), m_Scale(scale), m_Value(0.0f), m_MinInterval(minInterval) {}

    std::string AxisCommand::GetName() const
    {
        return m_Name;
    }

    EventCategory::Code AxisCommand::GetCategory() const
    {
        return m_Category;
    }

    int64 AxisCommand::GetCode() const
    {
        return m_Code;
    }

    float32 AxisCommand::GetScale() const
    {
        return m_Scale;
    }

    float32 AxisCommand::GetValue() const
    {
        return m_Value;
    }

    float32 AxisCommand::GetMinInterval() const
    {
        return m_MinInterval;
    }

    bool AxisCommand::operator==(const AxisCommand& other) const
    {
        return m_Category == other.m_Category && m_Code == other.m_Code;;
    }

    bool AxisCommand::operator!=(const AxisCommand& other) const
    {
        return !(*this == other);
    }

    // ************************
    void ActionManager::CreateAction(const std::string& actionName, const ActionSubscriber& handler)
    {
        // Verifier si l'action n'existe pas deja
        if (m_Actions.find(actionName) != m_Actions.end()) {
            Log_nts.Warning("L'action {0} existe deja. La creation de l'evenement a ete ignoree.", actionName);
            return;
        }
        // Ajouter la nouvelle action avec son gestionnaire d'evenement
        m_Actions[actionName] = handler;
    }

    void ActionManager::AddCommand(const ActionCommand& command)
    {
        const std::string& commandName = command.GetName();
        // Verifier si l'action existe deja
        if (m_Actions.find(commandName) == m_Actions.end()) {
            Log_nts.Warning("L\'action {0} n'existe pas. L\'ajout d'evenement a ete ignoree.", commandName);
            return;
        }

        // Verifier si l'action existe deja
        if (m_Commands.find(commandName) == m_Commands.end()) {
            m_Commands[commandName] = std::vector<ActionCommand>();
        }

        // Verifier si l'action a deja ete ajoute
        auto& actions = m_Commands[commandName];
        auto actions_it = std::find_if(actions.begin(), actions.end(), [&command](const ActionCommand& a) {
            return a == command;
            });

        if (actions_it != actions.end()) {
            Log_nts.Warning("L\'action a deja ete ajoutee. L'ajout d'evenement a ete ignore.");
            return;
        }

        // Ajouter l'evenement a l'action correspondante
        actions.push_back(command);
    }

    void ActionManager::RemoveAction(const std::string& actionName)
    {
        m_Actions.erase(actionName);
        m_Commands.erase(actionName);
    }

    void ActionManager::RemoveCommand(const ActionCommand& command)
    {
        auto it = m_Commands.find(command.GetName());
        if (it == m_Commands.end()) {
            Log_nts.Warning("L\'action {0} n'existe pas. La suppression d'evenement a ete ignoree.", command.GetName());
            return;
        }
        // Supprimer l'vnement de l'action correspondante
        auto& actions = it->second;
        actions.erase(std::remove(actions.begin(), actions.end(), command), actions.end());
    }

    void ActionManager::CheckEventStatus(const std::string& name, bool isValueDown, bool isValueUp, ActionCommand& command)
    {
        if (isValueDown) {
            if (command.IsRepeatable() || (!command.IsRepeatable() && !command.IsPrivateRepeatable())) {
                // Declencher l'action correspondante avec ses parametres
                ActionSubscriber subscriber = m_Actions.at(name);
                subscriber(name, command.GetCategory(), command.GetCode(), true, false);
                Log_nts.Debug();

                command.SetPrivateRepeatable(true);
            }
        }

        if (isValueUp) {
            command.SetPrivateRepeatable(false);
        }
    }

    void ActionManager::TriggerAction()
    {
        bool ctrl = Input.IsKeyDown(Keyboard::LCtrl_ev) || Input.IsKeyDown(Keyboard::RCtrl_ev);
        bool alt = Input.IsKeyDown(Keyboard::LAlt_ev) || Input.IsKeyDown(Keyboard::RAlt_ev);
        bool shift = Input.IsKeyDown(Keyboard::LShift_ev) || Input.IsKeyDown(Keyboard::RShift_ev);
        bool super = Input.IsKeyDown(Keyboard::LSuper_ev) || Input.IsKeyDown(Keyboard::RSuper_ev);

        // Verifier chaque action et ses evenements pour voir si un evenement est declencher
        for (auto&& [actionName, events] : m_Commands) {
            //LOG.Debug("{0}-{1}", actionName, events.size());
            for (auto& action : events) {
                if (action.GetModifierState().Alt != alt && action.GetModifierState().Ctrl != ctrl &&
                    action.GetModifierState().Shift != shift && action.GetModifierState() != super)
                    continue;
                /* clavier */
                if (EventCategory::Keyboard_ev == action.GetCategory()) {
                    bool isKeyDown = Input.IsKeyDown((Keyboard::Code)action.GetCode());
                    bool isKeyUp = Input.IsKeyUp((Keyboard::Code)action.GetCode());

                    Log_nts.Debug("{0}-{1}-{2}", Keyboard::GetKeycode((Keyboard::Code)action.GetCode()), (int32)isKeyDown, (int32)isKeyUp);

                    CheckEventStatus(actionName, isKeyDown, isKeyUp, action);

                    continue;
                }

                /* Souris */
                if (EventCategory::Mouse_ev == action.GetCategory()) {
                    bool isKeyDown = Input.IsMouseDown((Mouse::Button)action.GetCode());
                    bool isKeyUp = Input.IsMouseUp((Mouse::Button)action.GetCode());

                    CheckEventStatus(actionName, isKeyDown, isKeyUp, action);

                    continue;
                }
            }
        }
    }

    // ****************************************************
    void AxisManager::CreateAxis(const std::string& axisName, const AxisSubscriber& handler)
    {
        if (m_Axes.find(axisName) == m_Axes.end()) {
            m_Axes[axisName] = handler;
        }
        else {
            Log_nts.Warning("Axis already exists with name: {0}", axisName);
        }
    }

    void AxisManager::AddCommand(const AxisCommand& command)
    {
        const std::string& axisName = command.GetName();

        if (m_Axes.find(axisName) == m_Axes.end()) {
            Log_nts.Warning("Axis does not exist with name: {0}", axisName);
            return;
        }

        // Vrifier si l'action a dj t ajoute
        auto& axis_ = m_Commands[axisName];
        auto axis_it = std::find_if(axis_.begin(), axis_.end(), [&command](const AxisCommand& a) {
            return a == command;
            });

        if (axis_it != axis_.end()) {
            Log_nts.Warning("Handler already subscribed to axis: {0}", axisName);
            return;
        }

        axis_.push_back(command);
    }

    void AxisManager::RemoveAxis(const std::string& axisName)
    {
        if (m_Commands.find(axisName) == m_Commands.end()) {
            Log_nts.Warning("Axis does not exist with name: {0}", axisName);
            return;
        }
        m_Commands.erase(axisName);
        m_Axes.erase(axisName);
    }

    void AxisManager::RemoveCommand(const AxisCommand& command)
    {
        const std::string& axisName = command.GetName();
        auto it = m_Commands.find(axisName);
        if (it == m_Commands.end()) {
            Log_nts.Warning("Axis does not exist with name: {0}", axisName);
            return;
        }
        // Supprimer l'vnement de l'axis correspondante
        auto& axis_ = it->second;
        axis_.erase(std::remove(axis_.begin(), axis_.end(), command), axis_.end());
    }

    bool AxisManager::CheckEventStatus(const std::string& name, bool isType, float32 value, AxisCommand& axis)
    {
        if (isType) {
            float32 min_a = 0.0f; // borne inferieure de l'intervalle de a
            float32 max_a = 1.0f; // borne superieure de l'intervalle de a
            float32 prop_a = (value - min_a) / (max_a - min_a);
            float32 result;

            result = (axis.GetMinInterval() + prop_a) * axis.GetScale();
            m_Axes.at(name)(name, axis.GetCategory(), axis.GetCode(), result);
            return true;
        }
        return false;
    }

    void AxisManager::UpdateAxis()
    {
        for (auto&& [axisName, events] : m_Commands) {
            for (auto& axis : events) {
                /* clavier */
                Keyboard::Code key = static_cast<Keyboard::Code>(axis.GetCode());
                if (CheckEventStatus(axisName, EventCategory::Keyboard_ev == axis.GetCategory(), Input.KeyAxis(key), axis)) continue;

                /* Souris */
                Mouse::Button mouse = static_cast<Mouse::Button>(axis.GetCode());
                if (CheckEventStatus(axisName, EventCategory::Mouse_ev == axis.GetCategory(), Input.MouseAxis(mouse), axis)) continue;
            }
        }
    }

}    // namespace nkentseu