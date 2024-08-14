//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-07-26 at 09:09:51 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Convert.h"
#include <NTSWindow/Platform/Window/WindowData.h>

namespace nkentseu {

    Memory::Shared<nkentseu::WindowInfo> Convert::ConvertWindowInfoFrom(Memory::Shared<nkentseu::Window> window)
    {
        if (window == nullptr) return nullptr;

        Memory::Shared<nkentseu::WindowInfo> windowInfos = Memory::AllocateShared<nkentseu::WindowInfo>();

        windowInfos->title = window->GetTitle();
        windowInfos->appName = window->GetTitle();
        windowInfos->engineName = window->GetProperties().engineName;

#ifdef NKENTSEU_PLATFORM_WINDOWS
        windowInfos->windowHandle = window->GetData()->windowHandle;
        windowInfos->instance = window->GetData()->instanceHandle;
#endif
        return windowInfos;
    }

}  //  nkentseu