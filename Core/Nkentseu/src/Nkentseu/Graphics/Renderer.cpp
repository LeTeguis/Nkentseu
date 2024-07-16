//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:12:38 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Renderer.h"

#include <Nkentseu/Platform/Internal/InternalMemory.h>
#include <Nkentseu/Core/NkentseuLogger.h>

#include "Internal/Vulkan/VulkanRenderer.h"
#include "Internal/Opengl/OpenglRenderer.h"

#include "Shader.h"
#include "VertexArray.h"

namespace nkentseu {
/*
#define NKENTSEU_CREATE_RENDERER(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
													return Memory::Alloc<class_>(context);	\
												}

#define NKENTSEU_CREATE_RENDERER_INTIALIZE(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
																auto data = Memory::Alloc<class_>(context);	\
																if (data != nullptr && data->Initialize()) {	\
																	return data;	\
																}	\
																Memory::Reset(data);	\
															}
*/
	Memory::Shared<Renderer> Renderer::Create(Memory::Shared<Context> context)
	{
		if (context == nullptr) {
			return nullptr;
		}

		//NKENTSEU_CREATE_RENDERER(GraphicsApiType::VulkanApi, VulkanRenderer);
		//NKENTSEU_CREATE_RENDERER(GraphicsApiType::OpenglApi, OpenglRenderer);
		return nullptr;
	}

	Memory::Shared<Renderer> Renderer::CreateInitialized(Memory::Shared<Context> context)
	{
		if (context == nullptr) {
			return nullptr;
		}
		//NKENTSEU_CREATE_RENDERER_INTIALIZE(GraphicsApiType::VulkanApi, VulkanRenderer);
		//NKENTSEU_CREATE_RENDERER_INTIALIZE(GraphicsApiType::OpenglApi, OpenglRenderer);
		return nullptr;
	}
}    // namespace nkentseu