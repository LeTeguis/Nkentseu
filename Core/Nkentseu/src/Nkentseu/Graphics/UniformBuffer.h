//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-06-03 at 08:32:23 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __UNIFORM_BUFFER_H__
#define __UNIFORM_BUFFER_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include "ShaderInfo.h"
#include "Context.h"
#include "Shader.h"

namespace nkentseu {
    
    class NKENTSEU_API UniformBuffer {
        public:
            virtual Memory::Shared<Context> GetContext() = 0;
            virtual bool Bind(const std::string& name, void* data, usize size) = 0;
            virtual bool Bind(const std::string& name, void* data, usize size, usize offset) = 0;
            //virtual bool Bind(const std::string& name, void* data, usize size, uint32 instance) = 0;
            virtual bool Destroy() = 0;

            static Memory::Shared<UniformBuffer> Create(Memory::Shared<Context> context, Memory::Shared<Shader> shader, const UniformBufferLayout& uniformLayout);
    };

}  //  nkentseu

#endif  // __UNIFORM_BUFFER_H__!