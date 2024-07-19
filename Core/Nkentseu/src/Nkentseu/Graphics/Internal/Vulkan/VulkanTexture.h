//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-16 at 11:23:53 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_TEXTURE_H__
#define __VULKAN_TEXTURE_H__

#pragma once

#include <System/System.h>
#include "Nkentseu/Graphics/Texture.h"
#include "Nkentseu/Graphics/ShaderInputLayout.h"
#include "VulkanContext.h"
#include "VulkanShaderInputLayout.h"
#include "Tools/VulkanDescriptorPool.h"

#include <vector>
#include <unordered_map>

namespace nkentseu {
    
	class NKENTSEU_API VulkanTexture2D : public Texture2D {
	public:
		VulkanTexture2D(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil);
		~VulkanTexture2D();

		bool Create(TextureFormat textureFormat, const maths::Vector2u& size) override;
		bool Create(const std::filesystem::path& filename, const maths::IntRect& area = maths::IntRect()) override;
		bool Create(const void* data, std::size_t size, const maths::IntRect& area = maths::IntRect()) override;
		bool Create(InputStream& stream, const maths::IntRect& area = maths::IntRect()) override;
		bool Create(const Image& image, const maths::IntRect& area = maths::IntRect()) override;

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

		Memory::Shared<VulkanContext> m_Context;
		Memory::Shared<VulkanShaderInputLayout> m_Vksil = nullptr;
		TextureFormat m_Format = TextureFormat::Enum::RGBA8;

		std::filesystem::path m_Path;
		uint64_t m_CacheId;
		uint32 m_Handle = 0;

		bool				m_IsRepeated{};
		bool				m_IsSmooth{};
		uint32				m_Channels{};
		maths::Vector2u     m_Size;
		maths::Vector2u     m_ActualSize;
		mutable bool		m_PixelsFlipped{};
		bool				m_FboAttachment{};
		bool				m_HasMipmap{};

		// vulkan image resource
		vk::Image image;
		vk::DeviceMemory imageMemory;
		vk::ImageView imageView;
		uint32 mipLevels = 1;
		vk::Sampler sampler = nullptr;

		bool CreateImage(vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags prop);
		bool UpdateImage(const uint8* pixel, const maths::Vector2u& size, const maths::Vector2i& dest);
		bool DestroyImage();
		bool CreateImageView();
		bool DestroyImageView();
		bool CreateSampler();
		bool DestroySampler();

		// tools
		void TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32 mipL);
		void CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32 width, uint32 height, const maths::Vector2i& offset);
		bool CreateDescriptorSet();
		bool DestroyDescriptorSet();

		struct DescriptorSetInternal {
			vk::Sampler sampler = nullptr;
			vk::ImageView imageView = nullptr;

			// descriptor pool
			vk::DescriptorPool descriptorPool = nullptr;
			std::vector<vk::DescriptorPoolSize> poolSizes;
			bool CreateDescriptorPool(Memory::Shared<VulkanContext> context);
			bool DestroyDescriptorPool(Memory::Shared<VulkanContext> context);
			void AddInDescriptorPool(vk::DescriptorType type, uint32 count);

			// descriptor set
			vk::DescriptorSet descriptorSet = nullptr;
			bool Create(Memory::Shared<VulkanContext> context, uint32 binding, vk::DescriptorSetLayout descriptorLayout);
			bool Destroy(Memory::Shared<VulkanContext> context);
		};
		std::unordered_map<std::string, DescriptorSetInternal> m_DescriptorSets;
	};

}  //  nkentseu

#endif  // __VULKAN_TEXTURE_H__!