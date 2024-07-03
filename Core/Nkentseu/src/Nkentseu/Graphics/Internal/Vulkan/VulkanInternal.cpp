//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:30:26 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanInternal.h"

/*
#if defined(XWIN_WIN32)
#elif defined(XWIN_COCOA)
#define VK_USE_PLATFORM_MACOS_MVK
#elif defined(XWIN_UIKIT)
#define VK_USE_PLATFORM_IOS_MVK
#elif defined(XWIN_XCB)
#elif defined(XWIN_ANDROID)
#define VK_USE_PLATFORM_ANDROID_KHR
#endif
*/

#ifdef NKENTSEU_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include "Nkentseu/Platform/Window/Windows/Win32Window.h"
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan_xlib.h>
#include "Nkentseu/Platform/Window/Linux/XLIB/WindowInternal.h"
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan_xcb.h>
#include "Nkentseu/Platform/Window/Linux/XCB/WindowInternal.h"
#endif

#include <Logger/Formatter.h>

#include "Nkentseu/Core/Window.h"
#include "Nkentseu/Platform/PlatformState.h"
#include "VulkanUtils.h"

namespace nkentseu {
	using namespace maths;

	// Vulkan Debug information
	static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity, VkDebugUtilsMessageTypeFlagsEXT msgFlags, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

		std::string file = VulkanStaticDebugInfo::file_call;
		std::string methode = VulkanStaticDebugInfo::methode_call;
		uint32 line = VulkanStaticDebugInfo::line_call;

		VulkanStaticDebugInfo::success = false;

