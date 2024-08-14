//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:23:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Log.h"

namespace nkentseu {
	LoggerBase& LoggerBase::Instance()
	{
		static LoggerBase mLog;
		return mLog;
	}

	LoggerBase::LoggerBase() : m_Logger(std::make_shared<Logger>("Log")) {
	}
} // namespace nkentseu