//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-15 at 04:56:52 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __VULKAN_INTERNAL_SHADER_H__
#define __VULKAN_INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#include <System/Definitions/Memory.h>
#include <unordered_map>
#include "Nkentseu/Graphics/ShaderInfo.h"
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>
#include "VulkanInternal.h"
#include "VulkanUtils.h"

#include <Nkentseu/Graphics/Shader.h>

namespace nkentseu {
    class VulkanContext;

    class NKENTSEU_API VulkanShader : public Shader {
        public:
            VulkanShader(Memory::Shared<Context> context);
            ~VulkanShader();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;

            bool LoadFromFile(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout);

            bool Bind(VkCommandBuffer commandBuffer, const VulkanDynamicMode& dynamicMode) const;
            bool Unbind(VkCommandBuffer commandBuffer) const;

            bool UseUniform(VkCommandBuffer commandBuffer, const std::string& name, void* data, usize size);
        private:
            Memory::Shared<VulkanContext> m_Context = nullptr;
            VkPipeline m_GraphicsPipeline = nullptr;
            VulkanPipelineConfig m_PipelineConfig;
            VkRect2D m_Scissor = {};
            VkViewport m_ViewPort = {};

            VulkanPipelineLayout m_PipelineLayout;

            std::unordered_map<ShaderType::Code, VkShaderModule> m_Modules;
            ShaderBufferLayout m_ShaderLayout = {};

            std::unordered_map<std::string, VulkanBuffer> m_UniformBuffers;

            // shader info
            std::vector<VkVertexInputBindingDescription> bindingDescriptions = {};
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};
            bool DefineVertexInput();

            std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
            bool DefinePipelineStage(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles);
        private:
            std::vector<char> LoadShader(const std::string& shaderFile);
            VkShaderModule MakeModule(const std::string& filepath, ShaderType::Code code);
            bool Recreate(bool force);
            bool CleanUp(bool force);

            std::string ReplaceShaderExtension(const std::string& shaderPath);
            bool CheckIfShaderExists(const std::string& shaderPath);
            bool CompileShader(const std::string& filePath, ShaderType::Code shaderType);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!