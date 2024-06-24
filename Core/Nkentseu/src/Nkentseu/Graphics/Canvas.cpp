//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-21 at 05:50:37 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "Canvas.h"
#include <Logger/Formatter.h>

#include "Internal/Opengl/OpenglCanvas.h"
#include "Internal/Vulkan/VulkanCanvas.h"

namespace nkentseu {

	Memory::Shared<Canvas> Canvas::Create(const Memory::Shared<Context>& context)
	{
		if (context == nullptr) return nullptr;

		Memory::Shared<Canvas> canvas = nullptr;

		if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			canvas = Memory::Alloc<OpenglCanvas>(context);
		} else if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
			canvas = Memory::Alloc<VulkanCanvas>(context);
		}

		return canvas;
	}

}  //  nkentseu