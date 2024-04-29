//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 12:16:32 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Image.h"
#include <Ntsm/Random.h>

#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>
#include <wchar.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <algorithm>

namespace nkentseu {

    Image::Image() : m_Size(0, 0), m_Channels(0), m_Data(nullptr) {}

    Image::Image(const Image& image) : m_Size(image.m_Size), m_Channels(image.m_Channels), m_Data(nullptr) {
        if (image.m_Data) {
            m_Data = new uint8[m_Size.x * m_Size.y * m_Channels];
            memcpy(m_Data, image.m_Data, m_Size.x * m_Size.y * m_Channels);
        }
    }

    Image::~Image() {
        if (m_Data) {
            delete[] m_Data;
        }
    }

    Image& Image::operator=(const Image& image) {
        if (this != &image) {
            if (m_Data) {
                delete[] m_Data;
            }

            m_Size = image.m_Size;
            m_Channels = image.m_Channels;

            if (image.m_Data) {
                m_Data = new uint8[m_Size.x * m_Size.y * m_Channels];
                memcpy(m_Data, image.m_Data, m_Size.x * m_Size.y * m_Channels);
            }
            else {
                m_Data = nullptr;
            }
        }

        return *this;
    }

    bool Image::LoadFromFile(const std::string& path, bool flipVertical) {
        if (m_Data) {
            delete[] m_Data;
            m_Data = nullptr;
        }

        Vector2i size_;
        int32 channels;

        if (flipVertical) {
            stbi_set_flip_vertically_on_load(true);
        }

        m_Data = stbi_load(path.c_str(), &size_.x, &size_.y, &channels, 4);

        m_Size = (Vector2u)size_;
        m_Channels = (uint32)channels;

        return m_Data != nullptr;
    }

    bool Image::LoadFromStream(std::istream& stream, bool flipVertical) {
        if (m_Data) {
            delete[] m_Data;
            m_Data = nullptr;
        }
        Vector2i size_;
        int32 channels;

        stream.seekg(0, std::ios::end);
        std::streampos size = stream.tellg();
        stream.seekg(0, std::ios::beg);

        m_Data = new uint8[size];

        stream.read(reinterpret_cast<char*>(m_Data), size);

        stbi_set_flip_vertically_on_load(flipVertical);
        m_Data = stbi_load_from_memory(m_Data, size, &size_.x, &size_.y, &channels, 0);

        m_Size = (Vector2u)size_;
        m_Channels = (uint32)channels;

        return m_Data != nullptr;
    }

    bool Image::LoadFromMemory(const std::vector<uint8_t>& imageData, bool flipVertical) {
        if (!imageData.empty()) {
            // Utilisez std::istringstream pour creer un flux memoire
            std::istringstream stream(std::string(imageData.begin(), imageData.end()));

            // Chargez l'image depuis le flux m�moire
            if (LoadFromStream(stream, flipVertical)) {
                return true;
            }
        }

        return false;
    }

    bool Image::Create(const Vector2u& size, uint32 channels) {
        if (m_Data) {
            delete[] m_Data;
        }

        m_Size = size;
        m_Channels = channels;
        m_Data = new uint8[m_Size.x * m_Size.y * m_Channels];
        memset(m_Data, 0, m_Size.x * m_Size.y * m_Channels);

        return true;
    }

    bool Image::Create(uint32 width, uint32 height, uint32 channels) {
        return Create(Vector2u(width, height), channels);
    }

    Image Image::Clone() const {
        return Image(*this);
    }

    Vector2u Image::GetSize() const {
        return m_Size;
    }

    uint32 Image::GetChannels() const {
        return m_Channels;
    }

    uint8* Image::GetPixels() const {
        return m_Data;
    }

    bool Image::Save(const std::string& path, bool flipVertical) {
        if (!m_Data) {
            return false;
        }

        if (flipVertical) {
            stbi_flip_vertically_on_write(true);
        }
        return stbi_write_png(path.c_str(), m_Size.x, m_Size.y, m_Channels, m_Data, m_Size.x * m_Channels) != 0;
    }

