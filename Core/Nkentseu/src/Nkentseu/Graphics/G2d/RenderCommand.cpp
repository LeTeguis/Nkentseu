//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 01:14:49 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "RenderCommand.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    RenderCommand::RenderCommand()
        : data(nullptr), transform(maths::matrix4f()), clipRect(), blendMode(BlendMode2D::Enum::AlphaBlending), isLoad(true){
        data = new RenderData2D();
    }

    RenderCommand::RenderCommand(RenderData2D* data)
        : data(data), transform(maths::matrix4f()), clipRect(), blendMode(BlendMode2D::Enum::AlphaBlending), isLoad(false){}

    RenderData2D* RenderCommand::GetData() const {
        return data;
    }

    RenderData2D* RenderCommand::GetData()
    {
        return data;
    }

    uint32 RenderCommand::GetTextureId() const {
        if (data == nullptr) return 0;
        return data->GetTextureId();
    }

    void RenderCommand::SetTextureId(uint32 textureId)
    {
        if (data == nullptr) return;
        data->SetTextureId(textureId);
    }

    maths::matrix4f RenderCommand::GetTransform() const {
        return transform;
    }

    void RenderCommand::SetTransform(const maths::matrix4f& transform) {
        this->transform = transform;
    }

    ClipRegion RenderCommand::GetClipRegion() const {
        return clipRect;
    }

    void RenderCommand::SetClipRegion(const ClipRegion& clipRect) {
        this->clipRect = clipRect;
    }

    void RenderCommand::AddInClipRegion(const maths::Vector2f& clipRect) {
        this->clipRect.controlPoints.push_back(clipRect);
    }

    BlendMode2D RenderCommand::GetBlendMode() const {
        return blendMode;
    }

    void RenderCommand::SetBlendMode(BlendMode2D blendMode) {
        this->blendMode = blendMode;
    }

    bool RenderCommand::Free()
    {
        if (isLoad && data != nullptr) {
            delete data;
            data = nullptr;
            true;
        }
        return false;
    }

    void RenderCommand::AutoFree(bool autofree)
    {
        isLoad = autofree;
    }

    void RenderCommand::AddVertex(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& textCord) {
        Vertex2D vertex;
        vertex.position = position;
        vertex.color = color;
        vertex.texCord = textCord;
        vertices.push_back(vertex);
    }

    void RenderCommand::AddVertex(const maths::Vector2f& position, const maths::Vector4f& color)
    {
        AddVertex(position, color, maths::Vector2f());
    }

    void RenderCommand::AddVertex(const Vertex2D& vertex) {
        vertices.push_back(vertex);
    }

    void RenderCommand::AddIndex(uint32 index) {
        indices.push_back(index);
    }

    void RenderCommand::SetBorderColor(const maths::Vector4f& color) {
        borderColor = color;
        hasBorder = true;
    }

    maths::Vector4f RenderCommand::GetBorderColor() const {
        return borderColor;
    }

    void RenderCommand::SetBorderWidth(float32 width) {
        borderWidth = width;
        hasBorder = true;
    }

    float32 RenderCommand::GetBorderWidth() const {
        return borderWidth;
    }

    bool RenderCommand::HasBorder() const {
        return hasBorder;
    }

}  //  nkentseu