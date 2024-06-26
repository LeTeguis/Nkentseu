//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-25 at 03:40:09 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Vertex2D.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    // Default constructor
    Vertex2D::Vertex2D()
        : position(0.0f, 0.0f), color(0.0f, 0.0f, 0.0f, 1.0f), texCord(0.0f, 0.0f) {}

    // Parameterized constructor
    Vertex2D::Vertex2D(const maths::Vector2f& pos, const maths::Vector4f& col, const maths::Vector2f& tex) : position(pos), color(col), texCord(tex) {}

    // Copy constructor
    Vertex2D::Vertex2D(const Vertex2D& other) : position(other.position), color(other.color), texCord(other.texCord) {}

    // Move constructor
    Vertex2D::Vertex2D(Vertex2D&& other) noexcept : position(std::move(other.position)), color(std::move(other.color)), texCord(std::move(other.texCord)) {}

    // Copy assignment operator
    Vertex2D& Vertex2D::operator=(const Vertex2D& other) {
        if (this != &other) {
            position = other.position;
            color = other.color;
            texCord = other.texCord;
        }
        return *this;
    }

    // Move assignment operator
    Vertex2D& Vertex2D::operator=(Vertex2D&& other) noexcept {
        if (this != &other) {
            position = std::move(other.position);
            color = std::move(other.color);
            texCord = std::move(other.texCord);
        }
        return *this;
    }

    // Destructor
    Vertex2D::~Vertex2D() {
        // Cleanup if needed
    }

}  //  nkentseu