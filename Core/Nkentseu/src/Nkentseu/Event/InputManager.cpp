//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/16/2024 at 5:37:18 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InputManager.h"
#include <Nkentseu/Core/NkentseuLogger.h>

namespace nkentseu {
    bool InputManager::Initialize = false;

    Vector2i InputManager::MousePosition() { return m_MousePosition; }
    int32 InputManager::MouseX() { return m_MousePosition.x; }
    int32 InputManager::MouseY() { return m_MousePosition.y; }

    ButtonState::Code InputManager::MouseButton(Mouse::Button button) {
        if (Mouse::IsButton(button)) return m_Mouse[button];
        return ButtonState::None_ev;
    }

    ButtonState::Code InputManager::MouseButton(const std::string& button) {
        Mouse::Button b = Mouse::FromString(button);
        return MouseButton(b);
    }

    bool InputManager::IsMouseDown(Mouse::Button button) {
        if (Mouse::IsButton(button)) return m_MouseDown[button];
        return false;
    }

    bool InputManager::IsMouseDown(const std::string& button) {
        Mouse::Button b = Mouse::FromString(button);
        return IsMouseDown(b);
    }

    bool InputManager::IsMouseUp(Mouse::Button button) {
        if (Mouse::IsButton(button)) return m_MouseUp[button];
        return false;
    }

    bool InputManager::IsMouseUp(const std::string& button) {
        Mouse::Button b = Mouse::FromString(button);
        return IsMouseUp(b);
    }

    float32 InputManager::MouseAxis(Mouse::Button button) {
        if (Mouse::IsButton(button)) return m_MouseAxis[button];
        return 0.0f;
    }

    float32 InputManager::MouseAxis(const std::string& button) {
        Mouse::Button b = Mouse::FromString(button);
        return MouseAxis(b);
    }