    uint32 Image::GetPixel(const Vector2i& position) const {
        if (!m_Data || position.x >= m_Size.x || position.y >= m_Size.y || position.x < 0 || position.y < 0) {
            return 0;
        }

        uint32 pixelIndex = (position.y * m_Size.x + position.x) * m_Channels;

        uint32 pixelValue = 0;

        if (m_Channels == 1) {
            // Si le canal est 1, on suppose que c'est un niveau de gris
            uint8 grayValue = m_Data[pixelIndex];
            pixelValue = (grayValue) | (grayValue << 8) | (grayValue << 16) | 0xFF000000;
        }
        else if (m_Channels == 2) {
            // Si le canal est 2, on suppose que c'est un niveau de gris avec alpha
            uint8 grayValue = m_Data[pixelIndex];
            uint32 alphaValue = static_cast<uint32>(m_Data[pixelIndex + 1]);
            pixelValue = (grayValue) | (grayValue << 8) | (grayValue << 16) | (alphaValue << 24);
        }
        else {
            // Sinon, traitez comme d'habitude avec la boucle
            for (uint32 i = 0; i < m_Channels; ++i) {
                pixelValue |= static_cast<uint32>(m_Data[pixelIndex + i]) << (i * 8);

                if (i >= 4) {
                    break;
                }
            }
        }

        return pixelValue;
    }

    uint32 Image::GetPixel(int32 x, int32 y) const {
        return GetPixel(Vector2i(x, y));
    }

    uint8 Image::GetPixelRed(const Vector2i& position) const {
        if (m_Channels >= 1) {
            uint32 pixelIndex = (position.y * m_Size.x + position.x) * m_Channels;
            return m_Data[pixelIndex];
        }
        else {
            // Gestion d'erreur ou comportement par d�faut
            return 0;
        }
    }

    uint8 Image::GetPixelRed(int32 x, int32 y) const {
        return GetPixelRed(Vector2i(x, y));
    }

    uint8 Image::GetPixelGreen(const Vector2i& position) const {
        if (m_Channels >= 2) {
            uint32 pixelIndex = (position.y * m_Size.x + position.x) * m_Channels;
            return m_Data[pixelIndex + 1];
        }
        else {
            // Gestion d'erreur ou comportement par d�faut
            return 0;
        }
    }

    uint8 Image::GetPixelGreen(int32 x, int32 y) const {
        return GetPixelGreen(Vector2i(x, y));
    }

    uint8 Image::GetPixelBlue(const Vector2i& position) const {
        if (m_Channels >= 3) {
            uint32 pixelIndex = (position.y * m_Size.x + position.x) * m_Channels;
            return m_Data[pixelIndex + 2];
        }
        else {
            // Gestion d'erreur ou comportement par d�faut
            return 0;
        }
    }

    uint8 Image::GetPixelBlue(int32 x, int32 y) const {
        return GetPixelBlue(Vector2i(x, y));
    }

    uint8 Image::GetPixelAlpha(const Vector2i& position) const {
        if (m_Channels >= 4) {
            uint32 pixelIndex = (position.y * m_Size.x + position.x) * m_Channels;
            return m_Data[pixelIndex + 3];
        }
        else {
            // Gestion d'erreur ou comportement par d�faut
            return 255; // Par d�faut, l'alpha est souvent consid�r� comme 255 si non disponible
        }
    }

    uint8 Image::GetPixelAlpha(int32 x, int32 y) const {
        return GetPixelAlpha(Vector2i(x, y));
    }

    Color Image::GetColor(const Vector2i& position) const {
        uint32 pixel = GetPixel(position);
        return Color(
            static_cast<uint8>((pixel >> 0) & 0xFF),
            static_cast<uint8>((pixel >> 8) & 0xFF),
            static_cast<uint8>((pixel >> 16) & 0xFF),
            static_cast<uint8>((pixel >> 24) & 0xFF)
        );
    }

    Color Image::GetColor(int32 x, int32 y) const {
        return GetColor(Vector2i(x, y));
    }

    /*bool Image::SetPixel(const Vector2u& position, uint8 r, uint8 g, uint8 b, uint8 a) {
        if (!m_Data || position.x >= m_Size.x || position.y >= m_Size.y) {
            return false;
        }

        uint32 color = (a << 24) | (b << 16) | (g << 8) | r;
        *(reinterpret_cast<uint32*>(m_Data + (position.y * m_Size.x + position.x) * m_Channels)) = color;

        return true;
    }*/

