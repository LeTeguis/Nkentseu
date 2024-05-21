//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-05-19 at 08:06:42 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_INTERNAL_PUSH_CONSTANT_H__
#define __OPENGL_INTERNAL_PUSH_CONSTANT_H__

#pragma once

#include <System/System.h>

namespace nkentseu {
    
    class NKENTSEU_API InternalPushConstant {
        public:
            InternalPushConstant();
            ~InternalPushConstant();

            std::string ToString() const;
            friend std::string ToString(const InternalPushConstant& internalPushConstant);
        private:
        protected:
    };

}  //  nkentseu

#endif  // __INTERNAL_PUSH_CONSTANT_H__!