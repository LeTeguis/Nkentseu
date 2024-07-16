//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 11:45:38 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Geometry2D.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    Geometry2D::Geometry2D() : vertices({}), indices({}), filled(false) {}

    const std::vector<Vertex2D>& Geometry2D::GetVertices() const { return vertices; }
    const std::vector<uint32>& Geometry2D::GetIndices() const { return indices; }
    bool Geometry2D::IsFilled() const { return filled; }

}  //  nkentseu