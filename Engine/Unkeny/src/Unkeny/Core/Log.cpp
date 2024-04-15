//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 1:29:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "UnkenyPch/ntspch.h"
#include "Log.h"

namespace nkentseu {

	UnkenyLogger& UnkenyLogger::Instance()
	{
		static UnkenyLogger mLog;
		return mLog;
	}

	UnkenyLogger::UnkenyLogger() : m_Logger(std::make_shared<Logger>("Unkeny")), m_Assertion(std::make_shared<Assertion>("Unkeny")) {
	}

}    // namespace nkentseu