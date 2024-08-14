//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-07-27 at 09:07:41 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_TEXT_H__
#define __NKENTSEU_TEXT_H__

#pragma once

#include <NTSCore/System.h>
#include "Font.h"

namespace nkentseu {

    struct NKENTSEU_API TextGeometry {
        std::vector<maths::Vector3f>    positions;
        std::vector<maths::Vector2f>    texCords;
        std::vector<maths::Vector4f>    colors;
        std::vector<uint32>             indices;

        void AddVertice(const maths::Vector3f& position, const maths::Vector2f& texCod) {
            positions.push_back(position);
            texCords.push_back(texCod);
        }

        void AddIndice(uint32 indice) {
            indices.push_back(indice);
        }

        void AddIndices(const std::vector<uint32>& indices) {
            this->indices.insert(this->indices.end(), indices.begin(), indices.end());
        }

        void Clear() {
            positions.clear();
            texCords.clear();
            indices.clear();
        }
    };

    class NKENTSEU_API TextShape {
        public:
            std::string             m_Content; // Contenu du texte
            Memory::Shared<Font>    m_Font; // Police utilis�e
            maths::Color            m_Color; // Couleur du texte
            float32                 m_CharacterSpacing; // Espacement entre les caract�res
            float32                 m_LineSpacing; // Espacement entre les lignes
            maths::Vector2f         m_Position; // Position du texte
            maths::Vector2f         m_ClampBounds; // Limites de clampage si 0, 0 alors pas de clamp activer
            bool                    m_HasBorder; // Si le texte a une bordure
            maths::Color            m_BorderColor; // Couleur de la bordure
            float32                 m_BorderWidth; // Largeur de la bordure
            maths::FloatRect        m_Bounds;
            TextStyle               m_Style = TextStyle::Enum::Regular;
            //bool                    m_GeometryNeedUpdate = true;
                
            // Propri�t�s suppl�mentaires
            TextAlign               m_HorizontalAlign = TextAlign::Enum::Left; // Alignement horizontal
            TextAlign               m_VerticalAlign = TextAlign::Enum::Top;    // Alignement vertical
            float32                 m_Rotation = 0.0f; // Rotation du texte en degr�s
            float32                 m_Opacity = 1.0f; // Opacit� du texte (0.0 � 1.0)
            bool                    m_WordWrap = true; // Activer le retour � la ligne automatique
            bool                    m_Justify = false; // Justification du texte
            bool                    m_UseGradient = false; // Utiliser un d�grad� pour le texte
            maths::Color            m_GradientStartColor; // Couleur de d�but du d�grad�
            maths::Color            m_GradientEndColor;   // Couleur de fin du d�grad�
            bool                    m_UseTexture = false; // Utiliser une texture pour le texte
            Memory::Shared<Texture> m_Texture; // Texture pour le texte
            bool                    m_EnableKerning = true; // Activer le kerning
            bool                    m_EnableLigatures = true; // Activer les ligatures
            float32                 m_MarginLeft = 0.0f;   // Marge � gauche
            float32                 m_MarginRight = 0.0f;  // Marge � droite
            float32                 m_MarginTop = 0.0f;    // Marge en haut
            float32                 m_MarginBottom = 0.0f; // Marge en bas
            float32                 m_MaxWidth;
    };

    /*class NKENTSEU_API TextShape {
        public:
            TextShape(const Memory::Shared<Font>& font, const std::string& string = "", uint32 characterSize = 30);

            const TextGeometry& GetGeometry();
            const TextGeometry& GetOutlineGeometry();

            void SetString(const std::string& string);
            void SetFont(const Memory::Shared<Font>& font);
            void SetCharacterSize(uint32 size);
            void SetLineSpacing(float32 spacingFactor);
            void SetLetterSpacing(float32 spacingFactor);
            void SetStyle(uint32 style);
            void SetOutlineThickness(float32 thickness);
            const std::string& GetString() const;
            const Memory::Shared<Font>& GetFont() const;
            uint32 GetCharacterSize() const;
            float32 GetLetterSpacing() const;
            float32 GetLineSpacing() const;
            uint32 GetStyle() const;
            float32 GetOutlineThickness() const;
            maths::Vector2f FindCharacterPos(usize index) const;
            maths::FloatRect GetLocalBounds() const;
            maths::FloatRect GetGlobalBounds() const;

        private:
            std::string                     m_String;
            Memory::Shared<Font>            m_Font = nullptr;
            uint32                          m_CharacterSize = 30;            
            float32                         m_LetterSpacingFactor = 1.0f;             
            float32                         m_LineSpacingFactor = 1.0f;               
            TextStyle                       m_Style = TextStyle::Enum::Regular;
            float32                         m_OutlineThickness = 0.f;
            TextGeometry                    m_Vertices;
            TextGeometry                    m_OutlineVertices;
            maths::FloatRect                m_Bounds;
            bool                            m_GeometryNeedUpdate = true;
            TextureID                       m_FontTextureId = nullptr;

            void AddLine(const maths::Vector2f& p1, const maths::Vector2f& p2, float32 lineWidth = 1.0f, maths::Vector2f uv0 = {}, maths::Vector2f uv1 = {});
            void AddGlyphQuad(maths::Vector2f position, const maths::Color& color, const Glyph& glyph, float32 italicShear);

            void EnsureGeometryUpdate();
    };*/

}  //  nkentseu

#endif  // __NKENTSEU_TEXT_H__!