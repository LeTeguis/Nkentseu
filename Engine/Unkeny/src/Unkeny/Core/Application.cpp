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

        if (m_Window == nullptr || (error != NTSErrorCode::Error_No && error != NTSErrorCode::Window_StayInWindowMode)) {
            Memory::Reset(m_Window);
            // Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", ErrorMessaging.GetErrorInfos(error));
            Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
            return false;
        }
        #ifdef NKENTSEU_PLATFORM_WINDOWS
        EventTrack->AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));

        Input.actionManager.CreateAction("Saut", REGISTER_ACTION_SUBSCRIBER(Application::Saut));
        Input.actionManager.AddCommand(ActionCommand("Saut", EventCategory::Keyboard_ev, Keyboard::Up_ev));

        Input.axisManager.CreateAxis("Course", REGISTER_AXIS_SUBSCRIBER(Application::Course));
        Input.axisManager.AddCommand(AxisCommand("Course", EventCategory::Keyboard_ev, Keyboard::Down_ev));
        #endif

        m_Running = true;
        return m_Running;
    }

    void Application::Run()
    {

        Log.Trace("Unkeny Engine in Run methode");
        if (m_Window == nullptr) return;
        Log.Trace("Unkeny Engine Run");

        while (m_Running) {
            #ifdef NKENTSEU_PLATFORM_WINDOWS
            EventTrack->Pick();
            #endif
        }

    }

    void Application::OnEvent(Event& event)
    {
        EventBroker broker(event);

        broker.Route<WindowCloseEvent>(REGISTER_CLIENT_EVENT(Application::OnWindowCloseEvent));
        broker.Route<KeyPressedEvent>(REGISTER_CLIENT_EVENT(Application::OnKeyPressedEvent));
    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
    {
        m_Running = false;
        return m_Running;
    }

    bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        return false;
    }

    void Application::Saut(const std::string& name, EventCategory::Code categorie, int64 code, bool pressed, bool released)
    {
        Log.Debug("Saut");
    }

    void Application::Course(const std::string& name, EventCategory::Code categorie, int64 code, float32 value)
    {
        //Log.Debug("Run");
    }

}    // namespace nkentseu