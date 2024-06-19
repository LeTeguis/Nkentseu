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

namespace nkentseu {
    class Window;

    struct NKENTSEU_API VulkanExtension {
        void Defined();
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        std::vector<const char*> instanceExtension = {};
        std::vector<const char*> deviceExtension = {};
        std::vector<const char*> layers = {};
    };

    struct NKENTSEU_API VulkanInstance {
        bool Create(Window* window, const ContextProperties& contextProperties, VulkanExtension* extension);
        bool Destroy();

        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debugMessenger = nullptr;
    };

    struct NKENTSEU_API VulkanSurface {
        bool Create(Window* window, VulkanInstance* instance);
        bool Destroy(VulkanInstance* instance);

        VkSurfaceKHR surface = nullptr;
    };

    struct VulkanGpu;

    struct NKENTSEU_API VulkanQueueFamilyIndices {
        int32   graphicsIndex = -1;
        int32   presentIndex = -1;

        bool    hasGraphicsFamily = false;
        bool    hasPresentFamily = false;

        bool    IsComplete() { return hasGraphicsFamily && hasPresentFamily; }
        bool    FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    };

    struct NKENTSEU_API VulkanQueue {
        VulkanQueueFamilyIndices queueFamily;
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;
    };

    struct NKENTSEU_API VulkanSwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats = {};
        std::vector<VkPresentModeKHR>   presentMode = {};

