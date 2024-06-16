//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:22:57 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include <string>
#include <Ntsm/Vector/Vector2.h>
#include <Ntsm/Shapes/Rectangle.h>

#include "Color.h"
#include "Image.h"

namespace nkentseu {
    
    class NKENTSEU_API Texture {
        public:
            static Memory::Shared<Texture> Create(const std::string& filePath);
            static Memory::Shared<Texture> Create(const Color* pixel, const Vector2u& size);
            static Memory::Shared<Texture> Create(const Color* pixel, const Vector2u& size, const IntRect& area);
            static Memory::Shared<Texture> Create(const Color& pixel, const Vector2u& size);
            static Memory::Shared<Texture> Create(const Image& image);
            static Memory::Shared<Texture> Create(const Image& image, const IntRect& area);

            virtual Vector2u GetSize() = 0;
            virtual Color* GetPixels() = 0;
            virtual Image GetImage() = 0;

            virtual bool Clear(const Color& color) = 0;

            virtual bool SetPixel(const Color& color, const Vector2u& position) = 0;

            virtual bool Update(const Color& color, const Vector2u& position) = 0;
            virtual bool Update(const Image& image, const IntRect& src, const IntRect& dts) = 0;
            virtual bool Update(const Image& image, const Vector2u& dtsPos) = 0;
            virtual bool Update(Memory::Shared<Texture> texture, const Vector2u& dtsPos, const Vector2u& dtsSize) = 0;
            virtual bool Update(Memory::Shared<Texture> texture, const Vector2u& dtsPos) = 0;
            virtual bool Update(const Color* pixel, const Vector2u& size, const Vector2u& dtsPos) = 0;
            virtual bool Update(const Color* pixel, const Vector2u& size, const Vector2u& dtsPos, const Vector2u& dtsSize) = 0;
        private:
        protected:
    };

}  //  nkentseu

#endif  // __TEXTURE_H__!