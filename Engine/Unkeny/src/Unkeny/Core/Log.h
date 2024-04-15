//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/14/2024 at 1:29:41 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#ifndef __NKENTSEU_LOG_H__
#define __NKENTSEU_LOG_H__

#pragma once

#include "System/System.h"
#include <System/Definitions/Memory.h>
#include <Logger/Logger.h>
#include <Logger/Assertion.h>

namespace nkentseu {
	class NKENTSEU_API UnkenyLogger {
	public:
		static UnkenyLogger& Instance();

		Memory::Shared<Logger> GetLog() {
			return m_Logger;
		}

		std::shared_ptr<Assertion> GetAssert() {
			return m_Assertion;
		}

		~UnkenyLogger() = default;
	private:
		std::shared_ptr<Logger> m_Logger;
		std::shared_ptr<Assertion> m_Assertion;

		UnkenyLogger();
	};

	#define Log		UnkenyLogger::Instance().GetLog()->Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
	#define Assert	UnkenyLogger::Instance().GetAssert()->Details(__FILE__, __LINE__, __FUNCTION__, nkentseu::Date::GetCurrent(), nkentseu::Time::GetCurrent())
} // namespace nkentseu

#endif    // __NKENTSEU_LOG_H__