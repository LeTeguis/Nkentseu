//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:52 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_SHADER_H__
#define __VULKAN_INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <System/Definitions/Memory.h>
#include <unordered_map>
#include "Nkentseu/Graphics/ShaderInfo.h"
#include <string>

#include <vulkan/vulkan.hpp>
#include "VulkanInternal.h"
#include "VulkanUtils.h"

namespace nkentseu {
    class Context;
    
    class NKENTSEU_API InternalShader {
        public:
            InternalShader(Context *context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout = {});
            ~InternalShader();

            bool Create();
            bool Destroy();
            void SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout = {});

            bool Bind(VkCommandBuffer commandBuffer, const VulkanDynamicMode& dynamicMode) const;
            bool Unbind(VkCommandBuffer commandBuffer) const;
        private:
            Context* m_Context = nullptr;
            VkPipeline m_GraphicsPipeline = {};
            VulkanPipelineConfig m_PipelineConfig;
            // VulkanPipelineLayout m_PipelineLayout;
            VkRect2D m_Scissor = {};
            VkViewport m_ViewPort = {};

            struct infos_shader {
                VkShaderModule module;
                VkPipelineShaderStageCreateInfo shaderStage;
            };

            std::unordered_map<ShaderType::Code, VkShaderModule> m_Modules;
            std::unordered_map<ShaderType::Code, std::string> m_ShaderFiles;
        private:
            std::vector<char> LoadShader(const std::string& shaderFile);
            VkShaderModule MakeModule(const std::string& filepath, ShaderType::Code code);
            bool Recreate(bool force);

            std::string ReplaceShaderExtension(const std::string& shaderPath);
            bool CheckIfShaderExists(const std::string& shaderPath);
            bool CompileShader(const std::string& filePath, ShaderType::Code shaderType);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!

#endif