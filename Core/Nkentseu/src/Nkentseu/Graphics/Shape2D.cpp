//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-25 at 06:35:53 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Shape2D.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    using namespace maths;

    CircleShape::CircleShape(float32 radiusX, float32 radiusY, int32 segments, const Color& color)
        : m_RadiusX(radiusX), m_RadiusY(radiusY), m_Segments(segments), m_Color(color) {}

    void CircleShape::GenerateVertices() {
        m_Vertices.clear();

        const Color& color = m_Color;

        // Center vertex
        m_Vertices.push_back(Vertex2D(maths::Vector2f(0.0f, 0.0f), Vector4f(color), maths::Vector2f(0.0f, 0.0f)));

        // Generate vertices around the ellipse/circle
        float32 angleIncrement = 2.0f * maths::Pi / m_Segments;
        for (int32 i = 0; i <= m_Segments; ++i) {
            float32 angle = angleIncrement * i;
            float32 x = m_RadiusX * std::cos(angle);
            float32 y = m_RadiusY * std::sin(angle);
            m_Vertices.push_back(Vertex2D(maths::Vector2f(x, y), maths::Vector4f(color), maths::Vector2f(0.0f, 0.0f)));
        }
    }

    void CircleShape::GenerateIndices() {
        m_Indices.clear();

        // Generate triangle fan indices
        for (int32 i = 1; i <= m_Segments; ++i) {
            m_Indices.push_back(0);        // Center vertex
            m_Indices.push_back(i);        // Current vertex
            m_Indices.push_back(i + 1);    // Next vertex (or vertex 1 for the last segment)
        }
        m_Indices.back() = 1; // Close the loop with the first vertex
    }

    RectangleShape::RectangleShape(float32 width, float32 height, float32 cornerRadius, int32 cornerSegments, const Color& color)
        : m_Width(width), m_Height(height), m_CornerRadius(cornerRadius), m_CornerSegments(cornerSegments), m_Color(color) {}

    void RectangleShape::GenerateVertices() {
        m_Vertices.clear();

        // Calculate vertices for rounded rectangle
        if (m_CornerRadius > 0.0f) {
            // Calculate vertices for rounded corners
            float32 halfWidth = m_Width * 0.5f;
            float32 halfHeight = m_Height * 0.5f;
            float32 minRadius = std::min(m_CornerRadius, std::min(halfWidth, halfHeight));

            std::vector<maths::Vector2f> cornerOffsets = {
                maths::Vector2f(halfWidth - minRadius, halfHeight - minRadius),
                maths::Vector2f(-(halfWidth - minRadius), halfHeight - minRadius),
                maths::Vector2f(-(halfWidth - minRadius), -(halfHeight - minRadius)),
                maths::Vector2f(halfWidth - minRadius, -(halfHeight - minRadius))
            };

            // Generate vertices for each corner
            for (int32 i = 0; i < 4; ++i) {
                float32 offsetX = cornerOffsets[i].x;
                float32 offsetY = cornerOffsets[i].y;

                for (int32 j = 0; j <= m_CornerSegments; ++j) {
                    float32 angle = static_cast<float32>(j) / m_CornerSegments * maths::Pi * 0.5f;
                    float32 x = offsetX + minRadius * std::cos(angle);
                    float32 y = offsetY + minRadius * std::sin(angle);
                    m_Vertices.push_back(Vertex2D(maths::Vector2f(x, y), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 0.0f)));
                }
            }

            // Generate vertices for straight edges
            m_Vertices.push_back(Vertex2D(maths::Vector2f(-halfWidth + minRadius, halfHeight), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 0.0f)));
            m_Vertices.push_back(Vertex2D(maths::Vector2f(halfWidth - minRadius, halfHeight), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 0.0f)));
            m_Vertices.push_back(Vertex2D(maths::Vector2f(-halfWidth + minRadius, -halfHeight), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 0.0f)));
            m_Vertices.push_back(Vertex2D(maths::Vector2f(halfWidth - minRadius, -halfHeight), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 0.0f)));
        }
        else {
            // Regular rectangle vertices
            m_Vertices.push_back(Vertex2D(maths::Vector2f(-m_Width * 0.5f, -m_Height * 0.5f), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 0.0f)));
            m_Vertices.push_back(Vertex2D(maths::Vector2f(m_Width * 0.5f, -m_Height * 0.5f), maths::Vector4f(m_Color), maths::Vector2f(1.0f, 0.0f)));
            m_Vertices.push_back(Vertex2D(maths::Vector2f(m_Width * 0.5f, m_Height * 0.5f), maths::Vector4f(m_Color), maths::Vector2f(1.0f, 1.0f)));
            m_Vertices.push_back(Vertex2D(maths::Vector2f(-m_Width * 0.5f, m_Height * 0.5f), maths::Vector4f(m_Color), maths::Vector2f(0.0f, 1.0f)));
        }
    }

    void RectangleShape::GenerateIndices() {
        m_Indices.clear();

        if (m_CornerRadius > 0.0f) {
            // Generate indices for rounded corners
            int32 numVertices = static_cast<int32>(m_Vertices.size());

            for (int32 i = 0; i < 4; ++i) {
                int32 baseIndex = i * (m_CornerSegments + 1);

                for (int32 j = 0; j < m_CornerSegments; ++j) {
                    m_Indices.push_back(baseIndex + j);
                    m_Indices.push_back(baseIndex + j + 1);
                    m_Indices.push_back(baseIndex + m_CornerSegments + 1 + j);

                    m_Indices.push_back(baseIndex + j + 1);
                    m_Indices.push_back(baseIndex + m_CornerSegments + 2 + j);
                    m_Indices.push_back(baseIndex + m_CornerSegments + 1 + j);
                }
            }

            // Indices for straight edges
            m_Indices.push_back(numVertices - 4); // Bottom-left
            m_Indices.push_back(numVertices - 2); // Bottom-right
            m_Indices.push_back(numVertices - 3); // Top-left

            m_Indices.push_back(numVertices - 2); // Bottom-right
            m_Indices.push_back(numVertices - 1); // Top-right
            m_Indices.push_back(numVertices - 3); // Top-left
        }
        else {
            // Indices for regular rectangle (two triangles)
            m_Indices = { 0, 1, 2, 2, 3, 0 };
        }
    }

}  //  nkentseu