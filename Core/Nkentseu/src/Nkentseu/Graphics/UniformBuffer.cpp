//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-03 at 08:32:23 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "UniformBuffer.h"
#include <Logger/Formatter.h>

#include "Internal/Vulkan/VulkanUniformBuffer.h"
#include "Internal/Opengl/OpenglUniformBuffer.h"

namespace nkentseu {

	Memory::Shared<UniformBuffer> UniformBuffer::Create(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout)
	{
		if (context == nullptr || shader == nullptr) {
			return nullptr;
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			auto uniform = Memory::Alloc<VulkanUniformBuffer>(context, shader, uniformLayout);
			if (uniform != nullptr && uniform->Create()) return uniform;
			Memory::Reset(uniform);
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			auto uniform = Memory::Alloc<OpenglUniformBuffer>(context, shader, uniformLayout);
			if (uniform != nullptr && uniform->Create()) return uniform;
			Memory::Reset(uniform);
		}
		return nullptr;
	}
}  //  nkentseu