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
        void OnEvent(class Event& event);
        bool OnWindowCloseEvent(class WindowCloseEvent& e);
        bool OnKeyPressedEvent(class KeyPressedEvent& e);

        void Saut(const std::string& name, EventCategory::Code categorie, int64 code, bool pressed, bool released);
        void Course(const std::string& name, EventCategory::Code categorie, int64 code, float32 value);
    private:
        Memory::Shared<class Window> m_Window;
        bool m_Running;
    };

    Memory::Shared<Application> MainApplication(const ARGV& argv);
} // namespace nkentseu

#endif    // __NKENTSEU_APPLICATION_H__