//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:56:52 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalShader.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>
#include <fstream>
#include <sstream>

#include "Nkentseu/Graphics/Context.h"
#include "InternalContext.h"
#include "VulkanUtils.h"

/*#include <glslang_c_interface.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_cross_util.hpp>
#include <spirv_cross/spirv_cross_parsed_ir.hpp>

#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/GLSL.std.450.h>
#include <SPIRV/GlslangToSpv.h>*/

#include <string>
#include <filesystem>

#include <utility>

namespace nkentseu {
    
    // Constructor
    InternalShader::InternalShader(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout) : m_GraphicsPipeline(nullptr), m_Context(nullptr){
        if (context == nullptr) return;

        SetShaderFiles(context, shaderFiles, bufferLayout);
    }

    // Destructor
    InternalShader::~InternalShader() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalShader::Create() {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || m_GraphicsPipeline != nullptr) return false;
        InternalContext* context = m_Context->GetInternal();

        VulkanResult result;
        bool first = true;

        if (context->m_PipelineLayout.pipelineLayout == VK_NULL_HANDLE) {
            Log_nts.Debug("Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
            return false;
        }

        if (context->m_RenderPass.renderPass == VK_NULL_HANDLE) {
            Log_nts.Debug("Cannot create graphics pipeline: no renderPass provided in configInfo");
            return false;
        }

        m_Modules.clear();
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

        for (auto [shaderType, shaderFile] : m_ShaderFiles) {
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

        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        
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
        pipelineInfo.layout = context->m_PipelineLayout.pipelineLayout;
        pipelineInfo.renderPass = context->m_RenderPass.renderPass;
        pipelineInfo.pVertexInputState = vertexInputStates.data();
        pipelineInfo.pColorBlendState = colorBlendStates.data();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pRasterizationState = rasterizationStates.data();
        pipelineInfo.pViewportState = viewportStates.data();
        pipelineInfo.pDynamicState = dynamicStateInfos.data();
        pipelineInfo.pMultisampleState = multiSamples.data();
        pipelineInfo.pInputAssemblyState = inputAssemblies.data();

        vkCheckError(first, result, vkCreateGraphicsPipelines(context->m_Gpu.device, 0, 1, &pipelineInfo, 0, &m_GraphicsPipeline), "cannot create graphics pipelines");

        if (result.success) {
            Log_nts.Info("Create gaphics pipeline is good");
            context->AddRecreateCallback(RECREATE_CALLBACK_FN(InternalShader::Recreate));
        }

        return result.success;
    }

    bool InternalShader::Destroy() {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return false;
        InternalContext* context = m_Context->GetInternal();

        for (auto [shaderType, module] : m_Modules) {
            vkCheckErrorVoid(vkDestroyShaderModule(context->m_Gpu.device, module, nullptr));
        }

        vkCheckErrorVoid(vkDestroyPipeline(context->m_Gpu.device, m_GraphicsPipeline, nullptr));

        context->RemoveRecreateCallback(RECREATE_CALLBACK_FN(InternalShader::Recreate));
        return false;
    }

    void InternalShader::SetShaderFiles(Context* context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const BufferLayout& bufferLayout) {
        if (context == nullptr) return;

        m_ShaderFiles.clear();
        m_ShaderFiles = shaderFiles;
        m_Context = context;
    }

    VkShaderModule InternalShader::MakeModule(const std::string& filepath, ShaderType::Code type) {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return nullptr;
        InternalContext* context = m_Context->GetInternal();

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

        vkCheckError(first, result, vkCreateShaderModule(context->m_Gpu.device, &createInfo, nullptr, &shaderModule), "failed to create shader module");

        if (!result.success || shaderModule == nullptr) {
            return nullptr;
        }

        return shaderModule;
    }

    bool InternalShader::Bind(VkCommandBuffer commandBuffer, const VulkanDynamicMode& dynamicMode) const {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || m_GraphicsPipeline == nullptr) return false;
        InternalContext* context = m_Context->GetInternal();

        Vector2u size = m_Context->GetInternal()->GetFrameBufferSize();

        VkRect2D scissor = {};
        scissor.extent = { size.width, size.height };

        VkViewport viewport = {};
        viewport.width = size.width;
        viewport.height = size.height;
        viewport.maxDepth = 1.0f;

        vkCheckErrorVoid(vkCmdSetScissor(commandBuffer, 0, 1, &scissor));
        vkCheckErrorVoid(vkCmdSetViewport(commandBuffer, 0, 1, &viewport));
        vkCheckErrorVoid(vkCmdSetPrimitiveTopology(commandBuffer, dynamicMode.primitiveTopology));
        vkCheckErrorVoid(vkCmdSetFrontFace(commandBuffer, dynamicMode.frontFace));
        vkCheckErrorVoid(vkCmdSetCullMode(commandBuffer, dynamicMode.cullMode));

        if (context->m_Gpu.cmdSetPolygonModeEXT != nullptr) {
            vkCheckErrorVoid(context->m_Gpu.cmdSetPolygonModeEXT(commandBuffer, dynamicMode.polygoneMode));
        }

        vkCheckErrorVoid(vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline));

        return VulkanStaticDebugInfo::success;
    }

    bool InternalShader::Unbind(VkCommandBuffer commandBuffer) const {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return false;
        InternalContext* context = m_Context->GetInternal();

        VkPipeline nullPipeline = VK_NULL_HANDLE;
        return true;
    }

    std::vector<char> InternalShader::LoadShader(const std::string& shaderFile) {
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

    bool InternalShader::Recreate(bool force)
    {
        return false;
    }

    std::string InternalShader::ReplaceShaderExtension(const std::string& shaderPath)
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

    bool InternalShader::CheckIfShaderExists(const std::string& shaderPath)
    {
        return std::filesystem::exists(shaderPath);
    }

    bool InternalShader::CompileShader(const std::string& filePath, ShaderType::Code shaderType) {
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

#endif