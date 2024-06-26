//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 08:17:20 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanRenderer.h"

#include <Logger/Formatter.h>

#include "Nkentseu/Graphics/Context.h"
#include "Nkentseu/Graphics/Shader.h"
#include "Nkentseu/Graphics/VertexArray.h"
#include "Nkentseu/Graphics/VertexBuffer.h"
#include "Nkentseu/Graphics/IndexBuffer.h"
#include "VulkanContext.h"
#include "VulkanShader.h"
#include "VulkanIndexBuffer.h"
#include "VulkanVertexArray.h"
#include "VulkanVertexBuffer.h"
#include "VulkanUtils.h"
#include "VulkanCanvas.h"

#include "Nkentseu/Core/Window.h"
#include <Nkentseu/Event/EventFilter.h>
#include <Nkentseu/Event/EventBroker.h>
#include "VulkanUniformBuffer.h"

namespace nkentseu {
    using namespace maths;

    VulkanRenderer::VulkanRenderer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
    }

    VulkanRenderer::~VulkanRenderer() {
    }

    Memory::Shared<Context> VulkanRenderer::GetContext() {
        return m_Context;
    }

    bool VulkanRenderer::Initialize() {
        if (m_Context == nullptr) {
            return false;
        }

        if (!m_Context->IsInitialize()) {
            if (!m_Context->Initialize()) {
                return false;
            }
        }

        Vector2u size = m_Context->GetFrameBufferSize();
        m_ViewportSize = { size.width, size.height };

        EventTraker.AddObserver(REGISTER_CLIENT_EVENT(VulkanRenderer::OnEvent));

        return true;
    }

    bool VulkanRenderer::Deinitialize() {
        if (m_Context == nullptr) {
            return false;
        }

        if (m_Canvas != nullptr) {
            m_Canvas->Present();
        }

        EventTraker.RemoveObserver(REGISTER_CLIENT_EVENT(VulkanRenderer::OnEvent));
        return true;
    }

    bool VulkanRenderer::Begin(uint8 r, uint8 g, uint8 b, uint8 a) {
        return Begin(r, g, b, a);
    }

    bool VulkanRenderer::Begin(const Color& color)
    {
        m_IsPrepare = false;
        m_PreviousColor = color;
        if (!CanRender()) return false;

        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, vkAcquireNextImageKHR(m_Context->GetGpu()->device, m_Context->GetSwapchain()->swapchain, UINT64_MAX, m_Context->GetSemaphore()->aquireSemaphore, VK_NULL_HANDLE, &m_Context->currentImageIndex), "cannot acquier next image khr ({0})", m_Context->currentImageIndex);

        if (!result.success) {
            if (result.result == VK_ERROR_OUT_OF_DATE_KHR) {
                m_Context->RecreateSwapChain();
                Vector2u size = m_Context->GetFrameBufferSize();
                m_ViewportSize = { size.width, size.height };
                return false;
            } else if (result.result != VK_SUBOPTIMAL_KHR && result.result != VK_SUCCESS){
                return false;
            }
        }

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkCheckError(first, result, vkBeginCommandBuffer(m_Context->GetCurrentCommandBuffer(), &beginInfo), "cannot start command buffer");

        VkClearValue clearColor = {};
        clearColor.color = { m_PreviousColor.Rf(), m_PreviousColor.Gf(), m_PreviousColor.Bf(), m_PreviousColor.Af() };

        VkClearValue clearDepth = {};
        clearDepth.depthStencil = { 1.0f, 0 };

        std::vector<VkClearValue> clearValues;
        clearValues.push_back(clearColor);
        clearValues.push_back(clearDepth);

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_Context->GetRenderPass()->renderPass;
        renderPassBeginInfo.renderArea.extent = m_ViewportSize;
        renderPassBeginInfo.framebuffer = m_Context->GetFramebuffer()->framebuffer[m_Context->currentImageIndex];
        renderPassBeginInfo.pClearValues = clearValues.data();
        renderPassBeginInfo.clearValueCount = clearValues.size();

        vkCheckErrorVoid(vkCmdBeginRenderPass(m_Context->GetCurrentCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE));

        m_IsPrepare = result.success;
        m_ClearColor = false;
        m_Context->renderThisFrame = true;
        return result.success;
    }

    bool VulkanRenderer::End()
    {
        if (!CanRender() || !m_IsPrepare) return false;

        VulkanResult result;
        bool first = true;

        if (m_Canvas != nullptr) {
            m_Canvas->Present();
        }

        vkCheckErrorVoid(vkCmdEndRenderPass(m_Context->GetCurrentCommandBuffer()));

        vkCheckError(first, result, vkEndCommandBuffer(m_Context->GetCurrentCommandBuffer()), "cannot finish command buffer");

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkCommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.pSignalSemaphores = &m_Context->GetSemaphore()->submitSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_Context->GetSemaphore()->aquireSemaphore;
        submitInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueueSubmit(m_Context->GetGpu()->queue.graphicsQueue, 1, &submitInfo, 0), "cannot submit command");

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pSwapchains = &m_Context->GetSwapchain()->swapchain;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &m_Context->currentImageIndex;
        presentInfo.pWaitSemaphores = &m_Context->GetSemaphore()->submitSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueuePresentKHR(m_Context->GetGpu()->queue.graphicsQueue, &presentInfo), "cannot present image");

        if (!result.success) {
            bool exited = false;

            if (result.result == VK_ERROR_OUT_OF_DATE_KHR || result.result == VK_SUBOPTIMAL_KHR) {
                m_Context->RecreateSwapChain();
                Vector2u size = m_Context->GetFrameBufferSize();
                m_ViewportSize = { size.width, size.height };

                exited = true;
            }

            if (exited) {
                if (m_Context->GetCurrentCommandBuffer() != nullptr) {
                    vkCheckError(first, result, vkDeviceWaitIdle(m_Context->GetGpu()->device), "cannot wait device idle");
                }
                return false;
            }
        }

        vkCheckError(first, result, vkDeviceWaitIdle(m_Context->GetGpu()->device), "cannot wait device idle");

        m_Context->currentImageIndex++;

        if (m_Context->currentImageIndex >= m_Context->GetSwapchain()->swapchainImages.size()) {
            m_Context->currentImageIndex = 0;
        }
        return false;
    }

    bool VulkanRenderer::SetViewport(const maths::Vector4f& viewport)
    {
        return false;
    }

    bool VulkanRenderer::SetViewport(float32 x, float32 y, float32 width, float32 height)
    {
        return false;
    }

    bool VulkanRenderer::ResetViewport()
    {
        return false;
    }

    bool VulkanRenderer::SetScissor(const maths::Vector4f& scissor)
    {
        return false;
    }

    bool VulkanRenderer::SetScissor(float32 x, float32 y, float32 width, float32 height)
    {
        return false;
    }

    bool VulkanRenderer::ResetScissor()
    {
        return false;
    }

    Memory::Shared<Canvas> VulkanRenderer::GetCanvas()
    {
        if (m_Canvas == nullptr) {
            m_Canvas = Memory::Alloc<VulkanCanvas>(m_Context);
        }
        if (m_Canvas != nullptr) m_Canvas->Prepare();
        return m_Canvas;
    }

    /*Memory::Shared<Canvas> VulkanRenderer::GetCanvas()
    {
        return nullptr;
    }*/

    bool VulkanRenderer::CanRender()
    {
        if (m_Context == nullptr) {
            return false;
        }

        if (m_Context->GetSwapchain()->swapchainImages.size() == 0) {
            return false;
        }

        if (m_Context->GetWindow()->GetSize() == Vector2u()) {
            return false;
        }
        return true;
    }

    void VulkanRenderer::OnEvent(Event& event) {
        if (m_Context == nullptr || !m_IsPrepare) return;

        EventBroker broker(event);

        broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(VulkanRenderer::OnWindowResizedEvent));
    }

    bool VulkanRenderer::OnWindowResizedEvent(WindowResizedEvent& event)
    {
        Log_nts.Debug("resize = {0}", event.GetSize());
        if (event.GetSize() == Vector2u()) {
            return false;
        }

        Log_nts.Debug("resize");

        m_Context->RecreateSwapChain();
        Vector2u size = m_Context->GetFrameBufferSize();
        m_ViewportSize = { size.width , size.height };

        return false;
    }

}  //  nkentseu
