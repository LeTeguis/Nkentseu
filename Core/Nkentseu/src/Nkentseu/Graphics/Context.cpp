//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/3/2024 at 12:48:26 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Context.h"

#include <Nkentseu/Platform/Internal/InternalMemory.h>
#include <Nkentseu/Core/NkentseuLogger.h>

#include "Internal/Vulkan/VulkanContext.h"
#include "Internal/Opengl/OpenglContext.h"

namespace nkentseu {

#define NKENTSEU_CREATE_CONTEXT(api_, class_)	if (contextProperties.graphicsApi == api_) {	\
													return Memory::Alloc<class_>(window, contextProperties);	\
												}

#define NKENTSEU_CREATE_CONTEXT_INTIALIZE(api_, class_)	if (contextProperties.graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(window, contextProperties);	\
															if (data != nullptr && data->Initialize()) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}

	Memory::Shared<Context> Context::Create(Memory::Shared<Window> window, const ContextProperties& contextProperties)
	{
		NKENTSEU_CREATE_CONTEXT(GraphicsApiType::VulkanApi, VulkanContext);
		NKENTSEU_CREATE_CONTEXT(GraphicsApiType::OpenglApi, OpenglContext);
		return nullptr;
	}

	Memory::Shared<Context> Context::CreateInitialized(Memory::Shared<Window> window, const ContextProperties& contextProperties)
	{
		NKENTSEU_CREATE_CONTEXT_INTIALIZE(GraphicsApiType::VulkanApi, VulkanContext);
		NKENTSEU_CREATE_CONTEXT_INTIALIZE(GraphicsApiType::OpenglApi, OpenglContext);
		return nullptr;
	}
}    // namespace nkentseu