		if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}
		else if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Warning("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}

		VulkanStaticDebugInfo::success = true;

		return VK_TRUE;
	}

	// Vulkan extension
	void VulkanExtension::Defined()
	{
		//  Instance extension
#ifdef NKENTSEU_PLATFORM_WINDOWS
		instanceExtension.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
		instanceExtension.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
		instanceExtension.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
		instanceExtension.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		instanceExtension.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		instanceExtension.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		//instanceExtension.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);

		// Device Extension
		//deviceExtension.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);
		deviceExtension.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME);

		// Layer
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	bool VulkanExtension::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtension.begin(), deviceExtension.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	// Vulkan Instance
	bool VulkanInstance::Create(Window* window, const ContextProperties& contextProperties, VulkanExtension* extension)
	{
		if (window == nullptr || extension == nullptr) return false;

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = window->GetTitle().c_str();
		appInfo.pEngineName = window->GetProperties().engineName.c_str();
		appInfo.apiVersion = VK_MAKE_VERSION(contextProperties.version.major, contextProperties.version.minor, 0);
		appInfo.engineVersion = VK_MAKE_VERSION(window->GetProperties().engineVersion.major, window->GetProperties().engineVersion.minor, 0);
		appInfo.applicationVersion = VK_MAKE_VERSION(window->GetProperties().applicationVersion.major, window->GetProperties().applicationVersion.minor, 0);

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledExtensionNames = extension->instanceExtension.data();
		instanceInfo.enabledExtensionCount = (uint32)extension->instanceExtension.size();
		instanceInfo.ppEnabledLayerNames = extension->layers.data();
		instanceInfo.enabledLayerCount = (uint32)extension->layers.size();

		VulkanResult result;
		bool first = true;
		vkCheckError(first, result, vkCreateInstance(&instanceInfo, 0, &instance), "Cannot create vulkan instance");

		if (result.success) {	
			Log_nts.Info("Create vulkan instance is good");

			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

			if (vkCreateDebugUtilsMessengerEXT) {
				VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
				debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
				debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
				debugInfo.pfnUserCallback = vkDebugCallback;

				vkCreateDebugUtilsMessengerEXT(instance, &debugInfo, 0, &debugMessenger);
			}
		}
		return result.success;
	}

	bool VulkanInstance::Destroy()
	{
		bool success = false;

		if (instance != nullptr) {
			auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (debugMessenger != nullptr && vkDestroyDebugUtilsMessengerEXT != nullptr && instance != nullptr) {
				vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
				debugMessenger = VK_NULL_HANDLE;
				success = true;
			}

			vkDestroyInstance(instance, nullptr);
			instance = VK_NULL_HANDLE;
			success = true && success;
		}
		return success;
	}

	// Vulkan surface
	bool VulkanSurface::Create(Window* window, VulkanInstance* instance)
	{
		if (window == nullptr || instance == nullptr) return false;
		//if (window->GetInternal() == nullptr || window->GetInternal()->GetWindowDisplay() == nullptr) return false;
		if (window->GetData() == nullptr || window->GetData()->windowHandle == nullptr) return false;
		VulkanResult result;
		bool first = true;
		//WindowInternal* internal = window->GetData();
		//WindowInternal* internal = window->GetInternal();

#ifdef NKENTSEU_PLATFORM_WINDOWS
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		//surfaceInfo.hwnd = (HWND)internal->GetWindowDisplay()->windowHandle;
		surfaceInfo.hwnd = (HWND)window->GetData()->windowHandle;
		//surfaceInfo.hinstance = internal->GetWindowDisplay()->instanceHandle;
		surfaceInfo.hinstance = window->GetData()->instanceHandle;

		vkCheckError(first, result, vkCreateWin32SurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface windows");

		if (result.success) {
			Log_nts.Info("Create vulkan surface is good for windows");
		}
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
		VkXlibSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.dpy = PlatformState.display;
		surfaceInfo.window = internal->GetWindowDisplay()->windowHandle;

		vkCheckError(first, result, vkCreateXlibSurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface linux xlib");

		if (result.success) {
			Log_nts.Info("Create vulkan surface is good for linux xlib");
		}
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
		VkXcbSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.dpy = PlatformState.connection;
		surfaceInfo.window = internal->GetWindowDisplay()->windowHandle;

		vkCheckError(first, result, vkCreateXcbSurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface linux xcb");

		if (result.success) {
			Log_nts.Info("Create vulkan surface is good for linux xcb");
		}
#endif
		return result.success;
	}

	bool VulkanSurface::Destroy(VulkanInstance* instance)
	{
		if (instance == nullptr || surface == nullptr) return false;
		vkCheckErrorVoid(vkDestroySurfaceKHR(instance->instance, surface, nullptr));
		surface = VK_NULL_HANDLE;
		return false;
	}

	// VulkanQueueFamilyIndices

	bool VulkanQueueFamilyIndices::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		if (device == nullptr || surface == nullptr) return false;

		uint32 queueFamilyCount = 0;

		vkCheckErrorVoid(vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr));

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

		vkCheckErrorVoid(vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()));

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsIndex = i;
				hasGraphicsFamily = true;
			}
			VkBool32 presentSupport = false;
			VulkanResult result;
			bool first = true;

			vkCheckError(first, result, vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport), "cannot get physical device surface support");

			if (queueFamily.queueCount > 0 && presentSupport) {
				presentIndex = i;
				hasPresentFamily = true;
			}
			if (IsComplete()) {
				break;
			}

			i++;
		}

		return true;
	}

	// VulkanSwapchainSupportDetails
	bool VulkanSwapchainSupportDetails::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities), "cannot get physical device surface capabilities");

		uint32 formatCount;
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr), "cannot cout physical device surface format");

		if (formatCount != 0) {
			formats.resize(formatCount);
			vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data()), "cannot get physical device surface format");
		}

		uint32 presentModeCount;
		vkCheckError(first, result, vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr), "cannot count physical device surface present mode");

		if (presentModeCount != 0) {
			presentMode.resize(presentModeCount);
			vkCheckError(first, result, vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentMode.data()), "cannot get physical device surface present mode");
		}
		return result.success;
	}

	// Vulkan Gpu
	bool VulkanGpu::Create(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension)
	{
		if (instance == nullptr || surface == nullptr) return false;
		VulkanResult result;
		bool first = true;

		uint32 gpuCount = 0;
		vkCheckError(first, result, vkEnumeratePhysicalDevices(instance->instance, &gpuCount, 0), "Cannot count physical device");
		std::vector<VkPhysicalDevice> gpusAll(gpuCount);
		vkCheckError(first, result, vkEnumeratePhysicalDevices(instance->instance, &gpuCount, gpusAll.data()), "Cannot get all physical device");

		if (result.success) {
			for (uint32 index = 0; index < gpuCount; index++) {
				if (IsDeviceSuitable(gpusAll[index], surface->surface, extension)) {
					gpu = gpusAll[index];
					break;
				}
			}

			if (gpu == nullptr) {
				return false;
			}

			std::copy(gpus.begin(), gpus.end(), gpusAll.data());

			queue.queueFamily.FindQueueFamilies(gpu, surface->surface);

			if (!queue.queueFamily.IsComplete()) {
				return false;
			}

			vkCheckErrorVoid(vkGetPhysicalDeviceProperties(gpu, &properties));
			result.success = GetLogicalDevice(surface, extension);

			if (result.success) {
				if (cmdSetPolygonModeEXT == nullptr) {
					cmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)vkGetDeviceProcAddr(device, "vkCmdSetPolygonModeEXT");

					if (!cmdSetPolygonModeEXT) {
						Log_nts.Error("Failed to load vkCmdSetPolygonModeEXT");
					}
				}
			}
		}

		return result.success;
	}

	bool VulkanGpu::Destroy()
	{
		if (device == nullptr) return false;
		vkCheckErrorVoid(vkDestroyDevice(device, nullptr));
		device = VK_NULL_HANDLE;
		return true;
	}

	bool VulkanGpu::GetLogicalDevice(VulkanSurface* surface, VulkanExtension* extension)
	{
		float32 queuePriority = 1.0f;

		std::vector<VkDeviceQueueCreateInfo> queueInfos;
		std::set<int32> uniqueQueueFamilies = { queue.queueFamily.graphicsIndex, queue.queueFamily.presentIndex };

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceExtendedDynamicState3FeaturesEXT extendedDynamicState3Features = {};
		extendedDynamicState3Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT;
		extendedDynamicState3Features.extendedDynamicState3PolygonMode = VK_TRUE;

		/*VkPhysicalDeviceFeatures2 deviceFeatures2 = {};
		deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures2.pNext = &extendedDynamicState3Features;*/

		VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature{};
		extendedDynamicStateFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
		extendedDynamicStateFeature.extendedDynamicState = VK_TRUE;
		extendedDynamicStateFeature.pNext = &extendedDynamicState3Features;

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pNext = &extendedDynamicStateFeature;
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.pQueueCreateInfos = queueInfos.data();
		deviceInfo.queueCreateInfoCount = (uint32)queueInfos.size();
		deviceInfo.ppEnabledExtensionNames = extension->deviceExtension.data();
		deviceInfo.enabledExtensionCount = (uint32)extension->deviceExtension.size();

		VulkanResult result;
		bool first = true;
		vkCheckError(first, result, vkCreateDevice(gpu, &deviceInfo, 0, &device), "cannot create device");

		if (result.success) {
			Log_nts.Info("Create vulkan device is good");
			vkCheckErrorVoid(vkGetDeviceQueue(device, queue.queueFamily.graphicsIndex, 0, &queue.graphicsQueue));
			vkCheckErrorVoid(vkGetDeviceQueue(device, queue.queueFamily.presentIndex, 0, &queue.presentQueue));
		}
		return result.success;
	}

	bool VulkanGpu::IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, VulkanExtension* extension)
	{
		VulkanQueueFamilyIndices indices;
		indices.FindQueueFamilies(device, surface);

		bool extensionsSupported = extension->CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			VulkanSwapchainSupportDetails swapChainSupport;
			swapChainSupport.QuerySwapChainSupport(device, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentMode.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	// Vulkan Image

	bool VulkanImage::Create(VulkanGpu* gpu, const Vector2u& size, ImageType imageType, VkFormat format, VkSampleCountFlagBits samples) {
		this->format = format;
		this->size = size;
		this->mipLevels = 1;
		this->layout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkImageUsageFlags usage;
		VkImageAspectFlags aspectMask;

		if (imageType == ImageType::DEPTH) {
			usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			FindDepthFormat(gpu, &this->format);
		}
		else {
			usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		CreateImage(gpu, imageType, samples, usage, aspectMask);
		CreateMemory(gpu);
		CreateImageView(gpu, aspectMask);

		layout = VK_IMAGE_LAYOUT_UNDEFINED;
		if (imageType == ImageType::DEPTH) {
			//TransitionImageLayout(gpu, pool->commandPool, gpu->queue.graphicsQueue, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}
		return true;
	}

	// Destructor to clean up Vulkan resources
	bool VulkanImage::Destroy(VulkanGpu* gpu) {

		if (gpu == nullptr) return false;
		vkCheckErrorVoid(vkDestroyImageView(gpu->device, imageView, nullptr));
		vkCheckErrorVoid(vkDestroyImage(gpu->device, image, nullptr));
		vkCheckErrorVoid(vkFreeMemory(gpu->device, memory, nullptr));
		return true;
	}

	// Initialize Vulkan image resources
	bool VulkanImage::CreateImage(VulkanGpu* gpu, ImageType imageType, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageAspectFlags aspectMask) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = size.width;
		imageInfo.extent.height = size.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = samples;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		vkCheckError(first, result, vkCreateImage(gpu->device, &imageInfo, nullptr, &image), "failed to create image!");
		return result.success;
	}

	bool VulkanImage::CreateMemory(VulkanGpu* gpu) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkMemoryRequirements memRequirements;
		vkCheckErrorVoid(vkGetImageMemoryRequirements(gpu->device, image, &memRequirements));

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(gpu->gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vkCheckError(first, result, vkAllocateMemory(gpu->device, &allocInfo, nullptr, &memory), "failed to allocate image memory!");

		vkCheckError(first, result, vkBindImageMemory(gpu->device, image, memory, 0), "failed to bind image memory");
		return result.success;
	}

	bool VulkanImage::CreateImageView(VulkanGpu* gpu, VkImageAspectFlags aspectMask) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		vkCheckError(first, result, vkCreateImageView(gpu->device, &viewInfo, nullptr, &imageView), "failed to create image view!");
		return result.success;
	}

	bool VulkanImage::FindSupportedFormat(VulkanGpu* gpu, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format)
	{
		if (gpu == nullptr) return false;

		for (VkFormat format_ : candidates) {
			VkFormatProperties props;
			vkCheckErrorVoid(vkGetPhysicalDeviceFormatProperties(gpu->gpu, format_, &props));

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				*format = format_;
				return true;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				*format = format_;
				return true;
			}
		}
		Log_nts.Error("failed to find supported format!");
		return false;
	}

	bool VulkanImage::FindDepthFormat(VulkanGpu* gpu, VkFormat* format)
	{
		return FindSupportedFormat(gpu, { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, format);
	}

	// Transition image layout to a specific stage
	void VulkanImage::TransitionImageLayout(VulkanGpu* gpu, VkCommandPool commandPool, VkQueue graphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(gpu, commandPool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (HasStencilComponent(format)) {
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCheckErrorVoid(vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		));

		EndSingleTimeCommands(gpu, commandBuffer, commandPool, graphicsQueue);

		layout = newLayout;
	}

	// Helper methods
	uint32 VulkanImage::FindMemoryType(VkPhysicalDevice physicalDevice, uint32 typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkCheckErrorVoid(vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties));

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		Log_nts.Error("failed to find suitable memory type!");
		return 0;
	}

	VkCommandBuffer VulkanImage::BeginSingleTimeCommands(VulkanGpu* gpu, VkCommandPool commandPool) {
		VulkanResult result;
		bool first = true;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkCheckError(first, result, vkAllocateCommandBuffers(gpu->device, &allocInfo, &commandBuffer), "failed to allocate command buffer");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkCheckError(first, result, vkBeginCommandBuffer(commandBuffer, &beginInfo), "failed to begin command buffer");

		return commandBuffer;
	}

	void VulkanImage::EndSingleTimeCommands(VulkanGpu* gpu, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue) {
		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkEndCommandBuffer(commandBuffer), "failed to end command buffer");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkCheckError(first, result, vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "failed to submit queue");
		vkCheckError(first, result, vkQueueWaitIdle(graphicsQueue), "failed to wait idle");

		vkCheckErrorVoid(vkFreeCommandBuffers(gpu->device, commandPool, 1, &commandBuffer));
	}

	bool VulkanImage::HasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	// VulkanSwapchain
	bool VulkanSwapchain::Create(VulkanGpu* gpu, VulkanSurface* surface, const Vector2u& size, const ContextProperties& contextProperties) {
		if (gpu == nullptr || surface == nullptr) return false;
		VulkanResult result;
		bool first = true;

		uint32 formatCount = 0;
		VkSurfaceFormatKHR surfaceFormats[10];
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, 0), "cannot count device surface format");
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, surfaceFormats), "cannot get device surface format");

		const std::vector<VkFormat> requestSurfaceImageFormat = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };

		for (uint32 indexFormat = 0; indexFormat < formatCount; indexFormat++) {
			VkSurfaceFormatKHR currentFormat = surfaceFormats[indexFormat];

			for (uint32 indexRequier = 0; indexRequier < requestSurfaceImageFormat.size(); indexRequier++) {
				if (currentFormat.format == requestSurfaceImageFormat[indexRequier] && currentFormat.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
					surfaceFormat = currentFormat;
					break;
				}
			}
		}

		presentMode = SelectPresentMode(gpu, surface, contextProperties);

		VkSurfaceCapabilitiesKHR surfaceCaps = {};
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->gpu, surface->surface, &surfaceCaps), "cannot get surface capabilities khr");

		uint32 imageCount = GetMinImageCountFromPresentMode(presentMode);

		if (imageCount < surfaceCaps.minImageCount) {
			imageCount = surfaceCaps.minImageCount;
		}
		else if (surfaceCaps.maxImageCount != 0 && imageCount > surfaceCaps.maxImageCount) {
			imageCount = surfaceCaps.maxImageCount;
		}

		VkExtent2D extend = {};

		if (surfaceCaps.currentExtent.width == 0xffffffff) {
			extend.width = size.x;
			extend.height = size.y;
		}
		else {
			extend = surfaceCaps.currentExtent;
		}

		std::vector<uint32> queueFamilyIndices;

		if (gpu->queue.queueFamily.graphicsIndex >= 0)
			queueFamilyIndices.push_back(gpu->queue.queueFamily.graphicsIndex);

		if (gpu->queue.queueFamily.presentIndex >= 0 && gpu->queue.queueFamily.presentIndex != gpu->queue.queueFamily.graphicsIndex)
			queueFamilyIndices.push_back(gpu->queue.queueFamily.presentIndex);

		VkSharingMode sharingMode;
		if (queueFamilyIndices.size() > 1) {
			sharingMode = VK_SHARING_MODE_CONCURRENT;
		}
		else {
			sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.surface = surface->surface;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.preTransform = surfaceCaps.currentTransform;
		swapchainInfo.imageExtent = extend;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.queueFamilyIndexCount = queueFamilyIndices.size();
		swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		swapchainInfo.imageSharingMode = sharingMode;

		vkCheckError(first, result, vkCreateSwapchainKHR(gpu->device, &swapchainInfo, 0, &swapchain), "cannot create swapchain");

		if (result.success) {
			Log_nts.Info("Create vulkan swapchain is good");
		}
		uint32 scImageCount = 0;
		VkImage scImages[16];

		vkCheckError(first, result, vkGetSwapchainImagesKHR(gpu->device, swapchain, &scImageCount, 0), "cannot count swapchain image khr");
		vkCheckError(first, result, vkGetSwapchainImagesKHR(gpu->device, swapchain, &scImageCount, scImages), "cannot getswapchain image khr");

		imageView.resize(scImageCount);

		VkImageSubresourceRange imageRange = {};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.layerCount = 1;
		imageRange.levelCount = 1;

		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.format = surfaceFormat.format;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // VK_IMAGE_VIEW_TYPE_3D, VK_IMAGE_VIEW_TYPE_CUBE_MAP
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.levelCount = 1;
		//viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		//viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		//viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		//viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.subresourceRange = imageRange;

		for (uint32 index = 0; index < scImageCount; index++) {
			swapchainImages.push_back(scImages[index]);
			viewInfo.image = swapchainImages[index];

			vkCheckError(first, result, vkCreateImageView(gpu->device, &viewInfo, 0, &imageView[index]), "cannot create image view number {0}", index);
		}
		
		if (result.success && depthImage.Create(gpu, size, VulkanImage::ImageType::DEPTH)) {
			Log_nts.Info("Create swapchain is good");
		}

		return result.success;
	}

	bool VulkanSwapchain::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		depthImage.Destroy(gpu);

		for (usize i = 0; i < imageView.size(); i++) {
			if (imageView[i] != nullptr) {
				vkCheckErrorVoid(vkDestroyImageView(gpu->device, imageView[i], nullptr));
				imageView[i] = VK_NULL_HANDLE;
			}
		}

		imageView.clear();
		swapchainImages.clear();

		if (swapchain != nullptr) {
			vkCheckErrorVoid(vkDestroySwapchainKHR(gpu->device, swapchain, nullptr));
			swapchain = VK_NULL_HANDLE;
		}
		swapchain = nullptr;
		return true;
	}

	bool VulkanSwapchain::FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkFormat* format)
	{
		for (VkFormat format_ : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device, format_, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				*format = format_;
				return true;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				*format = format_;
				return true;
			}
		}
		Log_nts.Error("failed to find supported format!");
		return false;
	}

	uint32 VulkanSwapchain::GetMinImageCountFromPresentMode(VkPresentModeKHR present_mode)
	{
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			return 3;
		if (present_mode == VK_PRESENT_MODE_FIFO_KHR || present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
			return 2;
		if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			return 1;
		return 1;
	}

	VkPresentModeKHR VulkanSwapchain::SelectPresentMode(VulkanGpu* gpu, VulkanSurface* surface, const ContextProperties& contextProperties)
	{
		if (gpu == nullptr || surface == nullptr) return VK_PRESENT_MODE_FIFO_KHR;

		uint32 availCount = 0;
		std::vector<VkPresentModeKHR> availPresentModes;
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->gpu, surface->surface, &availCount, nullptr);
		availPresentModes.resize(availCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->gpu, surface->surface, &availCount, availPresentModes.data());

		std::vector<VkPresentModeKHR> requestPresentMode;
		if (contextProperties.pixelFormat.flags & GraphicsFlag::DoubleBuffer) {
			requestPresentMode.push_back(VK_PRESENT_MODE_FIFO_KHR);
			requestPresentMode.push_back(VK_PRESENT_MODE_FIFO_RELAXED_KHR);
		}
		else if (contextProperties.pixelFormat.flags & GraphicsFlag::TripleBuffer) {
			requestPresentMode.push_back(VK_PRESENT_MODE_MAILBOX_KHR);
		}
		else {
			requestPresentMode.push_back(VK_PRESENT_MODE_IMMEDIATE_KHR);
		}

		for (uint32 request_i = 0; request_i < requestPresentMode.size(); request_i++) {
			for (uint32 avail_i = 0; avail_i < availPresentModes.size(); avail_i++) {
				if (requestPresentMode[request_i] == availPresentModes[avail_i]) {
					return requestPresentMode[request_i];
				}
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	// VulkanCommandPool
	bool VulkanCommandPool::Create(VulkanGpu* gpu) {
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = gpu->queue.queueFamily.graphicsIndex;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkCheckError(first, result, vkCreateCommandPool(gpu->device, &poolInfo, 0, &commandPool), "cannot getswapchain image khr");

		if (result.success) {
			Log_nts.Info("Create command pool is good");
		}

		return result.success;
	}

	bool VulkanCommandPool::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr || commandPool == nullptr) return false;
		vkCheckErrorVoid(vkDestroyCommandPool(gpu->device, commandPool, nullptr));
		commandPool = VK_NULL_HANDLE;
		return false;
	}

	// VulkanSemaphore
	bool VulkanSemaphore::Create(VulkanGpu* gpu) {
		if (gpu == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		vkCheckError(first, result, vkCreateSemaphore(gpu->device, &semaphoreInfo, 0, &aquireSemaphore), "cannot create acquire semaphore");
		vkCheckError(first, result, vkCreateSemaphore(gpu->device, &semaphoreInfo, 0, &submitSemaphore), "cannot create submit semaphore");

		if (result.success) {
			Log_nts.Info("Create semaphore is good");
		}

		return result.success;
	}

	bool VulkanSemaphore::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		if (submitSemaphore != nullptr) {
			vkDestroySemaphore(gpu->device, submitSemaphore, nullptr);
			submitSemaphore = VK_NULL_HANDLE;
		}

		if (aquireSemaphore != nullptr) {
			vkDestroySemaphore(gpu->device, aquireSemaphore, nullptr);
			aquireSemaphore = VK_NULL_HANDLE;
		}
		return false;
	}

	// VulkanPipelineConfig
	void VulkanPipelineConfig::DefaultPipelineConfig(VulkanPipelineConfig* configInfo){
		if (configInfo == nullptr) return;
		configInfo->inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo->inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo->inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		configInfo->inputAssemblyInfo.pNext = nullptr;
		configInfo->inputAssemblyInfo.flags = 0;

		configInfo->viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo->viewportInfo.viewportCount = 1;
		configInfo->viewportInfo.pViewports = nullptr;
		configInfo->viewportInfo.scissorCount = 1;
		configInfo->viewportInfo.pScissors = nullptr;

		configInfo->rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo->rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo->rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo->rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo->rasterizationInfo.lineWidth = 1.0f;
		configInfo->rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo->rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo->rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo->rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo->rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo->rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		configInfo->multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo->multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo->multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo->multisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo->multisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo->multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo->multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		configInfo->colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo->colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo->colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo->colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo->colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo->colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo->colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo->colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo->colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo->colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo->colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo->colorBlendInfo.attachmentCount = 1;
		configInfo->colorBlendInfo.pAttachments = &configInfo->colorBlendAttachment;
		configInfo->colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo->colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo->colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo->colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		configInfo->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo->depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo->depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo->depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo->depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo->depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo->depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo->depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo->depthStencilInfo.front = {};  // Optional
		configInfo->depthStencilInfo.back = {};   // Optional

		configInfo->dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo->dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo->dynamicStateInfo.pDynamicStates = configInfo->dynamicStateEnables.data();
		configInfo->dynamicStateInfo.dynamicStateCount = static_cast<uint32>(configInfo->dynamicStateEnables.size());
		configInfo->dynamicStateInfo.flags = 0;
		configInfo->dynamicStateInfo.pNext = nullptr; // a chercher

		configInfo->bindingDescriptions = VulkanDefaultVertex::GetBindingDescriptions();
		configInfo->attributeDescriptions = VulkanDefaultVertex::GetAttributeDescriptions();
	}

	void VulkanPipelineConfig::EnableAlphaBlending(VulkanPipelineConfig* configInfo){
		if (configInfo == nullptr) return;

		configInfo->colorBlendAttachment.blendEnable = VK_TRUE;
		configInfo->colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo->colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		configInfo->colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		configInfo->colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo->colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo->colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo->colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	}

	// VulkanDefaultVertex
	std::vector<VkVertexInputBindingDescription> VulkanDefaultVertex::GetBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(VulkanDefaultVertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> VulkanDefaultVertex::GetAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanDefaultVertex, position) });
		attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanDefaultVertex, color) });
		attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanDefaultVertex, normal) });
		attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VulkanDefaultVertex, uv) });

		return attributeDescriptions;
	}

	// VulkanRenderPass
	bool VulkanRenderPass::Create(VulkanGpu* gpu, VulkanSwapchain* swapchain)
	{
		if (gpu == nullptr || swapchain == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.format = swapchain->surfaceFormat.format;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		VkAttachmentDescription depthAttachment = {};
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		VulkanImage::FindDepthFormat(gpu, &depthAttachment.format);
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentRef;
		subpassDescription.pDepthStencilAttachment = &depthAttachmentRef;

		std::vector<VkAttachmentDescription> attachments;
		attachments.push_back(colorAttachment);
		attachments.push_back(depthAttachment);

		std::vector<VkSubpassDescription> subpassDescriptions;
		subpassDescriptions.push_back(subpassDescription);

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = VK_ACCESS_NONE;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		std::vector<VkSubpassDependency> subpassDependencies;
		subpassDependencies.push_back(dependency);

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.attachmentCount = attachments.size();
		renderPassInfo.pSubpasses = subpassDescriptions.data();
		renderPassInfo.subpassCount = subpassDescriptions.size();
		renderPassInfo.pDependencies = subpassDependencies.data();
		renderPassInfo.dependencyCount = subpassDependencies.size();

		vkCheckError(first, result, vkCreateRenderPass(gpu->device, &renderPassInfo, 0, &renderPass), "cannot create render pass");

		if (result.success) {
			Log_nts.Info("Create render pass is good");
		}

		return result.success;
	}

	bool VulkanRenderPass::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr || renderPass == nullptr) return false;
		vkCheckErrorVoid(vkDestroyRenderPass(gpu->device, renderPass, nullptr));
		renderPass = VK_NULL_HANDLE;
		return true;
	}

	// VulkanFramebuffer
	bool VulkanFramebuffer::Create(VulkanGpu* gpu, const Vector2u& size, VulkanRenderPass* renderPass, VulkanSwapchain* swapchain)
	{
		if (gpu == nullptr || renderPass == nullptr || swapchain == nullptr) return false;

		VulkanResult result;
		bool first = true;

		this->size = size;

		framebuffer.resize(swapchain->swapchainImages.size());

		std::vector<VkImageView> attachments = {};
		attachments.resize(2);
		attachments[1] = swapchain->depthImage.imageView;

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.width = this->size.width;
		framebufferInfo.height = this->size.height;
		framebufferInfo.renderPass = renderPass->renderPass;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = attachments.size();

		for (uint32 index = 0; index < swapchain->swapchainImages.size(); index++) {
			attachments[0] = swapchain->imageView[index];
			framebufferInfo.pAttachments = attachments.data();

			vkCheckError(first, result, vkCreateFramebuffer(gpu->device, &framebufferInfo, 0, &framebuffer[index]), "cannot create frame buffer number {0}", index);
		}

		if (result.success) {
			Log_nts.Info("Create framebuffer is good");
		}

		return result.success;
	}

	bool VulkanFramebuffer::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		for (usize i = 0; i < framebuffer.size(); i++) {
			vkCheckErrorVoid(vkDestroyFramebuffer(gpu->device, framebuffer[i], nullptr));
			framebuffer[i] = VK_NULL_HANDLE;
		}

		framebuffer.clear();
		size = Vector2u();
		return true;
	}

	// vulkan descriptor pool

	bool VulkanDescriptorPool::Create(VulkanGpu* gpu, VulkanSwapchain* swapchain) {
		if (gpu == nullptr || swapchain == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchain->swapchainImages.size());

		vkCheckError(first, result, vkCreateDescriptorPool(gpu->device, &poolInfo, nullptr, &descriptorPool), "cannot create descriptor pool");

		return result.success;
	}

	bool VulkanDescriptorPool::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr) return false;

		if (descriptorPool != VK_NULL_HANDLE) {
			vkCheckErrorVoid(vkDestroyDescriptorPool(gpu->device, descriptorPool, nullptr));
			descriptorPool = VK_NULL_HANDLE;
		}
		return VulkanStaticDebugInfo::success;
	}

	void VulkanDescriptorPool::Add(VkDescriptorType dType, uint32 count)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = dType;
		poolSize.descriptorCount = count;
	}

	// VulkanPipelineLayout
	bool VulkanPipelineLayout::Create(VulkanGpu* gpu, VulkanSwapchain* swapchain)
	{
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		if (!CreateDescriptorSetLayout(gpu, swapchain)) {
			return false;
		}

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = 1;
		layoutInfo.pSetLayouts = &descriptorSetLayout;

		vkCheckError(first, result, vkCreatePipelineLayout(gpu->device, &layoutInfo, 0, &pipelineLayout), "cannot create pipeline layout");

		if (result.success) {
			Log_nts.Info("Create pipeline layout is good");
		}

		return result.success;
	}

	bool VulkanPipelineLayout::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		if (descriptorSetLayout != VK_NULL_HANDLE) {
			vkCheckErrorVoid(vkDestroyDescriptorSetLayout(gpu->device, descriptorSetLayout, nullptr));
			descriptorSetLayout = VK_NULL_HANDLE;
		}

		if (pipelineLayout != VK_NULL_HANDLE) {
			vkCheckErrorVoid(vkDestroyPipelineLayout(gpu->device, pipelineLayout, nullptr));
			pipelineLayout = VK_NULL_HANDLE;
		}
		return true;
	}
	bool VulkanPipelineLayout::CreateDescriptorSetLayout(VulkanGpu* gpu, VulkanSwapchain* swapchain)
	{
		if (gpu == nullptr || swapchain == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkDescriptorSetLayoutCreateInfo layoutDescriptorInfo{};
		layoutDescriptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		if (layoutBindings.size() > 0) {
			layoutDescriptorInfo.bindingCount = layoutBindings.size();
			layoutDescriptorInfo.pBindings = layoutBindings.data();
		}

		vkCheckError(first, result, vkCreateDescriptorSetLayout(gpu->device, &layoutDescriptorInfo, nullptr, &descriptorSetLayout), "cannot create descriptor set layout");

		return result.success;
	}

	// VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
	void VulkanPipelineLayout::Add(uint32 binding, VkDescriptorType type, VkShaderStageFlags shaderStage)
	{
		layoutBindings.push_back({ binding, type, 1, shaderStage, nullptr });
	}

	void VulkanPipelineLayout::AddPushConstantRange(const VkPushConstantRange& pushConstantRange)
	{
		pushConstantRanges.push_back(pushConstantRange);
	}

	bool VulkanPipelineLayout::IsValid()
	{
		return pipelineLayout != VK_NULL_HANDLE;
	}

	// vulkan buffer
	bool VulkanBuffer::WriteToBuffer(const void* data, usize size, usize offset)
	{
		if (mappedData == nullptr) return false;

		if (size == VK_WHOLE_SIZE) {
			memcpy(mappedData, data, this->size);
		}
		else {
			char* memOffset = (char*)mappedData;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
		return true;
	}

	bool VulkanBuffer::Destroy(VulkanGpu* gpu)
	{
		if (gpu == nullptr || gpu->device == nullptr) return false;

		if (buffer != nullptr) {
			vkCheckErrorVoid(vkDestroyBuffer(gpu->device, buffer, nullptr));
			buffer = VK_NULL_HANDLE;
		}

		bool success = VulkanStaticDebugInfo::success;

		if (bufferMemory != nullptr) {
			vkCheckErrorVoid(vkFreeMemory(gpu->device, bufferMemory, nullptr));
			bufferMemory = VK_NULL_HANDLE;
		}

		return success && VulkanStaticDebugInfo::success;
	}

	bool VulkanBuffer::Mapped(VulkanGpu* gpu, usize size, usize offset, VkMemoryMapFlags flag)
	{
		if (gpu == nullptr || bufferMemory == nullptr || buffer == nullptr || mappedData != nullptr) return false;

		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkMapMemory(gpu->device, bufferMemory, offset, size, flag, (void**)&mappedData), "cannot map buffer memory");
		return result.success;
	}

	bool VulkanBuffer::UnMapped(VulkanGpu* gpu)
	{
		if (gpu == nullptr || bufferMemory == nullptr || buffer == nullptr || mappedData == nullptr || mappedData == nullptr) return false;
		vkCheckErrorVoid(vkUnmapMemory(gpu->device, bufferMemory));
		mappedData = nullptr;
		return VulkanStaticDebugInfo::success;
	}

	bool VulkanBuffer::Flush(VulkanGpu* gpu, usize size, usize offset)
	{
		if (gpu == nullptr || bufferMemory == nullptr || buffer == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = bufferMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;

		vkCheckError(first, result, vkFlushMappedMemoryRanges(gpu->device, 1, &mappedRange), "cannot flush buffer memory");
		return result.success;
	}

	int64 VulkanBuffer::FindMemoryType(VulkanGpu* gpu, uint32 typeFilter, VkMemoryPropertyFlags properties)
	{
		if (gpu == nullptr) return 0;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(gpu->gpu, &memProperties);

		for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		return -1;
	}

	bool VulkanBuffer::CreateBuffer(VulkanGpu* gpu, VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
	{
		if (gpu == nullptr) {
			return false;
		}

		VulkanResult result;
		bool first = true;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = sharingMode;

		vkCheckError(first, result, vkCreateBuffer(gpu->device, &bufferInfo, nullptr, &buffer), "cannot create buffer");

		if (!result.success) {
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkCheckErrorVoid(vkGetBufferMemoryRequirements(gpu->device, buffer, &memRequirements));

		if (!VulkanStaticDebugInfo::success) {
			Log_nts.Error("Cannot get memory buffer requirements");
			return false;
		}

		int64 index = FindMemoryType(gpu, memRequirements.memoryTypeBits, properties);

		if (index < 0) {
			Log_nts.Error("Cannot find correct memory type");
			return false;
		}

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = index;

		vkCheckError(first, result, vkAllocateMemory(gpu->device, &allocInfo, nullptr, &bufferMemory), "cannot allocat memory buffer");
		vkCheckError(first, result, vkBindBufferMemory(gpu->device, buffer, bufferMemory, 0), "cannot bind buffer memory");

		return result.success;
	}

	bool VulkanBuffer::CopyBuffer(VulkanGpu* gpu, VulkanCommandPool* commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		if (gpu == nullptr || commandPool == nullptr) {
			return false;
		}

		VulkanResult result;
		bool first = true;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool->commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;

		vkCheckError(first, result, vkAllocateCommandBuffers(gpu->device, &allocInfo, &commandBuffer), "cannot allocate command buffer");

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkCheckError(first, result, vkBeginCommandBuffer(commandBuffer, &beginInfo), "cannot begin command buffer");

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optionnel
		copyRegion.dstOffset = 0; // Optionnel
		copyRegion.size = size;

		if (result.success) {
			vkCheckErrorVoid(vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion));
			result.success = VulkanStaticDebugInfo::success;
		}
		vkCheckError(first, result, vkEndCommandBuffer(commandBuffer), "cannot end command buffer");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkCheckError(first, result, vkQueueSubmit(gpu->queue.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "cannot submit queue");
		vkCheckError(first, result, vkQueueWaitIdle(gpu->queue.graphicsQueue), "cannot wait idle queue");

		if (result.success) {
			vkCheckErrorVoid(vkFreeCommandBuffers(gpu->device, commandPool->commandPool, 1, &commandBuffer));
			result.success = VulkanStaticDebugInfo::success;
		}
		return result.success;
	}

	// uniform buffer

	bool VulkanUBO::Create(VulkanGpu* gpu, const UniformInputAttribute& uba, VkBufferUsageFlags usage, std::vector<VkDescriptorSet>& descriptorSets, VkDescriptorType descriptorType)
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

		if (uniformInput.usage == BufferUsageType::DynamicDraw) {
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

	bool VulkanUBO::Destroy(VulkanGpu* gpu)
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

	bool VulkanUBO::Binds(VulkanGpu* gpu, void* data, usize size, uint32 instanceIndex)
	{
		if (gpu == nullptr) return false;

		bool success = false;

		currentOffset = 0;
		if (uniformInput.usage == BufferUsageType::DynamicDraw) {
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

	bool VulkanUBO::Bind(VulkanGpu* gpu, void* data, usize size, uint32 index, uint32 instanceIndex)
	{
		if (gpu == nullptr) return false;

		bool success = false;

		currentOffset = 0;

		if (uniformInput.usage == BufferUsageType::DynamicDraw) {
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

	bool VulkanUBO::BindSimple(VulkanGpu* gpu, void* data, usize size, uint32 index, uint32 instanceIndex)
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

	// command buffer
	bool VulkanCommandBuffer::Create(VulkanGpu* gpu, VulkanSwapchain* swapchain, VulkanCommandPool* commandPool)
	{
		if (gpu == nullptr || swapchain == nullptr || commandPool == nullptr) return false;

		VulkanResult result;
		bool first = true;

		commandBuffers.resize(swapchain->swapchainImages.size());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool->commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		vkCheckError(first, result, vkAllocateCommandBuffers(gpu->device, &allocInfo, commandBuffers.data()), "cannot allocate command buffer");
		return true;
	}

	bool VulkanCommandBuffer::Destroy(VulkanGpu* gpu, VulkanCommandPool* commandPool)
	{
		if (gpu == nullptr || gpu->device == nullptr || commandBuffers.size() == 0) return false;

		VulkanResult result;
		bool first = true;

		vkCheckError(first, result, vkDeviceWaitIdle(gpu->device), "cannot wait device idle");
		vkCheckErrorVoid(vkFreeCommandBuffers(gpu->device, commandPool->commandPool, 1, commandBuffers.data()));
		commandBuffers.clear();
		return true;
	}

}  //  nkentseu