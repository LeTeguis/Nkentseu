//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 10:46:54 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "IndexBuffer.h"
#include <Logger/Formatter.h>

#include "Internal/Opengl/OpenglIndexBuffer.h"
#include "Internal/Vulkan/VulkanIndexBuffer.h"

#include <Nkentseu/Core/NkentseuLogger.h>
#include "Context.h"

namespace nkentseu {
#define NKENTSEU_CREATE_INDEX_BUFFER_P1(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(context);	\
															if (data != nullptr && data->Create(bufferUsage, indices)) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}
#define NKENTSEU_CREATE_INDEX_BUFFER_P2(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(context);	\
															if (data != nullptr && data->Create(bufferUsage, indexType, indices, leng)) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}

    Memory::Shared<IndexBuffer> IndexBuffer::Create(Memory::Shared<Context> context, BufferDataUsage::Code bufferUsage, const std::vector<uint32>& indices)
    {
		if (context == nullptr) {
			return nullptr;
		}
		NKENTSEU_CREATE_INDEX_BUFFER_P1(GraphicsApiType::VulkanApi, VulkanIndexBuffer);
		NKENTSEU_CREATE_INDEX_BUFFER_P1(GraphicsApiType::OpenglApi, OpenglIndexBuffer);
		return nullptr;
    }

    Memory::Shared<IndexBuffer> IndexBuffer::Create(Memory::Shared<Context> context, BufferDataUsage::Code bufferUsage, DrawIndexType::Code indexType, const void* indices, uint32 leng)
    {
		if (context == nullptr) {
			return nullptr;
		}
		NKENTSEU_CREATE_INDEX_BUFFER_P2(GraphicsApiType::VulkanApi, VulkanIndexBuffer);
		NKENTSEU_CREATE_INDEX_BUFFER_P2(GraphicsApiType::OpenglApi, OpenglIndexBuffer);
		return nullptr;
    }

}  //  nkentseu