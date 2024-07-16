//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 12:07:34 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_SURFACE_H__
#define __VULKAN_SURFACE_H__

#pragma once

#include <System/System.h>
#include <System/Nature/Base.h>
#include "VulkanInstance.h"

#include <vulkan/vulkan.hpp>

namespace nkentseu {

    class Window;
    
    struct NKENTSEU_API VulkanSurface {
        bool Create(Window* window, VulkanInstance* instance);
        bool Destroy(VulkanInstance* instance);

        VkSurfaceKHR surface = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_SURFACE_H__!