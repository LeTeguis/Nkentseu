//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 03:06:30 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __RENDER_CACHE2_D_H__
#define __RENDER_CACHE2_D_H__

#pragma once

#include <System/System.h>
#include "RenderData2D.h"
#include "RenderCommand.h"

namespace nkentseu {
    
    class NKENTSEU_API RenderCache2D {
    public:
        RenderCache2D();

        // Ajoute une commande de rendu à la liste
        void AddCommand(const RenderCommand& command);

        // Démarre l'ajout d'une commande de rendu
        void BeginCommand(RenderPrimitive primitive, float32 lineWidth = 1.0f);

        // Ajoute un vertex à la commande de rendu en cours
        void AddVertex(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv);
        // Ajoute un indice à la commande de rendu en cours
        void AddIndex(uint32 index);

        // Termine l'ajout d'une commande de rendu
        void EndCommand();

        // Ajoute un point à la liste de vertex
        void AddPoint(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv1);

        // Ajoute une ligne à la liste de vertex et d'indices
        void AddLine(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2);
        void AddLine(const maths::Vector2f& point, const maths::Vector2f& direction, float32 length, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2);

        // Ajoute un rectangle à la liste de vertex et d'indices
        void AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);
        void AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, float32 borderRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);
        void AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);
        void AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);



        /*/ Ajoute un point à la liste de vertex
        void AddPoint(const maths::Vector2f& position, const maths::Vector4f& color);

        // Ajoute une ligne à la liste de vertex et d'indices
        void AddLine(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color);
        void AddLine(const maths::Vector2f& point, const maths::Vector2f& direction, float length, const maths::Vector4f& color);

        // Ajoute un segment à la liste de vertex et d'indices
        void AddSegment(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color);

        // Ajoute un rectangle à la liste de vertex et d'indices
        void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color);
        void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, float32 borderRadius);
        void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii);
        void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& bottomRightRadius);

        // Ajoute un cercle à la liste de vertex et d'indices
        void AddCircle(const maths::Vector2f& center, float32 radius, const maths::Vector4f& color);

        // Ajoute une ellipse à la liste de vertex et d'indices
        void AddEllipse(const maths::Vector2f& center, const maths::Vector2f& radius, const maths::Vector4f& color);

        // Ajoute un chemin à la liste de vertex et d'indices
        void AddPath(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, bool closed);

        // Ajoute une courbe de Bézier à la liste de vertex et d'indices
        void AddBezier(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color);

        // Ajoute une courbe à la liste de vertex et d'indices
        void AddCurve(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color);

        
        

        // Ajoute un segment à la liste de vertex et d'indices
        void AddSegment(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2);

        // Ajoute un rectangle à la liste de vertex et d'indices
        //void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);
        //void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, float32 borderRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);
        //void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);
        //void AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& bottomRightRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4);

        // Ajoute un cercle à la liste de vertex et d'indices
        void AddCircle(const maths::Vector2f& center, float32 radius, const maths::Vector4f& color, const maths::Vector2f& uv1);

        // Ajoute une ellipse à la liste de vertex et d'indices
        void AddEllipse(const maths::Vector2f& center, const maths::Vector2f& radius, const maths::Vector4f& color, const maths::Vector2f& uv1);

        // Ajoute un chemin à la liste de vertex et d'indices
        void AddPath(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, const std::vector<maths::Vector2f>& uvs, bool closed);

        // Ajoute une courbe de Bézier à la liste de vertex et d'indices
        void AddBezier(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, const std::vector<maths::Vector2f>& uvs);

        // Ajoute une courbe à la liste de vertex et d'indices
        void AddCurve(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, const std::vector<maths::Vector2f>& uvs);*/

        // Propriétés spécifiques
        void SetBorderColor(const maths::Vector4f& color);
        void SetBorderWidth(float32 width);

        void SetBlendMode(BlendMode2D mode);
        void SetClipRegion(const ClipRegion& region);
        void SetTransform(const maths::matrix4& transform);
        void SetTexture(uint32 textureId);

    private:
        std::vector<RenderCommand> commands;
        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;
        RenderCommand* currentCommand;
        uint32 currentVertice = 0;
    };

}  //  nkentseu

#endif  // __RENDER_CACHE2_D_H__!