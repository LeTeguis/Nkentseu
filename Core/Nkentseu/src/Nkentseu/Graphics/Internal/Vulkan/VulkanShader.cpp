//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:56:52 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanShader.h"

#include <Logger/Formatter.h>
#include <fstream>
#include <sstream>

#include "VulkanContext.h"
#include "VulkanUtils.h"

#include <string>
#include <filesystem>

#include <utility>

namespace nkentseu {
    
    VulkanShader::VulkanShader(Memory::Shared<Context> context) : m_GraphicsPipeline(nullptr), m_Context(Memory::SharedCast<VulkanContext>(context)) {
        if (m_Context != nullptr) {
            m_Context->AddRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanShader::Recreate));
            m_Context->AddCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanShader::CleanUp));
        }
    }

    VulkanShader::~VulkanShader() {
    }

    Memory::Shared<Context> VulkanShader::GetContext() {
        return m_Context;
    }

    bool VulkanShader::LoadFromFile(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout) {
        if (m_Context == nullptr || shaderFiles.size() == 0 || m_GraphicsPipeline != nullptr) return false;

        m_ShaderLayout = shaderLayout;

        VulkanResult result;
        bool first = true;

        if (m_Context->GetRenderPass()->renderPass == VK_NULL_HANDLE) {
            Log_nts.Debug("Cannot create graphics pipeline: no renderPass provided in configInfo");
            return false;
        }

        if (m_ShaderLayout.uniformBuffer.attributes.size() > 0) {
            for (auto& attribut : m_ShaderLayout.uniformBuffer.attributes) {
                VkDescriptorType dtype = (attribut.uType == UniformBufferType::Static) ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
                m_PipelineLayout.Add(attribut.binding, dtype, VK_SHADER_STAGE_VERTEX_BIT);
            }
        }

        if (!m_PipelineLayout.Create(m_Context->GetGpu(), m_Context->GetSwapchain())) {
            Log_nts.Debug("Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
            return false;
        }
        m_Modules.clear();

        DefinePipelineStage(shaderFiles);

        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        if (DefineVertexInput()) {
            vertexInputState.vertexBindingDescriptionCount = bindingDescriptions.size();
            vertexInputState.pVertexBindingDescriptions = bindingDescriptions.data();
            vertexInputState.vertexAttributeDescriptionCount = attributeDescriptions.size();
            vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();
        }

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optionnel
        depthStencil.maxDepthBounds = 1.0f; // Optionnel
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {}; // Optionnel
        depthStencil.back = {}; // Optionnel

        std::vector<VkPipelineVertexInputStateCreateInfo> vertexInputStates;
        vertexInputStates.push_back(vertexInputState);

        VkPipelineColorBlendAttachmentState colorAttachment = {};
        colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorAttachment.blendEnable = VK_FALSE;

        std::vector<VkPipelineColorBlendAttachmentState> colorAttachments;
        colorAttachments.push_back(colorAttachment);

        VkPipelineColorBlendStateCreateInfo colorBlendState = {};
        colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendState.pAttachments = colorAttachments.data();
        colorBlendState.attachmentCount = colorAttachments.size();

        std::vector<VkPipelineColorBlendStateCreateInfo> colorBlendStates;
        colorBlendStates.push_back(colorBlendState);

        VkPipelineRasterizationStateCreateInfo rasterizationState = {};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationState.cullMode = VK_CULL_MODE_NONE;
        rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationState.lineWidth = 1.0f;

        std::vector<VkPipelineRasterizationStateCreateInfo> rasterizationStates;
        rasterizationStates.push_back(rasterizationState);

        VkPipelineMultisampleStateCreateInfo multiSample = {};
        multiSample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multiSample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        std::vector<VkPipelineMultisampleStateCreateInfo> multiSamples;
        multiSamples.push_back(multiSample);

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        std::vector<VkPipelineInputAssemblyStateCreateInfo> inputAssemblies;
        inputAssemblies.push_back(inputAssembly);

        std::vector<VkRect2D> scissors;
        scissors.push_back({});
        std::vector<VkViewport> viewports;
        viewports.push_back({});

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.scissorCount = scissors.size();
        viewportState.pScissors = scissors.data();
        viewportState.viewportCount = viewports.size();
        viewportState.pViewports = viewports.data();

        std::vector<VkPipelineViewportStateCreateInfo> viewportStates;
        viewportStates.push_back(viewportState);

        std::vector<VkDynamicState> dynamicStates;
        dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
        dynamicStates.push_back(VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY);
        dynamicStates.push_back(VK_DYNAMIC_STATE_FRONT_FACE);
        dynamicStates.push_back(VK_DYNAMIC_STATE_CULL_MODE);
        dynamicStates.push_back(VK_DYNAMIC_STATE_POLYGON_MODE_EXT);

        VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.pDynamicStates = dynamicStates.data();
        dynamicStateInfo.dynamicStateCount = dynamicStates.size();

        std::vector<VkPipelineDynamicStateCreateInfo> dynamicStateInfos;
        dynamicStateInfos.push_back(dynamicStateInfo);

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.layout = m_PipelineLayout.pipelineLayout;
        pipelineInfo.renderPass = m_Context->GetRenderPass()->renderPass;
        pipelineInfo.pVertexInputState = vertexInputStates.data();
        pipelineInfo.pColorBlendState = colorBlendStates.data();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pRasterizationState = rasterizationStates.data();
        pipelineInfo.pViewportState = viewportStates.data();
        pipelineInfo.pDynamicState = dynamicStateInfos.data();
        pipelineInfo.pMultisampleState = multiSamples.data();
        pipelineInfo.pInputAssemblyState = inputAssemblies.data();
        pipelineInfo.pDepthStencilState = &depthStencil;

        vkCheckError(first, result, vkCreateGraphicsPipelines(m_Context->GetGpu()->device, 0, 1, &pipelineInfo, 0, &m_GraphicsPipeline), "cannot create graphics pipelines");

        if (!result.success) {
            return false;
        }

        //CreateUniform();

        Log_nts.Info("Create gaphics pipeline is good");

        return result.success;
    }

    bool VulkanShader::Destroy() {
        if (m_Context == nullptr) return false;

        bindingDescriptions.clear();
        attributeDescriptions.clear();

        for (auto [shaderType, module] : m_Modules) {
            vkCheckErrorVoid(vkDestroyShaderModule(m_Context->GetGpu()->device, module, nullptr));
        }

        vkCheckErrorVoid(vkDestroyPipeline(m_Context->GetGpu()->device, m_GraphicsPipeline, nullptr));

        m_PipelineLayout.Destroy(m_Context->GetGpu());

        if (m_Context != nullptr) {
            m_Context->RemoveRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanShader::Recreate));
            m_Context->RemoveCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanShader::CleanUp));
        }
        return false;
    }

    VkShaderModule VulkanShader::MakeModule(const std::string& filepath, ShaderType::Code type) {
        if (m_Context == nullptr) return nullptr;
        VkShaderModule shaderModule = nullptr;
        VulkanResult result;
        bool first = true;

        std::string new_filepath = ReplaceShaderExtension(filepath);

        if (!CheckIfShaderExists(new_filepath)) {
            new_filepath = filepath;
        }

        std::vector<char> code = LoadShader(new_filepath);

        if (code.size() == 0) {
            return nullptr;
        }

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32*>(code.data());

        vkCheckError(first, result, vkCreateShaderModule(m_Context->GetGpu()->device, &createInfo, nullptr, &shaderModule), "failed to create shader module");

        if (!result.success || shaderModule == nullptr) {
            return nullptr;
        }

        return shaderModule;
    }

    bool VulkanShader::Bind() {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;
        VkCommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
        if (commandBuffer == nullptr) return false;

        Vector2u size = m_Context->GetFrameBufferSize();

        VkRect2D scissor = {};
        scissor.extent = { size.width, size.height };

        VkViewport viewport = {};
        viewport.width = size.width;
        viewport.height = size.height;
        viewport.maxDepth = 1.0f;

        vkCheckErrorVoid(vkCmdSetScissor(commandBuffer, 0, 1, &scissor));
        vkCheckErrorVoid(vkCmdSetViewport(commandBuffer, 0, 1, &viewport));
        vkCheckErrorVoid(vkCmdSetPrimitiveTopology(commandBuffer, m_DynamicMode.primitiveTopology));
        vkCheckErrorVoid(vkCmdSetFrontFace(commandBuffer, m_DynamicMode.frontFace));
        vkCheckErrorVoid(vkCmdSetCullMode(commandBuffer, m_DynamicMode.cullMode));

        if (m_Context->GetGpu()->cmdSetPolygonModeEXT != nullptr) {
            vkCheckErrorVoid(m_Context->GetGpu()->cmdSetPolygonModeEXT(commandBuffer, m_DynamicMode.polygoneMode));
        }

        vkCheckErrorVoid(vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline));

        return VulkanStaticDebugInfo::success;
    }

    bool VulkanShader::Unbind() {
        if (m_Context == nullptr) return false;
        return true;
    }

    bool VulkanShader::DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;
        m_DynamicMode.cullMode = VulkanConvert::CullModeType(mode);
        m_DynamicMode.polygoneMode = VulkanConvert::PolygonModeType(contentMode);
        return true;
    }

    bool VulkanShader::PolygonMode(PolygonModeType::Code mode)
    {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;

        if (m_Context->GetGpu()->cmdSetPolygonModeEXT != nullptr) {
            vkCheckErrorVoid(m_Context->GetGpu()->cmdSetPolygonModeEXT(m_Context->GetCurrentCommandBuffer(), VulkanConvert::PolygonModeType(mode)));
        }
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanShader::CullMode(CullModeType::Code mode)
    {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;
        vkCheckErrorVoid(vkCmdSetCullMode(m_Context->GetCurrentCommandBuffer(), VulkanConvert::CullModeType(mode)));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanShader::FrontFaceMode(FrontFaceType::Code mode)
    {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;
        vkCheckErrorVoid(vkCmdSetFrontFace(m_Context->GetCurrentCommandBuffer(), VulkanConvert::FrontFaceType(mode)));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanShader::PrimitiveTopologyMode(PrimitiveTopologyType::Code mode)
    {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;
        vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_Context->GetCurrentCommandBuffer(), VulkanConvert::PrimitiveTopologyType(mode)));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanShader::ScissorMode(const Vector2i& offset, const Vector2u& extend)
    {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;

        VkRect2D scissor = {};
        scissor.extent = { extend.width, extend.height };
        scissor.offset = { offset.width, offset.height };

        vkCheckErrorVoid(vkCmdSetScissor(m_Context->GetCurrentCommandBuffer(), 0, 1, &scissor));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanShader::ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth)
    {
        if (m_Context == nullptr || m_GraphicsPipeline == nullptr) return false;

        VkViewport viewport = {};
        viewport.x = position.x;
        viewport.y = position.y;
        viewport.width = size.width;
        viewport.height = size.height;
        viewport.maxDepth = depth.x;
        viewport.minDepth = depth.y;

        vkCheckErrorVoid(vkCmdSetViewport(m_Context->GetCurrentCommandBuffer(), 0, 1, &viewport));
        return VulkanStaticDebugInfo::success;
    }

    VulkanPipelineLayout* VulkanShader::GetPipelineLayout()
    {
        return &m_PipelineLayout;
    }

    bool VulkanShader::DefineVertexInput()
    {
        if (!m_ShaderLayout.vertexInput.attributes.empty()) {
            bindingDescriptions.resize(1);

            bindingDescriptions[0].binding = 0;
            bindingDescriptions[0].stride = m_ShaderLayout.vertexInput.stride;
            bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            attributeDescriptions.resize(m_ShaderLayout.vertexInput.attributes.size());

            uint32 index = 0;
            bool set = true;

            for (const auto& attribute : m_ShaderLayout.vertexInput.attributes) {
                VkFormat format = VulkanConvert::ShaderFormatToVkFormat(attribute.type);

                // Vérification si le format est valide
                if (format <= VK_FORMAT_UNDEFINED || format > VK_FORMAT_ASTC_12x12_SRGB_BLOCK) {
                    Log_nts.Error("Invalid format for attribute type: {0}", ShaderDataType::ToString(attribute.type));
                    set = false;
                    attributeDescriptions.clear();
                    break;
                }

                attributeDescriptions[index].binding = 0;
                attributeDescriptions[index].location = attribute.location;
                attributeDescriptions[index].format = VulkanConvert::ShaderFormatToVkFormat(attribute.type);
                attributeDescriptions[index].offset = attribute.offset;

                index++;
            }
            return set;
        }
        return false;
    }

    bool VulkanShader::DefinePipelineStage(const std::unordered_map<ShaderType::Code, std::string>& shaderFiles)
    {
        for (auto [shaderType, shaderFile] : shaderFiles) {
            m_Modules[shaderType] = MakeModule(shaderFile, shaderType);

            if (m_Modules[shaderType] == nullptr) {
                m_Modules.clear();
                Log_nts.Debug("Cannot create shader module; Type {0}, File = {1}", ShaderType::ToString(shaderType), shaderFile);
                return false;
            }

            VkPipelineShaderStageCreateInfo shaderStage = {};
            shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStage.stage = VulkanConvert::GetshaderStageType(shaderType);
            shaderStage.module = m_Modules[shaderType];
            shaderStage.pName = "main";
            shaderStage.flags = 0;
            shaderStage.pNext = nullptr;
            shaderStage.pSpecializationInfo = nullptr;

            shaderStages.push_back(shaderStage);
        }
        return true;
    }

    std::vector<char> VulkanShader::LoadShader(const std::string& shaderFile) {
        std::ifstream file(shaderFile, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            Log_nts.Error("failed to open file : {0}", shaderFile);
            return {};
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    bool VulkanShader::Recreate(bool force)
    {
        if (m_Context != nullptr) {
        }
        return true;
    }

    bool VulkanShader::CleanUp(bool force)
    {
        if (m_Context != nullptr) {
        }
        return true;
    }

    std::string VulkanShader::ReplaceShaderExtension(const std::string& shaderPath)
    {
        std::filesystem::path path(shaderPath);

        // Mapping des extensions � leurs �quivalents pour l'extension .spv
        std::unordered_map<std::string, std::string> extensionMappings = {
            {".glsl", ".spv"},    // Extension .glsl sera remplac�e par .spv
            {".vert", ".vert.spv"},
            {".frag", ".frag.spv"},
            {".comp", ".comp.spv"},
            {".geom", ".geom.spv"},
            {".tesc", ".tesc.spv"},
            {".tese", ".tese.spv"}
        };

        // Obtention de l'extension du fichier
        std::string extension = path.extension().string();

        // V�rification si l'extension est dans la liste des extensions � mapper
        if (extensionMappings.find(extension) != extensionMappings.end()) {
            // Si oui, remplacement de l'extension
            path.replace_extension(extensionMappings[extension]);
        }

        // Retourner le chemin mis � jour
        return path.string();
    }

    bool VulkanShader::CheckIfShaderExists(const std::string& shaderPath)
    {
        return std::filesystem::exists(shaderPath);
    }

    bool VulkanShader::CompileShader(const std::string& filePath, ShaderType::Code shaderType) {
        // Mapping des type a leurs equivalence pour compilation
        std::unordered_map<ShaderType::Code, std::string> fileTypeMappings = {
            {ShaderType::Vertex, "--vert"},
            {ShaderType::Fragment, "--frag"},
            {ShaderType::Compute, "--comp"},
            {ShaderType::Geometry, "--geom"},
            {ShaderType::TesControl, "--tesc"},
            {ShaderType::TesEvaluation, "--tese"}
        };

        // S�paration du chemin du fichier en r�pertoire et nom de fichier avec extension
        std::filesystem::path path(filePath);
        std::string directory = path.parent_path().string();
        std::string fileName = path.filename().string();

        std::string command = FORMATTER.Format("python Scripts/commands/buildshdr.py --name {0} --directory {1} {2}", fileName, directory, fileTypeMappings[shaderType]);

        int32 result = std::system(command.c_str());
        if (result != 0) {
            Log_nts.Error("Failed to compile shader.");
            return false;
        }
        return true;
    }
}  //  nkentseu