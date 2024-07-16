//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-08 at 05:28:16 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "RenderWindow.h"
#include <Logger/Formatter.h>
#include "Internal/Vulkan/VulkanRenderWindow.h"
#include "Internal/Opengl/OpenglRenderWindow.h"

namespace nkentseu {

    Memory::Shared<RenderWindow> RenderWindow::Create(Memory::Shared<Context> context)
    {
        if (context == nullptr) {
            return nullptr;
        }

        if (context->GetProperties().graphicsApi == GraphicsApiType::VulkanApi) {
            return Memory::Alloc<VulkanRenderWindow>(context);
        }

        if (context->GetProperties().graphicsApi == GraphicsApiType::OpenglApi) {
            return Memory::Alloc<OpenglRenderWindow>(context);
        }

        return nullptr;
    }

    Memory::Shared<RenderWindow> RenderWindow::CreateInitialized(Memory::Shared<Context> context)
    {
        auto renderWindow = Create(context);

        if (renderWindow == nullptr || !renderWindow->Initialize()) {
            Memory::Reset(renderWindow);
        }

        return renderWindow;
    }

}  //  nkentseu