#include <iostream>
#include "Nkentseu/Nkentseu.h"
#include <Nkentseu/Core/Events.h>

#include <Nkentseu/Core/Window.h>
#include <Nkentseu/Event/InputManager.h>

using namespace nkentseu;
using namespace nkentseu::maths;

nkentseu::int32 Main(const nkentseu::ARGV& argv) {
    WindowProperties windowProperty;
    windowProperty.title = "Window Test";
    windowProperty.size = Vector2u(1000, 600);

    //Memory::Shared<nkentseu::Window> window = Memory::Alloc<nkentseu::Window>(windowProperty);
    Memory::Shared<nkentseu::Window> window = Window::Create(windowProperty);

    NTSErrorCode error = ErrorMessaging.PopError();

    if (window == nullptr || (error != NTSErrorCode::NoError && error != NTSErrorCode::Window_StayInWindowMode)) {
        Memory::Reset(window);
        Assert_nts.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
        return false;
    }
    else {
        Log_nts.Trace("creation reussi");
    }

    bool running = true;

    while (running) {
        while (EventTraker.ProcessQueue()) {
            Log_nts.Debug("{0}", EventTraker.Front());
            if (EventTraker.Front().IsA<WindowStatusEvent>()) {
                WindowStatusEvent& status = EventTraker.Front().Cast<WindowStatusEvent>();

                if (status.GetState() == WindowState::Closed) {
                    running = false;
                }
            }
        }
        
        if (Input.IsKeyDown(Keyboard::Up)) {

        }
    }

    if (window != nullptr) {
        window->Close();
    }
    Log_nts.Debug("finish");
    // Memory::Reset(window);
    return 0;
}