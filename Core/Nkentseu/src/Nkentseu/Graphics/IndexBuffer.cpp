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

	Memory::Shared<IndexBuffer> IndexBuffer::Create(Memory::Shared<Context> context)
	{
		if (context == nullptr) return nullptr;
		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			return Memory::Alloc<VulkanIndexBuffer>(context);
		}
		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			return Memory::Alloc<OpenglIndexBuffer>(context);
		}
		return nullptr;
	}

	Memory::Shared<IndexBuffer> IndexBuffer::Create(Memory::Shared<Context> context, BufferUsageType bufferUsage, const std::vector<uint32>& indices)
    {
		auto index = Create(context);
		if (index == nullptr || !index->Create(bufferUsage, indices)) {
			Memory::Reset(index);
		}
		return index;
    }

    Memory::Shared<IndexBuffer> IndexBuffer::Create(Memory::Shared<Context> context, BufferUsageType bufferUsage, IndexBufferType indexType, const void* indices, uint32 leng)
    {
		auto index = Create(context);
		if (index == nullptr || !index->Create(bufferUsage, indexType, indices, leng)) {
			Memory::Reset(index);
		}
		return index;
    }

}  //  nkentseu