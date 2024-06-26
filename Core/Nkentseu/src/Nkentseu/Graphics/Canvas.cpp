//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 05:50:37 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Canvas.h"
#include <Logger/Formatter.h>

#include "Internal/Opengl/OpenglCanvas.h"
#include "Internal/Vulkan/VulkanCanvas.h"

namespace nkentseu {

    using namespace maths;

	Memory::Shared<Canvas> Canvas::Create(const Memory::Shared<Context>& context)
	{
		if (context == nullptr) return nullptr;

		Memory::Shared<Canvas> canvas = nullptr;

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			canvas = Memory::Alloc<OpenglCanvas>(context);
		} else if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			canvas = Memory::Alloc<VulkanCanvas>(context);
		}

		return canvas;
	}

    void Canvas::Prepare()
    {
        if (!m_IsPresent) {
            if (m_IndexCount != 0) {
                Flush();
                m_IsPresent = false;
                m_Vertices.clear();
                m_Indices.clear();
                m_CanvasCommands.clear();
                m_IndexCount = 0;
                m_ScissorEnable = false;
                m_RenderEnable = false;
            }
        }

        m_IsPresent = false;
    }

    void Canvas::Present()
    {
        if (m_IsPresent) {
            m_IsPresent = false;
        }
        else {
            if (m_IndexCount != 0) {
                Flush();
                m_IsPresent = false;
                m_Vertices.clear();
                m_Indices.clear();
                m_CanvasCommands.clear();
                m_IndexCount = 0;
                m_ScissorEnable = false;
                m_RenderEnable = false;
            }

            m_IsPresent = true;
        }
    }

    void Canvas::SetScissor(const Scissor& scissor) {
        if (scissor.enabled == false) return;
        m_ScissorEnable = true;
        AddCommand(Memory::Alloc<CanvasScissorCommand>(scissor));
    }

    void Canvas::SetViewport(const Viewport& viewport) {
        AddCommand(Memory::Alloc<CanvasViewportCommand>(viewport));
    }

    void Canvas::AddCommand(CanvasCommandShared command)
    {
        m_CanvasCommands.insert(m_CanvasCommands.begin(), command);
    }

    void Canvas::SetScissor(int32 x, int32 y, int32 width, int32 height) {
        SetScissor(Scissor(true, x, y, width, height));
    }

    void Canvas::SetViewport(float32 x, float32 y, float32 width, float32 height) {
        SetViewport(Viewport(true, x, y, width, height));
    }

    void Canvas::DrawPoint(const maths::Vector2f& position, const Color& color, CanvasTexture texture) {
        // Création d'un sommet unique pour le point
        std::vector<Vertex2D> vertices = {
            Vertex2D(position, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f))
        };

        // Utilisation de la méthode Draw pour dessiner le point
        std::vector<uint32> indices = { 0 };
        Draw(RenderPrimitive::Points, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawLine(const maths::Vector2f& start, const maths::Vector2f& end, const Color& color, CanvasTexture texture) {
        // Création des sommets pour la ligne
        std::vector<Vertex2D> vertices = {
            Vertex2D(start, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)),
            Vertex2D(end, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f))
        };

        // Utilisation de la méthode Draw pour dessiner la ligne
        std::vector<uint32> indices = { 0, 1 };
        Draw(RenderPrimitive::Lines, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, bool filled, CanvasTexture texture) {
        // Création des sommets pour le rectangle
        std::vector<Vertex2D> vertices = {
            Vertex2D(position, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x, position.y), maths::Vector4f(color), maths::Vector2f(1.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x, position.y + size.y), maths::Vector4f(color), maths::Vector2f(1.0f, 1.0f)),
            Vertex2D(Vector2f(position.x, position.y + size.y), maths::Vector4f(color), maths::Vector2f(0.0f, 1.0f))
        };

        std::vector<uint32> indices;
        RenderPrimitive::Enum primitive;

        if (filled) {
            // Indices pour le rectangle rempli (deux triangles)
            indices = { 0, 1, 2, 2, 3, 0 };
            primitive = RenderPrimitive::Triangles;
        }
        else {
            // Indices pour le rectangle en contour (quatre lignes)
            indices = { 0, 1, 1, 2, 2, 3, 3, 0 };
            primitive = RenderPrimitive::Lines;
        }

        // Utilisation de la méthode Draw pour dessiner le rectangle
        Draw(primitive, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawFilledRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, CanvasTexture texture) {
        std::vector<Vertex2D> vertices = {
            Vertex2D(position, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x, position.y), maths::Vector4f(color), maths::Vector2f(1.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x, position.y + size.y), maths::Vector4f(color), maths::Vector2f(1.0f, 1.0f)),
            Vertex2D(Vector2f(position.x, position.y + size.y), maths::Vector4f(color), maths::Vector2f(0.0f, 1.0f))
        };

        std::vector<uint32> indices = {
            0, 1, 2, 2, 3, 0
        };

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawHollowRect(const maths::Vector2f& position, const maths::Vector2f& size, const Color& color, CanvasTexture texture) {
        std::vector<Vertex2D> vertices = {
            Vertex2D(position, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x, position.y), maths::Vector4f(color), maths::Vector2f(1.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x, position.y + size.y), maths::Vector4f(color), maths::Vector2f(1.0f, 1.0f)),
            Vertex2D(Vector2f(position.x, position.y + size.y), maths::Vector4f(color), maths::Vector2f(0.0f, 1.0f))
        };

        std::vector<uint32> indices = {
            0, 1, 1, 2, 2, 3, 3, 0
        };

        Draw(RenderPrimitive::Lines, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawThickOutlineRect(const maths::Vector2f& position, const maths::Vector2f& size, float32 thickness, const Color& color, CanvasTexture texture) {
        
        std::vector<Vertex2D> vertices = {
            Vertex2D(position - Vector2f(thickness, thickness), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x + thickness, position.y - thickness), maths::Vector4f(color), maths::Vector2f(1.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x + thickness, position.y + size.y + thickness), maths::Vector4f(color), maths::Vector2f(1.0f, 1.0f)),
            Vertex2D(Vector2f(position.x - thickness, position.y + size.y + thickness), maths::Vector4f(color), maths::Vector2f(0.0f, 1.0f)),

            Vertex2D(position + Vector2f(thickness, thickness), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x - thickness, position.y + thickness), maths::Vector4f(color), maths::Vector2f(1.0f, 0.0f)),
            Vertex2D(Vector2f(position.x + size.x - thickness, position.y + size.y - thickness), maths::Vector4f(color), maths::Vector2f(1.0f, 1.0f)),
            Vertex2D(Vector2f(position.x + thickness, position.y + size.y - thickness), maths::Vector4f(color), maths::Vector2f(0.0f, 1.0f))
        };

        std::vector<uint32> indices;

        // Ajout des indices pour dessiner les triangles de la bordure
        for (usize i = 0; i < 4; ++i) {
            int32 next = (i + 1) % 4;

            // Indices pour le contour extérieur
            indices.push_back(i);
            indices.push_back(i + 4);
            indices.push_back(next + 4);

            // Indices pour relier le contour extérieur
            indices.push_back(i);
            indices.push_back(next + 4);
            indices.push_back(next);
        }

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawRoundedRect(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& cornerRadii, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 16; // Nombre de segments pour approximer le cercle pour les coins

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(4 + segments * 4); // 4 coins + segments pour chaque coin

        // Ajout des sommets pour chaque coin avec arrondi
        for (int32 corner = 0; corner < 4; ++corner) {
            float32 radius = cornerRadii[corner];
            float32 centerX = position.x + (corner == 1 || corner == 2 ? size.x : 0.0f);
            float32 centerY = position.y + (corner > 1 ? size.y : 0.0f);

            // Centre du cercle pour ce coin
            vertices.push_back(Vertex2D(Vector2f(centerX, centerY), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));

            // Ajout des sommets du cercle pour ce coin
            for (int32 i = 0; i <= segments; ++i) {
                float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 0.5f * maths::Pi;
                float32 x = centerX - radius + radius * std::cos(theta);
                float32 y = centerY - radius + radius * std::sin(theta);
                vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
            }
        }

        // Ajout des indices pour dessiner les triangles des coins
        for (int32 corner = 0; corner < 4; ++corner) {
            int32 centerIndex = corner * (segments + 1);
            for (int32 i = 0; i < segments; ++i) {
                indices.push_back(centerIndex);
                indices.push_back(centerIndex + i + 1);
                indices.push_back(centerIndex + i + 2);
            }
        }

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawFilledCircle(const maths::Vector2f& center, float32 radius, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 36; // Nombre de segments pour approximer le cercle

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(segments + 1); // +1 pour le centre du cercle

        // Ajout du centre du cercle
        vertices.push_back(Vertex2D(center, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));

        // Ajout des sommets du cercle
        for (int32 i = 0; i <= segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + radius * std::cos(theta);
            float32 y = center.y + radius * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des indices pour dessiner les triangles
        for (int32 i = 1; i <= segments; ++i) {
            indices.push_back(0); // Centre du cercle
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawHollowCircle(const maths::Vector2f& center, float32 radius, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 36; // Nombre de segments pour approximer le cercle

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(segments);

        // Ajout des sommets du cercle
        for (int32 i = 0; i < segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + radius * std::cos(theta);
            float32 y = center.y + radius * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des indices pour dessiner les lignes du contour
        for (int32 i = 0; i < segments - 1; ++i) {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.push_back(segments - 1); // Bouclage pour relier le dernier au premier
        indices.push_back(0); // Bouclage pour relier le dernier au premier

        Draw(RenderPrimitive::LineStrip, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawThickOutlineCircle(const maths::Vector2f& center, float32 radius, float32 thickness, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 36; // Nombre de segments pour approximer le cercle

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(segments * 2); // Pour les deux cercles

        // Ajout des sommets du cercle extérieur (plus grand)
        for (int32 i = 0; i < segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + (radius + thickness * 0.5f) * std::cos(theta);
            float32 y = center.y + (radius + thickness * 0.5f) * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des sommets du cercle intérieur (plus petit)
        for (int32 i = 0; i < segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + (radius - thickness * 0.5f) * std::cos(theta);
            float32 y = center.y + (radius - thickness * 0.5f) * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des indices pour dessiner les lignes de la bordure
        for (int32 i = 0; i < segments; ++i) {
            int32 next = (i + 1) % segments;

            // Indices pour le contour extérieur
            indices.push_back(i);
            indices.push_back(i + segments);
            indices.push_back(next + segments);

            // Indices pour relier le contour extérieur
            indices.push_back(i);
            indices.push_back(next + segments);
            indices.push_back(next);
        }

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawFilledEllipse(const maths::Vector2f& center, float32 radiusX, float32 radiusY, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 36; // Nombre de segments pour approximer l'ellipse

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(segments + 1); // +1 pour le centre de l'ellipse

        // Ajout du centre de l'ellipse
        vertices.push_back(Vertex2D(center, maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));

        // Ajout des sommets de l'ellipse
        for (int32 i = 0; i <= segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + radiusX * std::cos(theta);
            float32 y = center.y + radiusY * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des indices pour dessiner les triangles
        for (int32 i = 1; i <= segments; ++i) {
            indices.push_back(0); // Centre de l'ellipse
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawHollowEllipse(const maths::Vector2f& center, float32 radiusX, float32 radiusY, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 36; // Nombre de segments pour approximer l'ellipse

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(segments);

        // Ajout des sommets de l'ellipse
        for (int32 i = 0; i < segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + radiusX * std::cos(theta);
            float32 y = center.y + radiusY * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des indices pour dessiner les lignes du contour
        for (int32 i = 0; i < segments - 1; ++i) {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.push_back(segments - 1); // Relier le dernier au premier pour fermer l'ellipse
        indices.push_back(0); // Relier le dernier au premier pour fermer l'ellipse

        Draw(RenderPrimitive::Lines, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::DrawThickOutlineEllipse(const maths::Vector2f& center, float32 radiusX, float32 radiusY, float32 thickness, const Color& color, CanvasTexture texture) {
        constexpr int32 segments = 36; // Nombre de segments pour approximer l'ellipse

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        vertices.reserve(segments * 2); // Pour les deux ellipses

        // Ajout des sommets de l'ellipse extérieure (plus grande)
        for (int32 i = 0; i < segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + (radiusX + thickness * 0.5f) * std::cos(theta);
            float32 y = center.y + (radiusY + thickness * 0.5f) * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des sommets de l'ellipse intérieure (plus petite)
        for (int32 i = 0; i < segments; ++i) {
            float32 theta = static_cast<float32>(i) / static_cast<float32>(segments) * 2.0f * maths::Pi;
            float32 x = center.x + (radiusX - thickness * 0.5f) * std::cos(theta);
            float32 y = center.y + (radiusY - thickness * 0.5f) * std::sin(theta);
            vertices.push_back(Vertex2D(Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }

        // Ajout des indices pour dessiner les lignes de la bordure
        for (int32 i = 0; i < segments; ++i) {
            int32 next = (i + 1) % segments;

            // Indices pour le contour extérieur
            indices.push_back(i);
            indices.push_back(i + segments);
            indices.push_back(next + segments);

            // Indices pour relier le contour extérieur
            indices.push_back(i);
            indices.push_back(next + segments);
            indices.push_back(next);
        }

        Draw(RenderPrimitive::Triangles, vertices, indices, texture, maths::matrix4f::Identity());
    }

    void Canvas::Draw(RenderPrimitive::Enum primitive, const std::vector<Vertex2D>& vertices, const std::vector<uint32>& indices, CanvasTexture texture, const maths::matrix4f& transform) {
        //Draw(primitive, vertices.data(), vertices.size(), indices.data(), indices.size(), texture, transform);

        m_Vertices.insert(m_Vertices.begin(), vertices.begin(), vertices.end());

        // Adjust existing indices to reflect the new vertices at the beginning
        for (usize i = 0; i < m_Indices.size(); ++i) {
            m_Indices[i] += vertices.size();
        }

        // Update indices
        std::vector<uint32> adjustedIndices(indices.size());
        for (usize i = 0; i < indices.size(); ++i) {
            adjustedIndices[i] = indices[i];
        }

        // Insert new indices at the beginning of m_Indices
        m_Indices.insert(m_Indices.begin(), adjustedIndices.begin(), adjustedIndices.end());

        // Increment the index count
        m_IndexCount += vertices.size();

        // Record the command
        AddCommand(Memory::Alloc<CanvasRenderCommand>(primitive, static_cast<uint32>(indices.size()), transform, texture));
        m_RenderEnable = true;
    }

    void Canvas::Draw(RenderPrimitive::Enum primitive, const Vertex2D* vertices, usize verticesNumber, const uint32* indices, usize indicesNumber, CanvasTexture texture, const maths::matrix4f& transform) {

        std::vector<Vertex2D> transformedVertices(vertices, vertices + verticesNumber);
        std::vector<uint32> adjustedIndices(indices, indices + indicesNumber);

        Draw(primitive, transformedVertices, adjustedIndices, texture, transform);
    }


    void Canvas::Draw(RenderPrimitive::Enum primitive, const Shape2D& shape, CanvasTexture texture, const maths::matrix4f& transform)
    {
        Draw(primitive, shape.GetVertices(), shape.GetIndices(), texture, maths::matrix4f::Identity());
    }

}  //  nkentseu