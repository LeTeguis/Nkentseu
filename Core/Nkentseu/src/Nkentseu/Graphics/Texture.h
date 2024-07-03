//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-16 at 11:22:57 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#pragma once

#include <System/System.h>
#include <System/Stream/InputStream.h>
#include <System/Definitions/Memory.h>

#include <string>
#include <atomic>
#include <filesystem>

#include <Ntsm/Vector/Vector2.h>
#include <Ntsm/Shapes/Rectangle.h>
#include <Ntsm/Matrix/Matrix4.h>

#include "Color.h"
#include "Image.h"
#include "Context.h"
#include "ShaderInputLayout.h"

namespace nkentseu {

    struct NKENTSEU_API TextCord {
        using Code = uint32;
        enum : Code {
            Normalized, //!< Texture coordinates in range [0 .. 1]
            Pixels      //!< Texture coordinates in range [0 .. size]
        };
    };

    struct NKENTSEU_API TextureFormat {
        using Code = uint32;
        enum : Code {
            None = 0,

            // Formats de couleur
            RGBA8,                 // Couleur RGBA 8 bits non normalisee
            RGB8,                  // Couleur RGB 8 bits non normalisee
            SRGB8_ALPHA8,          // Couleur sRGB avec canal alpha 8 bits non normalisee
            RED_INTEGER,

            // Formats de profondeur
            DEPTH_COMPONENT16,     // Profondeur 16 bits non normalisee
            DEPTH_COMPONENT24,     // Profondeur 24 bits non normalisee
            DEPTH_COMPONENT32F,    // Profondeur 32 bits en virgule flottante

            // Formats de stencil
            STENCIL_INDEX8,        // Index de stencil 8 bits
            DEPTH24_STENCIL8,      // Profondeur 24 bits avec index de stencil 8 bits
            DEPTH32F_STENCIL8      // Profondeur 32 bits en virgule flottante avec index de stencil 8 bits
        };

        // Verifier si un format est un format de couleur
        static bool IsColor(TextureFormat::Code format);

        // Verifier si un format est un format de profondeur
        static bool IsDepth(TextureFormat::Code format);

        // Verifier si un format est un format de stencil
        static bool IsStencil(TextureFormat::Code format);
    };

    class NKENTSEU_API Texture {
        public:
            virtual maths::Vector2u GetSize() const = 0;
            virtual Color* GetColors() = 0;
            virtual uint8* GetPixels() = 0;
            virtual Image CopyToImage() const = 0;
            virtual void Update(const uint8* pixels) = 0;
            virtual void Update(const uint8* pixels, const maths::Vector2u& size, const maths::Vector2i& dest) = 0;
            virtual void Update(const Texture& texture) = 0;
            virtual void Update(const Texture& texture, const maths::Vector2i& dest) = 0;
            virtual void Update(const Image& image) = 0;
            virtual void Update(const Image& image, const maths::Vector2i& dest) = 0;
            virtual void SetSmooth(bool smooth) = 0;
            virtual bool IsSmooth() const = 0;
            virtual void SetTextureFormat(TextureFormat::Code textureFormat) = 0;
            virtual TextureFormat::Code GetTextureFormat() const = 0;
            virtual void SetRepeated(bool repeated) = 0;
            virtual bool IsRepeated() const = 0;
            virtual bool GenerateMipmap() = 0;
            virtual void Swap(Texture& right) noexcept = 0;
            virtual void InvalidateMipmap() = 0;
            virtual maths::matrix4f Binds(uint32 slot, TextCord::Code coordinateType = TextCord::Normalized) = 0;
            virtual const std::filesystem::path& GetPath() const = 0;
        private:
        protected:
    };

    class NKENTSEU_API Texture2D : public Texture {
    public:
        virtual bool Create(TextureFormat::Code textureFormat, const maths::Vector2u& size) = 0;
        virtual bool Create(const std::filesystem::path& filename, const maths::IntRect& area = maths::IntRect()) = 0;
        virtual bool Create(const void* data, usize size, const maths::IntRect& area = maths::IntRect()) = 0;
        virtual bool Create(InputStream& stream, const maths::IntRect& area = maths::IntRect()) = 0;
        virtual bool Create(const Image& image, const maths::IntRect& area = maths::IntRect()) = 0;

        virtual void Destroy() = 0;
        virtual bool Bind() = 0;
        virtual bool Unbind() = 0;

        static uint32 GetMaximumSize(Memory::Shared<Context> context);
        static uint32 GetValidSize(Memory::Shared<Context> constext, uint32 size);

        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, TextureFormat::Code textureFormat, const maths::Vector2u& size);
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const std::filesystem::path& filename, const maths::IntRect& area = maths::IntRect());
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const void* data, usize size, const maths::IntRect& area = maths::IntRect());
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, InputStream& stream, const maths::IntRect& area = maths::IntRect());
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const Image& image, const maths::IntRect& area = maths::IntRect());
    };

}  //  nkentseu

#endif  // __TEXTURE_H__!