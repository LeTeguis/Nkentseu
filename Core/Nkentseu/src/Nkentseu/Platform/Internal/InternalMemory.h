//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 5/5/2024 at 3:27:33 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_INTERNALMEMORY_H__
#define __NKENTSEU_INTERNALMEMORY_H__

#pragma once

#include "System/System.h"

namespace nkentseu {
#define INTERNAL_INFO_INITIALIZATION(InternalInfo, ... )	if (m_##InternalInfo##Info != nullptr) return false; \
                                                            m_##InternalInfo##Info = Memory::Alloc<InternalInfo##Info>(); \
                                                            if (m_##InternalInfo##Info != nullptr) { \
                                                                m_##InternalInfo##Info->isLoad = false; \
                                                                m_##InternalInfo##Info->internalInfo = Memory::Alloc<InternalInfo>(); \
                                                                if (m_##InternalInfo##Info->internalInfo != nullptr) { \
                                                                    m_##InternalInfo##Info->isLoad = m_##InternalInfo##Info->internalInfo->Initialize(__VA_ARGS__); \
                                                                } \
                                                                else { \
                                                                    return false; \
                                                                } \
                                                                if (!m_##InternalInfo##Info->isLoad) { \
                                                                    Memory::Reset(m_##InternalInfo##Info->internalInfo); \
                                                                    Memory::Reset(m_##InternalInfo##Info); \
                                                                } \
                                                            } \
                                                            else { \
                                                                return false; \
                                                            } \
                                                            return m_##InternalInfo##Info->isLoad;

#define INTERNAL_INFO_CALL_METHODE(internalInfo, internalData, ret, vret, methode, ... )	    if (internalInfo == nullptr || internalInfo->internalData == nullptr) { \
																			                        return vret; \
																		                        } \
																		                        if (ret) { \
																			                        return internalInfo->internalData->methode(__VA_ARGS__); \
																		                        } else { \
																			                        internalInfo->internalData->methode(__VA_ARGS__); \
																		                        }
} // namespace nkentseu

#endif    // __NKENTSEU_INTERNALMEMORY_H__