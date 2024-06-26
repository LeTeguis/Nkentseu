//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-25 at 03:40:09 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VERTEX2_D_H__
#define __VERTEX2_D_H__

#pragma once

#include <System/System.h>
#include <Ntsm/Matrix/Matrix4.h>

namespace nkentseu {
    
    struct NKENTSEU_API Vertex2D {
        maths::Vector2f position;
        maths::Vector4f color;
        maths::Vector2f texCord;

        // Constructors
        Vertex2D();
        Vertex2D(const maths::Vector2f& pos, const maths::Vector4f& col, const maths::Vector2f& tex);

        // Copy constructor
        Vertex2D(const Vertex2D& other);

        // Move constructor
        Vertex2D(Vertex2D&& other) noexcept;

        // Copy assignment operator
        Vertex2D& operator=(const Vertex2D& other);

        // Move assignment operator
        Vertex2D& operator=(Vertex2D&& other) noexcept;

        // Destructor
        ~Vertex2D();
    };

}  //  nkentseu

#endif  // __VERTEX2_D_H__!