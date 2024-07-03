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

            bool DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) override;

            bool PolygonMode(PolygonModeType::Code mode) override;
            bool CullMode(CullModeType::Code mode) override;
            bool FrontFaceMode(FrontFaceType::Code mode) override;
            bool PrimitiveTopologyMode(PrimitiveTopologyType::Code mode) override;
            bool ScissorMode(const maths::Vector2i& offset, const maths::Vector2u& extend) override;
            bool ViewportMode(const maths::Vector2f& position, const maths::Vector2f& size, const maths::Vector2f& depth) override;

            //bool UseUniform(VkCommandBuffer commandBuffer, const std::string& name, void* data, usize size);
            //bool BindDescriptorsSet(VkCommandBuffer commandBuffer);

            VulkanPipelineLayout* GetPipelineLayout();
        private:
            Memory::Shared<VulkanContext> m_Context = nullptr;
            VkPipeline m_GraphicsPipeline = nullptr;
            VulkanPipelineConfig m_PipelineConfig;
            VkRect2D m_Scissor = {};
            VkViewport m_ViewPort = {};

            VulkanPipelineLayout m_PipelineLayout;
            VulkanDynamicMode m_DynamicMode;

            std::unordered_map<uint32, VkShaderModule> m_Modules;
            ShaderBufferLayout m_ShaderLayout = {};
            // shader info
            std::vector<VkVertexInputBindingDescription> bindingDescriptions = {};
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};
            bool DefineVertexInput();
            bool DefineVertexInput(Memory::Shared<VulkanShaderInputLayout> shaderInputLayout);

            std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
            bool DefinePipelineStage(const ShaderFilePathLayout& shaderFiles);
        private:
            std::vector<char> LoadShader(const std::string& shaderFile);
            VkShaderModule MakeModule(const std::string& filepath, ShaderStage code);
            bool Recreate(bool force);
            bool CleanUp(bool force);

            std::string ReplaceShaderExtension(const std::string& shaderPath);
            bool CheckIfShaderExists(const std::string& shaderPath);
            bool CompileShader(const std::string& filePath, ShaderStage shaderType);
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!