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

	Memory::Shared<VertexBuffer> VertexBuffer::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil) {
		if (context == nullptr) {
			return nullptr;
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			return Memory::Alloc<VulkanVertexBuffer>(context, sil);
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			return Memory::Alloc<OpenglVertexBuffer>(context, sil);
		}

		return nullptr;
	}

	Memory::Shared<VertexBuffer> VertexBuffer::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, BufferDataUsage::Code bufferUsage, const void* vertices, uint32 leng)
    {
		auto vertexbuffer = Create(context, sil);

		if (vertexbuffer == nullptr || !vertexbuffer->Create(bufferUsage, vertices, leng)) {
			Memory::Reset(vertexbuffer);
			return nullptr;
		}

		return vertexbuffer;
    }

	Memory::Shared<VertexBuffer> VertexBuffer::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, BufferDataUsage::Code bufferUsage, const std::vector<float32>& vertices)
    {
		auto vertexbuffer = Create(context, sil);

		if (vertexbuffer == nullptr || !vertexbuffer->Create(bufferUsage, vertices)) {
			Memory::Reset(vertexbuffer);
			return nullptr;
		}

		return vertexbuffer;
    }
}  //  nkentseu