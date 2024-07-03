//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:53:53 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Shader.h"

#include <Logger/Formatter.h>

#include <Nkentseu/Core/NkentseuLogger.h>

#include "Internal/Vulkan/VulkanShader.h"
#include "Internal/Opengl/OpenglShader.h"

//#include "Context.h"
//#include "ShaderInputLayout.h"

namespace nkentseu {

	Memory::Shared<Shader> Shader::Create(Memory::Shared<Context> context)
	{
		if (context == nullptr) {
			return nullptr;
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
			return Memory::Alloc<VulkanShader>(context);
		}

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			return Memory::Alloc<OpenglShader>(context);
		}

		return nullptr;
	}


	Memory::Shared<Shader> Shader::Create(Memory::Shared<Context> context, const ShaderFilePathLayout& shaderFiles, Memory::Shared<ShaderInputLayout> shaderInputLayout)
	{
		auto shader = Create(context);

		if (shader == nullptr || !shader->LoadFromFile(shaderFiles, shaderInputLayout)) {
			Memory::Reset(shader);
		}

		return shader;
	}
}  //  nkentseu