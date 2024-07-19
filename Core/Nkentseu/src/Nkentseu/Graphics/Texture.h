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
#include "GraphicsEnum.h"

namespace nkentseu {

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
            virtual void SetTextureFormat(TextureFormat textureFormat) = 0;
            virtual TextureFormat GetTextureFormat() const = 0;
            virtual void SetRepeated(bool repeated) = 0;
            virtual bool IsRepeated() const = 0;
            virtual bool GenerateMipmap() = 0;
            virtual void Swap(Texture& right) noexcept = 0;
            virtual void InvalidateMipmap() = 0;
            virtual void Bind(const std::string& name) = 0;
            virtual void Bind(uint32 binding) = 0;
            virtual const std::filesystem::path& GetPath() const = 0;
        private:
        protected:
    };

    class NKENTSEU_API Texture2D : public Texture {
    public:
        virtual bool Create(TextureFormat textureFormat, const maths::Vector2u& size) = 0;
        virtual bool Create(const std::filesystem::path& filename, const maths::IntRect& area = maths::IntRect()) = 0;
        virtual bool Create(const void* data, usize size, const maths::IntRect& area = maths::IntRect()) = 0;
        virtual bool Create(InputStream& stream, const maths::IntRect& area = maths::IntRect()) = 0;
        virtual bool Create(const Image& image, const maths::IntRect& area = maths::IntRect()) = 0;

        virtual bool Destroy() = 0;

        static uint32 GetMaximumSize(Memory::Shared<Context> context);
        static uint32 GetValidSize(Memory::Shared<Context> constext, uint32 size);

        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, TextureFormat textureFormat, const maths::Vector2u& size);
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const std::filesystem::path& filename, const maths::IntRect& area = maths::IntRect());
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const void* data, usize size, const maths::IntRect& area = maths::IntRect());
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, InputStream& stream, const maths::IntRect& area = maths::IntRect());
        static Memory::Shared<Texture2D> Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const Image& image, const maths::IntRect& area = maths::IntRect());
    };

}  //  nkentseu

#endif  // __TEXTURE_H__!