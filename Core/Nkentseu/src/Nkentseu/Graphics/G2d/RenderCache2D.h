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
#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/Shader.h"
#include "Nkentseu/Graphics/VertexArray.h"
#include "Nkentseu/Graphics/UniformBuffer.h"
#include "Nkentseu/Graphics/Renderer.h"

namespace nkentseu {

    #define RENDER_CACH_MAX_QUADS 50000

    #define RENDER_CACH_MAX_VERTICES RENDER_CACH_MAX_QUADS * 4
    #define RENDER_CACH_MAX_INDICES RENDER_CACH_MAX_QUADS * 6

    class NKENTSEU_API RenderCache2D {
    public:
        RenderCache2D(const Memory::Shared<Context>& context);
        ~RenderCache2D();

        bool Destroy();

        bool Initialize();
        void Render(Memory::Shared<Renderer> renderer);

        // Démarre l'ajout d'une commande de rendu
        void BeginCommand(RenderPrimitive primitive);

        // Ajoute une commande de rendu à la liste
        void AddCommand(const RenderCommand2D& command);

        // Modifie les informations de la commande en cours
        void SetCommandClipRegion(const ClipRegion& clipRegion);
        void SetCommandTextureId(int32 textureId);

        // Crée une commande de rendu directement
        RenderCommand2D* CreateCommand(RenderPrimitive primitive, const ClipRegion& clipRegion, int32 textureId);

        // Renvoie la commande en cours
        RenderCommand2D* GetCurrentCommand();

        // Renvoie une commande par son index
        RenderCommand2D* GetCommand(uint32 index);
        RenderCommand2D* GetLastCommand();
        RenderCommand2D* GetBeginCommand();

        // Ajoute un vertex à la commande en cours
        void AddVertex(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv);

        // Ajoute un indice à la commande en cours
        void AddIndex(uint32 index);

        // Termine l'ajout d'une commande de rendu
        // Si send est true, la commande actuelle est sauvegardée dans commands
        void EndCommand(bool send = true);

        // Vide la liste des commandes de rendu
        void Clear();

        // Ajoute un rectangle plein
        RenderCommand2D* AddFilledRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle avec contour
        RenderCommand2D* AddOutlineRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, float32 lineWidth = 1.0f, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle arrondi plein
        RenderCommand2D* AddFilledRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, const Color& color, uint32 segments = 16, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle arrondi avec contour
        RenderCommand2D* AddOutlineRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, float32 radius, const Color& color, float32 lineWidth = 1.0f, uint32 segments = 16, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle arrondi plein
        RenderCommand2D* AddFilledRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& radius, const Color& color, uint32 segments = 16, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle arrondi avec contour
        RenderCommand2D* AddOutlineRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& radius, const Color& color, float32 lineWidth = 1.0f, uint32 segments = 16, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle arrondi plein
        RenderCommand2D* AddFilledRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector2f radius[4], const Color& color, uint32 segments = 16, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un rectangle arrondi avec contour
        RenderCommand2D* AddOutlineRoundedRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector2f radius[4], const Color& color, float32 lineWidth = 1.0f, uint32 segments = 16, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un cercle plein
        RenderCommand2D* AddFilledCircle(const maths::Vector2f& position, float32 radius, const Color& color, uint32 segments = 32, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un cercle avec contour
        RenderCommand2D* AddOutlineCircle(const maths::Vector2f& position, float32 radius, const Color& color, float32 lineWidth = 1.0f, uint32 segments = 32, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute une ellipse plein
        RenderCommand2D* AddFilledEllipse(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, uint32 segments = 32, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute une ellipse avec contour
        RenderCommand2D* AddOutlineEllipse(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, float32 lineWidth = 1.0f, uint32 segments = 32, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un triangle plein
        RenderCommand2D* AddFilledTriangle(const maths::Vector2f& p1, const maths::Vector2f& p2, const maths::Vector2f& p3, const Color& color, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute un triangle avec contour
        RenderCommand2D* AddOutlineTriangle(const maths::Vector2f& p1, const maths::Vector2f& p2, const maths::Vector2f& p3, const Color& color, float32 lineWidth = 1.0f, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        RenderCommand2D* AddPoint(const maths::Vector2f& p, const Color& color, float32 lineWidth = 1.0f, int32 textureId = -1, maths::Vector2f uv = maths::Vector2f());

        // Ajoute une ligne
        RenderCommand2D* AddLine(const maths::Vector2f& p1, const maths::Vector2f& p2, const Color& color, float32 lineWidth = 1.0f, int32 textureId = -1, maths::Vector2f uv0 = {}, maths::Vector2f uv1 = {});

        // Ajoute un chemin (path)
        RenderCommand2D* AddFillPath(const std::vector<maths::Vector2f>& points, const Color& color, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        RenderCommand2D* AddOutlinePath(const std::vector<maths::Vector2f>& points, const Color& color, bool closed, float32 lineWidth = 1.0f, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute une courbe de Bézier
        RenderCommand2D* AddBezierCurve(const std::vector<maths::Vector2f>& points, int32 degree, bool closed, const std::vector<maths::Vector2f>& controlPoints, float32 lineWidth = 1.0f, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

        // Ajoute une courbe
        RenderCommand2D* AddCurve(const std::vector<maths::Vector2f>& points, int32 degree, bool closed, float32 lineWidth = 1.0f, int32 textureId = -1, const std::vector<maths::Vector2f>& uvs = {});

    private:
        std::vector<RenderCommand2D> commands; // Liste des commandes de rendu
        std::vector<Vertex2D> vertices; // Vertex de la géométrie 2D
        std::vector<uint32> indices; // Indices de la géométrie 2D
        RenderCommand2D currentCommand; // Commande en cours
        bool beginCommand = false;
        Geometry2D currentGeometry;
        uint32 currentVertice = 0; // Index du vertex en cours
        uint32 currentIndice = 0; // Index de l'indice en cours

        bool isPresent = false;

        // renderer
        Memory::Shared<Context> m_Context = nullptr;
        Memory::Shared<Shader> m_Shader = nullptr;
        Memory::Shared<ShaderInputLayout> shaderInputLayout = nullptr;

        std::vector<Memory::Shared<Texture2D>> textures;

        Memory::Shared<UniformBuffer> uniformBuffer = nullptr;

        Memory::Shared<VertexBuffer> vertexBuffer = nullptr;
        Memory::Shared<IndexBuffer> indexBuffer = nullptr;
        Memory::Shared<VertexArray> vertexArray = nullptr;

        Memory::Shared<Texture2D> defaultTetxure = nullptr;

        private:
            int32 AddInternalArc(const maths::Vector2f& center, const maths::Vector2f& radius, const maths::Vector2f& rad, const Color& color, float32 departAngle, float32 endAngle, float32 segments, int32 decal, const std::vector<maths::Vector2f>& uvs = {});
    };

    using Renderer2D = RenderCache2D;
}  //  nkentseu

#endif  // __RENDER_CACHE2_D_H__!