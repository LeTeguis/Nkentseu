//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 5:37:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INPUT_MANAGER_H__
#define __NKENTSEU_INPUT_MANAGER_H__

#pragma once

#include "NTSCore/System.h"
#include <NTSMaths/Vector/Vector2.h>
#include "EventState.h"

#include "InputController.h"

#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "GenericInputEvent.h"
#include "GamepadEvent.h"

#include <NTSMaths/Random.h>


namespace nkentseu {
    #define UPDATE_AXIS(axis, down, up, elapse) for (auto& [key, value] : axis) {\
                                                    if (up[key] && axis[key] != 0.0f) {\
                                                        axis[key] -= elapse;\
                                                        if (axis[key] < 0.0f) {\
                                                            axis[key] = 0.0f;\
                                                        }\
                                                    }\
                                                    if (down[key] && axis[key] != 1.0f) {\
                                                        axis[key] += elapse;\
                                                        if (axis[key] >= 1.0f) {\
                                                            axis[key] = 1.0f;\
                                                        }\
                                                    }\
                                                }\

    class NKENTSEU_API InputManager
    {

    public:
        static InputManager& Instance();

        /* Moouse */
        inline maths::Vector2i MousePosition();
        inline int32 MouseX();
        inline int32 MouseY();
        inline maths::Vector2i MouseDelta();
        inline int32 MouseXDelta();
        inline int32 MouseYDelta();

        inline ButtonState::Code MouseButton(Mouse::Button button);

        inline ButtonState::Code MouseButton(const std::string& button);

        inline bool IsMouseDown(Mouse::Button button);

        inline bool IsMouseDown(const std::string& button);

        inline bool IsMouseUp(Mouse::Button button);

        inline bool IsMouseUp(const std::string& button);

        inline float32 MouseAxis(Mouse::Button button);

        inline float32 MouseAxis(const std::string& button);

        /* Keyboard */
        bool IsKeyDown(Keyboard::Code keycode);
        bool IsKeyDown(const std::string& keycode);

        bool IsKeyUp(Keyboard::Code keycode);
        bool IsKeyUp(const std::string& keycode);

        ButtonState::Code Key(Keyboard::Code keycode);

        ButtonState::Code Key(const std::string& keycode);

        inline float32 KeyAxis(Keyboard::Code keycode);

        inline float32 KeyAxis(const std::string& keycode);

        /* Generic Hid */
        bool IsGenericInputDown(GenericInput::Button buttoncode);
        bool IsGenericInputDown(const std::string& buttoncode);

        bool IsGenericInputUp(GenericInput::Button buttoncode);
        bool IsGenericInputUp(const std::string& buttoncode);

        ButtonState::Code GenericInput(GenericInput::Button buttoncode);
        ButtonState::Code GenericInput(const std::string& buttoncode);

        inline float32 GenericInputButtonAxis(GenericInput::Button buttoncode);
        inline float32 GenericInputButtonAxis(const std::string& buttoncode);

        inline float32 GenericInputAxis(GenericInput::Axis axis);
        inline float32 GenericInputAxis(const std::string& axis);

        inline uint32 GenericInputHat(GenericInput::Hat hate);
        inline uint32 GenericInputHat(const std::string& hate);

        /* Gamepad */
        /*bool IsGamepadButtonDown(GenericInput::Button buttoncode);
        bool IsGamepadButtonDown(const std::string& buttoncode);

        bool IsGamepadButtonUp(GenericInput::Button buttoncode);
        bool IsGamepadButtonUp(const std::string& buttoncode);

        ButtonState::Code GamepadButton(GenericInput::Button buttoncode);
        ButtonState::Code GamepadButton(const std::string& buttoncode);

        inline float32 GamepadButtonAxis(GenericInput::Button buttoncode);
        inline float32 GamepadButtonAxis(const std::string& buttoncode);

        inline float32 GamepadButtonAxis(GenericInput::Axis axis);
        inline float32 GamepadButtonAxis(const std::string& axis);*/

        void CreateAction(const std::string& actionName, const ActionSubscriber& handler);
        void AddCommand(const ActionCommand& command);
        void RemoveAction(const std::string& actionName);
        void RemoveCommand(const ActionCommand& command);

        void CreateAxis(const std::string& axisName, const AxisSubscriber& handler);
        void AddCommand(const AxisCommand& command);
        void RemoveAxis(const std::string& axisName);
        void RemoveCommand(const AxisCommand& command);

        uint64 GetActionNumber();
        uint64 GetAxisNumber();
        uint64 GetCommandNumber(bool action, const std::string& actionName);
        uint64 GetCommandNumber(bool action);

        float32 CalibrerAxis = 100.0f;

        void private__update__axis();

    private:

        Timer m_Clock;

        /* Event Binding */
        AxisManager m_AxisManager;
        ActionManager m_ActionManager;

        maths::Vector2i m_MousePosition;
        maths::Vector2i m_MouseDelta;
        std::unordered_map<Mouse::Button, float32> m_MouseAxis;
        std::unordered_map<Mouse::Button, ButtonState::Code> m_Mouse;
        std::unordered_map<Mouse::Button, bool> m_MouseDown;
        std::unordered_map<Mouse::Button, bool> m_MouseUp;

        std::unordered_map<Keyboard::Code, float32> m_KeyAxis;
        std::unordered_map<Keyboard::Code, ButtonState::Code> m_Key;
        std::unordered_map<Keyboard::Code, bool> m_KeyDown;
        std::unordered_map<Keyboard::Code, bool> m_KeyUp;

        std::unordered_map<GenericInput::Axis, float32> m_GenericsAxis;
        std::unordered_map<GenericInput::Hat, float32> m_GenericsHat;
        std::unordered_map<GenericInput::Button, float32> m_GenericsButtonAxis;
        std::unordered_map<GenericInput::Button, ButtonState::Code> m_GenericsButton;
        std::unordered_map<GenericInput::Button, bool> m_GenericsButtonDown;
        std::unordered_map<GenericInput::Button, bool> m_GenericsButtonUp;

        std::unordered_map<Gamepad::Axis, float32> m_GamepadAxis;
        std::unordered_map<Gamepad::Button, float32> m_GamepadButtonAxis;
        std::unordered_map<Gamepad::Button, ButtonState::Code> m_GamepadButton;
        std::unordered_map<Gamepad::Button, bool> m_GamepadButtonDown;
        std::unordered_map<Gamepad::Button, bool> m_GamepadButtonUp;


        static bool s_initialize;

        InputManager();

        void OnEvent(Event& event);
        bool OnKeyboardEvent(KeyboardEvent& event);
        bool OnMouseInputEvent(MouseInputEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnGenericInputEvent(GenericInputEvent& event);
        bool OnGenericAxisEvent(GenericAxisEvent& event);
        bool OnGenericHatEvent(GenericHatEvent& event);
        bool OnGamepadInputEvent(GamepadInputEvent& event);
        bool OnGamepadAxisEvent(GamepadAxisEvent& event);
    };

    #define Input       InputManager::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_INPUT_MANAGER_H__