//
// Created by TEUGUIA TADJUIDJE Rodolf S�deris on 2024-05-15 at 04:57:08 PM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INTERNAL_SHADER_H__
#define __INTERNAL_SHADER_H__

#pragma once

#include <System/System.h>

#ifdef NKENTSEU_GRAPHICS_API_DIRECTX11

#include <System/Definitions/Memory.h>

namespace nkentseu {
    
    class NKENTSEU_API InternalShader {
        public:
            InternalShader();
            ~InternalShader();

            std::string ToString() const;
            friend std::string ToString(const InternalShader& internalShader);
        private:
        protected:
    };

}  //  nkentseu

#endif  // __INTERNAL_SHADER_H__!

#endif