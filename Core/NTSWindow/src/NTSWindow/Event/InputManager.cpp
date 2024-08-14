//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 5:37:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "InputManager.h"
#include <NTSWindow/Core/NkentseuLogger.h>

#include "EventFilter.h"
#include "EventBroker.h"

namespace nkentseu {
    using namespace maths;

    bool InputManager::s_initialize = false;

    InputManager& InputManager::Instance() {
        static InputManager input_manager;
        return input_manager;
    }

    Vector2i InputManager::MousePosition() { return m_MousePosition; }
    int32 InputManager::MouseX() { return m_MousePosition.x; }
    int32 InputManager::MouseY() { return m_MousePosition.y; }

    Vector2i InputManager::MouseDelta()
    {
        return m_MouseDelta;
    }

    int32 InputManager::MouseXDelta()
    {
        return m_MouseDelta.x;
    }

    int32 InputManager::MouseYDelta()
    {
        return m_MouseDelta.y;
    }

    ButtonState::Code InputManager::MouseButton(Mouse::Button button) {
        if (Mouse::Buttons::IsButton(button)) return m_Mouse[button];
        return ButtonState::NotDefine;
    }

    ButtonState::Code InputManager::MouseButton(const std::string& button) {
        Mouse::Button b = Mouse::Buttons::FromString(button);
        return MouseButton(b);
    }

    bool InputManager::IsMouseDown(Mouse::Button button) {
        if (Mouse::Buttons::IsButton(button)) return m_MouseDown[button];
        return false;
    }

    bool InputManager::IsMouseDown(const std::string& button) {
        Mouse::Button b = Mouse::Buttons::FromString(button);
        return IsMouseDown(b);
    }

    bool InputManager::IsMouseUp(Mouse::Button button) {
        if (Mouse::Buttons::IsButton(button)) return m_MouseUp[button];
        return false;
    }

    bool InputManager::IsMouseUp(const std::string& button) {
        Mouse::Button b = Mouse::Buttons::FromString(button);
        return IsMouseUp(b);
    }

    float32 InputManager::MouseAxis(Mouse::Button button) {
        if (Mouse::Buttons::IsButton(button)) return m_MouseAxis[button];
        return 0.0f;
    }

    float32 InputManager::MouseAxis(const std::string& button) {
        Mouse::Button b = Mouse::Buttons::FromString(button);
        return MouseAxis(b);
    }

