//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/8/2024 at 5:23:57 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "LoggerPch/ntspch.h"
#include "Log.h"

namespace nkentseu {
	MLog& MLog::Instance()
	{
		static MLog mLog;
		return mLog;
	}

	MLog::MLog() : m_Logger(std::make_shared<Logger>("Log")) {
	}
} // namespace nkentseu