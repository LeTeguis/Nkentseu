//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 8:55:16 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "LoggerPch/ntspch.h"
#include "Assert.h"

namespace nkentseu {

	MAssert& MAssert::Instance()
	{
		static MAssert mLog;
		return mLog;
	}

	MAssert::MAssert() : m_Assertion("Assert") {
	}

}    // namespace nkentseu