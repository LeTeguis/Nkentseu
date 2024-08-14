//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 2024-06-18 at 07:55:06 AM AM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __INPUT_STREAM_H__
#define __INPUT_STREAM_H__

#pragma once

#include "NTSCore/Platform/Exports.h"
#include "NTSCore/Nature/Base.h"

namespace nkentseu {

    class NKENTSEU_API InputStream {
    public:
        virtual ~InputStream() = default;
        virtual int64 Read(void* data, int64 size) = 0;
        virtual int64 Seek(int64 position) = 0;
        virtual int64 Tell() = 0;
        virtual int64 GetSize() = 0;
    };

}  //  nkentseu

#endif  // __INPUT_STREAM_H__!