//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:08:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanUBO.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    
	bool VulkanUBO::Create(VulkanDevice* gpu, const UniformInputAttribute& uba, VkBufferUsageFlags usage, std::vector<VkDescriptorSet>& descriptorSets, VkDescriptorType descriptorType)
	{
		if (gpu == nullptr) return false;
		uniformInput = uba;

		this->usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
		VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkMemoryPropertyFlags propertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		uniformBuffers.resize(descriptorSets.size());
		writeDescriptorSets.resize(descriptorSets.size());
		descriptorBufferInfos.resize(descriptorSets.size());
		uint32 index = 0;
		bool success = true;
		//uint32 size = uba.instance > 1 && uba.uType == UniformBufferType::Dynamic ? uba.instance + 1 : uba.instance;
		//size *= uba.size;

		uint32 size = uniformInput.size;
		uint32 range = size;
		dynamicAlignment = 0;

		if (uniformInput.usage == BufferUsageType::Enum::DynamicDraw) {
			uint32 minUboAlignment = gpu->properties.limits.minUniformBufferOffsetAlignment;

			if (minUboAlignment > 0) {
				dynamicAlignment = (size + minUboAlignment - 1) & ~(minUboAlignment - 1);
			}
			range = dynamicAlignment;
			size = dynamicAlignment * uniformInput.instance;
		}

		for (auto& uniform : uniformBuffers) {
			if (!VulkanBuffer::CreateBuffer(gpu, size, this->usage, sharingMode, propertyFlags, uniform.buffer, uniform.bufferMemory)) {
				Log_nts.Error("Cannot create uniforme buffer : name = {0} at index = {1}", uniformInput.name, index);
				success = false;
			}
			else {
				descriptorBufferInfos[index].buffer = uniform.buffer;
				descriptorBufferInfos[index].offset = 0;
				//descriptorBufferInfos[index].offset = dynamicAlignment;
				//descriptorBufferInfos[index].range = uba.size; // La taille du tampon uniforme
				descriptorBufferInfos[index].range = range; // La taille du tampon uniforme

				writeDescriptorSets[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDescriptorSets[index].dstSet = descriptorSets[index];
				writeDescriptorSets[index].dstBinding = uniformInput.binding; // L'index de la liaison dans le descripteur d'ensemble
				writeDescriptorSets[index].dstArrayElement = 0;
				writeDescriptorSets[index].descriptorType = descriptorType;
				writeDescriptorSets[index].descriptorCount = 1;
				writeDescriptorSets[index].pBufferInfo = &descriptorBufferInfos[index];
			}
			index++;
		}

		return success;
	}

	bool VulkanUBO::Destroy(VulkanDevice* gpu)
	{
		if (gpu == nullptr) return false;

		if (uniformBuffers.size() <= 0) return true;

		for (auto& buffer : uniformBuffers) {
			buffer.Destroy(gpu);
		}

		uniformBuffers.clear();
		writeDescriptorSets.clear();
		descriptorBufferInfos.clear();

		return true;
	}

	bool VulkanUBO::Binds(VulkanDevice* gpu, void* data, usize size, uint32 instanceIndex)
	{
		if (gpu == nullptr) return false;

		bool success = false;

		currentOffset = 0;
		if (uniformInput.usage == BufferUsageType::Enum::DynamicDraw) {
			currentOffset = instanceIndex * dynamicAlignment;
		}

		for (auto& uniform : uniformBuffers) {
			uniform.Mapped(gpu, size, 0);
			success = uniform.WriteToBuffer(data, size, currentOffset);
			success = uniform.Flush(gpu, size, 0);
			uniform.UnMapped(gpu);
		}

		return success;
	}

	bool VulkanUBO::Bind(VulkanDevice* gpu, void* data, usize size, uint32 index, uint32 instanceIndex)
	{
		if (gpu == nullptr) return false;

		bool success = false;

		currentOffset = 0;

		if (uniformInput.usage == BufferUsageType::Enum::DynamicDraw) {
			currentIndex++;

			if (currentIndex >= uniformInput.instance) {
				currentIndex = 0;
			}
			currentOffset = currentIndex * dynamicAlignment;
		}

		auto& uniform = uniformBuffers[index];

		uniform.Mapped(gpu, size, 0);
		success = uniform.WriteToBuffer(data, size, currentOffset);
		success = uniform.Flush(gpu, size, 0);
		uniform.UnMapped(gpu);

		return success;
	}

	bool VulkanUBO::BindSimple(VulkanDevice* gpu, void* data, usize size, uint32 index, uint32 instanceIndex)
	{
		if (gpu == nullptr) return false;

		bool success = false;

		currentOffset = 0;


		currentIndex++;

		if (currentIndex >= uniformInput.instance) {
			currentIndex = 0;
		}

		//currentOffset = currentIndex * dynamicAlignment;
		currentOffset = currentIndex * uniformInput.size;

		auto& uniform = uniformBuffers[index];

		uniform.Mapped(gpu, size, 0);
		success = uniform.WriteToBuffer(data, size, currentOffset);
		success = uniform.Flush(gpu, size, 0);
		uniform.UnMapped(gpu);

		return success;
	}

	void* VulkanUBO::AlignedAlloc(size_t size, size_t alignment)
	{
		void* data = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__)
		data = _aligned_malloc(size, alignment);
#else
		int res = posix_memalign(&data, alignment, size);
		if (res != 0)
			data = nullptr;
#endif
		return data;
	}

	void VulkanUBO::AlignedFree(void* data)
	{
#if	defined(_MSC_VER) || defined(__MINGW32__)
		_aligned_free(data);
#else
		free(data);
#endif
	}

}  //  nkentseu