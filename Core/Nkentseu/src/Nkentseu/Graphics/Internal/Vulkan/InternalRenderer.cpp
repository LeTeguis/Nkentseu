//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-20 at 08:17:20 AM.
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

#include "Nkentseu/Core/Window.h"
#include <Nkentseu/Event/EventFilter.h>
#include <Nkentseu/Event/EventBroker.h>

namespace nkentseu {
    
    // Constructor
    InternalRenderer::InternalRenderer() : m_CurrentImageIndice(0) {
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
            if (!m_Context->Initialize()) {
                return false;
            }
        }
        InternalContext* icontext = m_Context->GetInternal();
        Vector2u size = icontext->GetFrameBufferSize();
        m_ViewportSize = { size.width, size.height };

        EventTrack.AddObserver(REGISTER_CLIENT_EVENT(InternalRenderer::OnEvent));

        m_CurrentImageIndice = 0;
        return true;
    }

    bool InternalRenderer::Deinitialize() {
        EventTrack.RemoveObserver(REGISTER_CLIENT_EVENT(InternalRenderer::OnEvent));
        return true;
    }

    void InternalRenderer::OnEvent(Event& event) {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || !m_IsPrepare) return;

        EventBroker broker(event);

        broker.Route<WindowResizedEvent>(REGISTER_CLIENT_EVENT(InternalRenderer::OnWindowResizedEvent));
    }

    bool InternalRenderer::OnWindowResizedEvent(WindowResizedEvent& event)
    {
        Log_nts.Debug("resize = {0}", event.GetSize());
        if (event.GetSize() == Vector2u()) {
            return false;
        }

        InternalContext* context = m_Context->GetInternal();

        Log_nts.Debug("resize");

        context->RecreateSwapChain();
        Vector2u size = context->GetFrameBufferSize();
        m_ViewportSize = { size.width , size.height };
        m_CurrentImageIndice = 0;

        return false;
    }

    bool InternalRenderer::Clear(const Color& color) {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || !m_IsPrepare) return false;
        InternalContext* context = m_Context->GetInternal();

        m_PreviousColor = color;

        return true;
    }

    bool InternalRenderer::Clear(uint8 r, uint8 g, uint8 b, uint8 a) {
        return Clear(Color(r, g, b, a));
    }

    bool InternalRenderer::UseShader(Memory::Shared<Shader> shader)
    {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || !m_IsPrepare) return false;
        if (m_CurrentCommandBuffer == nullptr || shader == nullptr || shader->GetInternal() == nullptr) return false;

        InternalContext* context = m_Context->GetInternal();
        InternalShader* shader_i = shader->GetInternal();

        if (shader != m_CurrentShader) {
            m_CurrentShader = shader;
        }
        return shader_i->Bind(m_CurrentCommandBuffer, m_DynamicMode);
    }

    bool InternalRenderer::UnuseShader()
    {
        if (m_Context == nullptr || m_Context->GetInternal() == nullptr || !m_IsPrepare || m_CurrentCommandBuffer == nullptr) return false;
        InternalContext* context = m_Context->GetInternal();

        m_CurrentShader = nullptr;

        return true;
    }

    bool InternalRenderer::DrawMode(CullModeType::Code mode, PolygonModeType::Code contentMode) {
        if (!CanRender()) {
            return false;
        }
        m_DynamicMode.cullMode = VulkanConvert::CullModeType(mode);
        m_DynamicMode.polygoneMode = VulkanConvert::PolygonModeType(contentMode);
        return true;
    }

    bool InternalRenderer::Prepare()
    {
        m_IsPrepare = false;

        if (!CanRender()) return false;
        InternalContext* context = m_Context->GetInternal();

        VulkanResult result;
        bool first = true;

        vkCheckError(first, result, vkAcquireNextImageKHR(context->m_Gpu.device, context->m_Swapchain.swapchain, UINT64_MAX, context->m_Semaphore.aquireSemaphore, VK_NULL_HANDLE, &m_CurrentImageIndice), "cannot acquier next image khr ({0})", m_CurrentImageIndice);
        //Log_nts.Debug("current buffer : {0}", m_CurrentImageIndice);

        if (!result.success) {
            if (result.result == VK_ERROR_OUT_OF_DATE_KHR) {
                context->RecreateSwapChain();
                Vector2u size = context->GetFrameBufferSize();
                m_ViewportSize = { size.width, size.height };
                m_CurrentImageIndice = 0;
                return false;
            } else if (result.result != VK_SUBOPTIMAL_KHR && result.result != VK_SUCCESS){
                return false;
            }
        }

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandBufferCount = 1;
        allocInfo.commandPool = context->m_CommandPool.commandPool;

        vkCheckError(first, result, vkAllocateCommandBuffers(context->m_Gpu.device, &allocInfo, &m_CurrentCommandBuffer), "cannot allocate command buffer");

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
        renderPassBeginInfo.renderPass = context->m_RenderPass.renderPass;
        renderPassBeginInfo.renderArea.extent = m_ViewportSize;
        renderPassBeginInfo.framebuffer = context->m_Framebuffer.framebuffer[m_CurrentImageIndice];
        renderPassBeginInfo.pClearValues = clearValues.data();
        renderPassBeginInfo.clearValueCount = clearValues.size();

        vkCheckErrorVoid(vkCmdBeginRenderPass(m_CurrentCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE));

        m_IsPrepare = result.success;
        m_ClearColor = false;
        return result.success;
    }

    bool InternalRenderer::Finalize()
    {
        if (!CanRender() || !m_IsPrepare) return false;
        InternalContext* context = m_Context->GetInternal();

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
        submitInfo.pSignalSemaphores = &context->m_Semaphore.submitSemaphore;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &context->m_Semaphore.aquireSemaphore;
        submitInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueueSubmit(context->m_Gpu.queue.graphicsQueue, 1, &submitInfo, 0), "cannot submit command");

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pSwapchains = &context->m_Swapchain.swapchain;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &m_CurrentImageIndice;
        presentInfo.pWaitSemaphores = &context->m_Semaphore.submitSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        vkCheckError(first, result, vkQueuePresentKHR(context->m_Gpu.queue.graphicsQueue, &presentInfo), "cannot present image");

        if (!result.success) {
            bool exited = false;

            if (result.result == VK_ERROR_OUT_OF_DATE_KHR || result.result == VK_SUBOPTIMAL_KHR) {
                context->RecreateSwapChain();
                Vector2u size = context->GetFrameBufferSize();
                m_ViewportSize = { size.width, size.height };
                m_CurrentImageIndice = 0;

                exited = true;
            }

            if (exited) {
                if (m_CurrentCommandBuffer != nullptr) {
                    vkCheckError(first, result, vkDeviceWaitIdle(context->m_Gpu.device), "cannot wait device idle");
                    vkCheckErrorVoid(vkFreeCommandBuffers(context->m_Gpu.device, context->m_CommandPool.commandPool, 1, &m_CurrentCommandBuffer));
                }
                Log_nts.Error("failed to present swap chain image!");
                return false;
            }
        }

        vkCheckError(first, result, vkDeviceWaitIdle(context->m_Gpu.device), "cannot wait device idle");
        vkCheckErrorVoid(vkFreeCommandBuffers(context->m_Gpu.device, context->m_CommandPool.commandPool, 1, &m_CurrentCommandBuffer));

        m_CurrentImageIndice++;

        if (m_CurrentImageIndice >= context->m_Swapchain.swapchainImages.size()) {
            m_CurrentImageIndice = 0;
        }
        return false;
    }

    bool InternalRenderer::Draw(Memory::Shared<VertexArray> vertexArray, DrawVertexType::Code drawVertex) {
        if (!CanRender() || m_CurrentShader == nullptr || drawVertex == DrawVertexType::NotDefine) {
            return false;
        }

        if (vertexArray == nullptr) {
            return false;
        }

        InternalVertexArray* internalVertexArray = vertexArray->GetInternal();
        if (internalVertexArray == nullptr) {
            return false;
        }

        InternalVertexBuffer* vertexBuffer = vertexArray->GetInternal()->GetInternalVertexBuffer();
        InternalIndexBuffer* indexBuffer = vertexArray->GetInternal()->GetInternalIndexBuffer();


        if (vertexBuffer == nullptr && internalVertexArray->GetVertexNumber() == 0) {
            return false;
        }

        uint32 count = internalVertexArray->GetVertexNumber();

        if (indexBuffer == nullptr) {
            vkCheckErrorVoid(vkCmdDraw(m_CurrentCommandBuffer, count, 1, 0, 0));
        }
        else {
        }

        return true;
    }

    bool InternalRenderer::Present() {
        return Prepare();
    }

    bool InternalRenderer::Swapbuffer() {
        return Prepare();
    }

    bool InternalRenderer::Resize(const Vector2u& size) {
        if (!CanRender() || !m_IsPrepare) return false;
        InternalContext* context = m_Context->GetInternal();

        Log_nts.Debug("resize");

        context->RecreateSwapChain();
        Vector2u size_current = context->GetFrameBufferSize();
        Vector2u size_translate = context->GetWindow()->ConvertPixelToDpi(size);

        if (size_current != size_translate){

            Log_nts.Warning("Potential error du to different window size when windows is resize");
        }
        m_ViewportSize = { size_current.width, size_current.height };
        return true;
    }

    bool InternalRenderer::CanRender()
    {
        if (m_Context == nullptr) {
            return false;
        }
        if (m_Context->GetInternal() == nullptr) {
            return false;
        }
        InternalContext* context = m_Context->GetInternal();
        if (context->m_Swapchain.swapchainImages.size() == 0) {
            return false;
        }
        if (context->GetWindow()->GetSize() == Vector2u()) {
            return false;
        }
        return true;
    }

}  //  nkentseu

#endif