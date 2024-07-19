//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-16 at 11:23:53 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanTexture.h"
#include <Logger/Formatter.h>
#include "VulkanUtils.h"
#include "VulkanInternal.h"

namespace nkentseu {
	using namespace maths;

	VulkanTexture2D::VulkanTexture2D(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) : m_Context(Memory::SharedCast<VulkanContext>(context)), m_Vksil(Memory::SharedCast<VulkanShaderInputLayout>(sil))
	{
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	bool VulkanTexture2D::Create(TextureFormat textureFormat, const maths::Vector2u& size)
	{
		if ((size.x == 0) || (size.y == 0)) {
			Log_nts.Error("Failed to create texture, invalid size ({0}x{1})", size.x, size.y);
			return false;
		}

		m_Format = textureFormat;

		// Compute the internal texture dimensions depending on NPOT textures support
		const maths::Vector2u actualSize(GetValidSize(m_Context, size.x), GetValidSize(m_Context, size.y));

		// Check the maximum texture size
		const uint32 maxSize = GetMaximumSize(m_Context);
		if ((actualSize.x > maxSize) || (actualSize.y > maxSize)) {
			Log_nts.Error("Failed to create texture, its internal size is too high ({0}x{1}, maximum is {2}x{2})", actualSize.x, actualSize.y, maxSize);
			return false;
		}

		m_Size = size;
		m_ActualSize = actualSize;
		m_PixelsFlipped = false;
		m_FboAttachment = false;
		m_HasMipmap = false;

		if (!CreateImage(VulkanConvert::ToTextureFormat(m_Format), vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal)) {
			Log_nts.Error("Cannot create Image texture");
			return false;
		}
		return true;
	}

	bool VulkanTexture2D::Create(const std::filesystem::path& filename, const maths::IntRect& area)
	{
		Image image;
		m_Path = filename;
		return image.LoadFromFile(filename) && Create(image, area);
	}

	bool VulkanTexture2D::Create(const void* data, std::size_t size, const maths::IntRect& area)
	{
		Image image;
		return image.LoadFromMemory(data, size) && Create(image, area);
	}

	bool VulkanTexture2D::Create(InputStream& stream, const maths::IntRect& area)
	{
		Image image;
		return image.LoadFromStream(stream) && Create(image, area);
	}

	bool VulkanTexture2D::Create(const Image& image, const maths::IntRect& area)
	{
		maths::IntRect rectangle = area;
		if (rectangle.x < 0) {
			rectangle.x = 0;
			rectangle.width -= rectangle.x;
			if (rectangle.width < 0) rectangle.width = 0;
		}

		if (rectangle.y < 0) {
			rectangle.y = 0;
			rectangle.height -= rectangle.y;
			if (rectangle.height < 0) rectangle.height = 0;
		}

		if (rectangle.x >= image.GetSize().width) {
			rectangle.x = 0;
			rectangle.width = 0;
		}

		if (rectangle.y >= image.GetSize().height) {
			rectangle.y = 0;
			rectangle.height = 0;
		}

		if (rectangle.x + rectangle.width > image.GetSize().width)
			rectangle.width = image.GetSize().width - rectangle.x;

		if (rectangle.y + rectangle.height > image.GetSize().height)
			rectangle.height = image.GetSize().height - rectangle.y;

		if (rectangle == maths::IntRect()) {
			rectangle.width = image.GetSize().width;
			rectangle.height = image.GetSize().height;
		}

		Image image_tmp;

		if (rectangle.width == image.GetSize().width && rectangle.height == image.GetSize().height && rectangle.x == 0 && rectangle.y == 0) {
			image_tmp = image;
		}
		else {
			image_tmp = image.GetSubImage(Vector2i(rectangle.x, rectangle.y), Vector2i(rectangle.width, rectangle.height));
		}

		if (Create(m_Format, image_tmp.GetSize())) {
			Update(image_tmp);

			CreateImageView();
			CreateSampler();
			return CreateDescriptorSet();
		}

		return false;
	}

	Color* VulkanTexture2D::GetColors()
	{
		return CopyToImage().GetColors();
	}

	uint8* VulkanTexture2D::GetPixels()
	{
		return CopyToImage().GetPixels();
	}

	maths::Vector2u VulkanTexture2D::GetSize() const
	{
		return m_Size;
	}

	Image VulkanTexture2D::CopyToImage() const
	{
		return Image();
	}

	void VulkanTexture2D::Update(const std::uint8_t* pixels)
	{
		Update(pixels, m_Size, { 0, 0 });
	}

	void VulkanTexture2D::Update(const std::uint8_t* pixels, const Vector2u& size, const Vector2i& dest)
	{
		UpdateImage(pixels, m_Size, dest);
	}

	void VulkanTexture2D::Update(const Texture& texture)
	{
		Update(texture, { 0, 0 });
	}

	void VulkanTexture2D::Update(const Texture& texture, const maths::Vector2i& dest)
	{
		Update(texture.CopyToImage());
	}

	void VulkanTexture2D::Update(const Image& image)
	{
		Update(image.GetPixels(), image.GetSize(), { 0, 0 });
	}

	void VulkanTexture2D::Update(const Image& image, const maths::Vector2i& dest)
	{
		Update(image.GetPixels(), image.GetSize(), dest);
	}

	void VulkanTexture2D::SetSmooth(bool smooth)
	{
		if (m_IsSmooth == smooth) return;
		DestroyDescriptorSet();
		DestroySampler();
		m_IsSmooth = smooth;
		CreateSampler();
		CreateDescriptorSet();
	}

	bool VulkanTexture2D::IsSmooth() const
	{
		return m_IsSmooth;
	}

	void VulkanTexture2D::SetTextureFormat(TextureFormat textureFormat)
	{
	}

	TextureFormat VulkanTexture2D::GetTextureFormat() const
	{
		return m_Format;
	}

	void VulkanTexture2D::SetRepeated(bool repeated)
	{
		if (m_IsRepeated == repeated) return;
		DestroyDescriptorSet();
		DestroySampler();
		m_IsRepeated = repeated;
		CreateSampler();
		CreateDescriptorSet();
	}

	bool VulkanTexture2D::IsRepeated() const
	{
		return m_IsRepeated;
	}

	bool VulkanTexture2D::GenerateMipmap()
	{
		return false;
	}

	void VulkanTexture2D::Swap(Texture& right) noexcept
	{
	}

	void VulkanTexture2D::InvalidateMipmap()
	{
	}

	void VulkanTexture2D::Bind(const std::string& name)
	{
		if (m_DescriptorSets.find(name) != m_DescriptorSets.end()) {
			vk::PipelineBindPoint pbp = vk::PipelineBindPoint::eGraphics;
			vk::DescriptorSet descriptorSet = m_DescriptorSets[name].descriptorSet;
			uint32 set = m_Vksil->sets[BufferSpecificationType::Enum::Texture];
			vk::PipelineLayout pipelineLayout = m_Vksil->pipelineLayout;
			vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

			if (m_DescriptorSets[name].sampler == nullptr) {
				Log_nts.Fatal("Error");
			}

			vkCheckErrorVoid(commandBuffer.bindDescriptorSets(pbp, pipelineLayout, set, 1, &descriptorSet, 0, nullptr));

			if (!VulkanStaticDebugInfo::Result()) {
				Log_nts.Error("cannot bind {0}", name);
				return;
			}
			return;
		}
		Log_nts.Error("cannot bind {0}", name);
	}

	void VulkanTexture2D::Bind(uint32 binding)
	{
		for (auto& attribut : m_Vksil->samplerInput) {
			if (attribut.binding == binding) {
				Bind(attribut.name);
				return;
			}
		}
		Log_nts.Error("cannot bind {0}", binding);
	}

	const std::filesystem::path& VulkanTexture2D::GetPath() const
	{
		// TODO: ins�rer une instruction return ici
		return m_Path;
	}

	uint32 VulkanTexture2D::GetMaximumSize(Memory::Shared<Context> context)
	{
		Memory::Shared<VulkanContext> vkcontext = Memory::SharedCast<VulkanContext>(context);
		if (vkcontext != nullptr) {
			return vkcontext->properties.limits.maxImageDimension2D;
		}
		return 0;
	}

	uint32 VulkanTexture2D::GetValidSize(Memory::Shared<Context> context, uint32 size)
	{
		if (size <= GetMaximumSize(context)) {
			// If hardware supports the desired size, return it
			return size;
		}
		else {
			// If hardware doesn't support the desired size, calculate the nearest power of two
			uint32 powerOfTwo = 1;
			while (powerOfTwo < size)
				powerOfTwo *= 2;

			return powerOfTwo;
		}
	}

	bool VulkanTexture2D::Destroy()
	{
		if (m_Context == nullptr) return false;

		bool success = DestroyDescriptorSet();
		success = DestroySampler() && success;
		success = DestroyImageView() && success;
		success = DestroyImage() && success;

		return success;
	}

	bool VulkanTexture2D::DescriptorSetInternal::Create(Memory::Shared<VulkanContext> context, uint32 binding, vk::DescriptorSetLayout descriptorLayout)
	{
		if (context == nullptr || descriptorPool == nullptr) return false;

		VulkanResult result;
		bool first = true;

		if (descriptorLayout == nullptr) {
			Log_nts.Fatal();
			return false;
		}

		vk::DescriptorSet descriptor_set = nullptr;
		{
			vk::DescriptorSetAllocateInfo allocInfo = {};
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descriptorLayout;
			vkCheckError(first, result, context->device.allocateDescriptorSets(&allocInfo, &descriptor_set), "cannot create descriptor sets");
			if (!result.success) {
				Log_nts.Error("Cannot allocate descriptor set");
			}
		}
		// Update the Descriptor Set:
		{
			vk::DescriptorImageInfo desc_image[1] = {};
			desc_image[0].sampler = sampler;
			desc_image[0].imageView = imageView;
			desc_image[0].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

			std::vector<vk::WriteDescriptorSet> descriptorWrites;
			descriptorWrites.resize(1);
			descriptorWrites[0].dstSet = descriptor_set;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].descriptorType = vk::DescriptorType::eCombinedImageSampler;
			descriptorWrites[0].pImageInfo = desc_image;
			descriptorWrites[0].dstBinding = binding;

			vkCheckErrorVoid(context->device.updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data(), 0, nullptr));

			if (!VulkanStaticDebugInfo::Result()) {
				Log_nts.Error("Cannot update descriptor set for image texture");
			}
		}
		descriptorSet = descriptor_set;

