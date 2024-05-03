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
        windowProperty.title = "Unkeny Engine";
        windowProperty.size.width = 1000;
        windowProperty.size.height = 600;

        Log.Debug("Size = {0}", windowProperty.size);

        m_Window = Memory::Alloc<Window>(windowProperty);
        nkentseu::Log.Debug("Unkeny Engine");
        NTSErrorCode error = ErrorMessaging.PopError();

        if (m_Window == nullptr || (error != NTSErrorCode::NoError && error != NTSErrorCode::Window_StayInWindowMode)) {
            Memory::Reset(m_Window);
            // Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", ErrorMessaging.GetErrorInfos(error));
            Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
            return false;
        }
        
        EventTrack.AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));
        
        Input.CreateAction("Saut", REGISTER_ACTION_SUBSCRIBER(Application::Saut));
        Input.AddCommand(ActionCommand("Saut", ActionCode(EventType::KeyboardInput, Keyboard::Space)));

        Input.CreateAxis("Course", REGISTER_AXIS_SUBSCRIBER(Application::Course));
        Input.AddCommand(AxisCommand("Course", AxisCode(EventType::KeyboardInput, Keyboard::Up)));

        m_Running = true;
        return m_Running;
    }

    void Application::Run()
    {
        if (m_Window == nullptr) return;

        while (m_Running) {
            EventTrack.Pick();
        }

    }

    void Application::OnEvent(Event& event)
    {
        EventBroker broker(event);

        broker.Route<WindowStatusEvent>(REGISTER_CLIENT_EVENT(Application::OnWindowStatusEvent));
        broker.Route<KeyboardEvent>(REGISTER_CLIENT_EVENT(Application::OnKeyboardEvent));
    }

    bool Application::OnWindowStatusEvent(WindowStatusEvent& event)
    {
        if (event.GetState() == WindowState::Closed) {
            m_Running = false;
            return m_Running;
        }
        return true;
    }

    bool Application::OnKeyboardEvent(KeyboardEvent& event)
    {
        //Log.Debug("{0}", e);
        if (event.GetKeycode() == Keyboard::Escape){
            m_Running = false;
        }
        //Log.Debug("Keycode = {0}, Scancode = {1}", Keyboard::GetKeycode(e.GetKey()), Keyboard::GetScancode(e.GetScan()));
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