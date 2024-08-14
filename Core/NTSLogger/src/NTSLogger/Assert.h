
//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/9/2024 at 8:55:16 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_ASSERT_H__
#define __NKENTSEU_ASSERT_H__

#pragma once

#include "NTSCore/System.h"
#include "Assertion.h"

namespace nkentseu {
	class NKENTSEU_API AssertionBase {
	public:
		static AssertionBase& Instance();

		Assertion& Get() {
			return m_Assertion;
		}

		~AssertionBase() = default;
	private:
		Assertion m_Assertion;

		AssertionBase();
	};

	#define SystemNameLoggerAssert "Assert"
	#define AssertBase(name)	AssertionBase::Instance().Get().SetName(name).Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
	#define sAssert				AssertBase(SystemNameLoggerAssert)

} // namespace nkentseu

#endif    // __NKENTSEU_ASSERT_H__