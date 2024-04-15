
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 8:55:16 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_ASSERT_H__
#define __NKENTSEU_ASSERT_H__

#pragma once

#include "System/System.h"
#include "Assertion.h"

namespace nkentseu {
	class NKENTSEU_API MAssert {
	public:
		static MAssert& Instance();

		Assertion& Get() {
			return m_Assertion;
		}

		~MAssert() = default;
	private:
		Assertion m_Assertion;

		MAssert();
	};

	#define Assert_base			MAssert::Instance().Get().Details(__FILE__, __LINE__, __FUNCTION__)
} // namespace nkentseu

#endif    // __NKENTSEU_ASSERT_H__