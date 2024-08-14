//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 06:41:44 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "UIElement.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    namespace ui {
        UIElement::UIElement()
            : position(0, 0)
            , size(0, 0)
            , graphics()
        {
        }

        UIElement::~UIElement() {
        }

        void UIElement::AddGraphics(const UIGraphics& graphics) {
            this->graphics.AddGraphics(graphics);

            this->size = this->graphics.GetMaxBounds() - this->graphics.GetMinBounds();
        }

        /*void UIElement::RemoveGraphics(const UIGraphics& graphics) {
            this->graphics.RemoveGraphics(graphics);

            this->size = this->graphics.GetMaxBounds() - this->graphics.GetMinBounds();
        }*/

        void UIElement::RemoveGraphics(uint32 index)
        {
            graphics.RemoveGraphics(index);
        }

        void UIElement::ClearGraphics() {
            this->graphics.ClearGraphics();
            this->size = maths::Vector2f(1.0f);
        }

        const UIGraphicsCollection& UIElement::GetGraphics() const {
            return this->graphics;
        }

        UIGraphicsCollection& UIElement::GetGraphics() {
            return this->graphics;
        }

        void UIElement::SetPosition(const maths::Vector2f& position) {
            this->position = position;
        }

        void UIElement::SetSize(const maths::Vector2f& size) {
            scale = size / this->size;
        }

        maths::Vector2f UIElement::GetPosition() const {
            return this->position;
        }

        maths::Vector2f UIElement::GetSize() const {
            return this->size * scale;
        }

        void UIElement::SetVisible(bool visible)
        {
            this->visible = visible;
        }

        bool UIElement::IsVisible() const
        {
            return visible;
        }

        void UIElement::SetRotation(float32 rotation)
        {
            this->rotation = rotation;
        }

        float32 UIElement::GetRotation() const
        {
            return rotation;
        }

        void UIElement::SetScale(const maths::Vector2f& scale)
        {
            this->scale = scale;
        }

        maths::Vector2f UIElement::GetScale() const
        {
            return scale;
        }

    }
}  //  nkentseu