        bool QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    };

    struct NKENTSEU_API VulkanGpu {
        bool Create(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension);
        bool Destroy();
        bool GetLogicalDevice(VulkanSurface* surface, VulkanExtension* extension);
        bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanExtension* extension);

        std::vector<VkPhysicalDevice> gpus = {};
        VkPhysicalDevice gpu = nullptr;
        VkPhysicalDeviceProperties properties = {};
        VulkanQueue queue;
        VkDevice device = nullptr;

        std::vector<VkQueueFamilyProperties> queueProperties = {};
        PFN_vkCmdSetPolygonModeEXT cmdSetPolygonModeEXT = nullptr;
    };

    struct NKENTSEU_API VulkanImage {
        // Enum to specify image type (depth or color)
        enum class ImageType {
            DEPTH,
            COLOR
        };

        // Constructor that takes device, width, height, format, and image type
        bool Create(VulkanGpu* gpu, const maths::Vector2u& size, ImageType imageType, VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
        bool Destroy(VulkanGpu* gpu);
        bool CreateImage(VulkanGpu* gpu, ImageType imageType, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspectMask);
        bool CreateMemory(VulkanGpu* gpu);
        bool CreateImageView(VulkanGpu* gpu, VkImageAspectFlags aspectMask);
        static bool FindSupportedFormat(VulkanGpu* gpu, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format);
        static bool FindDepthFormat(VulkanGpu* gpu, VkFormat* format);
        void TransitionImageLayout(VulkanGpu* gpu, VkCommandPool commandPool, VkQueue graphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout);
        uint32 FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties);
        VkCommandBuffer BeginSingleTimeCommands(VulkanGpu* gpu, VkCommandPool commandPool);
        void EndSingleTimeCommands(VulkanGpu* gpu, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);
        bool HasStencilComponent(VkFormat format);

        // Private members
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory memory;
        VkFormat format;
        maths::Vector2u size = {};
        uint32 mipLevels;
        VkImageLayout layout;
    };

    struct NKENTSEU_API VulkanSwapchain {
        bool Create(VulkanGpu* gpu, VulkanSurface* surface, const maths::Vector2u& size, const ContextProperties& contextProperties);
        bool Destroy(VulkanGpu* gpu);
        bool FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format);
        static uint32 GetMinImageCountFromPresentMode(VkPresentModeKHR present_mode);
        VkPresentModeKHR SelectPresentMode(VulkanGpu* gpu, VulkanSurface* surface, const ContextProperties& contextProperties);

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        VkSwapchainKHR swapchain = nullptr;
        VkSurfaceFormatKHR surfaceFormat = {};

        std::vector<VkImage> swapchainImages = {};
        std::vector<VkImageView> imageView = {};
        VulkanImage depthImage = {};
    };

    struct NKENTSEU_API VulkanCommandPool {
        bool Create(VulkanGpu* gpu);
        bool Destroy(VulkanGpu* gpu);

        VkCommandPool commandPool = nullptr;
    };

    struct NKENTSEU_API VulkanSemaphore {
        bool Create(VulkanGpu* gpu);
        bool Destroy(VulkanGpu* gpu);

        VkSemaphore submitSemaphore = nullptr;
        VkSemaphore aquireSemaphore = nullptr;
    };

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
        maths::Vector2 uv{};

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

        bool operator==(const VulkanDefaultVertex& other) const {
            return position == other.position && color == other.color && normal == other.normal &&
                uv == other.uv;
        }
    };

    struct NKENTSEU_API VulkanRenderPass {
        bool Create(VulkanGpu* gpu, VulkanSwapchain* swapchain);
        bool Destroy(VulkanGpu* gpu);

        VkRenderPass renderPass = nullptr;
    };

    struct NKENTSEU_API VulkanFramebuffer {
        bool Create(VulkanGpu* gpu, const maths::Vector2u& size, VulkanRenderPass* renderPass, VulkanSwapchain* swapchain);
        bool Destroy(VulkanGpu* gpu);

        std::vector<VkFramebuffer> framebuffer = {};
        maths::Vector2u size = {};
    };

    struct NKENTSEU_API VulkanDescriptorPool {
        bool Create(VulkanGpu* gpu, VulkanSwapchain* swapchain);
        bool Destroy(VulkanGpu* gpu);
        void Add(VkDescriptorType dType, uint32 count);
        VkDescriptorPool descriptorPool = nullptr;
        std::vector<VkDescriptorPoolSize> poolSizes;
    };

    struct NKENTSEU_API VulkanPipelineLayout {
        bool Create(VulkanGpu* gpu, VulkanSwapchain* swapchain);
        bool Destroy(VulkanGpu* gpu);

        bool CreateDescriptorSetLayout(VulkanGpu* gpu, VulkanSwapchain* swapchain);

        void Add(uint32 binding, VkDescriptorType type, VkShaderStageFlags shaderStage);

        bool IsValid();

        std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};

        VkPipelineLayout pipelineLayout = nullptr;
        VkDescriptorSetLayout descriptorSetLayout = nullptr;
    };

    struct NKENTSEU_API VulkanDynamicMode {
        VkPrimitiveTopology primitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
        VkPolygonMode polygoneMode = VK_POLYGON_MODE_LINE;
    };

    struct NKENTSEU_API VulkanBuffer {
        bool WriteToBuffer(const void* data, usize size, usize offset);
        bool Destroy(VulkanGpu* gpu);
        bool Mapped(VulkanGpu* gpu, usize size, usize offset = 0, VkMemoryMapFlags flag = 0);
        bool UnMapped(VulkanGpu* gpu);
        bool Flush(VulkanGpu* gpu, usize size, usize offset = 0);

        static int64 FindMemoryType(VulkanGpu* gpu, uint32 typeFilter, VkMemoryPropertyFlags properties);
        static bool CreateBuffer(VulkanGpu* gpu, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        static bool CopyBuffer(VulkanGpu* gpu, VulkanCommandPool* commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

        VkBuffer buffer = nullptr;
        VkDeviceMemory bufferMemory = nullptr;
        void* mappedData = nullptr;
        VkDeviceSize size = 0;
    };

    struct NKENTSEU_API VulkanUBO {
        bool Create(VulkanGpu* gpu, const UniformBufferAttribut& uba, VkBufferUsageFlags usage, std::vector<VkDescriptorSet>& descriptorSets, VkDescriptorType descriptorType);
        bool Destroy(VulkanGpu* gpu);

        bool Binds(VulkanGpu* gpu, void* data, usize size, uint32 instanceIndex);
        bool Bind(VulkanGpu* gpu, void* data, usize size, uint32 index, uint32 instanceIndex);

        static void* AlignedAlloc(size_t size, size_t alignment);

        static void AlignedFree(void* data);

        std::vector<VulkanBuffer> uniformBuffers;
        std::vector<VkWriteDescriptorSet> writeDescriptorSets;
        std::vector<VkDescriptorBufferInfo> descriptorBufferInfos;
        VkBufferUsageFlags usage;
        UniformBufferAttribut uniformBufferAttribut;
        uint32 dynamicAlignment = 0;

        uint32 currentOffset = 0;
        void* dataModel = nullptr;
    };

    struct NKENTSEU_API VulkanCommandBuffer {
        bool Create(VulkanGpu* gpu, VulkanSwapchain* swapchain, VulkanCommandPool* commandPool);
        bool Destroy(VulkanGpu* gpu, VulkanCommandPool* commandPool);

        std::vector<VkCommandBuffer> commandBuffers = {};
    };

    struct NKENTSEU_API VulkanTexture {
        void CreateDescriptorSets();

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
        maths::Vector2u m_Size;
    };
    
}  //  nkentseu

#endif  // __VULKAN_INTERNAL_H__!