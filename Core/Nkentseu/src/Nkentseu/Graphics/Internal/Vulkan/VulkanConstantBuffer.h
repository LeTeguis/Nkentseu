//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-22 at 06:39:06 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_CONSTANT_BUFFER_H__
#define __VULKAN_CONSTANT_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>

#include "Nkentseu/Graphics/Context.h"

#include "VulkanContext.h"

namespace nkentseu {
    
    class NKENTSEU_API VulkanConstantBuffer {
        public:
            VulkanConstantBuffer(Memory::Shared<Context> context);
            ~VulkanConstantBuffer();

        private:
            Memory::Shared<VulkanContext> m_Context = nullptr;
    };

}  //  nkentseu

#endif  // __VULKAN_CONSTANT_BUFFER_H__!