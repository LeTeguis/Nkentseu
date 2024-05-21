//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 08:06:06 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __PUSH_CONSTANT_H__
#define __PUSH_CONSTANT_H__

#pragma once

#include <System/System.h>
#include <System/Definitions/Memory.h>
#include "ShaderInfo.h"

namespace nkentseu {
    class InternalPushConstant;
    
    class NKENTSEU_API PushConstant {
        public:
            PushConstant();
            ~PushConstant();

            bool Create(const PushConstantLayout& pushConstantLayout);
            bool Destroy();

            std::string ToString() const;
            friend std::string ToString(const PushConstant& pushConstant);
        private:
            Memory::Shared<InternalPushConstant> m_InternalPushConstant = null_pointer;
    };

}  //  nkentseu

#endif  // __PUSH_CONSTANT_H__!