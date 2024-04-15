//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:24:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_NKENTSEU_ERROR_H__
#define __NKENTSEU_NKENTSEU_ERROR_H__

#pragma once

#include "System/System.h"
#include "System/Nature/Base.h"
#include <vector>


namespace nkentseu {
    enum class NKENTSEU_API NTSErrorCode : bool32 {
        Error_No = 0x0000,
        Window_CreateNative = 0x0001,
        Window_RegisterWindowClass = 0x0002,
        Window_StayInWindowMode = 0x0003,
        Window_Create = 0x0004,
        Window_ExternalAssign = 0x0005
    };

    enum class NKENTSEU_API NTSErrorSeverity : bool32{
        Error_No = 0x0000
    };

    struct NKENTSEU_API NTSErrorInfo {
        NTSErrorCode code;
        NTSErrorSeverity gravity;
        std::string description;

        friend std::string ToString(const NTSErrorInfo& code);
    };

    class NKENTSEU_API NTSErrorMessenger {
    public:
        static NTSErrorMessenger& Instance() {
            static NTSErrorMessenger errorMessaging;
            return errorMessaging;
        }

        void PushError(NTSErrorCode Message);
        NTSErrorCode PopError();
        NTSErrorCode GetError();

        NTSErrorInfo GetErrorInfos(NTSErrorCode Message);

    private:
        std::vector<NTSErrorCode> m_Messages;

        bool HasErrorInfos(NTSErrorCode error, NTSErrorInfo* infos);
    };

#define ErrorMessaging     NTSErrorMessenger::Instance()
} // namespace nkentseu

#endif    // __NKENTSEU_NKENTSEU_ERROR_H__