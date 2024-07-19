//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:51:42 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanContext.h"
#include "Nkentseu/Core/Window.h"

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

#include "VulkanUtils.h"

#include <Nkentseu/Event/EventFilter.h>
#include <Nkentseu/Event/EventBroker.h>

namespace nkentseu {
    using namespace maths;

    VulkanContext::VulkanContext(Memory::Shared<Window> window, const ContextProperties& contextProperties) : m_Window(window), m_IsInitialize(false), m_ContextProperties(contextProperties) {
    }

    VulkanContext::~VulkanContext(){
    }

    bool VulkanContext::Initialize() {
        if (m_Window == nullptr) return false;

        if (!InitInstanceExtensionAndLayer()) {
            Log_nts.Error("Cannot init instance extension");
        }

        if (!CreateInstance()) {
            Log_nts.Error("Cannot create instance");
            return false;
        }

        if (!CreateSurface()) {
            Log_nts.Error("Cannot create surface");
            return false;
        }

        if (!GetPhysicalDevice()) {
            Log_nts.Error("Cannot get physical device");
            return false;
        }

        if (!CreateLogicalDevice()) {
            Log_nts.Error("Cannot create logical device");
            return false;
        }

        if (!CreateCommandPool()) {
            Log_nts.Error("Cannot create command pool");
            return false;
        }

        if (!CreateSwapchaine(m_Window->ConvertPixelToDpi(m_Window->GetSize()))) {
            Log_nts.Error("Cannot create swapchaine");
            return false;
        }

        if (!CreateSemaphores()) {
            Log_nts.Error("Cannot create semaphore");
            return false;
        }

        if (!CreateCommandBuffer()) {
            Log_nts.Error("Cannot create semaphore");
            return false;
        }

        if (!CreateRenderPass()) {
            Log_nts.Error("Cannot create render pass");
            return false;
        }
        EventTraker.AddObserver(REGISTER_CLIENT_EVENT(VulkanContext::OnEvent));
        m_IsInitialize = true;
        return m_IsInitialize;
    }

    bool VulkanContext::IsInitialize() {
        return m_IsInitialize;
    }

    bool VulkanContext::EnableVSync() {
        return false;
    }

    bool VulkanContext::DisableVSync() {
        return false;
    }

    bool VulkanContext::Deinitialize()
    {
        bool success = true;
        EventTraker.RemoveObserver(REGISTER_CLIENT_EVENT(VulkanContext::OnEvent));

        if (defaultCommandBuffer != nullptr) {
            device.freeCommandBuffers(commandPool, defaultCommandBuffer);
        }

        if (!DestroyCommandBuffer()) {
            Log_nts.Error("cannot destroy command buffer"); success = false;
        }
        if (!DestroyRenderPass()) {
            Log_nts.Error("cannot destroy render pass"); success = false;
        }
        if (!DestroySemaphores()) {
            Log_nts.Error("cannot destroy semaphore"); success = false;
        }
        if (!DestroySwapchaine()) {
            Log_nts.Error("cannot destroy swapchaine"); success = false;
        }
        if (!DestroyCommandPool()) {
            Log_nts.Error("cannot destroy command pool"); success = false;
        }
        if (!DestroyDevice()) {
            Log_nts.Error("cannot destroy device"); success = false;
        }
        if (!DestroySurface()) {
            Log_nts.Error("cannot destroy surface"); success = false;
        }
        if (!DestroyInstance()) {
            Log_nts.Error("cannot destroy instance"); success = false;
        }
        return success;
    }

    Memory::Shared<Window> VulkanContext::GetWindow() {
        return m_Window;
    }

    const ContextProperties& VulkanContext::GetProperties()
    {
        return m_ContextProperties;
    }


    const GraphicsInfos& VulkanContext::GetGraphicsInfo() {
        static GraphicsInfos tmp;
        return tmp;
    }

    bool VulkanContext::AddCleanUpCallback(CleanUpCallBackFn func)
    {
        return AddRecreateCallback(m_CleanList, func); // Function not found
    }

    bool VulkanContext::RemoveCleanUpCallback(CleanUpCallBackFn func)
    {
        return RemoveRecreateCallback(m_CleanList, func); // Function not found
    }

    bool VulkanContext::AddRecreateCallback(RerecreateCallBackFn func)
    {
        return AddRecreateCallback(m_RecreateList, func); // Function not found
    }

    bool VulkanContext::RemoveRecreateCallback(RerecreateCallBackFn func)
    {
        return RemoveRecreateCallback(m_RecreateList, func); // Function not found
    }

    bool VulkanContext::CanUse()
    {
        return m_CanUse;
    }

    vk::CommandBuffer VulkanContext::GetCommandBuffer(uint32 id)
    {
        if (id < commandBuffers.size()) {
            return commandBuffers[id];
        }
        return nullptr;
    }

    vk::CommandBuffer VulkanContext::GetCommandBuffer()
    {
        return commandBuffers[currentFrameIndex];
    }

    bool VulkanContext::IsValidContext()
    {
        return m_IsInitialize;
    }

    bool VulkanContext::Prepare()
    {
        isAcquiere = false;
        if (!isPresent || !CanAquire()) return false;

        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, device.acquireNextImageKHR(swapchain, UINT64_MAX, aquireSemaphore, nullptr, &currentFrameIndex), "cannot acquier next image khr ({0})", currentImageIndex);

        if (!result.success) {
            if (result.result == vk::Result::eErrorOutOfDateKHR) {
                RecreateSwapChain();
                isPresent = true;
                return false;
            }
            else if (result.result != vk::Result::eSuboptimalKHR && result.result != vk::Result::eSuccess) {
                isPresent = true;
                return false;
            }
        }

        vk::CommandBufferBeginInfo beginInfo = {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit ;
        vkCheckError(first, result, commandBuffers[currentFrameIndex].begin(&beginInfo), "cannot start command buffer");

        isAcquiere = result.success;

        return result.success;
    }

    bool VulkanContext::Present()
    {
        if (!isAcquiere) return false;
        VulkanResult result;
        bool first = true;

        vkCheckErrorVoid(commandBuffers[currentFrameIndex].end());

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot finish command buffer");
        }

        vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        vk::SubmitInfo submitInfo = {};
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrameIndex];
        submitInfo.pSignalSemaphores = &submitSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &aquireSemaphore;
        submitInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, queueFamilly.graphicsQueue.submit(1, &submitInfo, {}), "cannot submit command");

        vk::PresentInfoKHR presentInfo = {};
        presentInfo.pSwapchains = &swapchain;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &currentFrameIndex;
        presentInfo.pWaitSemaphores = &submitSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, queueFamilly.graphicsQueue.presentKHR(&presentInfo), "cannot present image");

        if (!result.success) {
            bool exited = false;
            if (result.result == vk::Result::eErrorOutOfDateKHR || result.result == vk::Result::eSuboptimalKHR) {
                if (result.result == vk::Result::eErrorOutOfDateKHR) {
                    vkCheckErrorVoid(device.waitIdle());
                }
                RecreateSwapChain();
                commandBuffers.clear();
                isPresent = true;
                exited = true;
            }
            else if (result.result != vk::Result::eSuboptimalKHR && result.result != vk::Result::eSuccess) {
                return false;
            }
        }

        vkCheckErrorVoid(device.waitIdle());
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot wait device idle");
        }

        currentFrameIndex++;

        if (currentFrameIndex >= swapchainImages.size()) {
            currentFrameIndex = 0;
        }
        isPresent = true;
        return true;
    }

    bool VulkanContext::CleanupSwapChain() {
        if (!m_IsInitialize) return false;

        bool success = true;

        CallRecreateCallback(m_CleanList);

        if (!DestroyCommandBuffer()) {
            Log_nts.Error("cannot destroy command buffer"); success = false;
        }

        if (!DestroySwapchaine()) {
            Log_nts.Error("cannot destroy swapchaine"); success = false;
        }

        return success;
    }

    bool VulkanContext::RecreateSwapChain() {
        if (!m_IsInitialize) return false;

        if (!CleanupSwapChain()) {
            Log_nts.Error("cannot clean up swap chain");
        }

        if (!CreateSwapchaine(m_Window->ConvertPixelToDpi(Vector2f(m_Window->GetSize().width, m_Window->GetSize().height)))) {
            Log_nts.Error("Cannot create swapchaine");
            return false;
        }

        if (!CreateCommandBuffer()) {
            Log_nts.Error("Cannot create semaphore");
            return false;
        }

        CallRecreateCallback(m_RecreateList);
        return true;
    }

    bool VulkanContext::AddRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe, RerecreateCallBackFn func)
    {
        if (func == nullptr) return false;

        auto it = std::find_if(callbackListe.begin(), callbackListe.end(),
            [&func](const RerecreateCallBackFn& registeredFunc) {
                // Compare target of std::function objects
                return registeredFunc.target<bool(bool)>() == func.target<bool(bool)>();
            });

        if (it == callbackListe.end()) {
            callbackListe.push_back(func);
            return true;
        }
        return false; // Function already exists
    }

    bool VulkanContext::RemoveRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe, RerecreateCallBackFn func)
    {
        if (func == nullptr) return false;

        auto it = std::find_if(callbackListe.begin(), callbackListe.end(),
            [&func](const RerecreateCallBackFn& registeredFunc) {
                // Compare target of std::function objects
                return registeredFunc.target<bool(bool)>() == func.target<bool(bool)>();
            });

        if (it != callbackListe.end()) {
            callbackListe.erase(it);
            return true;
        }
        return false; // Function not found
    }

    bool VulkanContext::CallRecreateCallback(std::vector<RerecreateCallBackFn>& callbackListe)
    {
        // Iterate over m_RecreateList and call non-null callbacks, remove null callbacks
        auto it = callbackListe.begin();
        while (it != callbackListe.end()) {
            if (*it) {
                // Call the callback with the forceRecreate parameter
                (*it)(true);
                ++it;
            }
            else {
                // Remove null callbacks
                it = callbackListe.erase(it);
            }
        }
        return true;
    }

    // Layer and extension
    bool VulkanContext::InitInstanceExtensionAndLayer()
    {
        std::vector<vk::ExtensionProperties> installedExtensions = vk::enumerateInstanceExtensionProperties();

        std::vector<const char*> wantedExtensions;
#ifdef NKENTSEU_DEBUG
        wantedExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // VK_KHR_SWAPCHAIN_EXTENSION_NAME
#endif

#ifdef NKENTSEU_PLATFORM_WINDOWS
        wantedExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(NKENTSEU_PLATFORM_LINUX_XLIB)
        wantedExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#elif defined(NKENTSEU_LINUX_WIN_API_XCB)
        wantedExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif
        wantedExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        wantedExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

        wantedExtensions.emplace_back("VK_KHR_shared_presentable_image");
        wantedExtensions.emplace_back("VK_KHR_push_descriptor");
        wantedExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        wantedExtensions.emplace_back(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
        wantedExtensions.emplace_back(VK_KHR_MULTIVIEW_EXTENSION_NAME);

        FindBestExtensions(installedExtensions, wantedExtensions, instanceExtension);

        std::vector<const char*> wantedLayers = {
            #ifdef NKENTSEU_DEBUG
                "VK_LAYER_LUNARG_standard_validation",
                "VK_LAYER_KHRONOS_validation"
            #endif
        };

        std::vector<vk::LayerProperties> installedLayers = vk::enumerateInstanceLayerProperties();

        FindBestLayers(installedLayers, wantedLayers, layers);

        //requiredDeviceExtensions.push_back(VK_NV_GLSL_SHADER_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_KHR_MULTIVIEW_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME);
        //requiredDeviceExtensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME);
        requiredDeviceExtensions.push_back(VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME);
        //requiredDeviceExtensions.push_back(VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME);
        //requiredDeviceExtensions.push_back(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);

        //enabledDynamicRenderingFeaturesKHR.dynamicRendering = VK_TRUE;

        return !layers.empty() && !instanceExtension.empty();
    }

    bool VulkanContext::InitDeviceExtension(vk::PhysicalDevice physicalDevice)
    {
        // Device Extension
        auto availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

        FindBestExtensions(availableExtensions, requiredDeviceExtensions, deviceExtension);
        return !deviceExtension.empty();
    }

    bool VulkanContext::FindBestExtensions(const std::vector<vk::ExtensionProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out)
    {
        out.clear();
        for (const char* const& w : wanted) {
            for (vk::ExtensionProperties const& i : installed) {
                if (std::string(i.extensionName.data()).compare(w) == 0) {
                    out.emplace_back(w);
                    break;
                }
            }
        }
        return out.size() > 0;
    }

    bool VulkanContext::FindBestLayers(const std::vector<vk::LayerProperties>& installed, const std::vector<const char*>& wanted, std::vector<const char*>& out)
    {
        out.clear();
        for (const char* const& w : wanted) {
            for (vk::LayerProperties const& i : installed) {
                if (std::string(i.layerName.data()).compare(w) == 0) {
                    out.emplace_back(w);
                    break;
                }
            }
        }
        return out.size() > 0;
    }

#ifdef NKENTSEU_DEBUG
    void DestroyDebugUtilsMessengerEXT(vk::Instance instance,
        vk::DebugUtilsMessengerEXT callback,
        const vk::AllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(static_cast<VkInstance>(instance), (VkDebugUtilsMessengerEXT)(callback), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
        }
    }

    vk::Result CreateDebugUtilsMessengerEXT(vk::Instance& instance,
        const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const vk::AllocationCallbacks* pAllocator,
        vk::DebugUtilsMessengerEXT* pCallback) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(static_cast<VkInstance>(instance), "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return static_cast<vk::Result>(func(static_cast<VkInstance>(instance), reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(pCreateInfo),
                reinterpret_cast<const VkAllocationCallbacks*>(pAllocator),
                reinterpret_cast<VkDebugUtilsMessengerEXT*>(pCallback)));
        }
        else {
            return vk::Result::eErrorExtensionNotPresent;
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        std::string file = VulkanStaticDebugInfo::file_call;
        std::string methode = VulkanStaticDebugInfo::methode_call;
        uint32 line = VulkanStaticDebugInfo::line_call;

        VulkanStaticDebugInfo::Result(true);

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Warning("Vulkan debug message -> {0}", pCallbackData->pMessage);
            VulkanStaticDebugInfo::Result(false);
            return VK_FALSE;
        }
        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Error("Vulkan debug message -> {0}", pCallbackData->pMessage);
            VulkanStaticDebugInfo::Result(false);
            return VK_FALSE;
        }
        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            NkentseuTrace::Instance().GetLog()->Details(file.c_str(), line, methode.c_str(), nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent()).Info("Vulkan debug message -> {0}", pCallbackData->pMessage);
            VulkanStaticDebugInfo::Result(false);
            return VK_FALSE;
        }
        return VK_TRUE;
    }
