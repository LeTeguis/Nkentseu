//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:12:05 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "Application.h"

#include <Nkentseu/Core/Window.h>

#include "Log.h"
#include <Ntsm/Ntsm.h>

#include <Nkentseu/Core/Events.h>
#include <Nkentseu/Graphics/Color.h>
#include <Nkentseu/Event/EventBroker.h>

#include <Nkentseu/Event/InputManager.h>

namespace nkentseu {

    Application::Application() : m_Running(false) {
    }

    Application::~Application(){
    }

    bool Application::Initialize()
    {
        WindowProperties windowProperty;
        windowProperty.title = "Window";
        windowProperty.size.width = 1000;
        windowProperty.size.height = 600;

        m_Window = Memory::Alloc<Window>(windowProperty);
        nkentseu::Log.Debug("Unkeny Engine");
        NTSErrorCode error = ErrorMessaging.PopError();

        if (m_Window == nullptr || (error != NTSErrorCode::NoError && error != NTSErrorCode::Window_StayInWindowMode)) {
            Memory::Reset(m_Window);
            // Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", ErrorMessaging.GetErrorInfos(error));
            Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
            return false;
        }
        
        EventTrack->AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));
        
        Input.CreateAction("Saut", REGISTER_ACTION_SUBSCRIBER(Application::Saut));
        Input.AddCommand(ActionCommand("Saut", ActionCode(EventCategory::Keyboard, Keyboard::Space)));

        Input.CreateAxis("Course", REGISTER_AXIS_SUBSCRIBER(Application::Course));
        Input.AddCommand(AxisCommand("Course", AxisCode(EventCategory::Keyboard, Keyboard::Up)));

        m_Running = true;
        return m_Running;
    }

    void Application::Run()
    {
        Log.Trace("Unkeny Engine in Run methode");
        if (m_Window == nullptr) return;
        Log.Trace("Unkeny Engine Run {0}", &Input);

        while (m_Running) {
            EventTrack->Pick();

            if (Input.IsKeyDown(Keyboard::Up)) {
                // Log.Debug("up pressed");
            }
        }

    }

    void Application::OnEvent(Event& event)
    {
        EventBroker broker(event);

        broker.Route<WindowCloseEvent>(REGISTER_CLIENT_EVENT(Application::OnWindowCloseEvent));
        broker.Route<KeyPressedEvent>(REGISTER_CLIENT_EVENT(Application::OnKeyPressedEvent));
        broker.Route<GenericInputConnectedEvent>(REGISTER_CLIENT_EVENT(Application::OnGamepadConnectedEvent));
        broker.Route<GenericInputDisconnectedEvent>(REGISTER_CLIENT_EVENT(Application::OnGamepadDisconnectedEvent));
        broker.Route<GenericInputButtonPressedEvent>(REGISTER_CLIENT_EVENT(Application::OnGamepadButtonPressedEvent));
        broker.Route<GenericInputButtonReleasedEvent>(REGISTER_CLIENT_EVENT(Application::OnGamepadButtonReleasedEvent));
        broker.Route<GenericInputAxisEvent>(REGISTER_CLIENT_EVENT(Application::OnGamepadAxisEvent));
    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
    {
        m_Running = false;
        Log.Debug("close");
        return m_Running;
    }

    bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        //Log.Debug("Keycode = {0}, Scancode = {1}", Keyboard::GetKeycode(e.GetKey()), Keyboard::GetScancode(e.GetScan()));
        return false;
    }

    bool Application::OnGamepadConnectedEvent(GenericInputConnectedEvent& e)
    {
        //Log.Debug("Connexion");
        return false;
    }

    bool Application::OnGamepadDisconnectedEvent(GenericInputDisconnectedEvent& e)
    {
        //Log.Debug("Deconnection");
        return false;
    }

    bool Application::OnGamepadButtonPressedEvent(GenericInputButtonPressedEvent& e)
    {
        //Log.Debug("Presse");
        return false;
    }

    bool Application::OnGamepadButtonReleasedEvent(GenericInputButtonReleasedEvent& e)
    {
        //Log.Debug("Release");
        return false;
    }

    bool Application::OnGamepadAxisEvent(GenericInputAxisEvent& e)
    {
        //Log.Debug("Axis");
        return false;
    }

    void Application::Saut(const std::string& name, const ActionCode& actionCode, bool pressed, bool released)
    {
        Log.Debug("Saut");
    }

    void Application::Course(const std::string& name, const AxisCode& axisCode, float32 value)
    {
        if (value != 0)
            Log.Debug("Run {0}", value);
    }

}    // namespace nkentseu