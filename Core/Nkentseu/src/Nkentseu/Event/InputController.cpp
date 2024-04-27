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

namespace nkentseu {
    ActionCommand::ActionCommand(const std::string& name, ActionCode code, bool repeatable)
        : m_Name(name), m_Repeatable(repeatable), m_Code(code) {
        m_PrivateRepeatable = m_Repeatable;
    }

    std::string ActionCommand::GetName() const
    {
        return m_Name;
    }

    bool ActionCommand::IsRepeatable() const
    {
        return m_Repeatable;
    }

    const ActionCode& ActionCommand::GetCode() const
    {
        return m_Code;
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
        return m_Code == other.m_Code;
    }

    bool ActionCommand::operator!=(const ActionCommand& other) const
    {
        return !(*this == other);
    }

    //************************************************
    AxisCommand::AxisCommand(const std::string& name, AxisCode code, float32 scale, float32 minInterval) : m_Name(name), m_Code(code), m_Scale(scale), m_Value(0.0f), m_MinInterval(minInterval) {}

    std::string AxisCommand::GetName() const
    {
        return m_Name;
    }

    const AxisCode& AxisCommand::GetCode() const
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
        return m_Code == other.m_Code;;
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

    void ActionManager::CheckEventStatus(const std::string& name, ButtonState::Code state, ActionCommand& command)
    {
        if (state == ButtonState::Pressed) {
            if (command.IsRepeatable() || (!command.IsRepeatable() && !command.IsPrivateRepeatable())) {
                // Declencher l'action correspondante avec ses parametres
                ActionSubscriber subscriber = m_Actions.at(name);
                subscriber(name, command.GetCode(), true, false);
                Log_nts.Debug();

                command.SetPrivateRepeatable(true);
            }
        }

        if (state == ButtonState::Released) {
            command.SetPrivateRepeatable(false);
        }
    }

    void ActionManager::TriggerAction(const ActionCode& actionCode, ButtonState::Code state)
    {
        // Verifier chaque action et ses evenements pour voir si un evenement est declencher
        for (auto&& [actionName, events] : m_Commands) {
            for (auto& action : events) {
                if (action.GetCode() == actionCode) {
                    CheckEventStatus(actionName, state, action);
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

    void AxisManager::CheckEventStatus(const std::string& name, float32 value, AxisCommand& axis)
    {
        float32 min_a = 0.0f; // borne inferieure de l'intervalle de a
        float32 max_a = 1.0f; // borne superieure de l'intervalle de a
        float32 prop_a = (value - min_a) / (max_a - min_a);
        float32 result;

        result = (axis.GetMinInterval() + prop_a) * axis.GetScale();
        m_Axes.at(name)(name, axis.GetCode(), result);
    }

    void AxisManager::UpdateAxis(const AxisUpdateFromCode& update)
    {
        // Log_nts.Debug("numper = {0}-{1}", m_Commands.size(), this);
        for (auto&& [axisName, events] : m_Commands) {
            //Log_nts.Debug("number running = {0}", m_Commands.size());
            for (auto& axis : events) {
                /* clavier */
                if (EventType::KeyboardInput == axis.GetCode().inputType) {
                    Keyboard::Code code = axis.GetCode().keyboard;
                    CheckEventStatus(axisName, update(EventCategory::Keyboard, code), axis);
                    continue;
                }

                /* Souris */
                if (EventType::MouseInput == axis.GetCode().inputType) {
                    Mouse::Code code = axis.GetCode().mouse;
                    CheckEventStatus(axisName, update(EventCategory::Mouse, code), axis);
                    continue;
                }//*/
            }
        }
    }

}    // namespace nkentseu