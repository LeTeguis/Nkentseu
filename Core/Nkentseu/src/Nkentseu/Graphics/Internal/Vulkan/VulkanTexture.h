//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:23:53 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_TEXTURE_H__
#define __VULKAN_TEXTURE_H__

#pragma once

#include <System/System.h>

namespace nkentseu {
    
    class NKENTSEU_API VulkanTexture {
        public:
            VulkanTexture();
            ~VulkanTexture();

            std::string ToString() const;
            friend std::string ToString(const VulkanTexture& vulkanTexture);
        private:
        protected:
    };

}  //  nkentseu

#endif  // __VULKAN_TEXTURE_H__!