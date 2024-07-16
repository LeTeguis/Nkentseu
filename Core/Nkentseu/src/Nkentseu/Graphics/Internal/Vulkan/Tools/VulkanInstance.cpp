//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 11:49:41 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanInstance.h"
#include <Logger/Formatter.h>

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

#include "Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h"

namespace nkentseu {
    
	// Vulkan Debug information
	static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity, VkDebugUtilsMessageTypeFlagsEXT msgFlags, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

		std::string file = VulkanStaticDebugInfo::file_call;
		std::string methode = VulkanStaticDebugInfo::methode_call;
		uint32 line = VulkanStaticDebugInfo::line_call;

		VulkanStaticDebugInfo::Result(false);

		if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}
		else if (msgSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Warning("{0}", pCallbackData->pMessage);
			return VK_FALSE;
		}

		VulkanStaticDebugInfo::Result(true);

		return VK_TRUE;
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

}  //  nkentseu