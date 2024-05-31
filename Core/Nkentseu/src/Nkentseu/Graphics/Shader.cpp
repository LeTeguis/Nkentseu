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

#include "Context.h"

namespace nkentseu {
#define NKENTSEU_CREATE_SHADER(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
													auto data = Memory::Alloc<class_>(context);	\
													if (data != nullptr && data->LoadFromFile(shaderFiles, shaderLayout)) {	\
														return data;	\
													}	\
													Memory::Reset(data);	\
												}

    Memory::Shared<Shader> Shader::Create(Memory::Shared<Context> context, const std::unordered_map<ShaderType::Code, std::string>& shaderFiles, const ShaderBufferLayout& shaderLayout)
    {
		if (context == nullptr) {
			return nullptr;
		}

		NKENTSEU_CREATE_SHADER(GraphicsApiType::VulkanApi, VulkanShader);
		NKENTSEU_CREATE_SHADER(GraphicsApiType::OpenglApi, OpenglShader);
		return nullptr;
    }
}  //  nkentseu