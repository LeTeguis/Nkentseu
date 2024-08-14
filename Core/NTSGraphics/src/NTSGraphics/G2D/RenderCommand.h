//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-07-16 at 01:14:49 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __RENDER_COMMAND_H__
#define __RENDER_COMMAND_H__

#pragma once

#include <NTSCore/System.h>
#include "RenderData2D.h"

namespace nkentseu {

    struct NKENTSEU_API ClipRegion {
        // Union pour stocker les informations de la r�gion de clipping
        union {
            struct {
                maths::Vector2f offsets; // Offset de la r�gion de clipping
                maths::Vector2f size; // Taille de la r�gion de clipping
            };
            struct {
                maths::Vector2f topLeft; // Coin sup�rieur gauche de la r�gion de clipping
                maths::Vector2f dimensions; // Dimensions de la r�gion de clipping
            };
            struct {
                union { float32 offsetX; float32 x; }; // Offset X de la r�gion de clipping
                union { float32 offsetY; float32 y; }; // Offset Y de la r�gion de clipping
                float32 width; // Largeur de la r�gion de clipping
                float32 height; // Hauteur de la r�gion de clipping
            };
            maths::Vector4f controlPoints; // Points de contr�le de la r�gion de clipping
        };

        // Constructeur par d�faut
        ClipRegion() : controlPoints(maths::Vector4f(0.0f, 0.0f, 0.0f, 0.0f)) {}

        // Constructeur de copie
        ClipRegion(const ClipRegion& other) {
            memcpy(this, &other, sizeof(ClipRegion));
        }

        // Op�rateur d'affectation de copie
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

        // Op�rateur d'affectation de mouvement
        ClipRegion& operator=(ClipRegion&& other) noexcept {
            if (this != &other) {
                memcpy(this, &other, sizeof(ClipRegion));
            }
            return *this;
        }
    };

    struct NKENTSEU_API RenderCommand2D {
        ClipRegion      clipRegion; // R�gion de clipping de la commande de rendu
        uint32          vertexCount; // Nombre de vertex de la commande de rendu
        uint32          vertexOffset; // Offset des vertex de la commande de rendu
        uint32          indexCount; // Nombre d'indices de la commande de rendu
        uint32          indexOffset; // Offset des indices de la commande de rendu
        int32           textureId; // ID de la texture de la commande de rendu
        float32         lineWidth; // line width
        RenderPrimitive primitive; // Primitive de la commande de rendu
        bool            useText = false;
        bool            useColor = true;
        bool            useTexture = false;
        uint32          fontTextureId;

        maths::Vector3f position;
        maths::Vector3f rotation;
        maths::Vector3f scale;

        // Constructeur par d�faut
        RenderCommand2D() : vertexCount(0), vertexOffset(0), indexCount(0), indexOffset(0), textureId(-1), primitive(RenderPrimitive::Enum::Triangles), lineWidth(1.0f), useText(false), useColor(true), useTexture(false){}

        // Constructeur de copie
        RenderCommand2D(const RenderCommand2D& other) : clipRegion(other.clipRegion), vertexCount(other.vertexCount), vertexOffset(other.vertexOffset), indexCount(other.indexCount), indexOffset(other.indexOffset), textureId(other.textureId), primitive(other.primitive), lineWidth(other.lineWidth), useText(other.useText), useColor(other.useColor), useTexture(other.useTexture), fontTextureId(other.fontTextureId) {}

        // Op�rateur d'affectation de copie
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
                useText = other.useText;
                useColor = other.useColor;
                useTexture = other.useTexture;
                fontTextureId = other.fontTextureId;
            }
            return *this;
        }

        // Constructeur de mouvement
        RenderCommand2D(RenderCommand2D&& other) noexcept : clipRegion(std::move(other.clipRegion)), vertexCount(other.vertexCount), vertexOffset(other.vertexOffset), indexCount(other.indexCount), indexOffset(other.indexOffset), textureId(other.textureId), primitive(other.primitive), lineWidth(other.lineWidth), useText(other.useText), useColor(other.useColor), useTexture(other.useTexture), fontTextureId(other.fontTextureId) {
            /*other.vertexCount = 0;
            other.vertexOffset = 0;
            other.indexCount = 0;
            other.indexOffset = 0;
            other.textureId = -1;
            other.primitive = RenderPrimitive::Enum::Triangles;*/
        }

        // Op�rateur d'affectation de mouvement
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
                useText = other.useText;
                useColor = other.useColor;
                useTexture = other.useTexture;
                fontTextureId = other.fontTextureId;
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