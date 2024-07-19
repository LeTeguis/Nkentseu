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

    struct NKENTSEU_API ClipRegion {
        // Union pour stocker les informations de la région de clipping
        union {
            struct {
                maths::Vector2f offsets; // Offset de la région de clipping
                maths::Vector2f size; // Taille de la région de clipping
            };
            struct {
                maths::Vector2f topLeft; // Coin supérieur gauche de la région de clipping
                maths::Vector2f dimensions; // Dimensions de la région de clipping
            };
            struct {
                union { float32 offsetX; float32 x; }; // Offset X de la région de clipping
                union { float32 offsetY; float32 y; }; // Offset Y de la région de clipping
                float32 width; // Largeur de la région de clipping
                float32 height; // Hauteur de la région de clipping
            };
            maths::Vector4f controlPoints; // Points de contrôle de la région de clipping
        };

        // Constructeur par défaut
        ClipRegion() : controlPoints(maths::Vector4f(0.0f, 0.0f, 0.0f, 0.0f)) {}

        // Constructeur de copie
        ClipRegion(const ClipRegion& other) {
            memcpy(this, &other, sizeof(ClipRegion));
        }

        // Opérateur d'affectation de copie
        ClipRegion& operator=(const ClipRegion& other) {
            if (this != &other) {
                memcpy(this, &other, sizeof(ClipRegion));
            }
            return *this;
        }

        // Constructeur de mouvement
        ClipRegion(ClipRegion&& other) noexcept {
            memcpy(this, &other, sizeof(ClipRegion));
        }

        // Opérateur d'affectation de mouvement
        ClipRegion& operator=(ClipRegion&& other) noexcept {
            if (this != &other) {
                memcpy(this, &other, sizeof(ClipRegion));
            }
            return *this;
        }
    };

    struct NKENTSEU_API RenderCommand2D {
        ClipRegion clipRegion; // Région de clipping de la commande de rendu
        uint32 vertexCount; // Nombre de vertex de la commande de rendu
        uint32 vertexOffset; // Offset des vertex de la commande de rendu
        uint32 indexCount; // Nombre d'indices de la commande de rendu
        uint32 indexOffset; // Offset des indices de la commande de rendu
        int32 textureId; // ID de la texture de la commande de rendu
        float32 lineWidth; // line width
        RenderPrimitive primitive; // Primitive de la commande de rendu

        // Constructeur par défaut
        RenderCommand2D() : vertexCount(0), vertexOffset(0), indexCount(0), indexOffset(0), textureId(-1), primitive(RenderPrimitive::Enum::Triangles), lineWidth(1.0f){}

        // Constructeur de copie
        RenderCommand2D(const RenderCommand2D& other) : clipRegion(other.clipRegion), vertexCount(other.vertexCount), vertexOffset(other.vertexOffset), indexCount(other.indexCount), indexOffset(other.indexOffset), textureId(other.textureId), primitive(other.primitive), lineWidth(other.lineWidth) {}

        // Opérateur d'affectation de copie
        RenderCommand2D& operator=(const RenderCommand2D& other) {
            if (this != &other) {
                clipRegion = other.clipRegion;
                vertexCount = other.vertexCount;
                vertexOffset = other.vertexOffset;
                indexCount = other.indexCount;
                indexOffset = other.indexOffset;
                textureId = other.textureId;
                primitive = other.primitive;
                lineWidth = other.lineWidth;
            }
            return *this;
        }

        // Constructeur de mouvement
        RenderCommand2D(RenderCommand2D&& other) noexcept : clipRegion(std::move(other.clipRegion)), vertexCount(other.vertexCount), vertexOffset(other.vertexOffset), indexCount(other.indexCount), indexOffset(other.indexOffset), textureId(other.textureId), primitive(other.primitive), lineWidth(other.lineWidth) {
            /*other.vertexCount = 0;
            other.vertexOffset = 0;
            other.indexCount = 0;
            other.indexOffset = 0;
            other.textureId = -1;
            other.primitive = RenderPrimitive::Enum::Triangles;*/
        }

        // Opérateur d'affectation de mouvement
        RenderCommand2D& operator=(RenderCommand2D&& other) noexcept {
            if (this != &other) {
                clipRegion = std::move(other.clipRegion);
                vertexCount = other.vertexCount;
                vertexOffset = other.vertexOffset;
                indexCount = other.indexCount;
                indexOffset = other.indexOffset;
                textureId = other.textureId;
                primitive = other.primitive;
                lineWidth = other.lineWidth;
                /*other.vertexCount = 0;
                other.vertexOffset = 0;
                other.indexCount = 0;
                other.indexOffset = 0;
                other.textureId = -1;
                other.primitive = RenderPrimitive::Enum::Triangles;*/
            }
            return *this;
        }
    };

}  //  nkentseu

#endif  // __RENDER_COMMAND_H__!