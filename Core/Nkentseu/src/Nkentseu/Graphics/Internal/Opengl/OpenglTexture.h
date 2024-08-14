//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:23:31 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_TEXTURE_H__
#define __OPENGL_TEXTURE_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/Texture.h"
#include "OpenglContext.h"

namespace nkentseu {
    
    class NKENTSEU_API OpenglTexture2D : public Texture2D {
    public:
		OpenglTexture2D(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);
        ~OpenglTexture2D();

		bool Create(TextureFormat textureFormat, const maths::Vector2u& size) override;
		bool Create(const std::filesystem::path& filename, const maths::IntRect& area = maths::IntRect()) override;
		bool Create(const void* data, std::size_t size, const maths::IntRect& area = maths::IntRect()) override;
		bool Create(InputStream& stream, const maths::IntRect& area = maths::IntRect()) override;
		bool Create(const Image& image, const maths::IntRect& area = maths::IntRect()) override;
		virtual Memory::Shared<Texture2D> Clone(Memory::Shared<ShaderInputLayout> sil) override;
		virtual void* InternalID(Memory::Shared<ShaderInputLayout> sil, uint32 slot) override;

		Color* GetColors() override;
		uint8* GetPixels() override;

		maths::Vector2u GetSize() const override;
		Image CopyToImage() const override;
		void Update(const std::uint8_t* pixels) override;
		void Update(const std::uint8_t* pixels, const maths::Vector2u& size, const maths::Vector2i& dest) override;
		void Update(const Texture& texture) override;
		void Update(const Texture& texture, const maths::Vector2i& dest) override;
		void Update(const Image& image) override;
		void Update(const Image& image, const maths::Vector2i& dest) override;
		void SetSmooth(bool smooth) override;
		bool IsSmooth() const override;
		void SetTextureFormat(TextureFormat textureFormat) override;
		TextureFormat GetTextureFormat() const override;
		void SetRepeated(bool repeated) override;
		bool IsRepeated() const override;
		bool GenerateMipmap() override;
		void Swap(Texture& right) noexcept override;
		void InvalidateMipmap() override;
		void Bind(const std::string& name) override;
		void Bind(uint32 binding) override;
		const std::filesystem::path& GetPath() const override;

        static uint32 GetMaximumSize(Memory::Shared<Context> context);
        static uint32 GetValidSize(Memory::Shared<Context> context, uint32 size);

		bool Destroy() override;
    private:
		friend class OpenglTexture2DBinding;

		SamplerInputLayout m_InputLayout;
        Memory::Shared<OpenglContext> m_Context;
        TextureFormat m_Format = TextureFormat::Enum::RGBA8;

        std::filesystem::path m_Path;
        uint64_t m_CacheId;
        uint32 m_Handle = 0;

        bool				m_IsRepeated{};
        bool				m_IsSmooth{};
        maths::Vector2u     m_Size;
        maths::Vector2u     m_ActualSize;
        mutable bool		m_PixelsFlipped{};
        bool				m_FboAttachment{};
        bool				m_HasMipmap{};
    };

	class NKENTSEU_API OpenglTexture2DBinding : public Texture2DBinding {
	public:
		OpenglTexture2DBinding(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);
		bool Initialize(Memory::Shared<Texture2D> texture) override;
		bool Destroy() override;
		void Bind(const std::string& name) override;
		void Bind(uint32 binding) override;
		bool Equal(Memory::Shared<Texture2DBinding> binding) override;
		bool IsDefined(Memory::Shared<Texture2D> binding) override;

	private:
		uint32 m_Handle = 0;
		Memory::Shared<OpenglContext> m_Context;
		SamplerInputLayout m_InputLayout;
	};

}  //  nkentseu

#endif  // __OPENGL_TEXTURE_H__!