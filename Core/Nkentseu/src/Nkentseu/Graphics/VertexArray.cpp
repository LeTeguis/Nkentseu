//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-16 at 10:10:05 AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VertexArray.h"
#include <Logger/Formatter.h>

#include "Internal/Opengl/OpenglVertexArray.h"
#include "Internal/Vulkan/VulkanVertexArray.h"

#include <Nkentseu/Core/NkentseuLogger.h>
#include "Context.h"

namespace nkentseu {
#define NKENTSEU_CREATE_VERTEX_ARRAY_P1(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(context, sil);	\
															if (data != nullptr && data->Create()) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}
#define NKENTSEU_CREATE_VERTEX_ARRAY_P2(api_, class_)	if (context->GetProperties().graphicsApi == api_) {	\
															auto data = Memory::Alloc<class_>(context, sil);	\
															if (data != nullptr && data->Create(vertexNumber)) {	\
																return data;	\
															}	\
															Memory::Reset(data);	\
														}

	Memory::Shared<VertexArray> VertexArray::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil)
    {
		if (context == nullptr) {
			return nullptr;
		}

		NKENTSEU_CREATE_VERTEX_ARRAY_P1(GraphicsApiType::VulkanApi, VulkanVertexArray);
		NKENTSEU_CREATE_VERTEX_ARRAY_P1(GraphicsApiType::OpenglApi, OpenglVertexArray);

		return nullptr;
    }

	Memory::Shared<VertexArray> VertexArray::Create(Memory::Shared<Context> context, Memory::Shared<ShaderInputLayout> sil, uint32 vertexNumber)
    {
		if (context == nullptr) {
			return nullptr;
		}

		NKENTSEU_CREATE_VERTEX_ARRAY_P2(GraphicsApiType::VulkanApi, VulkanVertexArray);
		NKENTSEU_CREATE_VERTEX_ARRAY_P2(GraphicsApiType::OpenglApi, OpenglVertexArray);

		return nullptr;
    }
}  //  nkentseu