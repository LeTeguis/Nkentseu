//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 8:55:16 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "Assert.h"

namespace nkentseu {

	AssertionBase& AssertionBase::Instance()
	{
		static AssertionBase mLog;
		return mLog;
	}

	AssertionBase::AssertionBase() : m_Assertion("Assert") {
	}

}    // namespace nkentseu