		return result.success;
	}

	bool VulkanTexture2D::DescriptorSetInternal::Destroy(Memory::Shared<VulkanContext> context)
	{
		if (context == nullptr) return false;

		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, context->device.freeDescriptorSets(descriptorPool, 1, &descriptorSet), "cannot free descriptor sets");
		return DestroyDescriptorPool(context) && result.success;
	}

	bool VulkanTexture2D::CreateImage(vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags prop)
	{
		if (m_Context == nullptr) return false;

		VulkanResult result;
		bool first = true;

		vk::ImageCreateInfo imageInfo{};
		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = m_Size.width;
		imageInfo.extent.height = m_Size.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		//imageInfo.format = ConvertTextureFormat(m_Format);
		imageInfo.format = format;
		//imageInfo.tiling = vk::ImageTiling::eOptimal;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		//imageInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
		imageInfo.usage = usage;
		imageInfo.sharingMode = vk::SharingMode::eExclusive;
		imageInfo.samples = vk::SampleCountFlagBits::e1;

		vkCheckError(first, result, m_Context->device.createImage(&imageInfo, m_Context->allocator, &image), "cannot create vulkan img");

		vk::MemoryRequirements memRequirements;
		vkCheckErrorVoid(m_Context->device.getImageMemoryRequirements(image, &memRequirements));

		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot get image memory requirement");
		}

		vk::MemoryAllocateInfo allocInfo{};
		allocInfo.allocationSize = memRequirements.size;
		//allocInfo.memoryTypeIndex = VulkanContext::FindMemoryType(m_Context->physicalDevice, memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		allocInfo.memoryTypeIndex = VulkanContext::FindMemoryType(m_Context->physicalDevice, memRequirements.memoryTypeBits, prop);

		vkCheckError(first, result, m_Context->device.allocateMemory(&allocInfo, m_Context->allocator, &imageMemory), "echec lors de lallocation de la memoire pour l'image !");

		vkCheckErrorVoid(m_Context->device.bindImageMemory(image, imageMemory, 0));

		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot bind image memory");
		}

		return true;
	}

	bool VulkanTexture2D::UpdateImage(const uint8* pixel, const maths::Vector2u& size, const maths::Vector2i& dest)
	{
		vk::DeviceSize deviceSize = size.x * size.y * 4;

		if (m_Context == nullptr || pixel == nullptr || deviceSize == 0) return false;

		VkBufferInternal stanging;

		vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferSrc;
		vk::SharingMode sharingMode = vk::SharingMode::eExclusive;
		vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

		if (!VkBufferInternal::CreateBuffer(m_Context.get(), deviceSize, usage, sharingMode, propertyFlags, stanging.buffer, stanging.bufferMemory)) {
			return false;
		}

		if (!stanging.Mapped(m_Context.get(), deviceSize, 0, {})) {
			return false;
		}

		if (!stanging.WriteToBuffer(pixel, deviceSize, 0)) {
			return false;
		}
		// VK_IMAGE_LAYOUT_GENERAL a utiliser pour l'access en lecture et en ecriture
		TransitionImageLayout(image, VulkanConvert::ToTextureFormat(m_Format), vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, mipLevels);

		CopyBufferToImage(stanging.buffer, image, m_Size.width, m_Size.height, dest);

		TransitionImageLayout(image, VulkanConvert::ToTextureFormat(m_Format), vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, mipLevels);

		if (!stanging.UnMapped(m_Context.get())) {
			return false;
		}

		if (!stanging.Destroy(m_Context.get())) {
			return false;
		}
		return true;
	}

	bool VulkanTexture2D::DestroyImage()
	{
		if (m_Context == nullptr) return false;

		bool success = true;

		vkCheckErrorVoid(m_Context->device.destroyImage(image, m_Context->allocator));
		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot destroy image texture");
			success = false;
		}

		vkCheckErrorVoid(m_Context->device.freeMemory(imageMemory, m_Context->allocator));
		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot free image texture memory");
			success = false;
		}

		return success;
	}

	bool VulkanTexture2D::CreateImageView()
	{

		if (m_Context == nullptr) return false;
		VulkanResult result;
		bool first = true;

		vk::ImageViewCreateInfo viewInfo{};
		viewInfo.image = image;
		viewInfo.viewType = vk::ImageViewType::e2D;
		viewInfo.format = VulkanConvert::ToTextureFormat(m_Format);
		viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.components = vk::ComponentSwizzle::eIdentity;

		vkCheckError(first, result, m_Context->device.createImageView(&viewInfo, m_Context->allocator, &imageView), "Echec de création de la vue pour la texture !");

		return result.success;
	}

	bool VulkanTexture2D::DestroyImageView()
	{
		if (m_Context == nullptr) return false;
		vkCheckErrorVoid(m_Context->device.destroyImageView(imageView, m_Context->allocator));
		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot destroy image view");
			return false;
		}
		return true;
	}

	bool VulkanTexture2D::CreateSampler()
	{
		if (m_Context == nullptr) return false;
		VulkanResult result;
		bool first = true;

		vk::SamplerCreateInfo samplerInfo{};
		samplerInfo.magFilter = m_IsSmooth ? vk::Filter::eLinear : vk::Filter::eNearest;
		samplerInfo.minFilter = m_IsSmooth ? vk::Filter::eLinear : vk::Filter::eNearest;
		samplerInfo.addressModeU = m_IsRepeated ? vk::SamplerAddressMode::eRepeat : vk::SamplerAddressMode::eClampToEdge;
		samplerInfo.addressModeV = m_IsRepeated ? vk::SamplerAddressMode::eRepeat : vk::SamplerAddressMode::eClampToEdge;
		samplerInfo.addressModeW = m_IsRepeated ? vk::SamplerAddressMode::eRepeat : vk::SamplerAddressMode::eClampToEdge;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = vk::CompareOp::eAlways;
		samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		//samplerInfo.maxLod = 0.0f;
		samplerInfo.maxLod = static_cast<float32>(mipLevels);

		vkCheckError(first, result, m_Context->device.createSampler(&samplerInfo, nullptr, &sampler), "Failed to create texture sampler!");

		return result.success;
	}

	bool VulkanTexture2D::DestroySampler()
	{
		if (m_Context == nullptr) return false;
		vkCheckErrorVoid(m_Context->device.destroySampler(sampler, m_Context->allocator));
		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot destroy texture image sampler");
			return false;
		}
		return true;
	}

	void VulkanTexture2D::TransitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32 mipL)
	{
		vk::CommandBuffer commandBuffer = VulkanContext::BeginSingleTimeCommands(m_Context.get());

		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
		barrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;

		barrier.image = image;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipL;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		barrier.srcAccessMask = vk::AccessFlagBits::eNone; // TODO
		barrier.dstAccessMask = vk::AccessFlagBits::eNone; // TODO

		vk::PipelineStageFlags sourceStage;
		vk::PipelineStageFlags destinationStage;

		if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eNone;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
			sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
			destinationStage = vk::PipelineStageFlagBits::eTransfer;
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
			sourceStage = vk::PipelineStageFlagBits::eTransfer;
			destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		}
		else {
			Log_nts.Error("Transition de l’agencement non supportee !");
		}
		
		vkCheckErrorVoid(commandBuffer.pipelineBarrier(sourceStage, destinationStage,
									  (vk::DependencyFlagBits)0,
									  0, nullptr,
									  0, nullptr,
									  1, &barrier));
		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot use pipeline barrier");
		}

		VulkanContext::EndSingleTimeCommands(m_Context.get(), commandBuffer);
	}

	void VulkanTexture2D::CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32 width, uint32 height, const maths::Vector2i& offset)
	{
		vk::CommandBuffer commandBuffer = VulkanContext::BeginSingleTimeCommands(m_Context.get());

		vk::BufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = vk::Offset3D{ offset.x, offset.y, 0 };
		region.imageExtent = vk::Extent3D{ width, height, 1 };

		vkCheckErrorVoid(commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region));
		if (!VulkanStaticDebugInfo::Result()) {
			Log_nts.Error("Cannot copy buffer to image");
		}

		VulkanContext::EndSingleTimeCommands(m_Context.get(), commandBuffer);
	}

	bool VulkanTexture2D::CreateDescriptorSet()
	{
		if (m_Context == nullptr) return false;

		// Recupere le layout du descriptor set pour les uniform buffers
		auto it = m_Vksil->descriptorSetLayouts.find(BufferSpecificationType::Enum::Texture);
		if (it == m_Vksil->descriptorSetLayouts.end()) {
			Log_nts.Error("Texture buffer descriptor set layout not found.");
			return false;
		}

		for (auto& attribut : m_Vksil->samplerInput) {
			if (sampler == nullptr || imageView == nullptr) {
				Log_nts.Fatal();
				continue;
			}

			m_DescriptorSets[attribut.name] = {};
			m_DescriptorSets[attribut.name].sampler = sampler;
			m_DescriptorSets[attribut.name].imageView = imageView;

			vk::DescriptorType dtype = VulkanConvert::SamplerInputAttributType(attribut.type);

			m_DescriptorSets[attribut.name].AddInDescriptorPool(dtype, m_Context->swapchainImages.size());
			if (!m_DescriptorSets[attribut.name].CreateDescriptorPool(m_Context)) {
				Log_nts.Error("Cannot create descriptor pool for texture");
				return false;
			}

			if (!m_DescriptorSets[attribut.name].Create(m_Context, attribut.binding, it->second)) {
				Log_nts.Error("Cannot create descriptor set for texture");
				return false;
			}
		}
		return true;
	}

	bool VulkanTexture2D::DestroyDescriptorSet()
	{
		bool success = true;
		for (auto [name, descriptor] : m_DescriptorSets) {
			if (!descriptor.Destroy(m_Context)) {
				success = false;
			}
		}
		return success;
	}

	bool VulkanTexture2D::DescriptorSetInternal::CreateDescriptorPool(Memory::Shared<VulkanContext> context)
	{
		VulkanResult result;
		bool first = true;

		vk::DescriptorPoolCreateInfo poolInfo{};
		poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		if (poolSizes.size() > 0) {
			poolInfo.poolSizeCount = poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
		}
		poolInfo.maxSets = static_cast<uint32_t>(context->swapchainImages.size());

		vkCheckError(first, result, context->device.createDescriptorPool(&poolInfo, context->allocator, &descriptorPool), "cannot create descriptor pool");

		return result.success;
	}

	bool VulkanTexture2D::DescriptorSetInternal::DestroyDescriptorPool(Memory::Shared<VulkanContext> context)
	{
		if (descriptorPool != nullptr) {
			vkCheckErrorVoid(context->device.destroyDescriptorPool(descriptorPool, context->allocator));
			descriptorPool = nullptr;

			if (!VulkanStaticDebugInfo::Result()) {
				Log_nts.Error("Cannot destroy descriptor pool");
			}
		}
		return true;
	}

	void VulkanTexture2D::DescriptorSetInternal::AddInDescriptorPool(vk::DescriptorType type, uint32 count)
	{
		vk::DescriptorPoolSize poolSize{};
		poolSize.type = type;
		poolSize.descriptorCount = count;
		poolSizes.push_back(poolSize);
	}

}  //  nkentseu