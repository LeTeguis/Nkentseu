//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 01:14:49 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __RENDER_COMMAND_H__
#define __RENDER_COMMAND_H__

#pragma once

#include <System/System.h>
#include "RenderData2D.h"

namespace nkentseu {

    struct ClipRegion {
        std::vector<maths::Vector2f> controlPoints;
    };
    
    class NKENTSEU_API RenderCommand {
    public:
        RenderCommand();
        RenderCommand(RenderData2D* data);

        RenderData2D* GetData() const;
        RenderData2D* GetData();
        uint32 GetTextureId() const;
        void SetTextureId(uint32 textureId);

        // Transformations
        maths::matrix4f GetTransform() const;
        void SetTransform(const maths::matrix4f& transform);

        // Clip rectangle
        ClipRegion GetClipRegion() const;
        void SetClipRegion(const ClipRegion& clipRect);
        void AddInClipRegion(const maths::Vector2f& clipRect);

        // Blend mode
        BlendMode2D GetBlendMode() const;
        void SetBlendMode(BlendMode2D blendMode);

        bool Free();
        void AutoFree(bool autofree);

        void AddVertex(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& textCord);
        void AddVertex(const maths::Vector2f& position, const maths::Vector4f& color);
        void AddVertex(const Vertex2D& vertex);
        void AddIndex(uint32 index);

        // Nouvelles méthodes pour la gestion de la bordure
        void SetBorderColor(const maths::Vector4f& color);
        maths::Vector4f GetBorderColor() const;

        void SetBorderWidth(float32 width);
        float32 GetBorderWidth() const;

        bool HasBorder() const;
    private:
        RenderData2D* data;
        maths::matrix4f transform;
        ClipRegion clipRect;
        BlendMode2D blendMode;

        bool isLoad = false;

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        // Nouveaux membres pour la gestion de la bordure
        maths::Vector4f borderColor;
        float borderWidth;
        bool hasBorder;
    };

}  //  nkentseu

#endif  // __RENDER_COMMAND_H__!