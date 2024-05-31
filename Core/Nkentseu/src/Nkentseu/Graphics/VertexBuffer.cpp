//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:46:43 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VertexBuffer.h"
#include <Logger/Formatter.h>

#include "Internal/Vulkan/VulkanVertexBuffer.h"
#include "Internal/Opengl/OpenglVertexBuffer.h"

#include <Nkentseu/Core/NkentseuLogger.h>
#include "Context.h"

namespace nkentseu {
#define NKENTSEU_CREATE_VERTEX_BUFFER_P1(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(context);	\
															if (data != nullptr && data->Create(bufferUsage, vertices, leng, bufferLayout)) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}
#define NKENTSEU_CREATE_VERTEX_BUFFER_P2(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(context);	\
															if (data != nullptr && data->Create(bufferUsage, vertices, bufferLayout)) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}

	Memory::Shared<VertexBuffer> VertexBuffer::Create(Memory::Shared<Context> context, BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng, const BufferLayout& bufferLayout)
    {
		if (context == nullptr) {
			return nullptr;
		}

		NKENTSEU_CREATE_VERTEX_BUFFER_P1(GraphicsApiType::VulkanApi, VulkanVertexBuffer);
		NKENTSEU_CREATE_VERTEX_BUFFER_P1(GraphicsApiType::OpenglApi, OpenglVertexBuffer);
		return nullptr;
    }

	Memory::Shared<VertexBuffer> VertexBuffer::Create(Memory::Shared<Context> context, BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices, const BufferLayout& bufferLayout)
    {
		if (context == nullptr) {
			return nullptr;
		}

		NKENTSEU_CREATE_VERTEX_BUFFER_P2(GraphicsApiType::VulkanApi, VulkanVertexBuffer);
		NKENTSEU_CREATE_VERTEX_BUFFER_P2(GraphicsApiType::OpenglApi, OpenglVertexBuffer);

		return nullptr;
    }
}  //  nkentseu