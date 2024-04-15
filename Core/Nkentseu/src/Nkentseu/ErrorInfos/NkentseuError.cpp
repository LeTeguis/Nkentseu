//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:24:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "NkentseuError.h"
#include <Logger/Formatter.h>

namespace nkentseu {
    std::string ToString(const NTSErrorInfo& info) {
        return FORMATTER.Format("code = {0}, gravity = {1}, description = {2}", (bool32)info.code, (bool32)info.gravity, info.description);
    }

    void NTSErrorMessenger::PushError(NTSErrorCode Message) {
        m_Messages.push_back(Message);
    }

    NTSErrorCode NTSErrorMessenger::PopError() {
        if (m_Messages.size() != 0) {
            NTSErrorCode error = m_Messages[m_Messages.size() - 1];

            m_Messages.erase(m_Messages.end() - 1);
            return error;
        }
        return NTSErrorCode::Error_No;
    }

    NTSErrorCode NTSErrorMessenger::GetError() {
        if (m_Messages.size() != 0) {
            NTSErrorCode error = m_Messages[m_Messages.size() - 1];
            return error;
        }
        return NTSErrorCode::Error_No;
    }

    NTSErrorInfo NTSErrorMessenger::GetErrorInfos(NTSErrorCode Message) {
        NTSErrorInfo infos = {};
        if (HasErrorInfos((NTSErrorCode)Message, &infos)) {
            return infos;
        }
        return infos;
    }

    bool NTSErrorMessenger::HasErrorInfos(NTSErrorCode error, NTSErrorInfo* infos) {
        return false;
    }

}    // namespace nkentseu