    /* Keyboard */
    bool InputManager::IsKeyDown(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) return m_KeyDown[keycode];
        return false;
    }
    bool InputManager::IsKeyDown(const std::string& keycode) {
        Keyboard::KeyCode b = Keyboard::GetKeycode(keycode);
        return IsKeyDown(b);
    }

    bool InputManager::IsKeyUp(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) return m_KeyUp[keycode];
        return false;
    }
    bool InputManager::IsKeyUp(const std::string& keycode) {
        Keyboard::KeyCode b = Keyboard::GetKeycode(keycode);
        return IsKeyUp(b);
    }

    ButtonState::Code InputManager::Key(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) return m_Key[keycode];
        return ButtonState::None_ev;
    }

    ButtonState::Code InputManager::Key(const std::string& keycode) {
        Keyboard::KeyCode b = Keyboard::GetKeycode(keycode);
        return Key(b);
    }

    float32 InputManager::KeyAxis(Keyboard::Code keycode) {
        if (Keyboard::IsKeyCode(keycode)) {
            return m_KeyAxis[keycode];
        }
        return 0.0f;
    }

    float32 InputManager::KeyAxis(const std::string& keycode) {
        Keyboard::KeyCode b = Keyboard::GetKeycode(keycode);
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
        return ButtonState::None_ev;
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

    void InputManager::private__update__axis() {
        float32 elapse = m_Clock.Elapsed() * CalibrerAxis;

        UPDATE_AXIS(m_MouseAxis, m_MouseDown, m_MouseUp, elapse);
        UPDATE_AXIS(m_KeyAxis, m_KeyDown, m_KeyUp, elapse);
        UPDATE_AXIS(m_GenericsButtonAxis, m_GenericsButtonDown, m_GenericsButtonUp, elapse);

        axisManager.UpdateAxis();
        m_Clock.Reset();
    }

    InputManager::InputManager() {
        if (!Initialize) {
            Initialize = true;
            EventTrack->AddObserver(REGISTER_CLIENT_EVENT(InputManager::OnEvent));
            m_Clock.Reset();

            for (uint64 i = Keyboard::FirstCode_ev; i <= Keyboard::LastCode_ev; i++) {
                if (Keyboard::IsKeyCode(i)) {
                    m_KeyAxis[i] = 0.0f;
                }
            }

            for (uint64 i = Mouse::FistButton_ev; i <= Mouse::LastButton_ev; i++) {
                if (Mouse::IsButton(i)) {
                    m_KeyAxis[i] = 0.0f;
                }
            }
        }
    }

    void InputManager::OnEvent(Event& event) {
        EventBroker broker(event);

        broker.Route<KeyReleasedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnKeyReleasedEvent));
        broker.Route<KeyPressedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnKeyPressedEvent));
        broker.Route<MouseButtonPressedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnMouseButtonPressedEvent));
        broker.Route<MouseButtonReleasedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnMouseButtonReleasedEvent));
        broker.Route<MouseMovedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnMouseMovedEvent));
        broker.Route<GenericInputButtonPressedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericsButtonPressedEvent));
        broker.Route<GenericInputButtonReleasedEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericsButtonReleasedEvent));
        broker.Route<GenericInputAxisEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericsAxisEvent));
        //broker.Route<GenericInputHatEvent>(REGISTER_CLIENT_EVENT(InputManager::OnGenericsHatEvent));
    }

    bool InputManager::OnKeyPressedEvent(KeyPressedEvent& e) {
        if (Keyboard::IsKeyCode(e.GetKey())) {
            // Log_nts.Warning();
            Log_nts.Warning("{0}-{1}", Keyboard::GetKeycode(e.GetKey()), (int32)m_KeyDown[e.GetKey()]);
            m_KeyDown[e.GetKey()] = true;
            Log_nts.Warning("{0}-{1}", Keyboard::GetKeycode(e.GetKey()), (int32)m_KeyDown[e.GetKey()]);
            m_KeyUp[e.GetKey()] = false;
            m_Key[e.GetKey()] = ButtonState::Down_ev;
            actionManager.TriggerAction();
        }
        return true;
    }

    bool InputManager::OnKeyReleasedEvent(KeyReleasedEvent& e) {
        if (Keyboard::IsKeyCode(e.GetKey())) {
            m_KeyDown[e.GetKey()] = false;
            m_KeyUp[e.GetKey()] = true;
            m_Key[e.GetKey()] = ButtonState::Up_ev;
            actionManager.TriggerAction();

            if (m_KeyAxis.find(e.GetKey()) == m_KeyAxis.end()) {
                m_KeyAxis[e.GetKey()] = 0.0f;
            }
        }
        return true;
    }

    bool InputManager::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
        if (Mouse::IsButton(e.GetButton())) {
            m_MouseDown[e.GetButton()] = true;
            m_MouseUp[e.GetButton()] = false;
            m_Mouse[e.GetButton()] = ButtonState::Down_ev;

            m_MousePosition = e.GetPosition();
            actionManager.TriggerAction();

            if (m_MouseAxis.find(e.GetButton()) == m_MouseAxis.end()) {
                m_MouseAxis[e.GetButton()] = 0.0f;
            }
        }
        return true;
    }

    bool InputManager::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
        if (Mouse::IsButton(e.GetButton())) {
            m_MouseDown[e.GetButton()] = false;
            m_MouseUp[e.GetButton()] = true;
            m_Mouse[e.GetButton()] = ButtonState::Up_ev;

            m_MousePosition = e.GetPosition();
            actionManager.TriggerAction();
        }
        return true;
    }

    bool InputManager::OnMouseMovedEvent(MouseMovedEvent& e) {
        m_MousePosition = e.GetPosition();
        return true;
    }

    bool InputManager::OnGenericsButtonPressedEvent(GenericInputButtonPressedEvent& e) {
        if (GenericInput::IsBoutton(e.GetButton())) {
            m_GenericsButtonDown[e.GetButton()] = false;
            m_GenericsButtonUp[e.GetButton()] = true;
            m_GenericsButton[e.GetButton()] = ButtonState::Up_ev;

            actionManager.TriggerAction();
        }
        return true;
    }

    bool InputManager::OnGenericsButtonReleasedEvent(GenericInputButtonReleasedEvent& e) {
        if (GenericInput::IsBoutton(e.GetButton())) {
            m_GenericsButtonDown[e.GetButton()] = false;
            m_GenericsButtonUp[e.GetButton()] = true;
            m_GenericsButton[e.GetButton()] = ButtonState::Up_ev;

            actionManager.TriggerAction();
        }
        return true;
    }

    bool InputManager::OnGenericsAxisEvent(GenericInputAxisEvent& e) {
        if (GenericInput::IsAxis(e.GetAxis())) {
            m_GenericsAxis[e.GetAxis()] = e.GetValue();

            actionManager.TriggerAction();
        }
        return true;
    }
}    // namespace nkentseu