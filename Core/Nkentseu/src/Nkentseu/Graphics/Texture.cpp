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

	uint32 Texture2D::GetMaximumSize(Memory::Shared<Context> context)
	{
		return uint32();
	}

	uint32 Texture2D::GetValidSize(Memory::Shared<Context> constext, uint32 size)
	{
		return uint32();
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil)
	{
		Memory::Shared<Texture2D> texture = nullptr;
		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			texture = Memory::Alloc<VulkanTexture2D>(context, sil);
		}
		else if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			texture = Memory::Alloc<OpenglTexture2D>(context, sil);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, TextureFormat textureFormat, const maths::Vector2u& size)
	{
		Memory::Shared<Texture2D> texture = Create(context, sil);

		if (texture == nullptr || !texture->Create(textureFormat, size)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const std::filesystem::path& filename, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context, sil);

		if (texture == nullptr || !texture->Create(filename, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const void* data, usize size, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context, sil);

		if (texture == nullptr || !texture->Create(data, size, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, InputStream& stream, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context, sil);

		if (texture == nullptr || !texture->Create(stream, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2D> Texture2D::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, const Image& image, const maths::IntRect& area)
	{
		Memory::Shared<Texture2D> texture = Create(context, sil);

		if (texture == nullptr || !texture->Create(image, area)) {
			Memory::Reset(texture);
		}
		return texture;
	}

	Memory::Shared<Texture2DBinding> Texture2DBinding::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil)
	{
		Memory::Shared<Texture2DBinding> textureBinding = nullptr;
		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			textureBinding = Memory::Alloc<VulkanTexture2DBinding>(context, sil);
		}
		else if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			textureBinding = Memory::Alloc<OpenglTexture2DBinding>(context, sil);
		}
		return textureBinding;
	}

	Memory::Shared<Texture2DBinding> Texture2DBinding::CreateInitialize(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, Memory::Shared<Texture2D> texture)
	{
		Memory::Shared<Texture2DBinding> textureBinding = Create(context, sil);

		if (textureBinding == nullptr || !textureBinding->Initialize(texture)) {
			Memory::Reset(textureBinding);
		}
		return textureBinding;
	}

}  //  nkentseu