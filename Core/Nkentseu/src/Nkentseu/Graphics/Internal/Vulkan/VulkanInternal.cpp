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
	static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity, VkDebugUtilsMessageTypeFlagsEXT msgFlags, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

		std::string file = VulkanStaticDebugInfo::file_call;
		std::string methode = VulkanStaticDebugInfo::methode_call;
		uint32 line = VulkanStaticDebugInfo::line_call;

		if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}
		else if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Warning("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}

		return VK_TRUE;
	}

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

		// Device Extension
		deviceExtension.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		// Layer
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

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
	bool VulkanGpu::GetDevice(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension)
	{
		if (instance == nullptr || surface == nullptr) return false;
		VulkanResult result;
		bool first = true;

		uint32 gpuCount = 0;
		VkPhysicalDevice gpusAll[10];
		graphicsIndex = -1;
		vkCheckError(first, result, vkEnumeratePhysicalDevices(instance->instance, &gpuCount, 0), "Cannot count physical device");
		vkCheckError(first, result, vkEnumeratePhysicalDevices(instance->instance, &gpuCount, gpusAll), "Cannot get all physical device");

		if (result.success) {
			for (uint32 index = 0; index < gpuCount; index++) {
				VkPhysicalDevice currentGpu = gpusAll[index];
				gpus.push_back(gpusAll[index]);

				uint32 currentQFC = 0;
				VkQueueFamilyProperties currentQP[10];

				vkCheckErrorVoid(vkGetPhysicalDeviceQueueFamilyProperties(currentGpu, &currentQFC, 0));
				vkCheckErrorVoid(vkGetPhysicalDeviceQueueFamilyProperties(currentGpu, &currentQFC, currentQP));

				for (uint32 indexQueue = 0; indexQueue < currentQFC; indexQueue++) {
					if (currentQP[indexQueue].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						VkBool32 surfaceSupport = VK_FALSE;
						VulkanResult result1;
						bool first1 = true;
						vkCheckError(first1, result1, vkGetPhysicalDeviceSurfaceSupportKHR(currentGpu, indexQueue, surface->surface, &surfaceSupport), "cannot support physical device surface");

						if (surfaceSupport) {
							graphicsIndex = indexQueue;
							queueProperties.push_back(currentQP[indexQueue]);
							gpu = currentGpu;
							VulkanConvert::GetResourceLimits(gpu);
							break;
						}
					}
				}
			}

			if (graphicsIndex < 0) {
				return false;
			}

			result.success = GetLogicalDevice(surface, extension);
		}

		return result.success;
	}

	bool VulkanGpu::GetLogicalDevice(VulkanSurface* surface, VulkanExtension* extension)
	{
		float32 queuePriority = 1.0f;

		std::vector<VkDeviceQueueCreateInfo> queueInfos;

		VkDeviceQueueCreateInfo queueGraphicsFamilyInfo = {};
		queueGraphicsFamilyInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueGraphicsFamilyInfo.queueFamilyIndex = graphicsIndex;
		queueGraphicsFamilyInfo.queueCount = 1;
		queueGraphicsFamilyInfo.pQueuePriorities = &queuePriority;

		queueInfos.push_back(queueGraphicsFamilyInfo);

		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pQueueCreateInfos = queueInfos.data();
		deviceInfo.queueCreateInfoCount = (uint32)queueInfos.size();
		deviceInfo.ppEnabledExtensionNames = extension->deviceExtension.data();
		deviceInfo.enabledExtensionCount = (uint32)extension->deviceExtension.size();

		VulkanResult result;
		bool first = true;
		vkCheckError(first, result, vkCreateDevice(gpu, &deviceInfo, 0, &device), "cannot create device");

		if (result.success) {
			Log_nts.Info("Create vulkan device is good");
			vkCheckErrorVoid(vkGetDeviceQueue(device, graphicsIndex, 0, &graphicsQueue));
		}
		// VK_ERROR_DEVICE_LOST
		return result.success;
	}

	bool VulkanSwapchain::Create(VulkanGpu* gpu, VulkanSurface* surface) {
		if (gpu == nullptr || surface == nullptr) return false;
		VulkanResult result;
		bool first = true;

		uint32 formatCount = 0;
		VkSurfaceFormatKHR surfaceFormats[10];
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, 0), "cannot count device surface format");
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, surfaceFormats), "cannot get device surface format");

		for (uint32 indexFormat = 0; indexFormat < formatCount; indexFormat++) {
			VkSurfaceFormatKHR currentFormat = surfaceFormats[indexFormat];

			if (currentFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
				surfaceFormat = currentFormat;
				break;
			}
		}

		VkSurfaceCapabilitiesKHR surfaceCaps = {};
		vkCheckError(first, result, vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->gpu, surface->surface, &surfaceCaps), "cannot get surface capabilities khr");

		uint32 imageCount;
		imageCount = surfaceCaps.minImageCount + 1;
		imageCount = imageCount > surfaceCaps.maxImageCount ? imageCount - 1 : imageCount;

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.surface = surface->surface;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.preTransform = surfaceCaps.currentTransform;
		swapchainInfo.imageExtent = surfaceCaps.currentExtent;
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageArrayLayers = 1;

		vkCheckError(first, result, vkCreateSwapchainKHR(gpu->device, &swapchainInfo, 0, &swapchain), "cannot create swapchain");

		if (result.success) {
			Log_nts.Info("Create vulkan swapchain is good");
		}
		uint32 scImageCount = 0;
		VkImage scImages[5];

		vkCheckError(first, result, vkGetSwapchainImagesKHR(gpu->device, swapchain, &scImageCount, 0), "cannot count swapchain image khr");
		vkCheckError(first, result, vkGetSwapchainImagesKHR(gpu->device, swapchain, &scImageCount, scImages), "cannot getswapchain image khr");

		for (uint32 index = 0; index < scImageCount; index++) {
			swapchainImages.push_back(scImages[index]);
		}

		return result.success;
	}

	bool VulkanCommandPool::Create(VulkanGpu* gpu) {
		if (gpu == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = gpu->graphicsIndex;
		vkCheckError(first, result, vkCreateCommandPool(gpu->device, &poolInfo, 0, &commandPool), "cannot getswapchain image khr");

		return result.success;
	}

	bool VulkanSemaphore::Create(VulkanGpu* gpu) {
		if (gpu == nullptr) return false;

		VulkanResult result;
		bool first = true;

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		vkCheckError(first, result, vkCreateSemaphore(gpu->device, &semaphoreInfo, 0, &aquireSemaphore), "cannot create acquire semaphore");
		vkCheckError(first, result, vkCreateSemaphore(gpu->device, &semaphoreInfo, 0, &submitSemaphore), "cannot create submit semaphore");

		return result.success;
	}
}  //  nkentseu

#endif