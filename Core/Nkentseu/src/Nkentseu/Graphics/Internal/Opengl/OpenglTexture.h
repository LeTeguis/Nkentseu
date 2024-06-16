//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-16 at 11:23:31 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __OPENGL_TEXTURE_H__
#define __OPENGL_TEXTURE_H__

#pragma once

#include <System/System.h>

namespace nkentseu {
    
    class NKENTSEU_API OpenglTexture {
        public:
            OpenglTexture();
            ~OpenglTexture();

            std::string ToString() const;
            friend std::string ToString(const OpenglTexture& openglTexture);
        private:
        protected:
    };

}  //  nkentseu

#endif  // __OPENGL_TEXTURE_H__!