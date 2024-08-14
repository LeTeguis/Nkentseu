//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-21 at 06:37:24 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "UIManager.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Event/EventBroker.h>
#include <Nkentseu/Event/EventFilter.h>

namespace nkentseu {
    
    namespace ui {

        UIManager::UIManager(Memory::Shared<Context> context)
        {
            EventTraker.AddObserver(EVENT_BIND_HANDLER(UIManager::OnEvent));

            renderer = Memory::Alloc<UIRenderer>(context);

            if (renderer) {
                renderer->Initialize();
            }
        }

        void UIManager::Render(Memory::Shared<Renderer> renderer) {
            if (renderer == nullptr) return;

            this->renderer->Prepare(renderer);
            for (auto& element : elements) {
                if (element) {
                    this->renderer->AddGraphicsCollection(element->GetGraphics());
                }
            }
            this->renderer->Present(renderer);
        }

        void UIManager::Update(float32 deltaTime)
        {
            for (auto& element : elements) {
                if (element && element->IsVisible()) {
                    element->Update(deltaTime);
                }
            }
        }

        Memory::Shared<UITheme> UIManager::GetTheme()
        {
            return theme;
        }

        Memory::Shared<UISkin> UIManager::GetSkin()
        {
            return skin;
        }

        void UIManager::SetTheme(Memory::Shared<UITheme> theme)
        {
            this->theme = theme;
        }

        void UIManager::SetSkin(Memory::Shared<UISkin> skin)
        {
            this->skin = skin;
        }

        void UIManager::OnEvent(Event& event)
        {
            EventBroker broker(event);

            broker.Route<KeyboardEvent>(REGISTER_CLIENT_EVENT(UIManager::OnKeyboardEvent));
            broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(UIManager::OnWindowResizedEvent));
            broker.Route<MouseInputEvent>(REGISTER_CLIENT_EVENT(UIManager::OnMouseInputEvent));
            broker.Route<MouseMovedEvent>(REGISTER_CLIENT_EVENT(UIManager::OnMouseMoved));
            broker.Route<MouseWheelEvent>(REGISTER_CLIENT_EVENT(UIManager::OnMouseScroll));
        }

        bool UIManager::OnKeyboardEvent(KeyboardEvent& event)
        {
            for (auto& element : elements) {
                if (event.GetState() == ButtonState::Pressed) {
                    element->OnKeyboardPressed(event);
                } else if (event.GetState() == ButtonState::Released) {
                    element->OnKeyboardReleased(event);
                }
            }
            return false;
        }

        bool UIManager::OnWindowResizedEvent(WindowResizedEvent& event)
        {
            return false;
        }

        bool UIManager::OnMouseInputEvent(MouseInputEvent& event)
        {
            for (auto& element : elements) {
                if (event.GetState() == ButtonState::Pressed) {
                    element->OnMousePressed(event);
                }
                else if (event.GetState() == ButtonState::Released) {
                    element->OnMouseReleased(event);
                }
            }
            return false;
        }

        bool UIManager::OnMouseMoved(MouseMovedEvent& event)
        {
            for (auto& element : elements) {
                element->OnMouseMoved(event);
            }
            return false;
        }

        bool UIManager::OnMouseScroll(MouseWheelEvent& event)
        {
            for (auto& element : elements) {
                element->OnMouseScrolled(event);
            }
            return false;
        }

    }

}  //  nkentseu