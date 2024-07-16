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

#include <Nkentseu/Graphics/VertexArray.h>

#include <vector>
#include <string>

namespace nkentseu {
    class Window;
    class Context;
    class Renderer;
    class RenderWindow;
    class Event;

    class NKENTSEU_API Application
    {
    public:
        Application();
        ~Application();


        virtual bool Initialize();
        virtual void Run();

    private:
        virtual void OnEvent(Event& event);
        virtual bool OnWindowStatusEvent(class WindowStatusEvent& event);
        virtual bool OnKeyboardEvent(class KeyboardEvent& event);
        virtual bool OnWindowResizedEvent(class WindowResizedEvent& event);
        virtual bool OnWindowMovedEvent(class WindowMovedEvent& event);
        virtual bool OnMouseInputEvent(class MouseInputEvent& event);
        virtual bool OnMouseMoved(class MouseMovedEvent& event);
        virtual bool OnMouseScroll(class MouseWheelEvent& event);

        void Saut(const std::string& name, const ActionCode& actionCode, bool pressed, bool released);
        void Course(const std::string& name, const AxisCode& axisCode, float32 value);
    private:
        Memory::Shared<Window> m_Window;
        Memory::Shared<Context> m_Context;
        Memory::Shared<Renderer> m_Renderer;
        Memory::Shared<RenderWindow> m_RenderWindow;
        bool m_Running;

        PolygonModeType m_PolygonMode = PolygonModeType::Enum::Fill;
    };

    Memory::Shared<Application> MainApplication(const ARGV& argv);
} // namespace nkentseu

#endif    // __NKENTSEU_APPLICATION_H__