    /* Keyboard */
    bool InputManager::IsKeyDown(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) return m_KeyDown[keycode];
        return false;
    }
    bool InputManager::IsKeyDown(const std::string& keycode) {
        Keyboard::Keycode b = Keyboard::GetKeycode(keycode);
        return IsKeyDown(b);
    }

    bool InputManager::IsKeyUp(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) return m_KeyUp[keycode];
        return false;
    }
    bool InputManager::IsKeyUp(const std::string& keycode) {
        Keyboard::Keycode b = Keyboard::GetKeycode(keycode);
        return IsKeyUp(b);
    }

    ButtonState::Code InputManager::Key(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) return m_Key[keycode];
        return ButtonState::NotDefine;
    }

    ButtonState::Code InputManager::Key(const std::string& keycode) {
        Keyboard::Keycode b = Keyboard::GetKeycode(keycode);
        return Key(b);
    }

    float32 InputManager::KeyAxis(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) {
            return m_KeyAxis[keycode];
        }
        return 0.0f;
    }

    float32 InputManager::KeyAxis(const std::string& keycode) {
        Keyboard::Keycode b = Keyboard::GetKeycode(keycode);
        return KeyAxis(b);
    }

    bool InputManager::IsGenericInputDown(GenericInput::Button buttoncode) {
        if (GenericInput::IsBoutton(buttoncode)) return m_GenericsButtonDown[buttoncode];
        return false;
    }

    bool InputManager::IsGenericInputDown(const std::string& buttoncode) {
        GenericInput::Button b = GenericInput::GetButton(buttoncode);
        return IsGenericInputDown(b);
    }

    bool InputManager::IsGenericInputUp(GenericInput::Button buttoncode) {
        if (GenericInput::IsBoutton(buttoncode)) return m_GenericsButtonUp[buttoncode];
        return false;
    }

    bool InputManager::IsGenericInputUp(const std::string& buttoncode) {
        GenericInput::Button b = GenericInput::GetButton(buttoncode);
        return IsGenericInputUp(b);
    }

    ButtonState::Code InputManager::GenericInput(GenericInput::Button buttoncode) {
        if (GenericInput::IsBoutton(buttoncode)) return m_GenericsButton[buttoncode];
        return ButtonState::NotDefine;
    }

    ButtonState::Code InputManager::GenericInput(const std::string& buttoncode) {
        GenericInput::Button b = GenericInput::GetButton(buttoncode);
        return GenericInput(b);
    }

    inline float32 InputManager::GenericInputButtonAxis(GenericInput::Button buttoncode) {
        if (GenericInput::IsBoutton(buttoncode)) {
            return m_GenericsButtonAxis[buttoncode];
        }
        return 0.0f;
    }

    inline float32 InputManager::GenericInputButtonAxis(const std::string& buttoncode) {
        GenericInput::Button b = Keyboard::GetKeycode(buttoncode);
        return GenericInputButtonAxis(b);
    }

    inline float32 InputManager::GenericInputAxis(GenericInput::Axis axis) {
        if (GenericInput::IsAxis(axis)) {
            return m_GenericsAxis[axis];
        }
        return 0.0f;
    }

    inline float32 InputManager::GenericInputAxis(const std::string& axis) {
        GenericInput::Axis b = GenericInput::GetAxis(axis);
        return GenericInputAxis(b);
    }

    inline uint32 InputManager::GenericInputHat(GenericInput::Hat hate) {
        if (GenericInput::IsHate(hate)) {
            return m_GenericsHat[hate];
        }
        return 0;
    }

    inline uint32 InputManager::GenericInputHat(const std::string& hate) {
        GenericInput::Hat b = GenericInput::GetHat(hate);
        return GenericInputHat(b);
    }

    void InputManager::CreateAction(const std::string& actionName, const ActionSubscriber& handler)
    {
        m_ActionManager.CreateAction(actionName, handler);
    }

    void InputManager::AddCommand(const ActionCommand& command)
    {
        m_ActionManager.AddCommand(command);
    }

    void InputManager::RemoveAction(const std::string& actionName)
    {
        m_ActionManager.RemoveAction(actionName);
    }

    void InputManager::RemoveCommand(const ActionCommand& command)
    {
        m_ActionManager.RemoveCommand(command);
    }

    void InputManager::CreateAxis(const std::string& axisName, const AxisSubscriber& handler)
    {
        m_AxisManager.CreateAxis(axisName, handler);
    }

    void InputManager::AddCommand(const AxisCommand& command)
    {
        m_AxisManager.AddCommand(command);
    }

    void InputManager::RemoveAxis(const std::string& axisName)
    {
        m_AxisManager.RemoveAxis(axisName);
    }

    void InputManager::RemoveCommand(const AxisCommand& command)
    {
        m_AxisManager.RemoveCommand(command);
    }

    void InputManager::private__update__axis() {
        bool ctrl = IsKeyDown(Keyboard::ControlLeft) || IsKeyDown(Keyboard::ControlRight);
        bool alt = IsKeyDown(Keyboard::AltLeft) || IsKeyDown(Keyboard::AltRight);
        bool shift = IsKeyDown(Keyboard::ShiftLeft) || IsKeyDown(Keyboard::ShiftRight);
        bool super = IsKeyDown(Keyboard::MetaLeft) || IsKeyDown(Keyboard::MetaRight);

        ModifierState state(ctrl, alt, shift, super);

        float32 elapse = m_Clock.Elapsed().seconds * CalibrerAxis;

        UPDATE_AXIS(m_MouseAxis, m_MouseDown, m_MouseUp, elapse);
        UPDATE_AXIS(m_KeyAxis, m_KeyDown, m_KeyUp, elapse);
        UPDATE_AXIS(m_GenericsButtonAxis, m_GenericsButtonDown, m_GenericsButtonUp, elapse);

        m_AxisManager.UpdateAxis([this](EventType::Code type, int64 code) {
            if (type == EventType::KeyboardInput) return m_KeyAxis[code];
            if (type == EventType::MouseInput) return m_MouseAxis[code];
            if (type == EventType::GenericInput) return m_GenericsButtonAxis[code];
            if (type == EventType::GamepadInput) return m_GamepadAxis[code];
            return 0.0f;
        });

        m_Clock.Reset();
    }

    InputManager::InputManager() {
        if (!s_initialize) {
            s_initialize = true;
            EventTraker.AddObserver(REGISTER_CLIENT_EVENT(InputManager::OnEvent));
            m_Clock.Reset();

            for (uint64 i = Keyboard::FirstCode; i <= Keyboard::LastCode; i++) {
                if (Keyboard::IsKeyCode(i)) {
                    m_KeyAxis[i] = 0.0f;
                }
            }

            for (uint64 i = Mouse::Buttons::FirstButton; i <= Mouse::Buttons::LastButton; i++) {
                if (Mouse::Buttons::IsButton(i)) {
                    m_MouseAxis[i] = 0.0f;
                }
            }
        }
    }

    void InputManager::OnEvent(Event& event) {
        m_MouseDelta.x = m_MouseDelta.y = 0;

        EventBroker broker(event);

        broker.Route<KeyboardEvent>(REGISTER_CLIENT_EVENT(InputManager::OnKeyboardEvent));
        broker.Route<MouseInputEvent>(REGISTER_CLIENT_EVENT(InputManager::OnMouseInputEvent));
        broker.Route<MouseMovedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnMouseMovedEvent));
        broker.Route<GenericInputEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericInputEvent));
        broker.Route<GenericAxisEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericAxisEvent));
        broker.Route<GenericHatEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericHatEvent));
        broker.Route<GamepadInputEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGamepadInputEvent));
        broker.Route<GamepadAxisEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGamepadAxisEvent));
    }

    bool InputManager::OnKeyboardEvent(KeyboardEvent& event)
    {
        if (!Keyboard::IsKeyCode(event.GetKeycode())) {
            return true;
        }

        if (!m_KeyDown[event.GetKeycode()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::KeyboardInput, event.GetKeycode(), event.GetModifierState()), ButtonState::Pressed);
        }

        if (!m_KeyUp[event.GetKeycode()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::KeyboardInput, event.GetKeycode(), event.GetModifierState()), ButtonState::Released);
        }

        m_KeyDown[event.GetKeycode()] = event.GetState() == ButtonState::Pressed;
        m_KeyUp[event.GetKeycode()] = event.GetState() == ButtonState::Released;
        m_Key[event.GetKeycode()] = event.GetState();

        if (event.GetState() == ButtonState::Released && m_KeyAxis.find(event.GetKeycode()) == m_KeyAxis.end()) {
            m_KeyAxis[event.GetKeycode()] = 0.0f;
        }
        return true;
    }

    bool InputManager::OnMouseInputEvent(MouseInputEvent& event)
    {
        if (!Mouse::Buttons::IsButton(event.GetButton())) {
            return true;
        }

        if (!m_MouseDown[event.GetButton()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::MouseInput, event.GetButton(), event.GetModifierState()), ButtonState::Pressed);
        }

        if (!m_MouseUp[event.GetButton()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::MouseInput, event.GetButton(), event.GetModifierState()), ButtonState::Released);
        }

        m_MouseDown[event.GetButton()] = event.GetState() == ButtonState::Pressed;
        m_MouseUp[event.GetButton()] = event.GetState() == ButtonState::Released;
        m_Mouse[event.GetButton()] = event.GetState();
        //m_MouseDelta = event.GetPosition() - m_MousePosition;
        m_MousePosition = event.GetPosition();

        if (event.GetState() == ButtonState::Released && m_MouseAxis.find(event.GetButton()) == m_MouseAxis.end()) {
            m_MouseAxis[event.GetButton()] = 0.0f;
        }
        return true;
    }

    bool InputManager::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        m_MouseDelta = event.GetSpeed();
        return true;
    }

    bool InputManager::OnGenericInputEvent(GenericInputEvent& event)
    {
        if (!GenericInput::IsBoutton(event.GetButton())) {
            return true;
        }

        bool ctrl = IsKeyDown(Keyboard::ControlLeft) || IsKeyDown(Keyboard::ControlRight);
        bool alt = IsKeyDown(Keyboard::AltLeft) || IsKeyDown(Keyboard::AltRight);
        bool shift = IsKeyDown(Keyboard::ShiftLeft) || IsKeyDown(Keyboard::ShiftRight);
        bool super = IsKeyDown(Keyboard::MetaLeft) || IsKeyDown(Keyboard::MetaRight);

        if (!m_GenericsButtonDown[event.GetButton()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::GenericInput, event.GetButton(), ModifierState(ctrl, alt, shift, super)), ButtonState::Pressed);
        }

        if (!m_GenericsButtonUp[event.GetButton()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::GenericInput, event.GetButton(), ModifierState(ctrl, alt, shift, super)), ButtonState::Released);
        }

        m_GenericsButtonDown[event.GetButton()] = event.GetState() == ButtonState::Pressed;
        m_GenericsButtonUp[event.GetButton()] = event.GetState() == ButtonState::Released;
        m_GenericsButton[event.GetButton()] = event.GetState();
        return true;
    }

    bool InputManager::OnGenericAxisEvent(GenericAxisEvent& event)
    {
        return true;
    }

    bool InputManager::OnGenericHatEvent(GenericHatEvent& event)
    {
        return true;
    }

    bool InputManager::OnGamepadInputEvent(GamepadInputEvent& event)
    {
        bool ctrl = IsKeyDown(Keyboard::ControlLeft) || IsKeyDown(Keyboard::ControlRight);
        bool alt = IsKeyDown(Keyboard::AltLeft) || IsKeyDown(Keyboard::AltRight);
        bool shift = IsKeyDown(Keyboard::ShiftLeft) || IsKeyDown(Keyboard::ShiftRight);
        bool super = IsKeyDown(Keyboard::MetaLeft) || IsKeyDown(Keyboard::MetaRight);

        if (!m_GamepadButtonDown[event.GetButton()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::GenericInput, event.GetButton(), ModifierState(ctrl, alt, shift, super)), ButtonState::Pressed);
        }

        if (!m_GamepadButtonUp[event.GetButton()]) {
            m_ActionManager.TriggerAction(ActionCode(EventType::GenericInput, event.GetButton(), ModifierState(ctrl, alt, shift, super)), ButtonState::Released);
        }

        m_GamepadButtonDown[event.GetButton()] = event.GetState() == ButtonState::Pressed;
        m_GamepadButtonUp[event.GetButton()] = event.GetState() == ButtonState::Released;
        m_GamepadButton[event.GetButton()] = event.GetState();
        return true;
    }

    bool InputManager::OnGamepadAxisEvent(GamepadAxisEvent& event)
    {
        return true;
    }

    uint64 InputManager::GetActionNumber() {
        return m_ActionManager.GetActionNumber();
    }

    uint64 InputManager::GetAxisNumber() {
        return m_AxisManager.GetAxisNumber();
    }

    uint64 InputManager::GetCommandNumber(bool action, const std::string& actionName) {
        if (action) {
            return m_ActionManager.GetCommandNumber(actionName);
        }
        return m_AxisManager.GetCommandNumber(actionName);
    }

    uint64 InputManager::GetCommandNumber(bool action) {
        if (action) {
            return m_ActionManager.GetCommandNumber();
        }
        return m_AxisManager.GetCommandNumber();
    }
}    // namespace nkentseu