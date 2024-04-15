//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:18:31 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NkentseuPch/ntspch.h"
#include "NkentseuLogger.h"

namespace nkentseu {

	NkentseuTrace& NkentseuTrace::Instance()
	{
		static NkentseuTrace mLog;
		return mLog;
	}

	NkentseuTrace::NkentseuTrace() : m_Logger(std::make_shared<Logger>("Nkentseu")), m_Assertion(std::make_shared<Assertion>("Nkentseu")) {
	}

}    // namespace nkentseu