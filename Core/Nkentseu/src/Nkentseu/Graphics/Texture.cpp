//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:22:57 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Texture.h"
#include <Logger/Formatter.h>

#include "Internal/Opengl/OpenglTexture.h"
#include "Internal/Vulkan/VulkanTexture.h"

namespace nkentseu {
	using namespace maths;

	bool TextureFormat::IsColor(TextureFormat::Code format)
	{
		return format == TextureFormat::RGBA8			||
			   format == TextureFormat::RGB8			||
			   format == TextureFormat::SRGB8_ALPHA8	||
			   format == TextureFormat::RED_INTEGER;
	}

	bool TextureFormat::IsDepth(TextureFormat::Code format)
	{
		return format == TextureFormat::DEPTH_COMPONENT16 ||
			   format == TextureFormat::DEPTH_COMPONENT24 ||
			   format == TextureFormat::DEPTH_COMPONENT32F;
	}

	bool TextureFormat::IsStencil(TextureFormat::Code format)
	{
		return format == TextureFormat::STENCIL_INDEX8		||
			   format == TextureFormat::DEPTH24_STENCIL8	||
			   format == TextureFormat::DEPTH32F_STENCIL8;
	}

	uint32 Texture2D::GetMaximumSize(Memory::Shared<Context> context)
	{
		return uint32();
	}

	uint32 Texture2D::GetValidSize(Memory::Shared<Context> constext, uint32 size)
	{
		return uint32();
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context)
	{
		Memory::Shared<Texture2D> texture = nullptr;
		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			// texture = Memory::Alloc<VukanTexture2D>(context);
		}
		else if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			texture = Memory::Alloc<OpenglTexture2D>(context);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, TextureFormat::Code textureFormat, const maths::Vector2u& size)
	{
		Memory::Shared<Texture2D> texture = Create(context);

		if (texture == nullptr || !texture->Create(textureFormat, size)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, const std::filesystem::path& filename, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context);

		if (texture == nullptr || !texture->Create(filename, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, const void* data, usize size, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context);

		if (texture == nullptr || !texture->Create(data, size, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, InputStream& stream, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context);

		if (texture == nullptr || !texture->Create(stream, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, const Image& image, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context);

		if (texture == nullptr || !texture->Create(image, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

}  //  nkentseu