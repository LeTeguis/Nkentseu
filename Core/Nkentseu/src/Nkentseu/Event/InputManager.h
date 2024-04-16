//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 5:37:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INPUTMANAGER_H__
#define __NKENTSEU_INPUTMANAGER_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector2.h>
#include <Nkentseu/Event/ModifierState.h>

#include "EventObservable.h"
#include "EventBroker.h"
#include "InputController.h"

#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "GenericInputEvent.h"

#include "System/Epoch/Timer.h"*


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
                                                }

    class NKENTSEU_API InputManager
    {

    public:
        static InputManager& Instance() {
            static InputManager input;
            return input;
        }

        /* Moouse */
        inline Vector2i MousePosition();
        inline int32 MouseX();
        inline int32 MouseY();

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

        /* Keyboard */
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

        /* Event Binding */
        AxisManager axisManager;
        ActionManager actionManager;

        float32 CalibrerAxis = 10.0f;

        void private__update__axis();

    private:

        Timer m_Clock;

        Vector2i m_MousePosition;
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


        static bool Initialize;

        InputManager();

        void OnEvent(Event& event);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);

        bool OnGenericsButtonPressedEvent(GenericInputButtonPressedEvent& e);
        bool OnGenericsButtonReleasedEvent(GenericInputButtonReleasedEvent& e);
        bool OnGenericsAxisEvent(GenericInputAxisEvent& e);
        //bool OnGenericsHatEvent(GenericInputHatEvent& e);
    };

    #define Input       InputManager::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_INPUTMANAGER_H__