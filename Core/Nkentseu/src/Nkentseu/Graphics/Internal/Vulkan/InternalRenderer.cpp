//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-20 at 08:17:20 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "InternalRenderer.h"

#ifdef NKENTSEU_GRAPHICS_API_VULKAN
#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/Shader.h"
#include "Nkentseu/Graphics/VertexArray.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "InternalContext.h"
#include "InternalShader.h"
#include "InternalIndexBuffer.h"
#include "InternalVertexArray.h"
#include "InternalVertexBuffer.h"
#include "VulkanUtils.h"

namespace nkentseu {
    
    // Constructor
    InternalRenderer::InternalRenderer() {
        // Ajoutez votre code de constructeur ici
    }

    // Destructor
    InternalRenderer::~InternalRenderer() {
        // Ajoutez votre code de destructeur ici
    }

    bool InternalRenderer::Initialize(Context* context) {
        if (context == nullptr) {
            return false;
        }
        m_Context = context;
        if (!m_Context->IsInitialize()) {
            return m_Context->Initialize();
        }
        return true;
    }

    bool InternalRenderer::Deinitialize() {
        return false;
    }

    bool InternalRenderer::Clear(const Color& color) {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || !m_IsPrepare) return false;
        InternalContext* context = m_Context->GetInternal();

        VulkanResult result;

        VkClearColorValue vkColor = { color.Rf(), color.Gf(), color.Bf(), color.Af() };
        VkImageSubresourceRange range = {};
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.layerCount = 1;
        range.levelCount = 1;

        vkCheckErrorVoid(vkCmdClearColorImage(m_CurrentCommandBuffer, context->m_Swapchain.swapchainImages[m_CurrentImageIndice], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &vkColor, 1, &range));
        //Log_nts.Debug();
        // return result.success;
        m_PreviousColor = color;
        m_ClearColor = true;
        return true;
    }

    bool InternalRenderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a) {
        return Clear(Color(r, g, b, a));
    }

    bool InternalRenderer::SetActiveShader(Memory::Shared<Shader> shader) {
        return false;
    }

    bool InternalRenderer::UnsetActiveShader() {
        return false;
    }

    bool InternalRenderer::DrawMode(DrawMode::Code mode, DrawContentMode::Code contentMode) {
        return false;
    }

    bool InternalRenderer::Prepare()
    {
        m_IsPrepare = false;

        if (m_Context == nullptr || m_Context->GetInternal() == nullptr) return false;
        InternalContext* context = m_Context->GetInternal();

        VulkanResult result;
        bool first = true;
        m_CurrentImageIndice = 0;

        vkCheckError(first, result, vkAcquireNextImageKHR(context->m_Gpu.device, context->m_Swapchain.swapchain, UINT64_MAX, context->m_Semaphore.aquireSemaphore, VK_NULL_HANDLE, &m_CurrentImageIndice), "cannot acquier next image khr");
        //result.success = true;

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandBufferCount = 1;
        allocInfo.commandPool = context->m_CommandPool.commandPool;

        vkCheckError(first, result, vkAllocateCommandBuffers(context->m_Gpu.device, &allocInfo, &m_CurrentCommandBuffer), "cannot allocate command buffer");

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkCheckError(first, result, vkBeginCommandBuffer(m_CurrentCommandBuffer, &beginInfo), "cannot start command buffer");

        if (!m_ClearColor && result.success) {
            Clear(m_PreviousColor);
        }

        m_IsPrepare = result.success;
        m_ClearColor = false;
        return result.success;
    }

    bool InternalRenderer::Finalize()
    {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || !m_IsPrepare) return false;
        InternalContext* context = m_Context->GetInternal();

        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, vkEndCommandBuffer(m_CurrentCommandBuffer), "cannot finish command buffer");

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CurrentCommandBuffer;
        submitInfo.pSignalSemaphores = &context->m_Semaphore.submitSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &context->m_Semaphore.aquireSemaphore;
        submitInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueueSubmit(context->m_Gpu.graphicsQueue, 1, &submitInfo, 0), "cannot submit command");

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pSwapchains = &context->m_Swapchain.swapchain;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &m_CurrentImageIndice;
        presentInfo.pWaitSemaphores = &context->m_Semaphore.submitSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueuePresentKHR(context->m_Gpu.graphicsQueue, &presentInfo), "cannot present image");

        vkCheckError(first, result, vkDeviceWaitIdle(context->m_Gpu.device), "cannot wait device idle");
        vkCheckErrorVoid(vkFreeCommandBuffers(context->m_Gpu.device, context->m_CommandPool.commandPool, 1, &m_CurrentCommandBuffer));
        return false;
    }

    bool InternalRenderer::Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) {
        return false;
    }

    bool InternalRenderer::Present() {
        return false;
    }

    bool InternalRenderer::Swapbuffer() {
        return false;
    }

    bool InternalRenderer::Resize(const Vector2u& size) {
        return false;
    }

}  //  nkentseu

#endif