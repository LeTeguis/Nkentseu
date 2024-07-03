//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-29 at 10:08:47 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_SHADER_INPUT_LAYOUT_H__
#define __VULKAN_SHADER_INPUT_LAYOUT_H__

#pragma once

#include <System/System.h>

#include "Nkentseu/Graphics/ShaderInputLayout.h"
#include "Nkentseu/Graphics/Context.h"
#include "VulkanInternal.h"
#include "VulkanContext.h"

namespace nkentseu {
    
    class NKENTSEU_API VulkanShaderInputLayout : public ShaderInputLayout {
    public:
        VulkanShaderInputLayout(Memory::Shared<Context> context);
        ~VulkanShaderInputLayout();

        virtual bool Initialize() override;
        virtual bool Release() override;

        virtual bool UpdatePushConstant(const std::string& name, void* data, usize size, Memory::Shared<Shader> shader = nullptr) override;

        VulkanPipelineLayout m_PipelineLayout;
    private:
        Memory::Shared<VulkanContext> m_Context;
    };

}  //  nkentseu

#endif  // __VULKAN_SHADER_INPUT_LAYOUT_H__!