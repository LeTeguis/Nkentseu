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

		if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			Log_nts.Error("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}
		else if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			Log_nts.Warning("{0}", pCallbackData->pMessage);
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
		instanceInfo.enabledExtensionCount = extension->instanceExtension.size();
		instanceInfo.ppEnabledLayerNames = extension->layers.data();
		instanceInfo.enabledLayerCount = extension->layers.size();

		VkResultNts result = vkCheckError(vkCreateInstance(&instanceInfo, 0, &instance), "Cannot create vulkan instance");

		if (result.success) {	
			Log_nts.Error("Create vulkan instance is good");

			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

			if (vkCreateDebugUtilsMessengerEXT) {
				VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
				debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
				debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
				debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
				debugInfo.pfnUserCallback = vkDebugCallback;

				vkCreateDebugUtilsMessengerEXT(instance, &debugInfo, 0, &debugMessenger);
			}

			return true;
		}
		return false;
	}

	bool VulkanSurface::Create(Window* window, VulkanInstance* instance)
	{
		if (window == nullptr || instance == nullptr) return false;
		if (window->GetInternal() == nullptr || window->GetInternal()->GetWindowDisplay() == nullptr) return false;
		VkResultNts result;

#ifdef NKENTSEU_PLATFORM_WINDOWS
		WindowInternal* internal = window->GetInternal();

		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.hwnd = (HWND)internal->GetWindowDisplay()->windowHandle;
		surfaceInfo.hinstance = internal->GetWindowDisplay()->instanceHandle;

		result = vkCheckError(vkCreateWin32SurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface");

		if (result.success) {
			Log_nts.Error("Create vulkan surface is good");
			return true;
		}
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
		WindowInternal* internal = window->GetInternal();

		VkXlibSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.dpy = PlatformState.display;
		surfaceInfo.window = internal->GetWindowDisplay()->windowHandle;

		result = vkCheckError(vkCreateXlibSurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface");

		if (result.success) {
			Log_nts.Error("Create vulkan surface is good");
			return true;
		}
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
		WindowInternal* internal = window->GetInternal();

		VkXcbSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.pNext = nullptr;
		surfaceInfo.flags = 0;
		surfaceInfo.dpy = PlatformState.connection;
		surfaceInfo.window = internal->GetWindowDisplay()->windowHandle;

		result = vkCheckError(vkCreateXcbSurfaceKHR(static_cast<VkInstance>(instance->instance), &surfaceInfo, 0, &surface), "Cannot create vulkan surface");

		if (result.success) {
			Log_nts.Error("Create vulkan surface is good");
			return true;
		}
#endif
		return result.success;
	}
	bool VulkanGpu::GetDevice(VulkanInstance* instance, VulkanSurface* surface, VulkanExtension* extension)
	{
		if (instance == nullptr || surface == nullptr) return false;
		VkResultNts result;
		uint32 gpuCount = 0;
		VkPhysicalDevice gpusAll[10];
		graphicsIndex = -1;
		result = vkCheckError(vkEnumeratePhysicalDevices(instance->instance, &gpuCount, 0), "Cannot count physical device");
		result = result.success ? vkCheckError(vkEnumeratePhysicalDevices(instance->instance, &gpuCount, gpusAll), "Cannot get all physical device") : result;

		for (uint32 index = 0; index < gpuCount; index++) {
			VkPhysicalDevice currentGpu = gpusAll[index];
			gpus.push_back(gpusAll[index]);

			uint32 currentQFC = 0;
			VkQueueFamilyProperties currentQP[10];

			vkGetPhysicalDeviceQueueFamilyProperties(currentGpu, &currentQFC, 0);
			vkGetPhysicalDeviceQueueFamilyProperties(currentGpu, &currentQFC, currentQP);

			for (uint32 indexQueue = 0; indexQueue < currentQFC; indexQueue++) {
				if (currentQP[indexQueue].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					VkBool32 surfaceSupport = VK_FALSE;

					vkCheckError(vkGetPhysicalDeviceSurfaceSupportKHR(currentGpu, indexQueue, surface->surface, &surfaceSupport), "cannot support physical device surface");

					if (surfaceSupport) {
						graphicsIndex = indexQueue;
						queueProperties.push_back(currentQP[indexQueue]);
						gpu = currentGpu;
						break;
					}
				}
			}
		}

		if (graphicsIndex < 0) {
			return false;
		}

		bool success = GetLogicalDevice(surface, extension);

		return true;
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
		deviceInfo.queueCreateInfoCount = queueInfos.size();
		deviceInfo.ppEnabledExtensionNames = extension->deviceExtension.data();
		deviceInfo.enabledExtensionCount = extension->deviceExtension.size();

		VkResultNts result = vkCheckError(vkCreateDevice(gpu, &deviceInfo, 0, &device), "cannot create device");

		if (result.success) {
			Log_nts.Error("Create vulkan device is good");
		}
		// VK_ERROR_DEVICE_LOST
		return result.success;
	}

	bool VulkanSwapchain::Create(VulkanGpu* gpu, VulkanSurface* surface) {
		VkResultNts result;

		uint32 formatCount = 0;
		VkSurfaceFormatKHR surfaceFormats[10];
		result = vkCheckError(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, 0), "cannot count device surface format");
		result = result.success ? vkCheckError(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface->surface, &formatCount, surfaceFormats), "cannot get device surface format") : result;

		for (uint32 indexFormat = 0; indexFormat < formatCount; indexFormat++) {
			VkSurfaceFormatKHR currentFormat = surfaceFormats[indexFormat];

			if (currentFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
				surfaceFormat = currentFormat;
				break;
			}
		}

		VkSurfaceCapabilitiesKHR surfaceCaps = {};
		result = result.success ? vkCheckError(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->gpu, surface->surface, &surfaceCaps), "cannot get surface capabilities khr") : result;

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

		result = result.success ? vkCheckError(vkCreateSwapchainKHR(gpu->device, &swapchainInfo, 0, &swapchain), "cannot create swapchain") : result;

		if (result.success) {
			Log_nts.Error("Create vulkan swapchain is good");
		}

		return result.success;
	}
}  //  nkentseu

#endif



/*
VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
swapchainCreateInfo.surface = surface;
swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
swapchainCreateInfo.imageFormat = chosenFormat.format;
swapchainCreateInfo.imageColorSpace = chosenFormat.colorSpace;
swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
swapchainCreateInfo.imageArrayLayers = 1;
swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
swapchainCreateInfo.presentMode = chosenPresentMode;
swapchainCreateInfo.clipped = VK_TRUE;
swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
*/