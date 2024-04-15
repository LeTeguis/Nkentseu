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

namespace nkentseu {

    Application::Application() : m_Running(false) {
    }

    Application::~Application(){
    }

    bool Application::Initialize()
    {
        WindowProperties windowProperty;
        windowProperty.Title = "Window";
        windowProperty.Size.width = 1000;
        windowProperty.Size.height = 600;

        m_Window = Memory::Alloc<Window>(windowProperty);

        NTSErrorCode error = ErrorMessaging.PopError();

        if (m_Window == nullptr || (error != NTSErrorCode::Error_No && error != NTSErrorCode::Window_StayInWindowMode)) {
            Memory::Reset(m_Window);
            // Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", ErrorMessaging.GetErrorInfos(error));
            Assert.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
            return false;
        }

        EventTrack->AddObserver(EVENT_BIND_HANDLER(Application::OnEvent));

        m_Running = true;
        return m_Running;
    }

    void Application::Run()
    {
        if (m_Window == nullptr) return;
        Log.Trace("Unkeny Engine Run");

        while (m_Running) {
            EventTrack->Pick();
        }

    }

    void Application::OnEvent(Event& event)
    {
        if (event.IsEqual<WindowCloseEvent>()) {
            m_Running = false;
        }
    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
    {
        return false;
    }

    bool Application::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        return false;
    }

}    // namespace nkentseu