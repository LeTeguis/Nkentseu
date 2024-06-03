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

#include "Nkentseu/Core/Window.h"
#include <Nkentseu/Event/EventFilter.h>
#include <Nkentseu/Event/EventBroker.h>

namespace nkentseu {
    //VulkanRenderer::VulkanRenderer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext, Context>(context)) {
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

        EventTrack.AddObserver(REGISTER_CLIENT_EVENT(VulkanRenderer::OnEvent));

        return true;
    }

    bool VulkanRenderer::Deinitialize() {
        EventTrack.RemoveObserver(REGISTER_CLIENT_EVENT(VulkanRenderer::OnEvent));
        return true;
    }

    bool VulkanRenderer::Begin(uint8 r, uint8 g, uint8 b, uint8 a) {
        return Begin(r, g, b, a);
    }

    bool VulkanRenderer::Begin(const Color& color)
    {
        m_IsPrepare = false;

        if (!CanRender()) return false;

        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, vkAcquireNextImageKHR(m_Context->m_Gpu.device, m_Context->m_Swapchain.swapchain, UINT64_MAX, m_Context->m_Semaphore.aquireSemaphore, VK_NULL_HANDLE, &m_Context->currentImageIndex), "cannot acquier next image khr ({0})", m_Context->currentImageIndex);

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
        m_CurrentCommandBuffer = m_Context->m_CommandBuffer.commandBuffers[m_Context->currentImageIndex];

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkCheckError(first, result, vkBeginCommandBuffer(m_CurrentCommandBuffer, &beginInfo), "cannot start command buffer");

        VkClearValue clearValue = {};
        clearValue.color = { m_PreviousColor.Rf(), m_PreviousColor.Gf(), m_PreviousColor.Bf(), m_PreviousColor.Af() };

        std::vector<VkClearValue> clearValues;
        clearValues.push_back(clearValue);

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = m_Context->m_RenderPass.renderPass;
        renderPassBeginInfo.renderArea.extent = m_ViewportSize;
        renderPassBeginInfo.framebuffer = m_Context->m_Framebuffer.framebuffer[m_Context->currentImageIndex];
        renderPassBeginInfo.pClearValues = clearValues.data();
        renderPassBeginInfo.clearValueCount = clearValues.size();

        vkCheckErrorVoid(vkCmdBeginRenderPass(m_CurrentCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE));

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

        vkCheckErrorVoid(vkCmdEndRenderPass(m_CurrentCommandBuffer));

        vkCheckError(first, result, vkEndCommandBuffer(m_CurrentCommandBuffer), "cannot finish command buffer");

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CurrentCommandBuffer;
        submitInfo.pSignalSemaphores = &m_Context->m_Semaphore.submitSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_Context->m_Semaphore.aquireSemaphore;
        submitInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueueSubmit(m_Context->m_Gpu.queue.graphicsQueue, 1, &submitInfo, 0), "cannot submit command");

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pSwapchains = &m_Context->m_Swapchain.swapchain;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &m_Context->currentImageIndex;
        presentInfo.pWaitSemaphores = &m_Context->m_Semaphore.submitSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueuePresentKHR(m_Context->m_Gpu.queue.graphicsQueue, &presentInfo), "cannot present image");

        if (!result.success) {
            bool exited = false;

            if (result.result == VK_ERROR_OUT_OF_DATE_KHR || result.result == VK_SUBOPTIMAL_KHR) {
                m_Context->RecreateSwapChain();
                Vector2u size = m_Context->GetFrameBufferSize();
                m_ViewportSize = { size.width, size.height };

                exited = true;
            }

            if (exited) {
                if (m_CurrentCommandBuffer != nullptr) {
                    vkCheckError(first, result, vkDeviceWaitIdle(m_Context->m_Gpu.device), "cannot wait device idle");
                }
                return false;
            }
        }

        vkCheckError(first, result, vkDeviceWaitIdle(m_Context->m_Gpu.device), "cannot wait device idle");

        m_Context->currentImageIndex++;

        if (m_Context->currentImageIndex >= m_Context->m_Swapchain.swapchainImages.size()) {
            m_Context->currentImageIndex = 0;
        }
        return false;
    }

    bool VulkanRenderer::Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) {
        if (!CanRender() || m_CurrentShader == nullptr || drawVertex == DrawVertexType::NotDefine) {
            return false;
        }

        Memory::Shared<VulkanVertexArray> vao = Memory::SharedCast<VulkanVertexArray>(vertexArray);

        if (vao == nullptr) {
            return false;
        }

        Memory::Shared<VulkanVertexBuffer> vertexBuffer = Memory::SharedCast<VulkanVertexBuffer>(vao->GetVertexBuffer());
        Memory::Shared<VulkanIndexBuffer> indexBuffer = Memory::SharedCast<VulkanIndexBuffer>(vao->GetIndexBuffer());

        if ((vertexBuffer == nullptr || vertexBuffer->GetBuffer() == nullptr) && vao->Leng() == 0) {
            return false;
        }

        if (indexBuffer == nullptr) {
            uint32 count = vao->Leng();

            if (vertexBuffer != nullptr && vertexBuffer->GetBuffer() != nullptr) {
                count = vertexBuffer->Leng();
                VkBuffer vertexBuffers[] = { vertexBuffer->GetBuffer()->buffer };
                VkDeviceSize offsets[] = { 0 };
                vkCheckErrorVoid(vkCmdBindVertexBuffers(m_CurrentCommandBuffer, 0, 1, vertexBuffers, offsets));
            }
            vkCheckErrorVoid(vkCmdDraw(m_CurrentCommandBuffer, count, 1, 0, 0));
        }
        else {
            VkBuffer vertexBuffers[] = { vertexBuffer->GetBuffer()->buffer };
            VkDeviceSize offsets[] = { 0 };

            vkCmdBindVertexBuffers(m_CurrentCommandBuffer, 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(m_CurrentCommandBuffer, indexBuffer->GetBuffer()->buffer, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(m_CurrentCommandBuffer, indexBuffer->Leng(), 1, 0, 0, 0);
        }

        return true;
    }
    bool VulkanRenderer::BindShader(Memory::Shared<Shader> shader)
    {
        if (m_Context == nullptr || !m_IsPrepare) return false;
        if (m_CurrentCommandBuffer == nullptr || shader == nullptr) return false;
        Memory::Shared<VulkanShader> sh = Memory::SharedCast<VulkanShader>(shader);

        if (sh != m_CurrentShader) {
            m_CurrentShader = sh;
        }

        return m_CurrentShader->Bind(m_CurrentCommandBuffer, m_DynamicMode);
    }

    bool VulkanRenderer::UnbindShader()
    {
        if (m_Context == nullptr || !m_IsPrepare || m_CurrentCommandBuffer == nullptr) return false;

        m_CurrentShader = nullptr;

        return true;
    }

    bool VulkanRenderer::BindUniform(const std::string& name, void* data, usize size)
    {
        if (!CanRender() || m_CurrentShader == nullptr || m_CurrentCommandBuffer == nullptr) return false;
        return m_CurrentShader->UseUniform(m_CurrentCommandBuffer, name, data, size);
    }

    bool VulkanRenderer::UnbindUniform(const std::string& name)
    {
        if (!CanRender() || m_CurrentShader == nullptr || m_CurrentCommandBuffer == nullptr) return false;
        return true;
    }

    bool VulkanRenderer::DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) {
        if (!CanRender()) {
            return false;
        }
        m_DynamicMode.cullMode = VulkanConvert::CullModeType(mode);
        m_DynamicMode.polygoneMode = VulkanConvert::PolygonModeType(contentMode);
        return true;
    }

    bool VulkanRenderer::PolygonMode(PolygonModeType::Code mode)
    {
        if (!CanRender()) {
            return false;
        }

        if (m_Context->m_Gpu.cmdSetPolygonModeEXT != nullptr) {
            vkCheckErrorVoid(m_Context->m_Gpu.cmdSetPolygonModeEXT(m_CurrentCommandBuffer, VulkanConvert::PolygonModeType(mode)));
        }
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanRenderer::CullMode(CullModeType::Code mode)
    {
        if (!CanRender()) {
            return false;
        }
        vkCheckErrorVoid(vkCmdSetCullMode(m_CurrentCommandBuffer, VulkanConvert::CullModeType(mode)));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanRenderer::FrontFaceMode(FrontFaceType::Code mode)
    {
        if (!CanRender()) {
            return false;
        }
        vkCheckErrorVoid(vkCmdSetFrontFace(m_CurrentCommandBuffer, VulkanConvert::FrontFaceType(mode)));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanRenderer::PrimitiveTopologyMode(PrimitiveTopologyType::Code mode)
    {
        if (!CanRender()) {
            return false;
        }
        vkCheckErrorVoid(vkCmdSetPrimitiveTopology(m_CurrentCommandBuffer, VulkanConvert::PrimitiveTopologyType(mode)));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanRenderer::ScissorMode(const Vector2i& offset, const Vector2u& extend)
    {
        if (!CanRender()) {
            return false;
        }

        VkRect2D scissor = {};
        scissor.extent = { extend.width, extend.height };
        scissor.offset = { offset.width, offset.height };

        vkCheckErrorVoid(vkCmdSetScissor(m_CurrentCommandBuffer, 0, 1, &scissor));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanRenderer::ViewportMode(const Vector2f& position, const Vector2f& size, const Vector2f& depth)
    {
        if (!CanRender()) {
            return false;
        }
        VkViewport viewport = {};
        viewport.x = position.x;
        viewport.y = position.y;
        viewport.width = size.width;
        viewport.height = size.height;
        viewport.maxDepth = depth.x;
        viewport.minDepth = depth.y;

        vkCheckErrorVoid(vkCmdSetViewport(m_CurrentCommandBuffer, 0, 1, &viewport));
        return VulkanStaticDebugInfo::success;
    }

    bool VulkanRenderer::CanRender()
    {
        if (m_Context == nullptr) {
            return false;
        }

        if (m_Context->m_Swapchain.swapchainImages.size() == 0) {
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
