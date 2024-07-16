//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:07:34 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanSurface.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Graphics/Internal/Vulkan/VulkanUtils.h>

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

namespace nkentseu {
    


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

}  //  nkentseu