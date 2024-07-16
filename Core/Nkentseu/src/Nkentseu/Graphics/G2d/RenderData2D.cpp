//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 11:50:30 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "RenderData2D.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    RenderData2D::RenderData2D() : renderMode(RenderMode2D::Enum::RenderColor), textureId(0), primitive(RenderPrimitive::Enum::Triangles) {
        renderDataId = nextId++;
    }

    RenderMode2D RenderData2D::GetRenderMode() {
        return renderMode;
    }

    void RenderData2D::SetRenderMode(RenderMode2D renderMode) {
        this->renderMode = renderMode;
    }

    void RenderData2D::SetPrimitive(RenderPrimitive primitive) {
        this->primitive = primitive;
    }

    RenderPrimitive RenderData2D::GetPrimitive() {
        return primitive;
    }

    uint32 RenderData2D::GetId() const {
        return renderDataId;
    }

    void RenderData2D::SetTextureId(uint32 textureId) {
        this->textureId = textureId;
    }

    uint32 RenderData2D::GetTextureId() const {
        return textureId;
    }

    uint32 RenderData2D::nextId = 0;

}  //  nkentseu