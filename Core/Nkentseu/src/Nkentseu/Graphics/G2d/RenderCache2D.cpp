//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-16 at 03:06:30 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "RenderCache2D.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    RenderCache2D::RenderCache2D() : currentCommand(nullptr) {}

    void RenderCache2D::AddCommand(const RenderCommand& command) {
        commands.push_back(*command);
    }

    void RenderCache2D::BeginCommand(RenderPrimitive primitive, float32 lineWidth) {
        currentCommand = new RenderCommand();
        currentCommand->GetData()->SetPrimitive(primitive);
        //currentCommand->lineWidth = lineWidth;
    }

    void RenderCache2D::AddVertex(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add vertex without starting a command");
        }
        currentCommand->AddVertex({ position, color, uv });
    }

    void RenderCache2D::AddIndex(uint32 index)
    {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add vertex without starting a command");
        }
        currentCommand->AddIndex(index);
    }

    void RenderCache2D::EndCommand() {
        if (!currentCommand) {
            throw std::runtime_error("Cannot end command without starting one");
        }
        // Ajouter la commande à la liste des commandes à rendre
        commands.push_back(std::move(currentCommand));
        currentCommand = nullptr;
    }

    void RenderCache2D::AddPoint(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv) 
    {
        AddVertex(position, color, uv);
        uint32 vertexIndex = 0;
        AddIndex(vertexIndex + currentVertice);

        currentVertice += vertexIndex;
    }

    void RenderCache2D::AddLine(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2) {
        AddVertex(start, color, uv1);
        AddVertex(end, color, uv2);

        uint32 vertexIndex = currentVertice;
        AddIndex(vertexIndex);
        AddIndex(vertexIndex + 1);

        currentVertice += 2;
    }

    void RenderCache2D::AddLine(const maths::Vector2f& point, const maths::Vector2f& direction, float32 length, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2) {
        maths::Vector2f start = point;
        maths::Vector2f end = point + direction * length;

        AddLine(start, end, color, uv1, uv2);
    }

    void RenderCache2D::AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4) {

        maths::Vector2f topLeft = position;
        maths::Vector2f topRight = position + maths::Vector2f(size.x, 0);
        maths::Vector2f bottomRight = position + size;
        maths::Vector2f bottomLeft = position + maths::Vector2f(0, size.y);

        AddVertex(topLeft, color, uv1);
        AddVertex(bottomLeft, color, uv4);
        AddVertex(bottomRight, color, uv3);
        AddVertex(topRight, color, uv2);

        if (filled) {
            uint32 vertexIndex = currentVertice;
            AddIndex(vertexIndex);
            AddIndex(vertexIndex + 1);
            AddIndex(vertexIndex + 2);
            AddIndex(vertexIndex);
            AddIndex(vertexIndex + 2);
            AddIndex(vertexIndex + 3);
        }
        else {
            uint32 vertexIndex = currentVertice;
            AddIndex(vertexIndex);
            AddIndex(vertexIndex + 1);
            AddIndex(vertexIndex + 1);
            AddIndex(vertexIndex + 2);
            AddIndex(vertexIndex + 2);
            AddIndex(vertexIndex + 3);
            AddIndex(vertexIndex + 3);
            AddIndex(vertexIndex);
        }

        currentVertice += 4;
    }

    void RenderCache2D::AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, float32 borderRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
    }

    void RenderCache2D::AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
        maths::Vector2f topLeftRadius(borderRadii.x);
        maths::Vector2f topRightRadius(borderRadii.y);
        maths::Vector2f bottomLeftRadius(borderRadii.z);
        maths::Vector2f bottomRightRadius(borderRadii.w);
        AddRectangle(filled, position, size, color, topLeftRadius, topRightRadius, bottomRightRadius, bottomLeftRadius, uv1, uv2, uv3, uv4);
    }

    void RenderCache2D::AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
        int32 numSegments = 16; // nombre de segments pour le cercle
        float32 theta = 2 * maths::Pi / numSegments; // angle d'incrémentation

        maths::Vector2f topLeft = position;
        maths::Vector2f topRight = position + maths::Vector2f(size.x, 0);
        maths::Vector2f bottomRight = position + size;
        maths::Vector2f bottomLeft = position + maths::Vector2f(0, size.y);

        std::vector<float32> radius;
        radius.push_back(maths::Min<float32>({ topLeftRadius.x, size.width * 0.5f }));
        radius.push_back(maths::Min<float32>({ topLeftRadius.y, size.height * 0.5f }));
        radius.push_back(maths::Min<float32>({ bottomLeftRadius.x, size.width * 0.5f }));
        radius.push_back(maths::Min<float32>({ bottomLeftRadius.y, size.height * 0.5f }));
        radius.push_back(maths::Min<float32>({ bottomRightRadius.x, size.width * 0.5f }));
        radius.push_back(maths::Min<float32>({ bottomRightRadius.y, size.height * 0.5f }));
        radius.push_back(maths::Min<float32>({ topRightRadius.x, size.width * 0.5f }));
        radius.push_back(maths::Min<float32>({ topRightRadius.y, size.height * 0.5f }));

        float32 rad = maths::Max<float32>(radius);

        // calcul des points de controle pour les coins arrondis
        maths::Vector2f radiu(topLeftRadius.x == 0 ? rad : topLeftRadius.x, topLeftRadius.y == 0 ? rad : topLeftRadius.y);
        maths::Vector2f topLeftCtrl = topLeft + maths::Vector2f(-radiu.x, radiu.y);
        radiu = maths::Vector2f(topRightRadius.x == 0 ? rad : topRightRadius.x, topRightRadius.y == 0 ? rad : topRightRadius.y);
        maths::Vector2f topRightCtrl = topRight + maths::Vector2f(radiu.x, radiu.y);
        radiu = maths::Vector2f(bottomRightRadius.x == 0 ? rad : bottomRightRadius.x, bottomRightRadius.y == 0 ? rad : bottomRightRadius.y);
        maths::Vector2f bottomRightCtrl = bottomRight + maths::Vector2f(radiu.x, -radiu.y);
        radiu = maths::Vector2f(bottomLeftRadius.x == 0 ? rad : bottomLeftRadius.x, bottomLeftRadius.y == 0 ? rad : bottomLeftRadius.y);
        maths::Vector2f bottomLeftCtrl = bottomLeft + maths::Vector2f(-radiu.x, -radiu.y);

        if (filled) {
            AddVertex(topLeftCtrl, color, uv1);
        }
        // ajout des vertices pour les coins arrondis
        for (int32 i = 0; i < numSegments; i++) {
            float32 angle = i * theta;
            maths::Vector2f vertex = topLeftCtrl + maths::Vector2f(cos(angle) * radius[0], sin(angle) * radius[1]);
            AddVertex(vertex, color, uv1);
        }

        if (filled) {
            AddVertex(bottomLeftCtrl, color, uv1);
        }
        for (int32 i = 0; i < numSegments; i++) {
            float32 angle = i * theta;
            maths::Vector2f vertex = bottomLeftCtrl + maths::Vector2f(cos(angle) * radius[2], sin(angle) * radius[3]);
            AddVertex(vertex, color, uv2);
        }

        if (filled) {
            AddVertex(bottomRightCtrl, color, uv1);
        }
        for (int32 i = 0; i < numSegments; i++) {
            float32 angle = i * theta;
            maths::Vector2f vertex = bottomRightCtrl + maths::Vector2f(cos(angle) * radius[4], sin(angle) * radius[5]);
            AddVertex(vertex, color, uv3);
        }

        if (filled) {
            AddVertex(topRightCtrl, color, uv1);
        }
        for (int32 i = 0; i < numSegments; i++) {
            float32 angle = i * theta;
            maths::Vector2f vertex = topRightCtrl + maths::Vector2f(cos(angle) * radius[6], sin(angle) * radius[7]);
            AddVertex(vertex, color, uv4);
        }

        if (filled) {
            // ajout des indices pour les triangles
            for (int32 i = 0; i <= numSegments; i++) {
                AddIndex(0); // controleur du coin supérieur gauche
                AddIndex(i + 1);
                AddIndex(i + 2);
            }
            AddIndex(numSegments + 1); // controleur du coin supérieur gauche
            AddIndex(numSegments + 3);
            AddIndex(numSegments + 2);

            for (int32 i = 0; i <= numSegments; i++) {
                AddIndex(numSegments); // controleur du coin inférieur gauche
                AddIndex(numSegments + i + 1);
                AddIndex(numSegments + i + 2);
            }
            AddIndex(numSegments * 2 + 1); // controleur du coin supérieur gauche
            AddIndex(numSegments * 2 + 3);
            AddIndex(numSegments * 2 + 2);

            for (int32 i = 0; i < numSegments; i++) {
                AddIndex(numSegments * 2); // controleur du coin inférieur droit
                AddIndex(numSegments * 2 + i + 1);
                AddIndex(numSegments * 2 + i + 2);
            }
            AddIndex(numSegments * 3 + 1); // controleur du coin supérieur gauche
            AddIndex(numSegments * 3 + 3);
            AddIndex(numSegments * 3 + 2);

            for (int32 i = 0; i < numSegments; i++) {
                AddIndex(numSegments * 3); // controleur du coin supérieur droit
                AddIndex(numSegments * 3 + i + 1);
                AddIndex(numSegments * 3 + i + 2);
            }
            AddIndex(numSegments * 3 + 1); // controleur du coin supérieur gauche
            AddIndex(1);
            AddIndex(0);
        } else {
            // ajout des indices pour les triangles
            for (int32 i = 0; i < numSegments; i++) {
                AddIndex(i);
                AddIndex(i + 1);
            }

            for (int32 i = 0; i < numSegments; i++) {
                AddIndex(numSegments + i);
                AddIndex(numSegments + i + 1);
            }

            for (int32 i = 0; i < numSegments; i++) {
                AddIndex(numSegments * 2 + i);
                AddIndex(numSegments * 2 + i + 1);
            }

            for (int32 i = 0; i < numSegments; i++) {
                AddIndex(numSegments * 3 + i);
                AddIndex(numSegments * 3 + i + 1);
            }
        }

        currentVertice += numSegments * 4;
    }

    /*void RenderCache2D::AddRectangle(bool filled, const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
        if (topLeftRadius == maths::Vector2f() && topLeftRadius == topRightRadius && topRightRadius == bottomRightRadius && bottomRightRadius == bottomLeftRadius) {
            return AddRectangle(filled, position, size, color, uv1, uv2, uv3, uv4);
        }

        if (filled) {
            BeginCommand(RenderPrimitive::Enum::Triangles);
        }
        else {
            BeginCommand(RenderPrimitive::Enum::Lines);
        }

        constexpr int32 segments = 16; // Nombre de segments pour approximer le cercle pour les coins
        constexpr float32 segments_haps = 1.0f / (float32)segments; // Nombre de segments pour approximer le cercle pour les coins

        std::vector<float32> radius_ajust;
        radius_ajust.push_back(maths::Min<float32>({ topLeftRadius.x, size.width * 0.5f}));
        radius_ajust.push_back(maths::Min<float32>({ topLeftRadius.y, size.height * 0.5f }));
        radius_ajust.push_back(maths::Min<float32>({ topRightRadius.x, size.width * 0.5f}));
        radius_ajust.push_back(maths::Min<float32>({ topRightRadius.y, size.height * 0.5f }));
        radius_ajust.push_back(maths::Min<float32>({ bottomRightRadius.x, size.width * 0.5f}));
        radius_ajust.push_back(maths::Min<float32>({ bottomRightRadius.y, size.height * 0.5f }));
        radius_ajust.push_back(maths::Min<float32>({ bottomLeftRadius.x, size.width * 0.5f}));
        radius_ajust.push_back(maths::Min<float32>({ bottomLeftRadius.y, size.height * 0.5f }));

        float32 rad = maths::Max<float32>(radius_ajust);

        float32 statical_radius = 50.f;

        std::vector<maths::Vector2f> centers;
        centers.push_back({ radius_ajust[0] == 0.0f ? position.x + rad : position.x + radius_ajust[0] , radius_ajust[1] == 0.0f ? position.y + rad : position.y + radius_ajust[1] });
        centers.push_back({ radius_ajust[2] == 0.0f ? position.x + size.x - rad : position.x + size.x - radius_ajust[2] , radius_ajust[3] == 0.0f ? position.y + rad : position.y + radius_ajust[3] });
        centers.push_back({ radius_ajust[4] == 0.0f ? position.x + size.x - rad : position.x + size.x - radius_ajust[4] , radius_ajust[5] == 0.0f ? position.y + size.y - rad : position.y + size.y - radius_ajust[5] });
        centers.push_back({ radius_ajust[6] == 0.0f ? position.x + rad : position.x + radius_ajust[6] , radius_ajust[7] == 0.0f ? position.y + size.y - rad : position.y + size.y - radius_ajust[7] });

        std::vector<maths::Vector2f> uvs;
        uvs.push_back(uv1);
        uvs.push_back(uv2);
        uvs.push_back(uv3);
        uvs.push_back(uv4);

        std::vector<Vertex2D> vertices;
        std::vector<uint32> indices;

        for (int32 corner = 0; corner < 4; ++corner) {
            AddVertex(centers[corner], color, uvs[corner]);

            int32 realsegment = segments;
            float32 realfactor = 0.5f;

            if (radius_ajust[corner * 2] == 0 && radius_ajust[corner * 2 + 1] == 0) {
                realsegment = 2;
                realfactor = 0.25f;
            }

            float32 angle = 0.0f;

            if (corner == 0) {
                angle = maths::Pi;
            }
            else if (corner == 1) {
                angle = maths::Pi * 1.5f;
            }
            else if (corner == 2) {
                angle = 0;
            }
            else if (corner == 3) {
                angle = maths::Pi * 0.5;
            }

            for (int32 i = 0; i <= realsegment; ++i) {
                float32 theta = (static_cast<float32>(i) / static_cast<float32>(segments) * realfactor * maths::Pi) + angle;

                float32 cosAngle = maths::Cos(maths::Angle::FromRadian(theta));
                float32 sinAngle = maths::Sin(maths::Angle::FromRadian(theta));

                float32 radius_calculus_x = radius_ajust[corner * 2];
                float32 radius_calculus_y = radius_ajust[corner * 2 + 1];

                if (radius_ajust[corner * 2] == 0) {
                    cosAngle = (cosAngle < 0) ? -1 : 1;
                    radius_calculus_x = rad;
                }

                if (radius_ajust[corner * 2 + 1] == 0) {
                    sinAngle = (sinAngle < 0) ? -1 : 1;
                    radius_calculus_y = rad;
                }

                maths::Vector2f vertex(
                    centers[corner].x + radius_calculus_x * cosAngle,
                    centers[corner].y + radius_calculus_y * sinAngle
                );

                AddVertex(vertex, color, uvs[corner]);
            }
        }

        std::vector<int32> center_indices;
        center_indices.resize(4);

        int32 next_center_indice = 0;

        for (int32 corner = 0; corner < 4; corner++) {
            center_indices[corner] = next_center_indice;

            int32 realsegment = segments;

            if (radius_ajust[corner] == 0.0f) {
                realsegment = 2;
            }

            // Ajout des indices pour dessiner les triangles
            for (int32 index = 1; index <= realsegment; ++index) {
                AddIndex(center_indices[corner]); // Centre du cercle
                AddIndex(center_indices[corner] + index);
                AddIndex(center_indices[corner] + index + 1);
            }

            next_center_indice = (center_indices[corner] + (realsegment + 1) + 1) % (vertices.size());

            AddIndex(center_indices[corner]);
            AddIndex(center_indices[corner] + (realsegment + 1));
            AddIndex(next_center_indice);

            AddIndex(center_indices[corner] + (realsegment + 1));
            AddIndex(next_center_indice + 1);
            AddIndex(next_center_indice);
        }

        AddIndex(center_indices[0]);
        AddIndex(center_indices[1]);
        AddIndex(center_indices[2]);

        AddIndex(center_indices[2]);
        AddIndex(center_indices[3]);
        AddIndex(center_indices[0]);

        EndCommand();
        // que vaut currentVertice ??
    }*/

    /*
    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4) {
        // TODO: implement border radius logic
        AddRectangle(position, size, color, uv1, uv2, uv3, uv4);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& bottomRightRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4) {
        maths::Vector4f borderRadii(topLeftRadius.x, topRightRadius.y, bottomLeftRadius.x, bottomRightRadius.y);
        AddRectangle(position, size, color, borderRadii, uv1, uv2, uv3, uv4);
    }




    void RenderCache2D::AddPoint(const maths::Vector2f& position, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add point32 without starting a command");
        }
        currentCommand->AddVertex(position, color);
    }

    void RenderCache2D::AddLine(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add line without starting a command");
        }
        currentCommand->AddVertex(start, color);
        currentCommand->AddVertex(end, color);
        currentCommand->AddIndex(0);
        currentCommand->AddIndex(1);
    }

    void RenderCache2D::AddLine(const maths::Vector2f& point, const maths::Vector2f& direction, float32 length, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add line without starting a command");
        }
        maths::Vector2f end = point + direction * length;
        AddLine(point, end, color);
    }

    void RenderCache2D::AddSegment(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add segment without starting a command");
        }
        AddLine(start, end, color);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color) {
        maths::Vector2f uv;
        AddRectangle(position, size, color, uv, uv, uv, uv);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, float32 borderRadius) {
        maths::Vector2f uv;
        AddRectangle(position, size, color, borderRadius, uv, uv, uv, uv);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii) {
        maths::Vector2f uv;
        AddRectangle(position, size, color, borderRadii, uv, uv, uv, uv);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& bottomRightRadius) {
        maths::Vector2f uv;
        AddRectangle(position, size, color, topLeftRadius, topRightRadius, bottomLeftRadius, bottomRightRadius, uv, uv, uv, uv);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, float32 borderRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add rectangle without starting a command");
        }
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector4f& borderRadii, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add rectangle without starting a command");
        }
        // Implementation de l'ajout d'un rectangle avec radii individuels
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& topLeftRadius, const maths::Vector2f& topRightRadius, const maths::Vector2f& bottomLeftRadius, const maths::Vector2f& bottomRightRadius, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4)
    {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add rectangle without starting a command");
        }
        // Implementation de l'ajout d'un rectangle avec radii individuels
    }

    void RenderCache2D::AddCircle(const maths::Vector2f& center, float32 radius, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add circle without starting a command");
        }
        // Implementation de l'ajout d'un cercle
    }

    void RenderCache2D::AddEllipse(const maths::Vector2f& center, const maths::Vector2f& radius, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add ellipse without starting a command");
        }
        // Implementation de l'ajout d'une ellipse
    }

    void RenderCache2D::AddPath(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, bool closed) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add path without starting a command");
        }
        // Implementation de l'ajout d'un chemin
    }

    void RenderCache2D::AddBezier(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add Bezier curve without starting a command");
        }
        // Implementation de l'ajout d'une courbe de Bézier
    }

    void RenderCache2D::AddCurve(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add curve without starting a command");
        }
        // Implementation de l'ajout d'une courbe
    }

    void RenderCache2D::AddPoint(const maths::Vector2f& position, const maths::Vector4f& color, const maths::Vector2f& uv1)
    {
    }

    void RenderCache2D::AddLine(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add line without starting a command");
        }
        currentCommand->AddVertex(start, color, uv1);
        currentCommand->AddVertex(end, color, uv2);
        currentCommand->AddIndex(0);
        currentCommand->AddIndex(1);
    }

    void RenderCache2D::AddLine(const maths::Vector2f& point, const maths::Vector2f& direction, float32 length, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2) {
        maths::Vector2f start = point;
        maths::Vector2f end = point + direction * length;
        AddLine(start, end, color, uv1, uv2);
    }

    void RenderCache2D::AddSegment(const maths::Vector2f& start, const maths::Vector2f& end, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2) {
        AddLine(start, end, color, uv1, uv2);
    }

    void RenderCache2D::AddRectangle(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector4f& color, const maths::Vector2f& uv1, const maths::Vector2f& uv2, const maths::Vector2f& uv3, const maths::Vector2f& uv4) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add rectangle without starting a command");
        }
        maths::Vector2f topLeft = position;
        maths::Vector2f topRight = position + maths::Vector2f(size.x, 0);
        maths::Vector2f bottomLeft = position + maths::Vector2f(0, size.y);
        maths::Vector2f bottomRight = position + size;
        currentCommand->AddVertex(topLeft, color, uv1);
        currentCommand->AddVertex(topRight, color, uv2);
        currentCommand->AddVertex(bottomRight, color, uv3);
        currentCommand->AddVertex(bottomLeft, color, uv4);
        currentCommand->AddIndex(0);
        currentCommand->AddIndex(1);
        currentCommand->AddIndex(2);
        currentCommand->AddIndex(2);
        currentCommand->AddIndex(3);
        currentCommand->AddIndex(0);
    }

    void RenderCache2D::AddCircle(const maths::Vector2f& center, float32 radius, const maths::Vector4f& color, const maths::Vector2f& uv) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add circle without starting a command");
        }
        int32 segments = 32;
        float32 angle = 0;
        float32 angleIncrement = 2 * maths::Pi / segments;
        for (int32 i = 0; i < segments; i++) {
            maths::Vector2f point = center + maths::Vector2f(cos(angle), sin(angle)) * radius;
            currentCommand->AddVertex(point, color, uv);
            angle += angleIncrement;
        }
        for (int32 i = 0; i < segments; i++) {
            currentCommand->AddIndex(i);
            currentCommand->AddIndex((i + 1) % segments);
            currentCommand->AddIndex(segments);
        }
    }

    void RenderCache2D::AddEllipse(const maths::Vector2f& center, const maths::Vector2f& radius, const maths::Vector4f& color, const maths::Vector2f& uv) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add ellipse without starting a command");
        }
        int32 segments = 32;
        float32 angle = 0;
        float32 angleIncrement = 2 * maths::Pi / segments;
        for (int32 i = 0; i < segments; i++) {
            maths::Vector2f point = center + maths::Vector2f(cos(angle) * radius.x, sin(angle) * radius.y);
            currentCommand->AddVertex(point, color, uv);
            angle += angleIncrement;
        }
        for (int32 i = 0; i < segments; i++) {
            currentCommand->AddIndex(i);
            currentCommand->AddIndex((i + 1) % segments);
            currentCommand->AddIndex(segments);
        }
    }

    void RenderCache2D::AddPath(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, const std::vector<maths::Vector2f>& uvs, bool closed) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot add path without starting a command");
        }
        for (int32 i = 0; i < points.size(); i++) {
            currentCommand->AddVertex(points[i], color, uvs[i]);
        }
        if (closed) {
            for (int32 i = 0; i < points.size(); i++) {
                currentCommand->AddIndex(i);
                currentCommand->AddIndex((i + 1) % points.size());
                currentCommand->AddIndex(points.size());
            }
        }
        else {
            for (int32 i = 0; i < points.size() - 1; i++) {
                currentCommand->AddIndex(i);
                currentCommand->AddIndex(i + 1);
                currentCommand->AddIndex(points.size());
            }
        }
    }

    void RenderCache2D::AddBezier(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, const std::vector<maths::Vector2f>& uvs) {
        if (points.size() < 3) {
            throw std::runtime_error("A Bezier curve must have at least 3 points");
        }
        if (points.size() != uvs.size()) {
            throw std::runtime_error("The number of points and uvs must be equal");
        }
        if (!currentCommand) {
            throw std::runtime_error("Cannot add Bezier curve without starting a command");
        }

        int32 segments = 16;
        for (int32 i = 0; i < points.size() - 1; i++) {
            maths::Vector2f prevPoint = points[i];
            maths::Vector2f prevPrevPoint = points[i == 0 ? points.size() - 1 : i - 1];
            for (int32 j = 1; j <= segments; j++) {
                float32 t = j / (float32)segments;
                float32 t2 = t * t;
                float32 t3 = t2 * t;
                maths::Vector2f p = (1 - t) * ((1 - t) * prevPrevPoint + t * prevPoint) + t * points[i + 1];
                currentCommand->AddVertex(p, color, uvs[i]);
            }
        }
        for (int32 i = 0; i < points.size() - 1; i++) {
            for (int32 j = 1; j <= segments; j++) {
                currentCommand->AddIndex(i * segments + j - 1);
                currentCommand->AddIndex((i + 1) * segments + j - 1);
                currentCommand->AddIndex((i + 1) * segments + j - 2);
                currentCommand->AddIndex(i * segments + j - 1);
                currentCommand->AddIndex((i + 1) * segments + j - 2);
                currentCommand->AddIndex(i * segments + j - 2);
            }
        }
    }

    void RenderCache2D::AddCurve(const std::vector<maths::Vector2f>& points, const maths::Vector4f& color, const std::vector<maths::Vector2f>& uvs) {
        if (points.size() < 2) {
            throw std::runtime_error("A curve must have at least 2 points");
        }
        if (points.size() != uvs.size()) {
            throw std::runtime_error("The number of points and uvs must be equal");
        }
        if (!currentCommand) {
            throw std::runtime_error("Cannot add curve without starting a command");
        }
        int32 segments = 16;
        for (int32 i = 0; i < points.size() - 1; i++) {
            maths::Vector2f prevPoint = points[i];
            for (int32 j = 1; j <= segments; j++) {
                float32 t = j / (float32)segments;
                maths::Vector2f p = (1 - t) * prevPoint + t * points[i + 1];
                currentCommand->AddVertex(p, color, uvs[i]);
            }
        }
        for (int32 i = 0; i < points.size() - 1; i++) {
            for (int32 j = 1; j <= segments; j++) {
                currentCommand->AddIndex(i * segments + j - 1);
                currentCommand->AddIndex((i + 1) * segments + j - 1);
                currentCommand->AddIndex((i + 1) * segments + j - 2);
                currentCommand->AddIndex(i * segments + j - 1);
                currentCommand->AddIndex((i + 1) * segments + j - 2);
                currentCommand->AddIndex(i * segments + j - 2);
            }
        }
    }*/

    void RenderCache2D::SetBorderColor(const maths::Vector4f& color) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot set border color without starting a command");
        }
        currentCommand->SetBorderColor(color);
    }

    void RenderCache2D::SetBorderWidth(float32 width) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot set border width without starting a command");
        }
        currentCommand->SetBorderWidth(width);
    }

    void RenderCache2D::SetBlendMode(BlendMode2D mode) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot set blend mode without starting a command");
        }
        currentCommand->SetBlendMode(mode);
    }

    void RenderCache2D::SetClipRegion(const ClipRegion& region) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot set clip region without starting a command");
        }
        currentCommand->SetClipRegion(region);
    }

    void RenderCache2D::SetTransform(const maths::matrix4& transform) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot set transform without starting a command");
        }
        currentCommand->SetTransform(transform);
    }

    void RenderCache2D::SetTexture(uint32 textureId) {
        if (!currentCommand) {
            throw std::runtime_error("Cannot set texture without starting a command");
        }
        currentCommand->SetTextureId(textureId);
    }

}  //  nkentseu