#include <iostream>
#include "Nkentseu/Nkentseu.h"
#include <Nkentseu/Core/Events.h>

#include <Nkentseu/Core/Window.h>

using namespace nkentseu;

nkentseu::int32 Main(const nkentseu::ARGV& argv) {
    WindowProperties windowProperty;
    windowProperty.title = "Window Test";
    windowProperty.size = Vector2u(1000, 600);

    Memory::Shared<nkentseu::Window> window = Memory::Alloc<nkentseu::Window>(windowProperty);

    /*NTSErrorCode error = ErrorMessaging.PopError();

    if (window == nullptr || (error != NTSErrorCode::Error_No && error != NTSErrorCode::Window_StayInWindowMode)) {
        Memory::Reset(window);
        Assert_nts.ATrue(true, "Erreur lord de la creation de la fenetre : Error ({0})", (bool32)error);
        return false;
    }
    else {
        Log_nts.Trace("creation reussi");
    }

    bool running = true;

    while (running) {
        Event* event;
        while (EventTrack->Pick(&event)) {
            if (event->IsEqual<WindowCloseEvent>()) {
                running = false;
            }
        }
    }*/

    // Memory::Reset(window);
    return 0;
}