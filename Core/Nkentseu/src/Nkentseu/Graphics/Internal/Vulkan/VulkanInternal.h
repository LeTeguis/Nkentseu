//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:30:26 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_H__
#define __VULKAN_INTERNAL_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS
//#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#define VK_USE_PLATFORM_XCB_KHR
#endif

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>
#include <Nkentseu/Graphics/GraphicsProperties.h>
#include <Nkentseu/Graphics/ShaderInfo.h>
#include <Ntsm/Vector/Vector3.h>

#include "Nkentseu/Graphics/ShaderInputLayout.h"
#include "Nkentseu/Graphics/Texture.h"

#include "Tools/VulkanExtension.h"
#include "Tools/VulkanInstance.h"
#include "Tools/VulkanSurface.h"
#include "Tools/VulkanQueue.h"
#include "Tools/VulkanDevice.h"
#include "Tools/VulkanImage.h"
#include "Tools/VulkanSwapchain.h"
#include "Tools/VulkanCommandPool.h"
#include "Tools/VulkanBuffer.h"

namespace nkentseu {
    /*
    

    

    

    

    struct NKENTSEU_API VulkanPipelineConfig {
        VulkanPipelineConfig() = default;
        VulkanPipelineConfig(const VulkanPipelineConfig&) = delete;
        VulkanPipelineConfig& operator=(const VulkanPipelineConfig&) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;

        uint32 subpass = 0;

        static void DefaultPipelineConfig(VulkanPipelineConfig* configInfo);
        static void EnableAlphaBlending(VulkanPipelineConfig* configInfo);
    };

    struct NKENTSEU_API VulkanDefaultVertex {
        maths::Vector3 position{};
        maths::Vector3 color{};
        maths::Vector3 normal{};
        maths::Vector2f uv{};

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

        bool operator==(const VulkanDefaultVertex& other) const {
            return position == other.position && color == other.color && normal == other.normal &&
                uv == other.uv;
        }
    };

    

    

    

    

    

    

    struct NKENTSEU_API VulkanTexture {
        bool Create(VulkanDevice* gpu, VulkanCommandPool* commandPool, const maths::Vector2u& size, TextureFormat::Code format);
        void Destroy(VulkanDevice* gpu);

        void Update(VulkanDevice* gpu, VulkanCommandPool* commandPool, const uint8* pixels, const maths::Vector2u& size);
        void Update(VulkanDevice* gpu, VulkanCommandPool* commandPool, const uint8* pixels, const maths::Vector2u& size, const maths::Vector2i& offset);

        void TransitionImageLayout(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyBufferToImage(VulkanDevice* gpu, VulkanCommandPool* commandPool, VkBuffer buffer, const maths::Vector2u& size, const maths::Vector2i& offset);
        void GenerateMipmap(VulkanDevice* gpu, VulkanCommandPool* commandPool, const maths::Vector2u& size);

        VkFormat GetVulkanFormat(TextureFormat::Code format);
        void CreateImageView(VulkanDevice* gpu, TextureFormat::Code format);
        
        VkImage image;
        VkDeviceMemory imageMemory;
        VkImageView imageView;
        uint32 mipLevels;
        /*void CreateDescriptorSets();

        bool CreateImage(const maths::Vector2u& size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void TransitionImageLayoutCmd(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);
        bool CreateTextureSampler(bool repeate, bool smooth);
        bool CreateImageView(VkImageAspectFlags aspectFlags);
        bool CreateTextureImage();
        static void CreateDepthResources(const maths::Vector2u& size);
        
        static bool HasStencilComponent(VkFormat format);
        static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        static VkFormat FindDepthFormat();

        static void CopyBufferToImage(VkBuffer buffer, VkImage image, const maths::Vector2u& size);
        static void CopyImageToBuffer(VkImage image, VkBuffer buffer, const maths::Vector2u& size);
        static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);

        static bool CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        static std::vector<std::uint8_t> GetImageData(const VkCommandPool& commandPool, const maths::Vector2u& size, VkFormat format, VkImage image);

        VkImage image = nullptr;
        VkDeviceMemory imageMemory = nullptr;
        VkImageView imageView = nullptr;
        VkSampler sampler;
        VkFormat format;
        VkDescriptorSet descriptorSet;
        maths::Vector2u m_Size;*/
    //};
    
}  //  nkentseu

#endif  // __VULKAN_INTERNAL_H__!