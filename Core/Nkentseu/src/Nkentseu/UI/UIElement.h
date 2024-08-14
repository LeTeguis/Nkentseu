//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 06:41:44 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __U_I_ELEMENT_H__
#define __U_I_ELEMENT_H__

#pragma once

#include <System/System.h>
#include "UIGraphics.h"
#include "Themes.h"
#include "Nkentseu/Core/Events.h"

namespace nkentseu {
    namespace ui {

        class NKENTSEU_API UIHandleEvent {
        public:
            virtual bool OnEventHandle(Event& event) { return false; }
            virtual bool OnMousePressed(MouseInputEvent& event) { return false; }
            virtual bool OnMouseReleased(MouseInputEvent& event) { return false; }
            virtual bool OnMouseMoved(MouseMovedEvent& event) { return false; }
            virtual bool OnMouseScrolled(MouseWheelEvent& event) { return false; }
            virtual bool OnKeyboardPressed(KeyboardEvent& event) { return false; }
            virtual bool OnKeyboardReleased(KeyboardEvent& event) { return false; }
        };

        class NKENTSEU_API UIElement : public UIHandleEvent {
        public:
            UIElement();
            virtual ~UIElement();

            virtual void SetTheme(Memory::Shared<UITheme> theme) = 0;
            virtual void SetSkin(Memory::Shared<UISkin> skin) = 0;

            void SetTheme(Memory::Shared<UIThemeProperty> themeProperty) { this->themeProperty = themeProperty; }
            void SetSkin(Memory::Shared<UISkinProperty> skinProperty) { this->skinProperty = skinProperty; }

            virtual void Update(float32 deltaTime) = 0;

            void AddGraphics(const UIGraphics& graphics);
            //void RemoveGraphics(const UIGraphics& graphics);
            void RemoveGraphics(uint32 index);
            void ClearGraphics();

            const UIGraphicsCollection& GetGraphics() const;
            UIGraphicsCollection& GetGraphics();

            void SetPosition(const maths::Vector2f& position);
            maths::Vector2f GetPosition() const;

            void SetSize(const maths::Vector2f& size);
            maths::Vector2f GetSize() const;

            void SetRotation(float32 rotation);
            float32 GetRotation() const;

            void SetScale(const maths::Vector2f& scale);
            maths::Vector2f GetScale() const;

            void SetVisible(bool visible);
            bool IsVisible() const;

        protected:
            Memory::Shared<UIThemeProperty> themeProperty = nullptr;
            Memory::Shared<UISkinProperty> skinProperty = nullptr;

            maths::Vector2f position;
            maths::Vector2f size;
            UIGraphicsCollection graphics;
            bool visible;
            float32 rotation;
            maths::Vector2f scale = maths::Vector2f(1.0f);

            maths::Vector4f margin;
            maths::Vector4f padding;
        };
    }
}  //  nkentseu

#endif  // __U_I_ELEMENT_H__!