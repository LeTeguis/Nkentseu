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

	Memory::Shared<UniformBuffer> UniformBuffer::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil)
	{
		if (context == nullptr) {
			return nullptr;
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			return Memory::Alloc<VulkanUniformBuffer>(context, sil);
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			return Memory::Alloc<OpenglUniformBuffer>(context, sil);
		}

		return nullptr;
	}

	Memory::Shared<UniformBuffer> UniformBuffer::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, Memory::Shared<Shader> shader, const std::vector<std::string> uniformsLoader)
	{
		auto uniform = Create(context, sil);

		if (shader == nullptr) {
			Log_nts.Fatal();
		}

		if (uniform == nullptr || !uniform->Create(shader, uniformsLoader)) {

			if (uniform != nullptr) {
				uniform->Destroy();
			}

			Memory::Reset(uniform);
			return nullptr;
		}

		return uniform;
	}

}  //  nkentseu