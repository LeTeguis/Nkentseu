//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 11:45:38 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __GEOMETRY2_D_H__
#define __GEOMETRY2_D_H__

#pragma once

#include <System/System.h>
#include "Vertex2D.h"

namespace nkentseu {
    class Geometry2D {
    public:
        Geometry2D();

        const std::vector<Vertex2D>& GetVertices() const;
        const std::vector<uint32>& GetIndices() const;
        bool IsFilled() const;

    protected:
        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;
        bool filled;
    };
}  //  nkentseu

#endif  // __GEOMETRY2_D_H__!