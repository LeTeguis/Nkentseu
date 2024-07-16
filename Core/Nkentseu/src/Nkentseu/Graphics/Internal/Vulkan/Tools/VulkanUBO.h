//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 02:08:41 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_U_B_O_H__
#define __VULKAN_U_B_O_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include <Nkentseu/Graphics/ShaderInputLayout.h>
#include "VulkanBuffer.h"

namespace nkentseu {
    
    struct NKENTSEU_API VulkanUBO {
        bool Create(VulkanDevice* gpu, const UniformInputAttribute& uba, VkBufferUsageFlags usage, std::vector<VkDescriptorSet>& descriptorSets, VkDescriptorType descriptorType);
        bool Destroy(VulkanDevice* gpu);

        bool Binds(VulkanDevice* gpu, void* data, usize size, uint32 instanceIndex);
        bool Bind(VulkanDevice* gpu, void* data, usize size, uint32 index, uint32 instanceIndex);
        bool BindSimple(VulkanDevice* gpu, void* data, usize size, uint32 index, uint32 instanceIndex);

        static void* AlignedAlloc(size_t size, size_t alignment);

        static void AlignedFree(void* data);

        std::vector<VulkanBuffer> uniformBuffers;
        std::vector<VkWriteDescriptorSet> writeDescriptorSets;
        std::vector<VkDescriptorBufferInfo> descriptorBufferInfos;
        VkBufferUsageFlags usage;
        UniformInputAttribute uniformInput;
        uint32 dynamicAlignment = 0;

        uint32 currentOffset = 0;
        void* dataModel = nullptr;
        uint32 currentIndex = 0;
    };

}  //  nkentseu

#endif  // __VULKAN_U_B_O_H__!