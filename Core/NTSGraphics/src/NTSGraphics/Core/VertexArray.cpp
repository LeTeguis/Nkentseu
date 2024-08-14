//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "VertexArray.h"

#include "NTSGraphics/Api/Opengl/OpenglVertexArray.h"
#include "NTSGraphics/Api/Vulkan/VulkanVertexArray.h"
#include "Context.h"

namespace nkentseu {

	Memory::Shared<VertexArray> VertexArray::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil)
    {
		auto vertexbuffer = CreatePrivate(context, sil);

		if (vertexbuffer == nullptr || !vertexbuffer->Create()) {
			Memory::Reset(vertexbuffer);
			return nullptr;
		}

		return vertexbuffer;
    }

	Memory::Shared<VertexArray> VertexArray::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, uint32 vertexNumber)
    {
		auto vertexbuffer = CreatePrivate(context, sil);

		if (vertexbuffer == nullptr || !vertexbuffer->Create(vertexNumber)) {
			Memory::Reset(vertexbuffer);
			return nullptr;
		}

		return vertexbuffer;
    }

	Memory::Shared<VertexArray> VertexArray::CreatePrivate(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil)
	{
		if (context == nullptr) {
			return nullptr;
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::Enum::VulkanApi) {
			return Memory::AllocateShared<VulkanVertexArray>(context, sil);
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::Enum::OpenglApi) {
			return Memory::AllocateShared<OpenglVertexArray>(context, sil);
		}

		return nullptr;
	}

}  //  nkentseu