//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 09:30:26 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanInternal.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN

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
#include "Nkentseu/Platform/Window/Windows/WindowInternal.h"
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
		//instanceExtension.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);

		// Device Extension
		//deviceExtension.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);
		deviceExtension.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME);
		deviceExtension.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME);

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
			auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

			if (vkCreateDebugUtilsMessengerEXT) {
				VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
				debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
				debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
				debugInfo.pfnUserCallback = vkDebugCallback;

				vkCreateDebugUtilsMessengerEXT(instance, &debugInfo, 0, &debugMessenger);
			}

			/*if (vkDestroyDebugUtilsMessengerEXT) {
				vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, 0);
			}*/
		}
		return result.success;
	}

	// Vulkan surface
	bool VulkanSurface::Create(Window* window, VulkanInstance* instance)
	{
		if (window == nullptr || instance == nullptr) return false;
		if (window->GetInternal() == nullptr || window->GetInternal()->GetWindowDisplay() == nullptr) return false;
		VulkanResult result;
		bool first = true;

#ifdef NKENTSEU_PLATFORM_WINDOWS
		WindowInternal* internal = window->GetInternal();

		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.hwnd = (HWND)internal->GetWindowDisplay()->windowHandle;
		surfaceInfo.hinstance = internal->GetWindowDisplay()->instanceHandle;

		vkCheckError(first, result, vkCreateWin32SurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface windows");

		if (result.success) {
			Log_nts.Info("Create vulkan surface is good for windows");
		}
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
		WindowInternal* internal = window->GetInternal();

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
		WindowInternal* internal = window->GetInternal();

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
	bool VulkanGpu::GetDevice(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension)
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

			vkGetPhysicalDeviceProperties(gpu, &properties);
			result.success = GetLogicalDevice(surface, extension);
		}

		return result.success;
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

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature{};
		extendedDynamicStateFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
		extendedDynamicStateFeature.extendedDynamicState = VK_TRUE;

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
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewInfo.subresourceRange = imageRange;

		for (uint32 index = 0; index < scImageCount; index++) {
			swapchainImages.push_back(scImages[index]);
			viewInfo.image = swapchainImages[index];

			vkCheckError(first, result, vkCreateImageView(gpu->device, &viewInfo, 0, &imageView[index]), "cannot create image view number {0}", index);
		}
		
		if (result.success) {
			Log_nts.Info("Create swapchain is good");
		}

		return result.success;
	}

	bool VulkanSwapchain::Destroy(VulkanGpu* gpu)
	{
		for (usize i = 0; i < imageView.size(); i++) {
			vkCheckErrorVoid(vkDestroyImageView(gpu->device, imageView[i], nullptr));
		}
		imageView.clear();
		swapchainImages.clear();

		vkCheckErrorVoid(vkDestroySwapchainKHR(gpu->device, swapchain, nullptr));
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

		VkAttachmentDescription attachment = {};
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.format = swapchain->surfaceFormat.format;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentRef;

		std::vector<VkAttachmentDescription> attachments;
		attachments.push_back(attachment);

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

	// VulkanFramebuffer
	bool VulkanFramebuffer::Create(VulkanGpu* gpu, const Vector2u& size, VulkanRenderPass* renderPass, VulkanSwapchain* swapchain)
	{
		if (gpu == nullptr || renderPass == nullptr || swapchain == nullptr) return false;

		VulkanResult result;
		bool first = true;

		this->size = size;

		framebuffer.resize(swapchain->swapchainImages.size());

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.width = this->size.width;
		framebufferInfo.height = this->size.height;
		framebufferInfo.renderPass = renderPass->renderPass;
		framebufferInfo.layers = 1;
		framebufferInfo.attachmentCount = 1;

		for (uint32 index = 0; index < swapchain->swapchainImages.size(); index++) {
			framebufferInfo.pAttachments = &swapchain->imageView[index];

			vkCheckError(first, result, vkCreateFramebuffer(gpu->device, &framebufferInfo, 0, &framebuffer[index]), "cannot create frame buffer number {0}", index);
		}

		if (result.success) {
			Log_nts.Info("Create framebuffer is good");
		}

		return result.success;
	}

	bool VulkanFramebuffer::Destroy(VulkanGpu* gpu)
	{
		for (usize i = 0; i < framebuffer.size(); i++) {
			vkCheckErrorVoid(vkDestroyFramebuffer(gpu->device, framebuffer[i], nullptr));
		}

		framebuffer.clear();
		size = Vector2u();
		return true;
	}

	// VulkanPipelineLayout
	bool VulkanPipelineLayout::Create(VulkanGpu* gpu)
	{
		if (gpu == nullptr) return false;
		VulkanResult result;
		bool first = true;

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		vkCheckError(first, result, vkCreatePipelineLayout(gpu->device, &layoutInfo, 0, &pipelineLayout), "cannot create pipeline layout");

		if (result.success) {
			Log_nts.Info("Create pipeline layout is good");
		}

		return result.success;
	}
}  //  nkentseu

#endif