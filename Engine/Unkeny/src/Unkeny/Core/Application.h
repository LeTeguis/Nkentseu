//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/7/2024 at 5:12:05 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_APPLICATION_H__
#define __NKENTSEU_APPLICATION_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"
#include "System/Definitions/Memory.h"

#include "Nkentseu/Event/EventCategory.h"
#include "Nkentseu/Core/Events.h"
#include "Nkentseu/Event/InputCode.h"

#include <vector>
#include <string>

namespace nkentseu {
    class Window;

    class NKENTSEU_API Application
    {
    public:
        Application();
        ~Application();


        virtual bool Initialize();
        virtual void Run();

    private:
        virtual void OnEvent(class Event& event);
        virtual bool OnWindowStatusEvent(class WindowStatusEvent& event);
        virtual bool OnKeyboardEvent(class KeyboardEvent& event);

        void Saut(const std::string& name, const ActionCode& actionCode, bool pressed, bool released);
        void Course(const std::string& name, const AxisCode& axisCode, float32 value);
    private:
        Memory::Shared<class Window> m_Window;
        bool m_Running;
    };

    Memory::Shared<Application> MainApplication(const ARGV& argv);
} // namespace nkentseu

#endif    // __NKENTSEU_APPLICATION_H__