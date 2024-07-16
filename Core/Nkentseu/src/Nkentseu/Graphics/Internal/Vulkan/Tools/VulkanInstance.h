//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 11:49:41 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INSTANCE_H__
#define __VULKAN_INSTANCE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include "VulkanExtension.h"
#include "Nkentseu/Graphics/GraphicsProperties.h"

#include <vulkan/vulkan.hpp>

namespace nkentseu {
    
    class Window;

    struct NKENTSEU_API VulkanInstance {
        bool Create(Window* window, const ContextProperties& contextProperties, VulkanExtension* extension);
        bool Destroy();

        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debugMessenger = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_INSTANCE_H__!