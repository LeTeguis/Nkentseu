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
        m_Commands.insert(m_Commands.begin(), { primitive, static_cast<uint32>(indices.size()), transform, texture });
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