#endif // NT_DEBUG

    bool VulkanContext::SetupDebugMessenger() {
#ifndef NKENTSEU_DEBUG
        return true;
#else
        // Definir les proprietes de la creation du gestionnaire de messages de debogage
        vk::DebugUtilsMessengerCreateInfoEXT createInfo;
        createInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                  .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                  vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                                  vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
                  .setPfnUserCallback(DebugCallback);

        vk::Result result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback);
        if (result != vk::Result::eSuccess) {
            Log_nts.Error("Impossible de creer le gestionnaire de messages de debogage. Code d'erreur : {0} ", result);
            return false;
        }
#endif
        return true;
    }

    // instance
    bool VulkanContext::CreateInstance()
    {
        Vector2i version = m_ContextProperties.version;
        uint32 appVersion = VK_MAKE_VERSION(1, 0, 0);
        uint32 apiVersion = VK_MAKE_API_VERSION(0, version.major, version.minor, 0);
        uint32 engineVersion = VK_MAKE_VERSION(1, 0, 0);
        std::string appName = m_Window->GetTitle();
        std::string engineName = m_Window->GetProperties().engineName;

        // Create Vulkan Instance
        vk::ApplicationInfo appInfo(appName.data(), appVersion, engineName.data(), engineVersion, apiVersion);
        vk::InstanceCreateInfo createInfo({}, &appInfo, layers.size(), layers.data(), instanceExtension.size(), instanceExtension.data());

        VulkanResult result;
        bool first = true;
        vkCheckError(first, result, vk::createInstance(&createInfo, allocator, &instance), "Echec de la creation de l\'instance!");

        return SetupDebugMessenger() && result.success;
    }

    bool VulkanContext::DestroyInstance()
    {
        #ifdef NKENTSEU_DEBUG
            vkCheckErrorVoid(DestroyDebugUtilsMessengerEXT(instance, callback, nullptr));
            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("cannot destroy debug util messanger ext");
            }
        #endif
        vkCheckErrorVoid(instance.destroy(allocator));
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanContext::FindSupportedFormat(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, vk::Format* format)
    {
        if (physicalDevice == nullptr) return false;

        for (vk::Format format_ : candidates) {
            vk::FormatProperties props;
            vkCheckErrorVoid(physicalDevice.getFormatProperties(format_, &props));

            if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                *format = format_;
                return true;
            }
            else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
                *format = format_;
                return true;
            }
        }
        Log_nts.Error("failed to find supported format!");
        return false;
    }

    bool VulkanContext::FindDepthFormat(vk::PhysicalDevice physicalDevice, vk::Format* format)
    {
        return FindSupportedFormat(physicalDevice, { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
            vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment, format);
    }

    bool VulkanContext::CreateSurface()
    {
        if (m_Window->GetData() == nullptr || m_Window->GetData()->windowHandle == nullptr) return false;
        VulkanResult result;
        bool first = true;
        VkSurfaceKHR surfaceKhr = nullptr;

#ifdef NKENTSEU_PLATFORM_WINDOWS
        VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.pNext = nullptr;
        surfaceInfo.flags = 0;
        surfaceInfo.hwnd = (HWND)m_Window->GetData()->windowHandle;
        surfaceInfo.hinstance = m_Window->GetData()->instanceHandle;

        vkCheckError(first, result, vkCreateWin32SurfaceKHR(instance, &surfaceInfo, 0, &surfaceKhr), "Cannot create vulkan surface windows");

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

        vkCheckError(first, result, vkCreateXlibSurfaceKHR(instance, &surfaceInfo, 0, &surfaceKhr), "Cannot create vulkan surface linux xlib");

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

        vkCheckError(first, result, vkCreateXcbSurfaceKHR(instance, &surfaceInfo, 0, &surfaceKhr), "Cannot create vulkan surface linux xcb");

        if (result.success) {
            Log_nts.Info("Create vulkan surface is good for linux xcb");
        }
#endif
        surface = surfaceKhr;
        return result.success;
    }

    bool VulkanContext::DestroySurface()
    {
        if (surface == nullptr || instance == nullptr) return false;
        vkCheckErrorVoid(instance.destroySurfaceKHR(surface, allocator));
        return VulkanStaticDebugInfo::Result();
    }

    bool VkQueueFamilyIndices::FindQueueFamilies(vk::PhysicalDevice physicaldevice, vk::SurfaceKHR surface)
    {
        if (physicaldevice == nullptr || surface == nullptr) return false;

        vkCheckErrorVoid(std::vector<vk::QueueFamilyProperties> queues = physicaldevice.getQueueFamilyProperties());
        bool select = false;

        for (uint32_t i = 0; i < queues.size(); i++) {
            vk::Bool32 presentSupport = false;

            VulkanResult result;
            bool first = true;

            vkCheckError(first, result, physicaldevice.getSurfaceSupportKHR(i, surface, &presentSupport), "cannot get surface support");

            if (queues[i].queueCount > 0 && queues[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsIndex = i;
                select = true;
                hasGraphicsFamily = true;
            }

            if (queues[i].queueCount > 0 && presentSupport) {
                presentIndex = i;
                select = true;
                hasPresentFamily = true;
            }

            if (queues[i].queueCount > 0 && queues[i].queueFlags & vk::QueueFlagBits::eCompute) {
                computeIndex = i;
                select = true;
                hasComputeFamily = true;
            }
        }
        if (!select) {
            Log_nts.Fatal("Cannot get Queue familly");
            return false;
        }
        return true;
    }

    bool VkQueueFamily::GetQeue(vk::Device device)
    {
        Log_nts.Debug();
        vkCheckErrorVoid(device.getQueue(queueFamilyIndices.graphicsIndex, 0, &graphicsQueue));

        if (VulkanStaticDebugInfo::Result()) {
            vkCheckErrorVoid(device.getQueue(queueFamilyIndices.presentIndex, 0, &presentQueue));
            Log_nts.Debug();
        }
        if (VulkanStaticDebugInfo::Result()) {
            vkCheckErrorVoid(device.getQueue(queueFamilyIndices.computeIndex, 0, &computeQueue));
            Log_nts.Debug();
        }
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanContext::IsDeviceSuitable(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, std::vector<const char*>& extension)
    {
        VkQueueFamilyIndices indices;
        indices.FindQueueFamilies(physicalDevice, surface);

        std::vector<vk::ExtensionProperties> availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();
        std::set<std::string> requiredExtensions(extension.begin(), extension.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        bool extensionsSupported = requiredExtensions.empty();

        for (const auto chaine : requiredExtensions) {
            Log_nts.Debug("extension = {0}", chaine);
        }

        bool swapChainAdequate = false;

        if (extensionsSupported) {
            VkSurfaceSupportDetails swapChainSupport;
            swapChainSupport.QuerySwapChainSupport(physicalDevice, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        vk::PhysicalDeviceFeatures supportedFeatures;
        physicalDevice.getFeatures(&supportedFeatures);

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    bool VulkanContext::GetPhysicalDevice()
    {
        vkCheckErrorVoid(installedPhysicalDevice = instance.enumeratePhysicalDevices());
        //int32 use_gpu = -1;

        Vector2i use_gpu = { -1, -1 };

        for (int32 i = 0; i < installedPhysicalDevice.size(); i++) {
            vkCheckErrorVoid(vk::PhysicalDeviceProperties properties = installedPhysicalDevice[i].getProperties());

            bool isSuitable = IsDeviceSuitable(installedPhysicalDevice[i], surface, requiredDeviceExtensions);

            if (!isSuitable) {
                Log_nts.Error("extension not support in device {0}", i);
            }

            if (use_gpu[0] == -1 && isSuitable && properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                use_gpu[0] = i;
                //break;
            }

            if (use_gpu[1] == -1 && isSuitable && properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
                use_gpu[1] = i;
                //break;
            }
        }

        if (use_gpu[0] == -1 && use_gpu[1] == -1) {
            Log_nts.Fatal("Cannot get physical device");
            return false;
        }

        physicalDevice = installedPhysicalDevice[use_gpu[0] == -1 ? use_gpu[1] : use_gpu[0]];

        if (physicalDevice != nullptr) {
            vkCheckErrorVoid(physicalDevice.getMemoryProperties(&memoryProperties));
            vkCheckErrorVoid(physicalDevice.getProperties(&properties));

            InitDeviceExtension(physicalDevice);

            queueFamilly.queueFamilyIndices.FindQueueFamilies(physicalDevice, surface);

            if (!queueFamilly.queueFamilyIndices.IsComplete()) {
                return false;
            }
        }

        return physicalDevice != nullptr;
    }

    bool VulkanContext::CreateLogicalDevice()
    {
        int32 device_extension_count = 1;
        const float32 queue_priority = 1.0f;

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<int32> uniqueQueueFamilies = { 
                                                  queueFamilly.queueFamilyIndices.graphicsIndex
                                                , queueFamilly.queueFamilyIndices.presentIndex
                                                , queueFamilly.queueFamilyIndices.computeIndex
                                              };

        for (int32 queueFamily : uniqueQueueFamilies) {
            vk::DeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.queueFamilyIndex = (uint32)queueFamily;
            queueCreateInfo.queueCount       = 1;
            queueCreateInfo.pQueuePriorities = &queue_priority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.fillModeNonSolid = VK_TRUE;
        deviceFeatures.multiViewport = VK_TRUE;
        deviceFeatures.largePoints = VK_TRUE;

        //vk::PhysicalDeviceFeatures2 physicalDeviceFeatures2{};
        //physicalDeviceFeatures2.features = deviceFeatures;
        //physicalDeviceFeatures2.pNext = &enabledDynamicRenderingFeaturesKHR;

        vk::PhysicalDeviceExtendedDynamicState3FeaturesEXT extendedDynamicState3Features = {};
        extendedDynamicState3Features.extendedDynamicState3PolygonMode = VK_TRUE;
        //extendedDynamicState3Features.pNext = &physicalDeviceFeatures2;deviceFeatures
        //extendedDynamicState3Features.pNext = &deviceFeatures;

        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeature = {};
        extendedDynamicStateFeature.extendedDynamicState = VK_TRUE;
        extendedDynamicStateFeature.setPNext(&extendedDynamicState3Features);

        vk::DeviceCreateInfo create_info = {};
        create_info.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());
        create_info.pQueueCreateInfos       = queueCreateInfos.data();
        create_info.enabledExtensionCount   = deviceExtension.size();
        create_info.ppEnabledExtensionNames = deviceExtension.data();
        create_info.pEnabledFeatures        = &deviceFeatures;
        create_info.setPNext(&extendedDynamicStateFeature);

        vkCheckErrorVoid(device = physicalDevice.createDevice(create_info, allocator));

        if (VulkanStaticDebugInfo::Result()) {
            if (cmdSetPolygonModeEXT == nullptr) {
                cmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)vkGetDeviceProcAddr(device, "vkCmdSetPolygonModeEXT");

                if (!cmdSetPolygonModeEXT) {
                    Log_nts.Error();
                    return false;
                }
            }
            return queueFamilly.GetQeue(device);
        }
        return false;
    }

    bool VulkanContext::DestroyDevice()
    {
        if (device == nullptr) return false;
        vkCheckErrorVoid(device.destroy(nullptr));
        if (VulkanStaticDebugInfo::Result()) device = nullptr;
        return VulkanStaticDebugInfo::Result();
    }

    bool VkSurfaceSupportDetails::QuerySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
    {
        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, physicalDevice.getSurfaceCapabilitiesKHR(surface, &capabilities), "cannot get physical device surface capabilities");
        
        vkCheckErrorVoid(formats = physicalDevice.getSurfaceFormatsKHR(surface));
        if (formats.size() == 0) {
            Log_nts.Error("cannot get physical device surface format");
        }
        
        vkCheckErrorVoid(presentModes = physicalDevice.getSurfacePresentModesKHR(surface));
        if (presentModes.size() == 0) {
            Log_nts.Error("cannot get physical device surface present mode");
        }
        return result.success && VulkanStaticDebugInfo::Result();
    }

    bool VulkanContext::CreateSwapchaine(const maths::Vector2u& size) {
        VulkanResult result;
        bool first = true;

        uint32 formatCount;
        std::vector<vk::SurfaceFormatKHR> surfaceFormats;

        vkCheckErrorVoid(surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot get device surface format");
        }

        const std::vector<vk::Format> requestSurfaceImageFormat = { vk::Format::eB8G8R8A8Srgb, vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm };

        for (uint32 indexFormat = 0; indexFormat < surfaceFormats.size(); indexFormat++) {
            vk::SurfaceFormatKHR currentFormat = surfaceFormats[indexFormat];

            for (uint32 indexRequier = 0; indexRequier < requestSurfaceImageFormat.size(); indexRequier++) {
                if (currentFormat.format == requestSurfaceImageFormat[indexRequier] && currentFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                    surfaceFormat = currentFormat;
                    break;
                }
            }
        }

        presentMode = SelectPresentMode();

        vk::SurfaceCapabilitiesKHR surfaceCaps = {};
        vkCheckError(first, result, physicalDevice.getSurfaceCapabilitiesKHR(surface, &surfaceCaps), "cannot get surface capabilities khr");

        uint32 imageCount = GetMinImageCountFromPresentMode(presentMode);

        if (imageCount < surfaceCaps.minImageCount) {
            imageCount = surfaceCaps.minImageCount;
        }
        else if (surfaceCaps.maxImageCount != 0 && imageCount > surfaceCaps.maxImageCount) {
            imageCount = surfaceCaps.maxImageCount;
        }

        vk::Extent2D extend = {};

        if (surfaceCaps.currentExtent.width == 0xffffffff) {
            extend.width = size.x;
            extend.height = size.y;
        }
        else {
            extend = surfaceCaps.currentExtent;
        }

        std::vector<uint32> queueFamilyIndices;

        if (queueFamilly.queueFamilyIndices.graphicsIndex >= 0)
            queueFamilyIndices.push_back(queueFamilly.queueFamilyIndices.graphicsIndex);

        if (queueFamilly.queueFamilyIndices.presentIndex >= 0 && 
            queueFamilly.queueFamilyIndices.presentIndex != queueFamilly.queueFamilyIndices.graphicsIndex)
            queueFamilyIndices.push_back(queueFamilly.queueFamilyIndices.presentIndex);

        // if (queueFamilly.queueFamilyIndices.computeIndex >= 0 && 
        //    queueFamilly.queueFamilyIndices.computeIndex != queueFamilly.queueFamilyIndices.graphicsIndex &&
        //    queueFamilly.queueFamilyIndices.computeIndex != queueFamilly.queueFamilyIndices.presentIndex)
        //    queueFamilyIndices.push_back(queueFamilly.queueFamilyIndices.presentIndex);

        vk::SharingMode sharingMode;
        if (queueFamilyIndices.size() > 1) {
            sharingMode = vk::SharingMode::eConcurrent;
        }
        else {
            sharingMode = vk::SharingMode::eExclusive;
        }

        vk::SwapchainCreateInfoKHR swapchainInfo;
        swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapchainInfo.surface = surface;
        swapchainInfo.imageFormat = surfaceFormat.format;
        swapchainInfo.preTransform = surfaceCaps.currentTransform;
        swapchainInfo.imageExtent = extend;
        swapchainInfo.minImageCount = imageCount;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.presentMode = presentMode;
        swapchainInfo.queueFamilyIndexCount = queueFamilyIndices.size();
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        swapchainInfo.imageSharingMode = sharingMode;

        vkCheckError(first, result, device.createSwapchainKHR(&swapchainInfo, allocator, &swapchain), "cannot create swapchain");

        if (result.success) {
            Log_nts.Info("Create vulkan swapchain is good");
        }

        std::vector<vk::Image> scImages;

        vkCheckErrorVoid(scImages = device.getSwapchainImagesKHR(swapchain), "cannot getswapchain image khr");
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot getswapchain image khr");
        }

        imageView.resize(scImages.size());
        swapchainImages.resize(scImages.size());

        vk::ImageSubresourceRange imageRange = {};
        imageRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        imageRange.layerCount = 1;
        imageRange.levelCount = 1;

        vk::ImageViewCreateInfo viewInfo = {};
        viewInfo.format = surfaceFormat.format;
        viewInfo.viewType = vk::ImageViewType::e2D; // e3D, eCubeMap
        viewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.levelCount = 1;
        //viewInfo.components.r = vk::ComponentSwizzle::eR;
        //viewInfo.components.g = vk::ComponentSwizzle::eG;
        //viewInfo.components.b = vk::ComponentSwizzle::eB;
        //viewInfo.components.a = vk::ComponentSwizzle::eA;
        viewInfo.components.r = vk::ComponentSwizzle::eIdentity;
        viewInfo.components.g = vk::ComponentSwizzle::eIdentity;
        viewInfo.components.b = vk::ComponentSwizzle::eIdentity;
        viewInfo.components.a = vk::ComponentSwizzle::eIdentity;
        viewInfo.subresourceRange = imageRange;

        for (uint32 index = 0; index < scImages.size(); index++) {
            swapchainImages[index] = scImages[index];
            viewInfo.image = swapchainImages[index];
            vkCheckError(first, result, device.createImageView(&viewInfo, allocator, &imageView[index]), "cannot create image view number {0}", index);
        }

        return result.success;
    }

    vk::PresentModeKHR VulkanContext::SelectPresentMode()
    {
        uint32 availCount = 0;
        vkCheckErrorVoid(std::vector<vk::PresentModeKHR> availPresentModes = physicalDevice.getSurfacePresentModesKHR(surface));

        std::vector<vk::PresentModeKHR> requestPresentMode;
        if (m_ContextProperties.pixelFormat.flags & GraphicsFlag::DoubleBuffer) {
            requestPresentMode.push_back(vk::PresentModeKHR::eFifo);
            requestPresentMode.push_back(vk::PresentModeKHR::eFifoRelaxed);
        }
        else if (m_ContextProperties.pixelFormat.flags & GraphicsFlag::TripleBuffer) {
            requestPresentMode.push_back(vk::PresentModeKHR::eMailbox);
        }
        else {
            requestPresentMode.push_back(vk::PresentModeKHR::eImmediate);
        }

        for (uint32 request_i = 0; request_i < requestPresentMode.size(); request_i++) {
            for (uint32 avail_i = 0; avail_i < availPresentModes.size(); avail_i++) {
                if (requestPresentMode[request_i] == availPresentModes[avail_i]) {
                    return requestPresentMode[request_i];
                }
            }
        }

        return vk::PresentModeKHR::eFifo;
    }

    uint32 VulkanContext::GetMinImageCountFromPresentMode(vk::PresentModeKHR present_mode)
    {
        if (present_mode == vk::PresentModeKHR::eMailbox)
            return 3;
        if (present_mode == vk::PresentModeKHR::eFifo || present_mode == vk::PresentModeKHR::eFifoRelaxed)
            return 2;
        if (present_mode == vk::PresentModeKHR::eImmediate)
            return 1;
        return 1;
    }

    bool VulkanContext::DestroySwapchaine() {
        for (usize i = 0; i < imageView.size(); i++) {
            if (imageView[i] != nullptr) {
                vkCheckErrorVoid(device.destroyImageView(imageView[i], allocator));
                if (!VulkanStaticDebugInfo::Result()) {
                    Log_nts.Error("cannot destroy swapchain image view {0}", i);
                }
                else {
                    imageView[i] = nullptr;
                }
            }
        }

        imageView.clear();
        swapchainImages.clear();

        if (swapchain != nullptr) {
            vkCheckErrorVoid(device.destroySwapchainKHR(swapchain, allocator));
            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("cannot destroy swapchain");
            }
            else {
                swapchain = nullptr;
            }
        }
        return true;
    }

    bool VulkanContext::CreateCommandPool() {
        VulkanResult result;
        bool first = true;

        vk::CommandPoolCreateInfo poolInfo = {};
        poolInfo.queueFamilyIndex = queueFamilly.queueFamilyIndices.graphicsIndex;
        poolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        vkCheckError(first, result, device.createCommandPool(&poolInfo, allocator, &commandPool), "cannot create command pool");

        if (result.success) {
            Log_nts.Info("Create command pool is good");
        }

        return result.success;
    }

    bool VulkanContext::DestroyCommandPool() {
        if (commandPool == nullptr) return false;
        vkCheckErrorVoid(device.destroyCommandPool(commandPool, allocator));
        commandPool = nullptr;
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanContext::CreateSemaphores() {
        DestroySemaphores();

        bool success = true;

        vk::SemaphoreCreateInfo semaphoreInfo = {};

        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, device.createSemaphore(&semaphoreInfo, allocator, &submitSemaphore), "cannot create submit semaphore");
        if (!result.success) {
            Log_nts.Error("cannot create submit semaphore");
        }
        success = success && result.success;

        vkCheckError(first, result, device.createSemaphore(&semaphoreInfo, allocator, &aquireSemaphore), "cannot create acquire semaphore");
        if (!result.success) {
            Log_nts.Error("cannot create acquire semaphore");
        }
        success = success && result.success;

        if (success) {
            Log_nts.Info("Create semaphore is good");
        }

        return success;
    }

    bool VulkanContext::DestroySemaphores() {
        bool success = true;
        vkCheckErrorVoid(device.destroySemaphore(submitSemaphore, allocator));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot destroy submit semaphore");
            success = false;
        }
        else submitSemaphore = nullptr;

        vkCheckErrorVoid(device.destroySemaphore(aquireSemaphore, allocator));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot destroy aquire semaphore");
            success = false;
        }
        else aquireSemaphore = nullptr;
        return success;
    }

    bool VulkanContext::CreateCommandBuffer() {
        VulkanResult result;
        bool first = true;

        commandBuffers.resize(swapchainImages.size());

        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.commandPool = commandPool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = (uint32)commandBuffers.size();

        vkCheckError(first, result, device.allocateCommandBuffers(&allocInfo, commandBuffers.data()), "cannot allocate command buffer");
        return result.success;
    }

    bool VulkanContext::DestroyCommandBuffer() {
        VulkanResult result;
        bool first = true;

        vkCheckErrorVoid(device.waitIdle(), "cannot wait device idle");
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot wait device idle");
        }
        vkCheckErrorVoid(device.freeCommandBuffers(commandPool, 1, commandBuffers.data()));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot free command buffer");
        }
        commandBuffers.clear();
        return true;
    }

    bool VulkanContext::CanAquire()
    {
        if (swapchainImages.size() == 0) {
            return false;
        }

        if (m_Window->GetSize() == Vector2u()) {
            return false;
        }
        return true;
    }

    void VulkanContext::OnEvent(Event& event) {
        EventBroker broker(event);

        broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(VulkanContext::OnWindowResizedEvent));
    }

    bool VulkanContext::OnWindowResizedEvent(WindowResizedEvent& event)
    {
        if (event.GetSize() == Vector2u()) {
            return false;
        }

        Log_nts.Debug("resize");

        RecreateSwapChain();

        return false;
    }

    bool VulkanContext::CreateRenderPass()
    {
        if (physicalDevice == nullptr || device == nullptr) return false;
        VulkanResult result;
        bool first = true;

        vk::AttachmentDescription colorAttachment = {};
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.format = surfaceFormat.format;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

        vk::AttachmentDescription depthAttachment = {};
        depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
        depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachment.samples = vk::SampleCountFlagBits::e1;
        VulkanContext::FindDepthFormat(physicalDevice, &depthAttachment.format);
        depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

        vk::AttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::SubpassDescription subpassDescription = {};
        subpassDescription.colorAttachmentCount = 1;
        subpassDescription.pColorAttachments = &colorAttachmentRef;
        subpassDescription.pDepthStencilAttachment = &depthAttachmentRef;

        std::vector<vk::AttachmentDescription> attachments;
        attachments.push_back(colorAttachment);
        attachments.push_back(depthAttachment);

        std::vector<vk::SubpassDescription> subpassDescriptions;
        subpassDescriptions.push_back(subpassDescription);

        vk::SubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask = vk::AccessFlagBits::eNone;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

        std::vector<vk::SubpassDependency> subpassDependencies;
        subpassDependencies.push_back(dependency);

        vk::RenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.attachmentCount = attachments.size();
        renderPassInfo.pSubpasses = subpassDescriptions.data();
        renderPassInfo.subpassCount = subpassDescriptions.size();
        renderPassInfo.pDependencies = subpassDependencies.data();
        renderPassInfo.dependencyCount = subpassDependencies.size();

        vkCheckError(first, result, device.createRenderPass(&renderPassInfo, allocator, &renderPass), "cannot create render pass");

        if (result.success) {
            Log_nts.Info("Create render pass is good");
        }

        return result.success;
    }

    bool VulkanContext::DestroyRenderPass()
    {
        if (renderPass == nullptr || device == nullptr) return false;
        vkCheckErrorVoid(device.destroyRenderPass(renderPass, allocator));
        renderPass = VK_NULL_HANDLE;
        return VulkanStaticDebugInfo::Result();
    }

    uint32 VulkanContext::FindMemoryType(vk::PhysicalDevice physicalDevice, uint32 typeFilter, vk::MemoryPropertyFlags properties) {
        if (physicalDevice == nullptr) return 0;

        vk::PhysicalDeviceMemoryProperties memProperties;
        vkCheckErrorVoid(physicalDevice.getMemoryProperties(&memProperties));

        for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        Log_nts.Error("failed to find suitable memory type!");
        return 0;
    }

    vk::CommandBuffer VulkanContext::BeginSingleTimeCommands(VulkanContext* context)
    {
        if (context == nullptr) return nullptr;

        VulkanResult result;
        bool first = true;

        vk::CommandBufferAllocateInfo allocInfo{};
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandPool = context->commandPool;
        allocInfo.commandBufferCount = 1;

        vk::CommandBuffer commandBuffer;
        vkCheckError(first, result, context->device.allocateCommandBuffers(&allocInfo, &commandBuffer), "failed to allocate command buffer");

        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        vkCheckError(first, result, commandBuffer.begin(&beginInfo), "failed to begin command buffer");

        return commandBuffer;
    }

    void VulkanContext::EndSingleTimeCommands(VulkanContext* context, vk::CommandBuffer commandBuffer)
    {
        if (context == nullptr) return;
        VulkanResult result;
        bool first = true;

        vkCheckErrorVoid(commandBuffer.end());

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to end command buffer");
        }

        vk::SubmitInfo submitInfo{};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkCheckError(first, result, context->queueFamilly.graphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE), "failed to submit queue");
        vkCheckErrorVoid(context->queueFamilly.graphicsQueue.waitIdle());

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to wait idle");
        }

        vkCheckErrorVoid(context->device.freeCommandBuffers(context->commandPool, 1, &commandBuffer));

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to free command buffer");
        }
    }

    bool VkBufferInternal::WriteToBuffer(const void* data, usize size, usize offset)
    {
        if (mappedData == nullptr || data == nullptr) {
            Log_nts.Error("mapped data or data is null");
            return false;
        }

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

    bool VkBufferInternal::Destroy(VulkanContext* context)
    {
        if (context == nullptr) return false;

        if (buffer != nullptr) {
            vkCheckErrorVoid(context->device.destroyBuffer(buffer, context->allocator));
            buffer = VK_NULL_HANDLE;
        }

        bool success = VulkanStaticDebugInfo::Result();

        if (bufferMemory != nullptr) {
            vkCheckErrorVoid(context->device.freeMemory(bufferMemory, context->allocator));
            bufferMemory = VK_NULL_HANDLE;
        }

        return success && VulkanStaticDebugInfo::Result();
    }

    bool VkBufferInternal::Mapped(VulkanContext* context, usize size, usize offset, vk::MemoryMapFlags flag)
    {
        if (context == nullptr || bufferMemory == nullptr || buffer == nullptr || mappedData != nullptr) return false;

        usize realsize = size;
        if (size == VK_WHOLE_SIZE) realsize = this->size;

        vkCheckErrorVoid(mappedData = context->device.mapMemory(bufferMemory, offset, realsize, flag));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot map buffer memory");
            return false;
        }
        return true;
    }

    bool VkBufferInternal::UnMapped(VulkanContext* context)
    {
        if (context == nullptr || bufferMemory == nullptr || buffer == nullptr || mappedData == nullptr) return false;
        vkCheckErrorVoid(context->device.unmapMemory(bufferMemory));
        mappedData = nullptr;
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot unmap buffer memory");
            return false;
        }
        return true;
    }

    bool VkBufferInternal::Flush(VulkanContext* context, usize size, usize offset)
    {
        if (context == nullptr || bufferMemory == nullptr || buffer == nullptr) return false;

        VulkanResult result;
        bool first = true;

        usize realsize = size;
        if (size == VK_WHOLE_SIZE) realsize = this->size;

        vk::MappedMemoryRange mappedRange = {};
        mappedRange.memory = bufferMemory;
        mappedRange.offset = offset;
        mappedRange.size = realsize;

        vkCheckError(first, result, context->device.flushMappedMemoryRanges(1, &mappedRange), "cannot flush buffer memory");
        return result.success;
    }

    int64 VkBufferInternal::FindMemoryType(VulkanContext* context, uint32 typeFilter, vk::MemoryPropertyFlags properties)
    {
        if (context == nullptr) return 0;

        vk::PhysicalDeviceMemoryProperties memProperties;
        vkCheckErrorVoid(context->physicalDevice.getMemoryProperties(&memProperties));

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot get memory properties");
            return false;
        }

        for (uint32 i = 0; i < memProperties.memoryTypeCount; i++) {
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        return -1;
    }

    bool VkBufferInternal::CreateBuffer(VulkanContext* context, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::SharingMode sharingMode, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& bufferMemory)
    {
        if (context == nullptr) {
            return false;
        }

        VulkanResult result;
        bool first = true;

        vk::BufferCreateInfo bufferInfo{};
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = sharingMode;

        vkCheckError(first, result, context->device.createBuffer(&bufferInfo, context->allocator, &buffer), "cannot create buffer");

        if (!result.success || buffer == nullptr) {
            Log_nts.Error("Cannot create buffer");
            return false;
        }

        vk::MemoryRequirements memRequirements;
        vkCheckErrorVoid(context->device.getBufferMemoryRequirements(buffer, &memRequirements));

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("Cannot get memory buffer requirements");
            return false;
        }

        int64 index = FindMemoryType(context, memRequirements.memoryTypeBits, properties);

        if (index < 0) {
            Log_nts.Error("Cannot find correct memory type");
            return false;
        }

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = index;

        vkCheckError(first, result, context->device.allocateMemory(&allocInfo, context->allocator, &bufferMemory), "cannot allocat memory buffer");
        vkCheckErrorVoid(context->device.bindBufferMemory(buffer, bufferMemory, 0));

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot bind buffer memory");
            return false;
        }

        return result.success;
    }

    bool VkBufferInternal::CopyBuffer(VulkanContext* context, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
    {
        if (context == nullptr || context->commandPool == nullptr) {
            return false;
        }

        vk::CommandBuffer commandBuffer = VulkanContext::BeginSingleTimeCommands(context);

        vk::BufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optionnel
        copyRegion.dstOffset = 0; // Optionnel
        copyRegion.size = size;

        if (commandBuffer != nullptr) {
            vkCheckErrorVoid(commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion));
            bool success = VulkanStaticDebugInfo::Result();

            VulkanContext::EndSingleTimeCommands(context, commandBuffer);
            return success;
        }
        return false;
    }

    bool VkBufferInternal::CopyBufferToImage(VulkanContext* context, vk::Buffer srcBuffer, vk::Image dstImage, const maths::Vector2u& size)
    {
        vk::CommandBuffer commandBuffer = VulkanContext::BeginSingleTimeCommands(context);

        vk::BufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = vk::Offset3D{ 0, 0, 0 };
        region.imageExtent = vk::Extent3D{ size.x, size.y, 1 };
        commandBuffer.copyBufferToImage(srcBuffer, dstImage, vk::ImageLayout::eTransferDstOptimal, 1, &region);

        VulkanContext::EndSingleTimeCommands(context, commandBuffer);
        return true;
    }

    bool VkUniformBufferInternal::Create(VulkanContext* context, const UniformInputAttribute& uba, vk::BufferUsageFlags usage, std::vector<vk::DescriptorSet>& descriptorSets, vk::DescriptorType descriptorType)
    {
        if (context == nullptr) return false;
        uniformInput = uba;

        this->usage = vk::BufferUsageFlagBits::eTransferDst | usage;
        vk::SharingMode sharingMode = vk::SharingMode::eExclusive;
        vk::MemoryPropertyFlags propertyFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eDeviceLocal;

        uniformBuffers.resize(descriptorSets.size());
        writeDescriptorSets.resize(descriptorSets.size());
        descriptorBufferInfos.resize(descriptorSets.size());

        uint32 index = 0;
        bool success = true;

        uint32 size = uniformInput.size;
        uint32 range = size;
        dynamicAlignment = 0;

        if (uniformInput.usage == BufferUsageType::Enum::DynamicDraw) {
            uint32 minUboAlignment = context->properties.limits.minUniformBufferOffsetAlignment;

            if (minUboAlignment > 0) {
                dynamicAlignment = (size + minUboAlignment - 1) & ~(minUboAlignment - 1);
            }
            range = dynamicAlignment;
            size = dynamicAlignment * uniformInput.instance;
        }

        for (auto& uniform : uniformBuffers) {
            if (!VkBufferInternal::CreateBuffer(context, size, this->usage, sharingMode, propertyFlags, uniform.buffer, uniform.bufferMemory)) {
                Log_nts.Error("Cannot create uniforme buffer : name = {0} at index = {1}", uniformInput.name, index);
                success = false;
            }
            else {
                descriptorBufferInfos[index].buffer = uniform.buffer;
                descriptorBufferInfos[index].offset = 0;
                descriptorBufferInfos[index].range = range; // La taille du tampon uniforme

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

    bool VkUniformBufferInternal::Destroy(VulkanContext* context)
    {
        if (context == nullptr) return false;

        if (uniformBuffers.size() <= 0) return true;

        for (auto& buffer : uniformBuffers) {
            buffer.Destroy(context);
        }

        uniformBuffers.clear();
        writeDescriptorSets.clear();
        descriptorBufferInfos.clear();

        return true;
    }

    bool VkUniformBufferInternal::Binds(VulkanContext* context, void* data, usize size, uint32 instanceIndex)
    {
        if (context == nullptr) return false;

        bool success = false;

        currentOffset = 0;
        if (uniformInput.usage == BufferUsageType::Enum::DynamicDraw) {
            currentOffset = instanceIndex * dynamicAlignment;
        }

        for (auto& uniform : uniformBuffers) {
            uniform.Mapped(context, size, 0);
            success = uniform.WriteToBuffer(data, size, currentOffset);
            success = uniform.Flush(context, size, 0);
            uniform.UnMapped(context);
        }

        return success;
    }

    bool VkUniformBufferInternal::Bind(VulkanContext* context, void* data, usize size, uint32 index, uint32 instanceIndex)
    {
        if (context == nullptr) return false;

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

        uniform.Mapped(context, size, 0);
        success = uniform.WriteToBuffer(data, size, currentOffset);
        success = uniform.Flush(context, size, 0);
        uniform.UnMapped(context);

        return success;
    }

}    // namespace nkentseu