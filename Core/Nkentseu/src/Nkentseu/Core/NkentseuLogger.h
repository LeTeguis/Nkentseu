//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:18:31 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_NKENTSEULOGGER_H__
#define __NKENTSEU_NKENTSEULOGGER_H__

#pragma once

#include "System/System.h"
#include <Logger/Logger.h>
#include <memory>
#include <Logger/Assertion.h>

namespace nkentseu {
	class NKENTSEU_API NkentseuTrace {
	public:
		static NkentseuTrace& Instance();

		std::shared_ptr<Logger> GetLog() {
			return m_Logger;
		}

		std::shared_ptr<Assertion> GetAssert() {
			return m_Assertion;
		}

		~NkentseuTrace() = default;
	private:
		std::shared_ptr<Logger> m_Logger;
		std::shared_ptr<Assertion> m_Assertion;

		NkentseuTrace();
	};

	#define Log_nts		NkentseuTrace::Instance().GetLog()->Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
	#define Assert_nts	NkentseuTrace::Instance().GetAssert()->Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
} // namespace nkentseu

#endif    // __NKENTSEU_NKENTSEULOGGER_H__