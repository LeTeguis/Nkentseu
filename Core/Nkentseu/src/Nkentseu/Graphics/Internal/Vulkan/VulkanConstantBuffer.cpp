//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-22 at 06:39:06 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "VulkanConstantBuffer.h"
#include <Logger/Formatter.h>

namespace nkentseu {

    VulkanConstantBuffer::VulkanConstantBuffer(Memory::Shared<Context> context) : m_Context(Memory::SharedCast<VulkanContext>(context))
    {
    }

    VulkanConstantBuffer::~VulkanConstantBuffer() {
    }

}  //  nkentseu