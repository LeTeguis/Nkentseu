//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-07-08 at 05:28:49 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanRenderWindow.h"
#include <Logger/Formatter.h>
#include <Nkentseu/Event/EventBroker.h>
#include <Nkentseu/Event/EventFilter.h>
#include "VulkanUtils.h"

#include "Nkentseu/Core/Window.h"

namespace nkentseu {
    VulkanRenderWindow::VulkanRenderWindow(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext>(context)) {
    }

    VulkanRenderWindow::~VulkanRenderWindow() {
    }

    Memory::Shared<Context> VulkanRenderWindow::GetContext() {
        return m_Context;
    }

    bool VulkanRenderWindow::Initialize() {
        if (m_Context == nullptr) {
            return false;
        }

        if (!m_Context->IsInitialize()) {
            if (!m_Context->Initialize()) {
                return false;
            }
        }

        if (m_Context != nullptr) {
            m_Context->AddRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanRenderWindow::Recreate));
            m_Context->AddCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanRenderWindow::CleanUp));
        }

        if (!CreateDepth(m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize()))) {
            Log_nts.Error("Cannot create frame buffer");
            return false;
        }

        if (!CreateFrameBuffer(m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize()))) {
            Log_nts.Error("Cannot create frame buffer");
            return false;
        }

        return true;
    }

    bool VulkanRenderWindow::Deinitialize() {
        if (m_Context == nullptr) {
            return false;
        }

        if (!DestroyFrameBuffer()) {
            Log_nts.Error("Cannot destroy frame buffer");
        }

        if (!DestroyDepth()) {
            Log_nts.Error("Cannot destroy depth image in frame buffer");
        }

        if (m_Context != nullptr) {
            m_Context->RemoveRecreateCallback(SWAPCHAIN_CALLBACK_FN(VulkanRenderWindow::Recreate));
            m_Context->RemoveCleanUpCallback(SWAPCHAIN_CALLBACK_FN(VulkanRenderWindow::CleanUp));
        }
        return true;
    }

    bool VulkanRenderWindow::Begin(uint8 r, uint8 g, uint8 b, uint8 a) {
        return Begin(r, g, b, a);
    }

    bool VulkanRenderWindow::Begin(const Color& color)
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer currentCommandBuffer = m_Context->GetCommandBuffer();

        vk::ClearValue clearColor = {};
        clearColor.color = { color.Rf(), color.Gf(), color.Bf(), color.Af() };

        vk::ClearValue clearDepth = {};
        clearDepth.depthStencil = vk::ClearDepthStencilValue{ 1.0f, 0 };

        std::vector<vk::ClearValue> clearValues;
        clearValues.push_back(clearColor);
        clearValues.push_back(clearDepth);

        maths::Vector2u size = m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize());
        m_ViewportSize = vk::Extent2D{ size.width , size.height };

        vk::RenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.renderPass = m_Context->renderPass;
        renderPassBeginInfo.renderArea.extent = m_ViewportSize;
        renderPassBeginInfo.framebuffer = framebuffer[m_Context->currentFrameIndex];
        renderPassBeginInfo.pClearValues = clearValues.data();
        renderPassBeginInfo.clearValueCount = clearValues.size();

        vkCheckErrorVoid(currentCommandBuffer.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline));

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot begin render pass");
        }

        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::End()
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer currentCommandBuffer = m_Context->GetCommandBuffer();

        vkCheckErrorVoid(currentCommandBuffer.endRenderPass());

        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot end render pass");
        }

        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::SetViewport(const maths::Vector4f& viewport)
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        vk::Viewport viewport_ = {};
        viewport_.x = viewport.x;
        viewport_.y = viewport.y;
        viewport_.width = viewport.width;
        viewport_.height = viewport.height;
        viewport_.maxDepth = 1.0f;

        vkCheckErrorVoid(commandBuffer.setViewport(0, 1, &viewport_));
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::SetViewport(float32 x, float32 y, float32 width, float32 height)
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        vk::Viewport viewport = {};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.maxDepth = 1.0f;

        vkCheckErrorVoid(commandBuffer.setViewport(0, 1, &viewport));
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::ResetViewport()
    {
        if (m_Context == nullptr) {
            return false;
        }
        maths::Vector2u size = m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize());
        return SetViewport(0, 0, size.width, size.height);
    }

    bool VulkanRenderWindow::EnableDepthTest(bool enabled)
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        if (enabled) {
            vkCheckErrorVoid(commandBuffer.setDepthTestEnable(VK_TRUE));
        }
        else {
            vkCheckErrorVoid(commandBuffer.setDepthTestEnable(VK_FALSE));
        }
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::EnableScissorTest(bool enabled)
    {
        if (m_Context == nullptr) {
            return false;
        }
        return true;
    }

    bool VulkanRenderWindow::SetScissor(const maths::Vector4f& scissor)
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        vk::Rect2D scissor_ = {};
        scissor_.extent = vk::Extent2D{ (uint32)scissor.width, (uint32)scissor.height };
        scissor_.offset = vk::Offset2D{ (int32)scissor.x, (int32)scissor.y };

        vkCheckErrorVoid(commandBuffer.setScissor(0, 1, &scissor_));
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::SetScissor(float32 x, float32 y, float32 width, float32 height)
    {
        if (m_Context == nullptr) {
            return false;
        }

        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();

        vk::Rect2D scissor = {};
        scissor.extent = vk::Extent2D{ (uint32)width, (uint32)height };
        scissor.offset = vk::Offset2D{ (int32)x, (int32)y };
        vkCheckErrorVoid(commandBuffer.setScissor(0, 1, &scissor));
        return VulkanStaticDebugInfo::Result();
    }

    bool VulkanRenderWindow::ResetScissor()
    {
        if (m_Context == nullptr) {
            return false;
        }
        maths::Vector2f size = m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize());
        return SetScissor(0, 0, size.width, size.height);
    }

    bool VulkanRenderWindow::SetPolygonMode(PolygonModeType mode)
    {
        if (m_Context == nullptr) {
            return false;
        }
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();
        if (commandBuffer == nullptr) return false;

        if (m_Context->cmdSetPolygonModeEXT != nullptr) {
            vkCheckErrorVoid(m_Context->cmdSetPolygonModeEXT((VkCommandBuffer)commandBuffer, (VkPolygonMode)VulkanConvert::ToPolygonModeType(mode)));
            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("cannot set polygon mode");
                return false;
            }
            return true;
        }
        return false;
    }

    bool VulkanRenderWindow::SetCullMode(CullModeType mode)
    {
        if (m_Context == nullptr) {
            return false;
        }
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();
        if (commandBuffer == nullptr) return false;

        vkCheckErrorVoid(commandBuffer.setCullMode(VulkanConvert::ToCullModeType(mode)));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot set cull mode");
            return false;
        }
        return true;
    }

    bool VulkanRenderWindow::SetFrontFaceMode(FrontFaceType mode)
    {
        if (m_Context == nullptr) {
            return false;
        }
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();
        if (commandBuffer == nullptr) return false;

        vkCheckErrorVoid(commandBuffer.setFrontFace(VulkanConvert::ToFrontFaceType(mode)));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot set front face");
            return false;
        }
        return true;
    }

    bool VulkanRenderWindow::SetRenderPrimitive(RenderPrimitive mode)
    {
        if (m_Context == nullptr) {
            return false;
        }
        vk::CommandBuffer commandBuffer = m_Context->GetCommandBuffer();
        if (commandBuffer == nullptr) return false;

        vkCheckErrorVoid(commandBuffer.setPrimitiveTopology(VulkanConvert::GetPrimitiveType(mode)));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("cannot set primitive topology");
            return false;
        }
        return true;
    }

    bool VulkanRenderWindow::Recreate(bool force)
    {
        if (m_Context == nullptr) return false;

        if (!CreateDepth(m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize()))) {
            Log_nts.Error("Cannot create frame buffer");
            return false;
        }

        if (!CreateFrameBuffer(m_Context->GetWindow()->ConvertPixelToDpi(m_Context->GetWindow()->GetSize()))) {
            Log_nts.Error("Cannot create frame buffer");
            return false;
        }

        return true;
    }

    bool VulkanRenderWindow::CleanUp(bool force)
    {
        if (m_Context == nullptr) {
            return false;
        }
        if (!DestroyFrameBuffer()) {
            Log_nts.Error("Cannot destroy frame buffer");
            return false;
        }

        if (!DestroyDepth()) {
            Log_nts.Error("Cannot destroy depth image in frame buffer");
        }
        return true;
    }

    bool VulkanRenderWindow::CreateFrameBuffer(const maths::Vector2u& size)
    {
        if (m_Context == nullptr || m_Context->renderPass == nullptr || m_Context->device == nullptr) return false;

        VulkanResult result;
        bool first = true;

        this->size = size;

        framebuffer.resize(m_Context->swapchainImages.size());

        std::vector<vk::ImageView> attachments = {};
        attachments.resize(2);
        attachments[1] = depthImageView;

        vk::FramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.width = this->size.width;
        framebufferInfo.height = this->size.height;
        framebufferInfo.renderPass = m_Context->renderPass;
        framebufferInfo.layers = 1;
        framebufferInfo.attachmentCount = attachments.size();

        for (uint32 index = 0; index < m_Context->swapchainImages.size(); index++) {
            attachments[0] = m_Context->imageView[index];
            framebufferInfo.pAttachments = attachments.data();

            vkCheckError(first, result, m_Context->device.createFramebuffer(&framebufferInfo, m_Context->allocator, &framebuffer[index]), "cannot create frame buffer index {0}", index);
        }

        if (result.success) {
            Log_nts.Info("Create framebuffer is good");
        }

        return result.success;
    }

    bool VulkanRenderWindow::DestroyFrameBuffer()
    {
        if (m_Context == nullptr || m_Context->device == nullptr) return false;

        for (usize i = 0; i < framebuffer.size(); i++) {
            vkCheckErrorVoid(m_Context->device.destroyFramebuffer(framebuffer[i], m_Context->allocator));
            framebuffer[i] = VK_NULL_HANDLE;

            if (!VulkanStaticDebugInfo::Result()) {
                Log_nts.Error("cannot destroy frame buffer index {0}", i);
            }
        }

        framebuffer.clear();
        size = maths::Vector2u();
        return true;
    }

    bool VulkanRenderWindow::CreateDepth(const maths::Vector2u& size, vk::Format format, vk::SampleCountFlagBits samples)
    {
        this->format = format;
        this->size = size;
        this->mipLevels = 1;
        this->layout = vk::ImageLayout::eUndefined;

        vk::ImageUsageFlags usage;
        vk::ImageAspectFlags aspectMask;

        usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        aspectMask = vk::ImageAspectFlagBits::eDepth;
        vk::Format format_ind;

        VulkanContext::FindDepthFormat(m_Context->physicalDevice, &format_ind);

        if (format_ind != this->format) {
            this->format = format_ind;
        }

        CreateImage(samples, usage, aspectMask);
        CreateMemory();
        CreateImageView(aspectMask);

        //TransitionImageLayout(gpu, pool->commandPool, gpu->queue.graphicsQueue, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        return true;
    }

    bool VulkanRenderWindow::DestroyDepth()
    {
        bool success = true;
        vkCheckErrorVoid(m_Context->device.destroyImageView(depthImageView, m_Context->allocator));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to destroy frame buffer epth image view");
            success = false;
        }
        vkCheckErrorVoid(m_Context->device.destroyImage(depthImage, m_Context->allocator));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to destroy frame buffer image");
            success = false;
        }
        vkCheckErrorVoid(m_Context->device.freeMemory(depthMemory, m_Context->allocator));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to free frame buffer memory");
            success = false;
        }
        return success;
    }

    bool VulkanRenderWindow::CreateImage(vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage, vk::ImageAspectFlags aspectMask)
    {
        VulkanResult result;
        bool first = true;

        vk::ImageCreateInfo imageInfo{};
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = size.width;
        imageInfo.extent.height = size.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = vk::ImageTiling::eOptimal;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = usage;
        imageInfo.samples = samples;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;

        vkCheckError(first, result, m_Context->device.createImage(&imageInfo, m_Context->allocator, &depthImage), "failed to create image depth!");
        return result.success;
    }

    bool VulkanRenderWindow::CreateMemory()
    {
        VulkanResult result;
        bool first = true;

        vk::MemoryRequirements memRequirements;
        vkCheckErrorVoid(m_Context->device.getImageMemoryRequirements(depthImage, &memRequirements));

        vk::MemoryAllocateInfo allocInfo{};
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VulkanContext::FindMemoryType(m_Context->physicalDevice, memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        vkCheckError(first, result, m_Context->device.allocateMemory(&allocInfo, m_Context->allocator, &depthMemory), "failed to allocate image memory!");

        vkCheckErrorVoid(m_Context->device.bindImageMemory(depthImage, depthMemory, 0));
        if (!VulkanStaticDebugInfo::Result()) {
            Log_nts.Error("failed to bind image memory");
            return false;
        }
        return result.success;
    }

    bool VulkanRenderWindow::CreateImageView(vk::ImageAspectFlags aspectMask)
    {
        VulkanResult result;
        bool first = true;

        vk::ImageViewCreateInfo viewInfo{};
        viewInfo.image = depthImage;
        viewInfo.viewType = vk::ImageViewType::e2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectMask;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        vkCheckError(first, result, m_Context->device.createImageView(&viewInfo, m_Context->allocator, &depthImageView), "failed to create image view!");
        return result.success;
    }

}  //  nkentseu