    bool Image::SetPixel(const Vector2i& position, uint8 r, uint8 g, uint8 b, uint8 a) {
        if (!m_Data || position.x >= m_Size.x || position.y >= m_Size.y) {
            return false;
        }

        uint32 pixelIndex = (position.y * m_Size.x + position.x) * m_Channels;

        if (m_Channels == 4) {
            m_Data[pixelIndex] = r;
            m_Data[pixelIndex + 1] = g;
            m_Data[pixelIndex + 2] = b;
            m_Data[pixelIndex + 3] = a;
        }
        else if (m_Channels == 3) {
            m_Data[pixelIndex] = r;
            m_Data[pixelIndex + 1] = g;
            m_Data[pixelIndex + 2] = b;
        }
        else if (m_Channels == 1) {
            // Si l'image a un seul canal, utilisez la valeur de niveau de gris (r, g, ou b)
            m_Data[pixelIndex] = (r + g + b) / 3;
        }

        return true;
    }

    bool Image::SetPixel(const Vector2i& position, uint8 r, uint8 g, uint8 b) {
        return SetPixel(position, r, g, b, 255);
    }

    bool Image::SetPixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b, uint8 a) {
        return SetPixel(Vector2i(x, y), r, g, b, a);
    }

    bool Image::SetPixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b) {
        return SetPixel(Vector2i(x, y), r, g, b, 255);
    }

    bool Image::SetColor(const Vector2i& position, const Color& color) {
        return SetPixel(position, color.R(), color.G(), color.B(), color.A());
    }

    bool Image::SetColor(int32 x, int32 y, const Color& color) {
        return SetPixel(Vector2i(x, y), color.R(), color.G(), color.B(), color.A());
    }

    void Image::ModifyHue(const Image& hueImage) {
        if (!m_Data) {
        }

        for (uint32 y = 0; y < m_Size.y; ++y) {
            for (uint32 x = 0; x < m_Size.x; ++x) {
                SetColor(x, y, Color::FromHSV(GetColor(x, y).ToHSV() + hueImage.GetColor(x, y).ToHSV()));
            }
        }
    }

    Image Image::GetSubImage(const Vector2i& position, const Vector2u& size) const {
        if (!m_Data || position.x + size.x > m_Size.x || position.y + size.y > m_Size.y) {
            return Image();
        }

        Image subImage;

        if (subImage.Create(size, m_Channels)) {
            for (uint32 y = 0; y < size.y; ++y) {
                for (uint32 x = 0; x < size.x; ++x) {
                    subImage.SetColor(x, y, GetColor(position.x + x, position.y + y));
                }
            }
        }
        return subImage;
    }

    void Image::ConvertToGrayscale() {
        if (!m_Data) {
            return;
        }
        for (uint32 y = 0; y < m_Size.y; ++y) {
            for (uint32 x = 0; x < m_Size.x; ++x) {
                Color origColor = GetColor(x, y);
                SetColor(x, y, origColor.ToGrayscaleWithAlpha());
            }
        }
    }

    void Image::ConvertToBlackAndWhite(uint8 threshold) {
        if (!m_Data) {
            return;
        }
        for (uint32 y = 0; y < m_Size.y; ++y) {
            for (uint32 x = 0; x < m_Size.x; ++x) {
                Color origColor = GetColor(x, y);
                SetColor(x, y, origColor.ToGrayscaleWithAlpha());
            }
        }
    }

    void Image::GenerateRandomImage() {
        if (!m_Data) {
            return;
        }
        for (uint32 y = 0; y < m_Size.y; ++y) {
            for (uint32 x = 0; x < m_Size.x; ++x) {
                SetColor(x, y, Color::RandomRGB());
            }
        }
    }

    void Image::GenerateRandomImageWithAlpha() {
        if (!m_Data) {
            return;
        }
        for (uint32 y = 0; y < m_Size.y; ++y) {
            for (uint32 x = 0; x < m_Size.x; ++x) {
                SetColor(x, y, Color::RandomRGBA());
            }
        }
    }

    void Image::GenerateRandomHeightmap() {
        if (!m_Data) {
            return;
        }
        for (uint32 y = 0; y < m_Size.y; ++y) {
            for (uint32 x = 0; x < m_Size.x; ++x) {
                uint8 height = static_cast<uint8>(Random.NextUInt32(256));
                SetColor(x, y, Color(height, height, height, 255));
            }
        }
    }

    Image Image::Darken(float32 amount) const {
        if (!m_Data) {
            return Image();
        }
        Image darkenedImage = Clone();
        if (!darkenedImage.m_Data) {
            return Image();
        }
        uint8* pixels = darkenedImage.GetPixels();

        for (uint32 i = 0; i < m_Size.x * m_Size.y * m_Channels; i += m_Channels) {
            Color color(pixels[i], pixels[i + 1], pixels[i + 2]);
            color = color.Darken(amount); // Assombrir la couleur
            pixels[i] = color.R();
            pixels[i + 1] = color.G();
            pixels[i + 2] = color.B();
        }

        return darkenedImage;
    }

    Image Image::Lighten(float32 amount) const {
        if (!m_Data) {
            return Image();
        }
        Image lightenedImage = Clone();
        if (!lightenedImage.m_Data) {
            return Image();
        }
        uint8* pixels = lightenedImage.GetPixels();

        for (uint32 i = 0; i < m_Size.x * m_Size.y * m_Channels; i += m_Channels) {
            Color color(pixels[i], pixels[i + 1], pixels[i + 2]);
            color = color.Lighten(amount); // �claircir la couleur
            pixels[i] = color.R();
            pixels[i + 1] = color.G();
            pixels[i + 2] = color.B();
        }

        return lightenedImage;
    }

    void Image::AddColor(const Color& color, float32 amount) {
        if (!m_Data) {
            return;
        }
        uint8* pixels = GetPixels();

        for (uint32 i = 0; i < m_Size.x * m_Size.y * m_Channels; i += m_Channels) {
            // Ajouter la couleur
            pixels[i] += color.R() * amount;
            pixels[i + 1] += color.G() * amount;
            pixels[i + 2] += color.B() * amount;

            // Clamping
            pixels[i] = RangeUInt(0, 255).Clamp(pixels[i]);
            pixels[i + 1] = RangeUInt(0, 255).Clamp(pixels[i + 1]);
            pixels[i + 2] = RangeUInt(0, 255).Clamp(pixels[i + 2]);
        }
    }

    void Image::AttenuateColor(const Color& color, float32 amount) {
        if (!m_Data) {
            return;
        }
        uint8* pixels = GetPixels();

        for (uint32 i = 0; i < m_Size.x * m_Size.y * m_Channels; i += m_Channels) {
            Color currentColor(pixels[i], pixels[i + 1], pixels[i + 2]);

            // Att�nuer la couleur
            pixels[i] = (uint8)(currentColor.R() * (1.0f - amount));
            pixels[i + 1] = (uint8)(currentColor.G() * (1.0f - amount));
            pixels[i + 2] = (uint8)(currentColor.B() * (1.0f - amount));

            // Clamping
            pixels[i] = RangeUInt(0, 255).Clamp(pixels[i]);
            pixels[i + 1] = RangeUInt(0, 255).Clamp(pixels[i + 1]);
            pixels[i + 2] = RangeUInt(0, 255).Clamp(pixels[i + 2]);
        }
    }

    void Image::ReplaceColor(const Color& colorToReplace, const Color& newColor) {
        if (!m_Data) {
            return;
        }
        uint8* pixels = GetPixels();

        for (uint32 i = 0; i < m_Size.x * m_Size.y * m_Channels; i += m_Channels) {
            Color currentColor(pixels[i], pixels[i + 1], pixels[i + 2]);

            if (currentColor == colorToReplace) {
                pixels[i] = newColor.R();
                pixels[i + 1] = newColor.G();
                pixels[i + 2] = newColor.B();
            }
        }
    }

    void Image::FindBoundingBox(Vector2i point, Color color, Vector2i& bbox_position, Vector2u& bbox_size) {
        // Initialisation de la bo�te englobante
        bbox_position.x = 0;
        bbox_position.y = 0;
        bbox_size.w = 0;
        bbox_size.h = 0;

        if (!m_Data || GetColor(point) == color) {
            return;
        }
        Vector2i corner1 = point;
        Vector2i corner2 = point;

        // Utilisez un set pour suivre les points visit�s, permettant une v�rification rapide
        std::vector<Vector2i> visited;

        // Recherche des pixels de la couleur donn�e
        std::queue<Vector2i> queue;
        queue.push(point);

        while (!queue.empty()) {
            Vector2i currentPoint = queue.front();
            queue.pop();

            // Marquer le point comme visite
            visited.push_back(currentPoint);

            // Mise a jour de la boite englobante
            if (currentPoint.x < corner1.x) {
                corner1.x = currentPoint.x;
            }
            if (currentPoint.y < corner1.y) {
                corner1.y = currentPoint.y;
            }
            if (currentPoint.x > corner2.x) {
                corner2.x = currentPoint.x;
            }
            if (currentPoint.y > corner2.y) {
                corner2.y = currentPoint.y;
            }

            // Ajout des pixels adjacents a la queue, uniquement s'ils ne sont pas deja visites
            for (int32 i = -1; i <= 1; i++) {
                for (int32 j = -1; j <= 1; j++) {
                    Vector2i newPoint(currentPoint.x + i, currentPoint.y + j);
                    auto itr = std::find_if(visited.begin(), visited.end(), [&newPoint](const Vector2i& current) {
                        return newPoint == current;
                        });
                    if (itr == visited.end() &&
                        newPoint.x >= 0 && newPoint.x < m_Size.x && newPoint.y >= 0 && newPoint.y < m_Size.y &&
                        GetColor(newPoint) != color) {
                        queue.push(newPoint);
                    }
                }
            }
        }

        bbox_position = corner1;
        bbox_size = (Vector2u)(corner1 - corner2 + Vector2i(1, 1));
    }

    std::vector<Rectangle> Image::FindAllBoundingBox(Color color) {
        std::vector<Rectangle> boundingBoxes;
        if (!m_Data) {
            return boundingBoxes;
        }
        std::set<Vector2i> visited;

        for (int32 y = 0; y < m_Size.y; y++) {
            for (int32 x = 0; x < m_Size.x; x++) {
                Vector2i point(x, y);
                bool continue_ = false;

                for (auto& rect : boundingBoxes) {
                    if (rect.corner.x <= point.x && rect.corner.x + rect.size.x >= point.x &&
                        rect.corner.y <= point.y && rect.corner.y + rect.size.y >= point.y) {
                        continue_ = true;
                        break;
                    }
                }

                if (continue_) continue;

                Vector2i bbox_position;
                Vector2u bbox_size;
                FindBoundingBox(point, color, bbox_position, bbox_size);

                if (bbox_size != Vector2u(0, 0)) {
                    // Cr�er un Rectangle � partir des r�sultats
                    Rectangle bbox(bbox_position.x, bbox_position.y, bbox_size.w, bbox_size.h);
                    boundingBoxes.push_back(bbox);
                }
            }
        }

        return boundingBoxes;
    }

    std::vector<Rectangle> Image::FindAllBoundingBoxOptimize(Color color) {
        std::vector<Rectangle> boundingBoxes;
        if (!m_Data) {
            return boundingBoxes;
        }

        Image clone = Clone();
        if (!clone.m_Data) {
            return boundingBoxes;
        }

        for (int32 y = 0; y < m_Size.y; y++) {
            for (int32 x = 0; x < m_Size.x; x++) {
                Vector2i point(x, y);

                if (clone.GetColor(point) == color) {
                    continue;
                }

                bool continue_ = false;

                for (auto& rect : boundingBoxes) {
                    if (rect.corner.x <= point.x && rect.corner.x + rect.size.x >= point.x &&
                        rect.corner.y <= point.y && rect.corner.y + rect.size.y >= point.y) {
                        continue_ = true;
                        break;
                    }
                }

                if (continue_) continue;

                Vector2i bbox_position;
                Vector2u bbox_size;
                clone.FindBoundingBoxOptimize(point, color, bbox_position, bbox_size);

                if (bbox_size != Vector2u(0, 0)) {
                    // Cr�er un Rectangle � partir des r�sultats
                    Rectangle bbox(bbox_position.x, bbox_position.y, bbox_size.w, bbox_size.h);
                    boundingBoxes.push_back(bbox);
                }
            }
        }

        return boundingBoxes;
    }

    void Image::FindBoundingBoxOptimize(Vector2i point, Color color, Vector2i& bbox_position, Vector2u& bbox_size) {
        // Initialisation de la bo�te englobante
        bbox_position.x = 0;
        bbox_position.y = 0;
        bbox_size.w = 0;
        bbox_size.h = 0;

        if (!m_Data || GetColor(point) == color) {
            return;
        }
        SetColor(point, color);
        Vector2i corner1 = point;
        Vector2i corner2 = point;

        // Recherche des pixels de la couleur donn�e
        std::queue<Vector2i> queue;
        queue.push(point);

        while (!queue.empty()) {
            Vector2i currentPoint = queue.front();
            queue.pop();

            // Mise a jour de la bo�te englobante
            if (currentPoint.x < corner1.x) {
                corner1.x = currentPoint.x;
            }
            if (currentPoint.y < corner1.y) {
                corner1.y = currentPoint.y;
            }
            if (currentPoint.x > corner2.x) {
                corner2.x = currentPoint.x;
            }
            if (currentPoint.y > corner2.y) {
                corner2.y = currentPoint.y;
            }

            // Ajout des pixels adjacents � la queue, uniquement s'ils ne sont pas d�j� visit�s
            for (int32 i = -1; i <= 1; i++) {
                for (int32 j = -1; j <= 1; j++) {
                    Vector2i newPoint(currentPoint.x + i, currentPoint.y + j);
                    if (newPoint.x >= 0 && newPoint.x < m_Size.x && newPoint.y >= 0 && newPoint.y < m_Size.y && GetColor(newPoint) != color) {
                        queue.push(newPoint);
                        SetColor(newPoint, color);
                    }
                }
            }
        }

        bbox_position = corner1;
        bbox_size = (Vector2u)(corner1 - corner2 + Vector2i(1, 1));
    }

    Image Image::DoOperation(Image& other, Operation::Code operation) {
        Image image;

        if (!m_Data || !image.Create(m_Size)) {
            return Image();
        }

        // Parcours de l'image
        for (int32 y = 0; y < m_Size.y; y++) {
            for (int32 x = 0; x < m_Size.x; x++) {
                // Obtenez les couleurs des pixels
                Color color1 = GetColor(x, y);
                Color color2 = Color::Transparent;

                if (x < other.m_Size.x && y < other.m_Size.y) {
                    color2 = other.GetColor(x, y);
                }

                // Effectuez l'op�ration
                switch (operation) {
                case Operation::Add:
                    image.SetColor(x, y, color1 + color2);
                    break;
                case Operation::Multiply:
                    image.SetColor(x, y, color1 * color2);
                    break;
                case Operation::Subtract:
                    image.SetColor(x, y, color1 - color2);
                    break;
                case Operation::Divide:
                    image.SetColor(x, y, color1 / color2);
                    break;
                case Operation::Modulo:
                    image.SetColor(x, y, color1 % color2);
                    break;
                case Operation::And:
                    image.SetColor(x, y, color1 & color2);
                    break;
                case Operation::Or:
                    image.SetColor(x, y, color1 | color2);
                    break;
                case Operation::Xor:
                    image.SetColor(x, y, color1 ^ color2);
                    break;
                }
            }
        }
        return image;
    }

    Image Image::DoOperation(Image& other, Vector2i otherPosition, Operation::Code operation) {
        Image image;

        if (!m_Data || !image.Create(m_Size)) {
            return Image();
        }

        // Parcours de l'image
        for (int32 y = 0; y < m_Size.y; y++) {
            for (int32 x = 0; x < m_Size.x; x++) {
                // Obtenez les couleurs des pixels
                Color color1 = GetColor(x, y);
                Color color2 = Color::Transparent;

                if (x < other.m_Size.x && y < other.m_Size.y && x >= otherPosition.x && y >= otherPosition.y) {
                    color2 = other.GetColor(x, y);
                }

                // Effectuez l'op�ration
                switch (operation) {
                case Operation::Add:
                    image.SetColor(x, y, color1 + color2);
                    break;
                case Operation::Multiply:
                    image.SetColor(x, y, color1 * color2);
                    break;
                case Operation::Subtract:
                    image.SetColor(x, y, color1 - color2);
                    break;
                case Operation::Divide:
                    image.SetColor(x, y, color1 / color2);
                    break;
                case Operation::Modulo:
                    image.SetColor(x, y, color1 % color2);
                    break;
                case Operation::And:
                    image.SetColor(x, y, color1 & color2);
                    break;
                case Operation::Or:
                    image.SetColor(x, y, color1 | color2);
                    break;
                case Operation::Xor:
                    image.SetColor(x, y, color1 ^ color2);
                    break;
                }
            }
        }
        return image;
    }

}    // namespace nkentseu