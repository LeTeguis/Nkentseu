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
#define NKENTSEU_CREATE_UNIFORM_BUFFER(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
																auto data = Memory::Alloc<class_>(context, shader, uniformLayout);	\
																if (data != nullptr && data->Create()) {	\
																	return data;	\
																}	\
																Memory::Reset(data);	\
															}

	Memory::Shared<UniformBuffer> UniformBuffer::Create(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout)
	{
		if (context == nullptr || shader == nullptr) {
			return nullptr;
		}

		NKENTSEU_CREATE_UNIFORM_BUFFER(GraphicsApiType::VulkanApi, VulkanUniformBuffer);
		NKENTSEU_CREATE_UNIFORM_BUFFER(GraphicsApiType::OpenglApi, OpenglUniformBuffer);
		return nullptr;
	}
}  //  nkentseu