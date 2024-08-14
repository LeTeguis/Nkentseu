//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-21 at 06:37:24 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __U_I_MANAGER_H__
#define __U_I_MANAGER_H__

#pragma once

#include <System/System.h>
#include "UIElement.h"
#include "UIRenderer.h"
#include "Nkentseu/Core/Events.h"
#include "Themes.h"

namespace nkentseu {
    
    namespace ui {
        class NKENTSEU_API UIManager {
        public:
            UIManager(Memory::Shared<Context> context);

            template <typename T, typename... Args>
            Memory::Shared<T> AddUI(Args&&... args) {
                if (std::is_base_of<UIElement, T>::value) {
                    auto element = Memory::Alloc<T>(std::forward<Args>(args)...);

                    if (element == nullptr) return nullptr;

                    element->SetTheme(theme);
                    element->SetSkin(skin);

                    elements.push_back(element);
                    return element;
                }
                return nullptr;
            }

            void Render(Memory::Shared<Renderer> renderer);
            void Update(float32 deltaTime);

            Memory::Shared<UITheme> GetTheme();
            Memory::Shared<UISkin> GetSkin();

            void SetTheme(Memory::Shared<UITheme> theme);
            void SetSkin(Memory::Shared<UISkin> skin);
        private:
            std::vector<Memory::Shared<UIElement>> elements;
            Memory::Shared<UIRenderer> renderer;
            Memory::Shared<UITheme> theme;
            Memory::Shared<UISkin> skin;

            virtual void OnEvent(Event& event);
            virtual bool OnKeyboardEvent(KeyboardEvent& event);
            virtual bool OnWindowResizedEvent(WindowResizedEvent& event);
            virtual bool OnMouseInputEvent(MouseInputEvent& event);
            virtual bool OnMouseMoved(MouseMovedEvent& event);
            virtual bool OnMouseScroll(MouseWheelEvent& event);
        };
    }
}  //  nkentseu

#endif  // __U_I_MANAGER_H__!