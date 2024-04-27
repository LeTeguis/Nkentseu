//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/19/2024 at 12:16:32 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_IMAGE_H__
#define __NKENTSEU_IMAGE_H__

#pragma once

#include "System/System.h"
#include <Ntsm/Vector/Vector2.h>

#include "Color.h"
#include "Operation.h"
#include <Ntsm/Shapes/Rectangle.h>

namespace nkentseu {
	class NKENTSEU_API Image {
	public:
		Image();
		Image(const Image& image);
		~Image();
		Image& operator=(const Image& image);

		bool LoadFromFile(const std::string& path, bool flipVertical = true);
		bool LoadFromStream(std::istream& stream, bool flipVertical = true);
		bool LoadFromMemory(const std::vector<uint8_t>& imageData, bool flipVertical = true);

		bool Create(const Vector2u& size, uint32 channels = 4);
		bool Create(uint32 width, uint32 height, uint32 channels = 4);

		Image Clone() const;

		Vector2u GetSize() const;
		uint32 GetChannels() const;
		uint8* GetPixels() const;

		bool Save(const std::string& path, bool flipVertical = true);

		uint32 GetPixel(const Vector2i& position) const;
		uint32 GetPixel(int32 x, int32 y) const;

		uint8 GetPixelRed(const Vector2i& position) const;
		uint8 GetPixelRed(int32 x, int32 y) const;

		uint8 GetPixelGreen(const Vector2i& position) const;
		uint8 GetPixelGreen(int32 x, int32 y) const;

		uint8 GetPixelBlue(const Vector2i& position) const;
		uint8 GetPixelBlue(int32 x, int32 y) const;

		uint8 GetPixelAlpha(const Vector2i& position) const;
		uint8 GetPixelAlpha(int32 x, int32 y) const;

		Color GetColor(const Vector2i& position) const;
		Color GetColor(int32 x, int32 y) const;

		bool SetPixel(const Vector2i& position, uint8 r, uint8 g, uint8 b, uint8 a);
		bool SetPixel(const Vector2i& position, uint8 r, uint8 g, uint8 b);
		bool SetPixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b, uint8 a);
		bool SetPixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b);

		bool SetColor(const Vector2i& position, const Color& color);
		bool SetColor(int32 x, int32 y, const Color& color);

		// Modification de la teinte d'une image par une autre
		void ModifyHue(const Image& hueImage);

		// R�cup�rer une partie de l'image
		Image GetSubImage(const Vector2i& position, const Vector2u& size) const;

		// Convertir une image en niveaux de gris
		void ConvertToGrayscale();

		// Convertir une image en noir et blanc
		void ConvertToBlackAndWhite(uint8 threshold = 128);

		// Generer une image aleatoire
		void GenerateRandomImage();
		void GenerateRandomImageWithAlpha();

		// Generer une heightmap aleatoire
		void GenerateRandomHeightmap();

		// Assombrir une image
		Image Darken(float32 amount = 0.5f) const;

		// eclaircir une image
		Image Lighten(float32 amount = 0.5f) const;

		void AddColor(const Color& color, float32 amount);
		void AttenuateColor(const Color& color, float32 amount);

		void ReplaceColor(const Color& colorToReplace, const Color& newColor);

		void FindBoundingBox(Vector2i point, Color color, Vector2i& bbox_position, Vector2u& bbox_size);
		std::vector<Rectangle> FindAllBoundingBox(Color color);

		void FindBoundingBoxOptimize(Vector2i point, Color color, Vector2i& bbox_position, Vector2u& bbox_size);
		std::vector<Rectangle> FindAllBoundingBoxOptimize(Color color);

		Image FindSubImage(Vector2i point, Color color);
		std::vector<Image> FindAllSubImage(Color color);

		std::vector<Image> FindAllSubImageOptimize(Color color);
		Image FindSubImageOptimize(Vector2i point, Color color);

		Image DoOperation(Image& other, Operation::Code operation);
		Image DoOperation(Image& other, Vector2i otherPosition, Operation::Code operation);
	private:
		Vector2u m_Size;
		uint32 m_Channels;
		uint8* m_Data;
	};
} // namespace nkentseu

#endif    // __NKENTSEU_IMAGE_H__