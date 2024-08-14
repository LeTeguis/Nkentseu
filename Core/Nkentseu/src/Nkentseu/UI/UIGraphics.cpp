//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-20 at 06:01:10 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "UIGraphics.h"
#include <Logger/Formatter.h>

// UIGraphics.cpp
namespace nkentseu {
    namespace ui {

        void UIGraphics::AddVertex(const Vertex2D& vertex) {
            vertices.push_back(vertex);

            if (vertices.size() == 1) {
                minBounds = vertex.position;
                maxBounds = vertex.position;
            }
            else {
                minBounds.x = minBounds.x > vertex.position.x ? vertex.position.x : minBounds.x;
                minBounds.y = minBounds.y > vertex.position.y ? vertex.position.y : minBounds.y;
                maxBounds.x = minBounds.x < vertex.position.x ? vertex.position.x : maxBounds.x;
                maxBounds.y = minBounds.y < vertex.position.y ? vertex.position.y : maxBounds.y;
            }
        }

        void UIGraphics::AddIndex(unsigned int index) {
            indices.push_back(index);
        }

        bool UIGraphics::Containe(const maths::Vector2f& point)
        {
            if (!ContaineInBound(point)) return false;

            return false;
        }

        bool UIGraphics::ContaineInBound(const maths::Vector2f& point)
        {
            if (collision) return collision(point);
            if (point.x < minBounds.x || point.x > maxBounds.x || 
                point.y < minBounds.y || point.y > maxBounds.y) return false;
            return true;
        }

        maths::Vector4f UIGraphics::GetGlobalBounds()
        {
            return maths::Vector4f(minBounds.x, minBounds.y, maxBounds.x - minBounds.x, maxBounds.y - minBounds.y);
        }

        maths::Vector2f UIGraphics::GetMinBounds()
        {
            return minBounds;
        }

        maths::Vector2f UIGraphics::GetMaxBounds()
        {
            return maxBounds;
        }

        bool UIGraphicsCollection::Containe(const maths::Vector2f& point)
        {
            if (!ContaineInBound(point)) return false;

            for (auto graph : this->graphics) {
                if (graph.Containe(point)) {
                    return true;
                }
            }

            return false;
        }

        bool UIGraphicsCollection::ContaineInBound(const maths::Vector2f& point)
        {
            if (collision) return collision(point);
            if (point.x < minBounds.x || point.x > maxBounds.x ||
                point.y < minBounds.y || point.y > maxBounds.y) return false;
            return true;
        }

        void UIGraphicsCollection::AddGraphics(UIGraphics graphics) {
            this->graphics.push_back(graphics);

            if (this->graphics.size() == 1) {
                minBounds = graphics.GetMinBounds();
                maxBounds = graphics.GetMaxBounds();
            }
            else {
                minBounds.x = minBounds.x > graphics.GetMinBounds().x ? graphics.GetMinBounds().x : minBounds.x;
                minBounds.y = minBounds.y > graphics.GetMinBounds().y ? graphics.GetMinBounds().y : minBounds.y;
                maxBounds.x = minBounds.x < graphics.GetMaxBounds().x ? graphics.GetMaxBounds().x : maxBounds.x;
                maxBounds.y = minBounds.y < graphics.GetMaxBounds().y ? graphics.GetMaxBounds().y : maxBounds.y;
            }
        }

        /*void UIGraphicsCollection::RemoveGraphics(UIGraphics graphics) {
            auto it = std::find(this->graphics.begin(), this->graphics.end(), &graphics);
            if (it != this->graphics.end()) {
                this->graphics.erase(it);
            }

            minBounds = maths::Vector2f();
            maxBounds = maths::Vector2f();

            if (this->graphics.size() == 0) {
                return;
            }

            minBounds = this->graphics[0].GetMinBounds();
            maxBounds = this->graphics[0].GetMaxBounds();

            if (this->graphics.size() == 1) {
                return;
            }

            for (auto graph : this->graphics) {
                minBounds.x = minBounds.x > graph.GetMinBounds().x ? graph.GetMinBounds().x : minBounds.x;
                minBounds.y = minBounds.y > graph.GetMinBounds().y ? graph.GetMinBounds().y : minBounds.y;
                maxBounds.x = minBounds.x < graph.GetMaxBounds().x ? graph.GetMaxBounds().x : maxBounds.x;
                maxBounds.y = minBounds.y < graph.GetMaxBounds().y ? graph.GetMaxBounds().y : maxBounds.y;
            }
        }*/

        void UIGraphicsCollection::RemoveGraphics(uint32 index)
        {
            if (index >= graphics.size()) return;

            graphics.erase(graphics.begin() + index);

            minBounds = maths::Vector2f();
            maxBounds = maths::Vector2f();

            if (this->graphics.size() == 0) {
                return;
            }

            minBounds = this->graphics[0].GetMinBounds();
            maxBounds = this->graphics[0].GetMaxBounds();

            if (this->graphics.size() == 1) {
                return;
            }

            for (auto graph : this->graphics) {
                minBounds.x = minBounds.x > graph.GetMinBounds().x ? graph.GetMinBounds().x : minBounds.x;
                minBounds.y = minBounds.y > graph.GetMinBounds().y ? graph.GetMinBounds().y : minBounds.y;
                maxBounds.x = minBounds.x < graph.GetMaxBounds().x ? graph.GetMaxBounds().x : maxBounds.x;
                maxBounds.y = minBounds.y < graph.GetMaxBounds().y ? graph.GetMaxBounds().y : maxBounds.y;
            }
        }

        uint32 UIGraphicsCollection::Size()
        {
            return graphics.size();
        }

        void UIGraphicsCollection::ClearGraphics() {
            graphics.clear();
        }

        UIGraphics UIGraphicsCollection::GetGraphics(int32 index) {
            if (index >= 0 && index < graphics.size()) {
                return graphics[index];
            }
            return UIGraphics(); // return an empty Graphics object if index is out of range
        }

        std::vector<UIGraphics> UIGraphicsCollection::GetGraphics() {
            std::vector<UIGraphics> graphicsList;
            for (UIGraphics graphics : graphics) {
                graphicsList.push_back(graphics);
            }
            return graphicsList;
        }

        maths::Vector4f UIGraphicsCollection::GetGlobalBounds()
        {
            return maths::Vector4f(minBounds.x, minBounds.y, maxBounds.x - minBounds.x, maxBounds.y - minBounds.y);
        }

        maths::Vector2f UIGraphicsCollection::GetMinBounds()
        {
            return minBounds;
        }

        maths::Vector2f UIGraphicsCollection::GetMaxBounds()
        {
            return maxBounds;
        }

    }
}  //  nkentseu