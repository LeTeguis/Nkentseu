//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:56:52 PM AM.
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
#include "VulkanShaderInputLayout.h"

#include <Nkentseu/Graphics/Shader.h>
#include "Tools/VulkanPipelineLayout.h"

namespace nkentseu {
    class VulkanContext;

    class NKENTSEU_API VulkanShader : public Shader {
        public:
            VulkanShader(Memory::Shared<Context> context);
            ~VulkanShader();

            Memory::Shared<Context> GetContext() override;
            bool Destroy() override;

            bool LoadFromFile(const ShaderFilePathLayout& shaderFiles, Memory::Shared<ShaderInputLayout> shaderInputLayout) override;

            bool Bind() override;
            bool Unbind() override;

            //bool UseUniform(VkCommandBuffer commandBuffer, const std::string& name, void* data, usize size);
            //bool BindDescriptorsSet(VkCommandBuffer commandBuffer);

            //VulkanPipelineLayout* GetPipelineLayout();
        private:
            Memory::Shared<VulkanContext> m_Context = nullptr;
            Memory::Shared<VulkanShaderInputLayout> m_VkSil = nullptr;

            // pipeline
            vk::Pipeline pipeline = nullptr;
            bool CreatePipeline(const ShaderFilePathLayout& shaderFiles);

            // shader module
            std::unordered_map<uint32, vk::ShaderModule> shaderModules;
            std::vector<char> LoadShader(const std::string& shaderFile);
            vk::ShaderModule MakeModule(const std::string& filepath, ShaderStage code);

            // binding and attribut
            std::vector<vk::VertexInputBindingDescription> bindingDescriptions = {};
            std::vector<vk::VertexInputAttributeDescription> attributeDescriptions = {};
            bool DefineVertexInput(VertexInputLayout vertexIL);

            // pipeline stage
            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
            bool DefinePipelineStage(const ShaderFilePathLayout& shaderFiles);

        private:
            //VulkanPipelineConfig m_PipelineConfig;
            VkRect2D m_Scissor = {};
            VkViewport m_ViewPort = {};

            //VulkanPipelineLayout m_PipelineLayout;
            VulkanDynamicMode m_DynamicMode;

            ShaderBufferLayout m_ShaderLayout = {};

            bool isCreate = false;
            
        private:
            bool Recreate(bool force);
            bool CleanUp(bool force);

            std::string ReplaceShaderExtension(const std::string& shaderPath);
            bool CheckIfShaderExists(const std::string& shaderPath);
            bool CompileShader(const std::string& filePath, ShaderStage shaderType);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!